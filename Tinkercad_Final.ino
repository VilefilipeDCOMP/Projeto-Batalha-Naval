// === BIBLIOTECAS NATIVAS ===
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>


// === 1. DECLARAÇÕES (.h) ===

// ========================================
// ARQUIVO: !Global.h
// ========================================
#define UP 2
#define RIGHT 3
#define DOWN 4
#define LEFT 5
#define CRIVAR 6

// ##define F(x) F(x)
#define printS(x) Serial.print(F(x))

int plx = 0, ply = 0;
int btn = 0;
LiquidCrystal_I2C lcd(0x20, 16, 2); 
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
        Jogador(int num);
};

int lerRecordes(int jogador_id);
int salvarNovaVitoria(int jogador_id);
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
int8_t tabuleiroBarco[10][10];
int8_t tabuleiroTiro[10][10]; 

const int navio = 1;
const int agua = 0;

void iniciarMapaVazio() {
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            tabuleiroBarco[i][j] = agua; // Zera tudo (água)
            tabuleiroTiro[i][j] = agua;
        }
    }
    // printS("Mapa Pronto!");
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
            if(tabuleiroBarco[linha][coluna+i] != 0) return false;
        }
    } else {
        if(linha + tamanho > 10) return false;
        for(int i = 0; i < tamanho; i++){
            if(tabuleiroBarco[linha+i][coluna] != 0) return false;
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
            tabuleiroBarco[linha][coluna+i] = id + 1; // Usa id+1 para diferenciar os navios no mapa
        }else{
            tabuleiroBarco[linha+i][coluna] = id + 1;
        }
    }   
}

void CadastroCompletao() {
    cadastro();

    for(int i = 0; i < 4; i++){
        bool posicionado = false;

        while(!posicionado) {
            // Serial.print(F("Entrando no teclado para digitar"));

            receberCoord(); // Vai retornar plx e ply global 
            // WARNING: Talvez bem talves nao altere as glovais, acho que altera

            int linha = plx;
            int coluna = ply;
            char orientacao = escolherOrientacao();
            lcd.clear();

            if(podeColocar(linha, coluna, orientacao, navios[i].tamanho)){
                colocarNavioDeLadinho(i, linha, coluna, orientacao);

                posicionado = true;
                if(navios[i].tamanho != 5) {
                    lcd.print(F("Navio Colocado")); // BOTAR NO LCD
                    mostrarTabuleiro();
                }
                delay(200);
            } else {

                lcd.print(F("Posicao Invalida")); // BOTAR NO LCD
                delay(200);
            }
        }
    }
    lcd.clear();
    lcd.print(F("Navios Alinhados")); // BOTAR NO LCD
    delay(200);
}

void mostrarTabuleiro() {
    Serial.println(F("\nTabuleiro de Barco"));
    Serial.println("\n   0 1 2 3 4 5 6 7 8 9");
    for (int i = 0; i < 10; i++) {
        Serial.print(i);
        Serial.print("  ");

        for (int j = 0; j < 10; j++) {
            if (tabuleiroBarco[i][j] == 0) {
                Serial.print(F("~ "));   // água
            } else {
                Serial.print(tabuleiroBarco[i][j]);
                Serial.print(F(" "));
            }
        }
        Serial.println();
    }
    Serial.println();
}

void mostrarTabuleiroTiro() {
    Serial.println(F("\n Verifique seu Tabuleiro de Tiros"));
    Serial.println("\n   0 1 2 3 4 5 6 7 8 9");
    for (int i = 0; i < 10; i++) {
        Serial.print(i);
        Serial.print("  ");

        for (int j = 0; j < 10; j++) {
            if (tabuleiroTiro[i][j] == 0) {
                Serial.print(F("~ "));   // água
            } else {
                Serial.print(tabuleiroTiro[i][j]);
                Serial.print(F(" "));
            }
        }
        Serial.println();
    }
    Serial.println();
}

