// === BIBLIOTECAS NATIVAS ===
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
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

void iniciarMapaVazio();
void cadastro();
bool podeColocar(int linha, int coluna, char orientacao, int tamanho);

void colocarNavioDeLadinho(int id, int linha, int coluna, char orientacao);
void CadastroCompletao();
void mostrarTabuleiro();
bool todosNaviosAfundados();
int registrarTiro(int x, int y);
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
            Serial.println(" --- Cadastro ---");
            Serial.print("Navio ");
            Serial.print(i + 1);
            Serial.print(" -> Tamanho = ");
            Serial.print(navios[i].tamanho);

            Serial.println("\n Digite a linha (0 a 9):");
            while(!Serial.available());
            int linha = Serial.parseInt();
            Serial.read(); // Limpa o buffer

            Serial.println("Digite a coluna (0 a 9):");
            while(!Serial.available());
            int coluna = Serial.parseInt();
            Serial.read();

            Serial.println("Digite a orientacao (H/V):");
            while(!Serial.available());
            char orientacao = Serial.read();
            Serial.read();

            if(podeColocar(linha, coluna, orientacao, navios[i].tamanho)){
                colocarNavioDeLadinho(i, linha, coluna, orientacao);
                Serial.println("Navio Posicionado"); // BOTAR NO LCD
                posicionado = true;
                if(navios[i].tamanho != 5)
                    mostrarTabuleiro();
            } else {
                Serial.println("Posicao Invalida ladrao"); // BOTAR NO LCD
            }
        }
    }
    Serial.println("Todos os navios posicionados"); // BOTAR NO LCD
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
bool fim;

void setup()
{
    Serial.begin(9600);

    pinMode(UP, INPUT_PULLUP);
  	pinMode(RIGHT, INPUT_PULLUP);
  	pinMode(DOWN, INPUT_PULLUP);
  	pinMode(LEFT, INPUT_PULLUP);
  	pinMode(CRIVAR, INPUT_PULLUP);


    Jogador jogador = Jogador(0);

    iniciarMapaVazio();
    CadastroCompletao();
    // funcao de sidnei
    registrarTiro(5, 5); // OS parametros vao vir da função de sidnei e isso vai pro loop dps
    //mostrarTabuleiro(); 

    // Como é feita a comunicação direta entre as placas, não existe esse objeto.
    // Jogador jogador2 = Jogador(1);

}


void loop()
{
    //PLAYER JOGA
    //hitou = registrarTiro(/*infoX,*/ /*infoY*/); //SE TOMOU RETORNA 6, SE ERROU RETORNA 5
    //As variaveis infox e infoy são recebidas pela placa através do serial(ou algo do tipo)
    //SIDNEI RESOLVE ISSO e PRECISA decidir como dizer quem começa atirando, para não começar registrando um tiro que não foi dado como acontece duas linhas acima
    //SIDNEI usa essa merda desse hitou pra informar a outra placa que o tiro dela pegou em mim
    //cena de acerto ou cena de erro

    //Verifica se alguem ganhou
    // if(registrarTiro(infoX, infoY) == 6){
    //     fim = todosNaviosAfundados();
    // };

    // if(fim){
    //     //cena de vitoria 
    //     while(1);
    // }
    
    {
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
        cenaXY(plx, ply);
    } while (btn);
    //Dá o tiro com as coordenadas (plx, ply) e manda essas coordenadas via serial para a outra placa verificar se mamou
    //Estado atual vira espectador
}

int move(){
    if(digitalRead(UP) == LOW){
        return UP;
        
    } else if(digitalRead(RIGHT) == LOW){
        return RIGHT;

    } else if(digitalRead(DOWN) == LOW){
        return DOWN;

    } else if(digitalRead(LEFT) == LOW){
        return LEFT;

    } else if(digitalRead(CRIVAR) == LOW){
        return CRIVAR;

    }
}