// === BIBLIOTECAS NATIVAS ===
#include <Arduino.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>


// === 1. DECLARAÇÕES (.h) ===

// ========================================
// ARQUIVO: Cadastro_mapear.h
// ========================================
#define TAM_TABULEIRO 10

struct Navios {
    int tamanho;
    bool vivo;
    int vida;
    int linha;
    int coluna;
    char orientacao;
};

extern int plx;
extern int ply;
int move();

void iniciarMapaVazio();
void cadastro();
bool podeColocar(int linha, int coluna, char orientacao, int tamanho);

void colocarNavioDeLadinho(int id, int linha, int coluna, char orientacao);
void CadastroCompletao();
void mostrarTabuleiro();
bool todosNaviosAfundados();
int registrarTiro(int x, int y);

void receberCoord();
char escolherOrientacao();
// ========================================
// ARQUIVO: Comunicacao.h
// ========================================
#ifndef COMUNICACAO_H
#define COMUNICACAO_H


// TX da placa conecta no RX da outra
#define PIN_RX 7
#define PIN_TX 8

// principais
void conectarPlacas();
void enviarTiro(int x, int y);
int  receberStatusDoTiro();

// adversário
bool receberTiroAdversario(int &x, int &y);
void enviarStatusDoTiro(int resultado);

extern bool meuTurno;
extern SoftwareSerial serialPlaca;

#endif
// ========================================
// ARQUIVO: Recordes.h
// ========================================
#ifndef RECORDES_H
#define RECORDES_H

class Jogador {
    public:
        int id{-1};
        int vitorias{0};
        int tabuleiro[10][10];
        Jogador(int num);
};

int lerRecordes();
int salvarNovaVitoria();
int zerarRecordes();

#endif
// ========================================
// ARQUIVO: Tela.h
// ========================================
void inicializarTela();
void cenaTitulo();
void cenaAtaque();
void cenaPontos();

// === 2. IMPLEMENTAÇÕES (.cpp) ===

// ========================================
// ARQUIVO: Cadastro_mapear.cpp
// ========================================

// // struct
// struct Navios {
//     int tamanho;
//     bool vivo;
//     int linha;
//     int coluna;
//     char orientacao;
// };


Navios navios[4];
int tamanhos[4] = {2, 3, 4, 5};
int tabuleiro[10][10]; 

const int navio = 1;
const int agua = 0;

void iniciarMapaVazio() {
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            tabuleiro[i][j] = agua; // Zera tudo (água)
        }
    }
}

void cadastro() {
    for(int i = 0; i < 4; i++) {
        navios[i].tamanho = tamanhos[i];
        navios[i].vida = tamanhos[i];
        navios[i].vivo = true;
    }  
}

bool podeColocar(int linha, int coluna, char orientacao, int tamanho) {
    if(orientacao == 'H') {
        if(coluna + tamanho > 10) return false;
        for(int i = 0; i < tamanho; i++){
            if(tabuleiro[linha][coluna+i] != 0) return false;
        }
    } else {
        if(linha + tamanho > 10) return false;
        for(int i = 0; i < tamanho; i++){
            if(tabuleiro[linha+i][coluna] != 0) return false;
        }        
    }
    return true;
}

void colocarNavioDeLadinho(int id, int linha, int coluna, char orientacao) {
    navios[id].linha = linha;
    navios[id].coluna = coluna;
    navios[id].orientacao = orientacao;

    for(int i = 0; i < navios[id].tamanho; i++){
        if(orientacao == 'H'){
            tabuleiro[linha][coluna+i] = id + 1; // Usa id+1 para diferenciar os navios no mapa
        }else{
            tabuleiro[linha+i][coluna] = id + 1;
        }
    }   
}