bool todosNaviosAfundados() {
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(tabuleiroBarco[i][j] == navio) return false; // Se achar qualquer '1', o jogo continua
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
        if(tabuleiroBarco[x][y] > 0 && tabuleiroBarco[x][y] < 5){
            // tabuleiroTiro[x][y] = 6;
            tabuleiroBarco[x][y] = 6;
            // navio atingido
            // Serial.println(F("Fez uma pra Deus ver pelo menos!"));
            return 6;
        }
        else if (tabuleiroBarco[x][y] == 0) {
            // tabuleiroTiro[x][y] = 5; // tiro na agua
            // Serial.println(F("Rapaz, tu eh ruim viu, errou o tiro"));
            return 5;
        }
    }
    else {
        // Serial.println("Tu eh burro ou o que? Tiro fora do tabuleiroBarco ou colocou numero negativo");
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
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Orientacao:");

    while (escolhendo) {

        btn = move();

        if (btn == UP || btn == DOWN) {
            o = (o == 'H') ? 'V' : 'H'; // ternario sagaz LIMONATIONS -> muda a orientação cada vez q vc aperta
        }

        if (btn == CRIVAR) {
            escolhendo = false;
        }

        lcd.setCursor(0,1);
        lcd.print(o);

        delay(25);
    }

    lcd.clear();
    return o;
}


// ========================================
// ARQUIVO: Comunicacao.cpp
// ========================================

SoftwareSerial serialPlaca(PIN_RX, PIN_TX); // RX=7, TX=8

bool meuTurno = false;

void conectarPlacas() {
    serialPlaca.begin(2400);   // 4800 baud: mais confiável no SoftwareSerial do Tinkercad
    serialPlaca.listen();       // Garante que esta porta SoftwareSerial está ativa

    // Serial.println(F("Aguardando conexao com a outra placa..."));

    bool conectado = false;
    unsigned long ultimoEnvio = 0;
    unsigned int semente = analogRead(A5);  // pino flutuante para aleatoriedade

    // PROTOCOLO SIMPLIFICADO:
    //   'R' = READY    'A' = ACK
    // Apenas 1 byte cada, lido byte a byte — sem readStringUntil.

    while (!conectado) {
        // --- RECEPÇÃO (byte a byte, sem bloqueio, sem prints) ---
        if (serialPlaca.available()) {
            char c = serialPlaca.read();

            if (c == 'R') {
                delay(100);
                serialPlaca.write('A');
                delay(50);
                conectado = true;
                meuTurno = false;
            }
            else if (c == 'A') {
                conectado = true;
                meuTurno = true;
            }
            // qualquer outro byte é ignorado silenciosamente
        }

        // --- ENVIO periódico com intervalo aleatório ---
        unsigned long agora = millis();
        unsigned long intervalo = 1000 + (semente % 500);
        if (!conectado && (agora - ultimoEnvio >= intervalo)) {
            serialPlaca.write('R');
            ultimoEnvio = agora;
            delay(200);  // janela de escuta obrigatória após envio
        }
    }

    delay(300);

    // Limpa lixo remanescente
    while (serialPlaca.available()) {
        serialPlaca.read();
    }

    // // Prints só DEPOIS de sair do loop
    // if (meuTurno) {
    //     Serial.println(F("Conectado! Voce ataca primeiro."));
    // } else {
    //     Serial.println(F("Conectado! Adversario ataca primeiro."));
    // }
    // Serial.println(F("=== JOGO INICIADO ==="));
}


void enviarTiro(int x, int y) {
    // String mensagem = "TIRO:" + String(x) + "," + String(y);
    // serialPlaca.println(mensagem);

    serialPlaca.print("TIRO:");
    serialPlaca.print(x);
    serialPlaca.print(',');
    serialPlaca.println(y);

    
    // Serial.print("Tiro enviado -> (");
    // Serial.print(x);
    // Serial.print(",");
    // Serial.print(y);
    // printS(")");
}

int receberStatusDoTiro() {
    //printS("Aguardando resposta do tiro...");
    
    unsigned long inicio = millis();
    while (!serialPlaca.available()) {
        if (millis() - inicio > 30000) {
            // printS("TIMEOUT! Sem resposta da outra placa.");
            return -1;
        }
    }
    
    String resposta = serialPlaca.readStringUntil('\n');
    resposta.trim();
    
    if (resposta == "HIT") {
        // printS("Status: ACERTOU!");
        return 6;
    } 
    else if (resposta == "MISS") {
        // printS("Status: ERROU!");
        return 5;
    } 
    else if (resposta == "WIN") {
        // printS("Status: VOCE VENCEU! Todos os navios inimigos foram afundados!");
        return -1;
    }
    
    Serial.println("Resposta desconhecida: ");
    Serial.println(resposta);
    return 0;
}