void CadastroCompletao() {
    cadastro();

    for(int i = 0; i < 4; i++){
        bool posicionado = false;

        while(!posicionado) {
            receberCoord(); // Vai retornar plx e ply global 
            // WARNING: Talvez bem talves nao altere as glovais, acho que altera

            int linha = plx;
            int coluna = ply;
            char orientacao = escolherOrientacao();

            if(podeColocar(linha, coluna, orientacao, navios[i].tamanho)){
                colocarNavioDeLadinho(i, linha, coluna, orientacao);
                lcd.print("Navio Posicionado"); // BOTAR NO LCD
                posicionado = true;
                if(navios[i].tamanho != 5)
                    mostrarTabuleiro();
            } else {
                lcd.clear();
                lcd.print("Posicao Invalida"); // BOTAR NO LCD
            }
        }
    }
    lcd.clear();
    lcd.print("Navios Alinhados"); // BOTAR NO LCD
}

void mostrarTabuleiro() {
    Serial.println("   0 1 2 3 4 5 6 7 8 9");
    for (int i = 0; i < 10; i++) {
        Serial.print(i);
        Serial.print("  ");

        for (int j = 0; j < 10; j++) {
            if (tabuleiro[i][j] == 0) {
                Serial.print("~ ");   // água
            } else {
                Serial.print(tabuleiro[i][j]);
                Serial.print(" ");
            }
        }
        Serial.println();
    }
    Serial.println();
}

bool todosNaviosAfundados() {
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(tabuleiro[i][j] == navio) return false; // Se achar qualquer '1', o jogo continua
        }
    }
    return true; // Ganhou, parabéns moral
}

// 0 -> agua
// 1 a 4 -> navios
// 5 -> tiro na agua (mudei para 20 para não confundir com o ID do navio 2)
// 6 -> navio atingido (mudei para 30 para não confundir com o ID do navio 3)
int registrarTiro(int x, int y) {
    if(x < 10 && x >= 0 && y < 10 && y >= 0){
        // Se acertou um navio (valores de 1 a 4)
        if(tabuleiro[x][y] > 0 && tabuleiro[x][y] < 5){
            tabuleiro[x][y] = 6; // navio atingido
            Serial.println("Fez uma pra Deus ver pelo menos!");
            return 6;
        }
        else if (tabuleiro[x][y] == 0) {
            tabuleiro[x][y] = 5; // tiro na agua
            Serial.println("Rapaz, tu eh ruim viu, errou o tiro");
            return 5;
        }
    }
    else {
        Serial.println("Tu eh burro ou o que? Tiro fora do tabuleiro ou colocou numero negativo");
        return -1;
    }
    return 0; 
}


// void setup() {
//     Serial.begin(9600);
//     iniciarMapaVazio();
//     CadastroCompletao();
//     // funcao de sidnei
//     registrarTiro(5, 5); // OS parametros vao vir da função de sidnei e isso vai pro loop dps
    
//     //mostrarTabuleiro(); 
// }

// void loop() {
//     // A lógica de turnos virá aqui depois
// }

int escolherPosicao() {
    plx = 0;
    ply = 0;

    bool escolhendo = true;

        receberCoord();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Linha: ");
        lcd.print(plx);

        lcd.setCursor(0,1);
        lcd.print("Coluna: ");
        lcd.print(ply);

        delay(150);
    
}

char escolherOrientacao() {

    char o = 'H';
    bool escolhendo = true;

    while (escolhendo) {

        btn = move();

        if (btn == UP || btn == DOWN) {
            o = (o == 'H') ? 'V' : 'H'; // ternario sagaz LIMONATIONS -> muda a orientação cada vez q vc aperta
        }

        if (btn == CRIVAR) {
            escolhendo = false;
        }

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Orientacao:");
        lcd.setCursor(0,1);
        lcd.print(o);

        delay(150);
    }

    return o;
}


// ========================================
// ARQUIVO: Comunicacao.cpp
// ========================================

SoftwareSerial serialPlaca(PIN_RX, PIN_TX); // RX=7, TX=8

bool meuTurno = false;

void conectarPlacas() {
    serialPlaca.begin(9600);
    
    Serial.println("Aguardando conexao com a outra placa...");
    
    bool conectado = false;
    bool jaEnvieiReady = false;
    unsigned long ultimoEnvio = 0;
    
    while (!conectado) {
        unsigned long agora = millis();
        if (agora - ultimoEnvio >= 500) {
            serialPlaca.println("READY");
            ultimoEnvio = agora;
            if (!jaEnvieiReady) {
                jaEnvieiReady = true;
            }
        }
        if (serialPlaca.available()) {
            String msg = serialPlaca.readStringUntil('\n');
            msg.trim();
            
            if (msg == "READY") {
                conectado = true;
                if (jaEnvieiReady) {
                    meuTurno = true;
                    Serial.println("Conectado! Voce ataca primeiro.");
                } else {
                    meuTurno = false;
                    Serial.println("Conectado! Adversario ataca primeiro.");
                }
                serialPlaca.println("READY");
            }
        }
    }
    
    delay(200);
    
    while (serialPlaca.available()) {
        serialPlaca.read();
    }
    
    Serial.println("=== JOGO INICIADO ===");
}

void enviarTiro(int x, int y) {
    String mensagem = "TIRO:" + String(x) + "," + String(y);
    serialPlaca.println(mensagem);
    
    Serial.print("Tiro enviado -> (");
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.println(")");
}

int receberStatusDoTiro() {
    Serial.println("Aguardando resposta do tiro...");
    
    unsigned long inicio = millis();
    while (!serialPlaca.available()) {
        if (millis() - inicio > 30000) {
            Serial.println("TIMEOUT! Sem resposta da outra placa.");
            return -1;
        }
    }
    
    String resposta = serialPlaca.readStringUntil('\n');
    resposta.trim();
    
    if (resposta == "HIT") {
        Serial.println("Status: ACERTOU!");
        return 6;
    } 
    else if (resposta == "MISS") {
        Serial.println("Status: ERROU!");
        return 5;
    } 
    else if (resposta == "WIN") {
        Serial.println("Status: VOCE VENCEU! Todos os navios inimigos foram afundados!");
        return -1;
    }
    
    Serial.print("Resposta desconhecida: ");
    Serial.println(resposta);
    return 0;
}

bool receberTiroAdversario(int &x, int &y) {
    Serial.println("Aguardando tiro do adversario...");

    unsigned long inicio = millis();
    while (!serialPlaca.available()) {
        if (millis() - inicio > 60000) {
            Serial.println("TIMEOUT! Adversario demorou demais.");
            return false;
        }
    }
    
    String msg = serialPlaca.readStringUntil('\n');
    msg.trim();
    
    if (msg.startsWith("TIRO:")) {
        String coords = msg.substring(5);
        
        int virgula = coords.indexOf(',');
        if (virgula > 0) {
            x = coords.substring(0, virgula).toInt();
            y = coords.substring(virgula + 1).toInt();
            
            Serial.print("Tiro recebido -> (");
            Serial.print(x);
            Serial.print(",");
            Serial.print(y);
            Serial.println(")");
            
            return true;
        }
    }
    
    Serial.print("Mensagem invalida recebida: ");
    Serial.println(msg);
    return false;
}

void enviarStatusDoTiro(int resultado) {
    if (resultado == 6) {
        if (todosNaviosAfundados()) {
            serialPlaca.println("WIN");
            Serial.println("Enviado: WIN - Todos os seus navios foram afundados!");
        } else {
            serialPlaca.println("HIT");
            Serial.println("Enviado: HIT");
        }
    } 
    else if (resultado == 5) {
        serialPlaca.println("MISS");
        Serial.println("Enviado: MISS");
    }
    else {
        serialPlaca.println("MISS");
        Serial.println("Enviado: MISS (tiro invalido/repetido)");
    }
}

// ========================================
// ARQUIVO: Recordes.cpp
// ========================================

Jogador::Jogador(int num) {
    id = num;
    vitorias = lerRecordes();
}

// Id player, Vitoria

int lerRecordes() {
    int valor_lido{0};
    int enderenco_inicial{0};
    
    Serial.print("Read int from EEPROM: ");
    
    EEPROM.get(enderenco_inicial, valor_lido);
    
    Serial.println(valor_lido);

    return valor_lido;
}

int salvarNovaVitoria() {
    EEPROM[0] += 1;
}

int zerarRecordes() {
    EEPROM.write(0, 0);
}
// ========================================
// ARQUIVO: Tela.cpp
// ========================================

// Endereço I2C: No Tinkercad geralmente é 32 (0x20) ou 39 (0x27).
// Tente 32 primeiro. Se não funcionar, mude para 0x27.
LiquidCrystal_I2C lcd(0x20, 16, 2); 