bool receberTiroAdversario(int &x, int &y) {
    //printS("Aguardando tiro do adversario...");

    unsigned long inicio = millis();
    while (!serialPlaca.available()) {
        if (millis() - inicio > 60000) {
            // printS("TIMEOUT! Adversario demorou demais.");
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
            
            // printS("Tiro recebido -> (");
            // Serial.print(x);
            // printS(",");
            // Serial.print(y);
            // printS(")");
            
            return true;
        }
    }
    
    printS("Mensagem invalida recebida: ");
    Serial.print(msg);
    return false;
}

void enviarStatusDoTiro(int resultado) {
    if (resultado == 6) {
        if (todosNaviosAfundados()) {
            serialPlaca.println("WIN");
            // printS("Enviado: WIN - Todos os seus navios foram afundados!");
        } else {
            serialPlaca.println("HIT");
            // printS("Enviado: HIT");
        }
    } 
    else if (resultado == 5) {
        serialPlaca.println("MISS");
        // printS("Enviado: MISS");
    }
    else {
        serialPlaca.println("MISS");
        // printS("Enviado: MISS (tiro invalido/repetido)");
    }
}

// ========================================
// ARQUIVO: Recordes.cpp
// ========================================

Jogador::Jogador(int num) {
    id = num;
    vitorias = lerRecordes(num);
}

int lerRecordes(int jogador_id) {
    int valor_lido{0};
    int enderenco_inicial{sizeof(int) * jogador_id};
    
    EEPROM.get(enderenco_inicial, valor_lido);
    
    Serial.println(valor_lido);

    return valor_lido;
}

int salvarNovaVitoria(int jogador_id) {
    // vitorias_jogador = lerRecordes(jogador); 
    // EEPROM[jogador_id] += 1;

    int valor_atual{lerRecordes(jogador_id)};
    int enderenco_inicial{sizeof(int) * jogador_id};

    EEPROM.put(enderenco_inicial, valor_atual + 1);
}

int zerarRecordes() {
    for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
    }
}
// ========================================
// ARQUIVO: Tela.cpp
// ========================================

// Endereço I2C: No Tinkercad geralmente é 32 (0x20) ou 39 (0x27).
// Tente 32 primeiro. Se não funcionar, mude para 0x27.
// LiquidCrystal_I2C lcd(0x20, 16, 2); 

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
    lcd.clear();
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
  lcd.setCursor(7, 1);
  lcd.print(lerRecordes(0));
  
  delay(3500);
    
  lcd.clear();
  
  lcd.setCursor(1, 0);
  lcd.print("Frota  Inimiga");
  lcd.setCursor(7, 1);
  lcd.print(lerRecordes(1));

  delay(3500);

  lcd.clear();
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
  lcd.clear();
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
  lcd.clear();
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


int hitou;
bool fim = false;
bool telaDesenhada = false;

void setup()
{
    Serial.begin(9600);

    pinMode(UP, INPUT_PULLUP);
    pinMode(RIGHT, INPUT_PULLUP);
    pinMode(DOWN, INPUT_PULLUP);
    pinMode(LEFT, INPUT_PULLUP);
    pinMode(CRIVAR, INPUT_PULLUP);

    inicializarTela();

    iniciarJogo();
}