byte explosao_tela[8] = {
  B10001,
  B01010,
  B00100, // Centro
  B11111, // Explosão
  B00100,
  B01010,
  B10001,
  B00000
};

byte agua_tela[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10101 // Ondinhas
};

byte missil_tela[8] = {
  B00000,
  B00000,
  B00110, // Corpo do missil
  B11111,
  B00110,
  B00000,
  B00000,
  B00000
};


void inicializarTela() {
  // Inicializa o LCD
  lcd.init(); 

  // Liga a luz de fundo
  lcd.backlight();
  
  // Registrar os desenhos na memória do LCD (Slots 0 a 4)
  lcd.createChar(2, explosao_tela);
  lcd.createChar(3, agua_tela);
  lcd.createChar(4, missil_tela);
}

// ---------------------------------------------
// Catalogo de Cenas

void cenaTitulo() {
    lcd.clear();
    
    lcd.setCursor(1, 0);
    lcd.print("BATALHA  NAVAL");
    
    // Desenha o mar
    for(int i=0; i<16; i++) {
        lcd.setCursor(i, 1);
        lcd.write(3); // Caractere da agua
    }
    
    cenaAtaque();
    
    delay(500);
}

void cenaAtaque() {
  // Anima o míssil indo da esquerda para a direita
  for(int i=0; i<17; i++) {
    // Apaga o rastro do missil (recoloca agua)
    if(i > 0) {
      lcd.setCursor(i-1, 1);
      lcd.write(3); 
    }
    
    // Desenha o missil
    lcd.setCursor(i, 1);
    lcd.write(4);
    
    delay(150); // Velocidade do tiro
  }
}

void cenaPontos() {
  lcd.clear();
  
  lcd.setCursor(1, 0);
  lcd.print("Frota  Aliada");
  lcd.setCursor(6, 1);
  lcd.print("555");
  
  delay(3500);
    
  lcd.clear();
  
  lcd.setCursor(1, 0);
  lcd.print("Frota  Inimiga");
  lcd.setCursor(6, 1);
  lcd.print("111");

  delay(3500);
}

void cenaXY(int linha, int coluna) {
  lcd.setCursor(1, 0);
  lcd.print("Coord. do Tiro");
  
  int posInicial{6};
  
  lcd.setCursor(posInicial, 1);
  lcd.print("(");
  
  lcd.setCursor(posInicial + 1, 1);
  lcd.print(linha);
  
  lcd.setCursor(posInicial + 2, 1);
  lcd.print(",");
  
  lcd.setCursor(posInicial + 3, 1);
  lcd.print(coluna);
  
  lcd.setCursor(posInicial + 4, 1);
  lcd.print(")");
}

void cenaAcertou() {
  lcd.clear();
  
  // Anima o míssil indo da esquerda para a direita
  for(int i=0; i<12; i++) {
    // Apaga o rastro do missil (recoloca agua)
    if(i > 0) {
      lcd.setCursor(i-1, 1);
      lcd.write(3); 
    }
    
    // Desenha o missil
    lcd.setCursor(i, 1);
    lcd.write(4);
    
    delay(150); // Velocidade do tiro
  }
  
  // O míssil atingiu (posição 11)
  
  // Efeito piscando (BOOM)
  for(int k=0; k<3; k++) {
    lcd.setCursor(11, 1);
    lcd.write(2); // Desenha explosão
    lcd.setCursor(12, 1);
    lcd.write(2); // Desenha explosão
    
    lcd.setCursor(4, 0);
    lcd.print("   KABOOM!!   ");
    
    delay(100);
    lcd.noBacklight(); // Apaga a luz da tela
    delay(100);
    lcd.backlight();   // Acende a luz da tela
  }
  
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("ALVO  ABATIDO");
  delay(1000);
}

void cenaErrou() {
  lcd.clear();
  
  // Anima o míssil indo da esquerda para a direita
  for(int i=0; i<17; i++) {
    // Apaga o rastro do missil (recoloca agua)
    if(i > 0) {
      lcd.setCursor(i-1, 1);
      lcd.write(3); 
    }
    
    // Desenha o missil
    lcd.setCursor(i, 1);
    lcd.write(4);
    
    delay(150); // Velocidade do tiro
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ERROU MEU CHEFE!");
  delay(1000);
}

// ---------------------------------------------

// void loop()
// {
//   cenaTitulo();
//   cenaPontos();
// }


// === 3. CÓDIGO PRINCIPAL (.ino) ===

// ========================================
// ARQUIVO: Projeto_BatalhaNaval.ino
// ========================================


#define UP 2
#define RIGHT 3
#define DOWN 4
#define LEFT 5
#define CRIVAR 6

int plx = 0, ply = 0;
int btn = 0;
int hitou;
bool fim = false;

void setup()
{
    Serial.begin(9600);

    pinMode(UP, INPUT_PULLUP);
    pinMode(RIGHT, INPUT_PULLUP);
    pinMode(DOWN, INPUT_PULLUP);
    pinMode(LEFT, INPUT_PULLUP);
    pinMode(CRIVAR, INPUT_PULLUP);

    inicializarTela();
    cenaTitulo();

    Jogador jogador = Jogador(0);

    // 1: Posicionar navios
    iniciarMapaVazio();
    CadastroCompletao();
    mostrarTabuleiro();

    // 2: Conectar com a outra placa (handshake)
    conectarPlacas();
}


void loop()
{
    if (fim) {
        return;
    }

    if (meuTurno) {
        // === MEU TURNO: ATACAR ===
        
        Serial.println("--- Seu turno! Escolha as coordenadas ---");
        lcd.clear();
        cenaXY(plx, ply); // mostra inicial

        // Usa a função de input unificada (vinda do merge, agora no cpp)
        receberCoord(); 

        // Envia o tiro para a outra placa
        enviarTiro(plx, ply);

        // Aguarda a resposta (HIT, MISS ou WIN)
        hitou = receberStatusDoTiro();

        if (hitou == 6) {
            cenaAcertou();
        } else if (hitou == 5) {
            cenaErrou();
        } else if (hitou == -1) {
            lcd.clear();
            lcd.setCursor(2, 0);
            lcd.print("VOCE  VENCEU!");
            lcd.setCursor(1, 1);
            lcd.print("PARABENS MORAL");
            Serial.println("=== VOCE VENCEU! ===");
            salvarNovaVitoria();
            fim = true;
            return;
        }

        // Passa o turno para o adversário
        meuTurno = false;

    } else {
        // === TURNO DO ADVERSÁRIO: DEFENDER ===
        Serial.println("--- Turno do adversario. Aguardando tiro... ---");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Turno  Inimigo");
        lcd.setCursor(0, 1);
        lcd.print("Aguardando...");

        int tiroX, tiroY;
        if (receberTiroAdversario(tiroX, tiroY)) {
            // Registra o tiro no meu tabuleiro
            int resultado = registrarTiro(tiroX, tiroY);

            // Mostra no LCD onde o adversário atirou
            lcd.clear();
            cenaXY(tiroX, tiroY);
            delay(500);

            // Envia o resultado para o adversário (HIT/MISS/WIN)
            enviarStatusDoTiro(resultado);

            if (resultado == 6) {
                cenaAcertou();

                if (todosNaviosAfundados()) {
                    lcd.clear();
                    lcd.setCursor(1, 0);
                    lcd.print("VOCE  PERDEU!");
                    lcd.setCursor(2, 1);
                    lcd.print("FIM DE JOGO");
                    Serial.println("=== VOCE PERDEU! ===");
                    fim = true;
                    return;
                }
            } else if (resultado == 5) {
                cenaErrou();
            }

            mostrarTabuleiro();
        }

        meuTurno = true;
    }
}

int move() {
    if (digitalRead(UP) == LOW) {
        return UP;
    } else if (digitalRead(RIGHT) == LOW) {
        return RIGHT;
    } else if (digitalRead(DOWN) == LOW) {
        return DOWN;
    } else if (digitalRead(LEFT) == LOW) {
        return LEFT;
    } else if (digitalRead(CRIVAR) == LOW) {
        return CRIVAR;
    }
    return 0;
}