void loop(){
    if (fim) {
        if (!telaDesenhada) {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Clique CRIVAR p/");
            lcd.setCursor(0,1);
            lcd.print("reiniciar jogo");
            telaDesenhada = true;
        }

        btn = move();
        if (btn == CRIVAR) {
            iniciarJogo();
        }

    }

    if (!fim) {
        if (meuTurno) {
            Serial.println("--- Seu turno ---");
            lcd.clear();

            mostrarTabuleiroTiro();
            mostrarTabuleiro();

            receberCoord();
    
            enviarTiro(plx, ply);
    
            hitou = receberStatusDoTiro();
    
            if (hitou == 6) {
                tabuleiroTiro[plx][ply] = 6;
                cenaAcertou();
            } else if (hitou == 5) {
                tabuleiroTiro[plx][ply] = 5;
                cenaErrou();
            } else if (hitou == -1) {
                cenaAcertou();
                lcd.clear();
                lcd.setCursor(2, 0);
                lcd.print("VOCE  VENCEU!");
                lcd.setCursor(1, 1);
                lcd.print("PARABENS MORAL");
                Serial.println("=== VOCE VENCEU! ===");
                salvarNovaVitoria(0);
                fim = true;
                return;
            }
    
            meuTurno = false;
    
        } else {
            Serial.println("--- Turno do adversario. ---");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Turno  Inimigo");
            lcd.setCursor(0, 1);
            lcd.print("Aguardando...");
    
            int tiroX, tiroY;
            if (receberTiroAdversario(tiroX, tiroY)) {
                int resultado = registrarTiro(tiroX, tiroY);
    
                lcd.clear();
                cenaXY(tiroX, tiroY);
                delay(500);
    
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
                        salvarNovaVitoria(1);
                        fim = true;
                        return;
                    }
                } else if (resultado == 5) {
                    cenaErrou();
                }
            }
    
            meuTurno = true;
    
        //Dá o tiro com as coordenadas (plx, ply) e manda essas coordenadas via serial para a outra placa verificar se mamou
        //Estado atual vira espectador
        }
    }
}

void iniciarJogo(){
    hitou = 0;
    fim = false;
    telaDesenhada = false;

    // cenaTitulo();
    menuStartGame();
    // cenaPontos();

    // 1: Posicionar navios
    iniciarMapaVazio();
    CadastroCompletao();

    // Mostrar na tela "Vez do jogador dois e repetir o processo de cadastro"
    
    // // funcao de sidnei
    // registrarTiro(5, 5); // OS parametros vao vir da função de sidnei e isso vai pro loop dps

    mostrarTabuleiro();

    // // Conecta com a outra placa (handshake)
    conectarPlacas();

    lcd.clear();
    lcd.print(" JOGO  INICIADO ");

    delay(500);
}

// bool cenaPontosTick() {
//   // retorna true enquanto a cena está rodando, false quando terminou
//   if (cpEstado == CP_INATIVA) return false;

//   if (cpEstado == CP_ALIADA) {
//     if (millis() - cpT0 >= 3500UL) {
//       cpEstado = CP_INIMIGA;
//       cpT0 = millis();

//       lcd.clear();
//       lcd.setCursor(1, 0);
//       lcd.print("Frota  Inimiga");
//       lcd.setCursor(6, 1);
//       lcd.print(lerRecordes(1));
//     }
//     return true;
//   }

//   if (cpEstado == CP_INIMIGA) {
//     if (millis() - cpT0 >= 3500UL) {
//       cpEstado = CP_FIM;
//       lcd.clear();
//     }
//     return true;
//   }

//   // CP_FIM
//   cpEstado = CP_INATIVA;
//   return false;
// }

void menuStartGame() {
    bool start_estado = true;

    while (start_estado) {
        cenaTitulo();
        cenaPontos();

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Zerar Record: <-");
        lcd.setCursor(0,1);
        lcd.print("Start:    CRIVAR");

        for (int i = 0; i < 10000; i++) {
            btn = move();
            if (btn == CRIVAR) {
                start_estado = false;
            } else if (btn == LEFT) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Seu recorde foi");
                lcd.setCursor(0,1);
                lcd.print("zerado!");
                zerarRecordes();
                delay(500);
            }
        }
    }
}

void receberCoord() {
    plx = 0; ply = 0;
  	btn = 0;

    //  Coord.
    //      (0,0)
    
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Coord.");

    lcd.setCursor(6, 1);   
    lcd.print("(");           
    lcd.print(0);
    lcd.print(",");
    lcd.print(0);
    lcd.print(")");
  
    do {
        btn = move();
        switch (btn){
            case UP:
                if(plx > 0) plx--;
                break;
            
            case RIGHT:
                if(ply < 9) ply++;
                break;

            case DOWN:
                if(plx < 9) plx++;
                break;

            case LEFT:
                if(ply > 0) ply--;
                break;

            case CRIVAR:
                btn = 0;
                break;

            default:
                break;
            }

        lcd.setCursor(7, 1);
        lcd.print(plx);
        
        lcd.setCursor(9, 1);
        lcd.print(ply);
      
      	delay(25);
    } while (btn);
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
     //return 0; Algum imbecil botou esse return 0 aqui animal estupido (pode ter sido eu)
}