// === BIBLIOTECAS NATIVAS ===
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


// === 1. DECLARAÇÕES (.h) ===

// ========================================
// ARQUIVO: Cadastro.h
// ========================================
#define TAM_TABULEIRO 10

struct Navios {
    int tamanho; // n vou explicar pq vc n eh broco
    int vida;   //  n vou explicar pq vc n eh broco pt 2
    int linha;  /* linha e coluna definem a posicao do navio */
    int coluna; 
    char orientacao; // se o navio ta na horizontal ou na vertical
    bool vivo; // pra ver se ta vivo
};

void cadastro();
bool podeColocar(int linha, int coluna, char orientacao, int tamanho);
void colocarNavioDeLadinho(int id, int linha, int coluna, char orientacao);
void CadastroCompletao();
// ========================================
// ARQUIVO: MapearNaviosDada.h
// ========================================
void iniciarMapaVazio();
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
// ARQUIVO: Cadastro.cpp
// ========================================

// void loop(){

//    if(estadoDoJogo == POSICIONAMENTO){
//        // cadastrar navios
//    }

//    else if(estadoDoJogo == JOGANDO){
//        // receber tiro
//        // verificar acerto
//        // atualizar vidas
//    }
// }


// #define TAM_TABULEIRO 10

// struct Navios {
//     int tamanho; // n vou explicar pq vc n eh broco
//     int vida;   //  n vou explicar pq vc n eh broco pt 2
//     int linha;  /* linha e coluna definem a posicao do navio */
//     int coluna; 
//     char orientacao; // se o navio ta na horizontal ou na vertical
//     bool vivo; // pra ver se ta vivo
// };

Navios navios[4];
//int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]; // tabuleiro de teste pra validar codigo

int tamanhos[4] = {2,3,4,5};

void cadastro(){
    for(int i=0; i < 4; i++) {
        navios[i].tamanho = tamanhos[i];
        navios[i].vida = tamanhos[i];
        navios[i].vivo = true;
    }  
}

// 0 -> agua
// 1 -> navio
// -1 -> tiro na agua
// -2 -> navio atingido

bool podeColocar(int linha, int coluna, char orientacao, int tamanho){
    if(orientacao == 'H') {
        if(coluna + tamanho > TAM_TABULEIRO) return false;

        for(int i =0; i < tamanho; i++){
            //vou usar o tabuleiro teste
            if(tabuleiro[linha][coluna+i] != 0) return false;
        }
    } else {
        if(linha + tamanho > TAM_TABULEIRO) return false;
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

    for(int i=0; i < navios[id].tamanho; i++){


        if(orientacao == 'H'){
            tabuleiro[linha][coluna+i] = id+1;
        }else{
            tabuleiro[linha+i][coluna] = id+1;
        }
    }   
}

// void ataque(int linha, int coluna){
//     int valor = tabuleiro[linha][coluna];

//     if(valor > 0) {
//         int idNavio = valor-1;
//         navios[idNavio].vida--;

//         tabuleiro[linha][coluna] = -2; // acertou um navio -> -2
//         Serial.println("Acertou!");

//         if(navios[idNavio].vida == 0) {
//             Serial.println("Navio Afundado!");
//         }
//     } else {
//         tabuleiro[linha][coluna] = -1;
//         Serial.println("Errou!");
//     }
// }

void CadastroCompletao(){

    cadastro();

    for(int i=0; i < 4; i++){

        bool posicionado = false;

        while(!posicionado) {
            Serial.println(" --- Cadastro ---");
            Serial.print("Navio ");
            Serial.print(i);
            Serial.print(" -> Tamanho = ");
            Serial.print(navios[i].tamanho);

            Serial.println("\n Digite a linha:");
            while(!Serial.available());
            int linha = Serial.parseInt();
            Serial.read();

            Serial.println("Digite a coluna:");
            while(!Serial.available());
            int coluna = Serial.parseInt();
            Serial.read();

            Serial.println("Digite a coluna(H/V):");
            while(!Serial.available());
            char orientacao = Serial.read();
            Serial.read();

            if(podeColocar(linha, coluna, orientacao, navios[i].tamanho)){

                colocarNavioDeLadinho(i, linha, coluna, orientacao);
                Serial.println("Navio Posicionado");
                posicionado = true;
            }else {
                Serial.println("Posicao Invalida ladrao");
            }
        }
    }

    Serial.println("Todos os navios posicionados");
}


// ========================================
// ARQUIVO: MapearNaviosDada.cpp
// ========================================
int tabuleiro[10][10];
const int navio = 1;
const int hagata = 0;

void iniciarMapaVazio(){
    for(int i = 0;i < 10; i++){
        for(int j = 0;j < 10; j++){
            tabuleiro[i][j] = hagata;
        }
    }
};

bool todosNaviosAfundados(){
    for(int i = 0;i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(tabuleiro[i][j] == navio) return false;//ainda tem navio nessa mizera
        }
    }
    return true;//ganhou, parabens moral
};

// 0 -> agua
// 1 -> navio
// 2 -> tiro na agua
// 3 -> navio atingido

int registrarTiro(int x, int y){
    if(x < 10 && x >= 0 && y < 10 && y >= 0){
        if(tabuleiro[x][y] == navio){
            tabuleiro[x][y] = 3;//navio atingido
            Serial.println("Fez uma pra Deus ver pelo menos!");
            return 3;
            }
        else{
            tabuleiro[x][y] = 2;//tiro na agua
            Serial.println("Rapaz, tu é ruim viu, errou o tiro");
            return 2;
        }
    }
    else{
        Serial.println("Tu é burro ou o que? Tiro fora do tabuleiro ou colocou numero negativo");
        return -1;
    }
    return 0;//tecnicamente isso nao é pra acontecer, mas é so pra compilar direitinho}
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

byte agua[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10101 // Ondinhas
};

byte missil[8] = {
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
  lcd.createChar(3, agua);
  lcd.createChar(4, missil);
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

void setup()
{
    pinMode(UP, INPUT_PULLUP);
  	pinMode(RIGHT, INPUT_PULLUP);
  	pinMode(DOWN, INPUT_PULLUP);
  	pinMode(LEFT, INPUT_PULLUP);
  	pinMode(CRIVAR, INPUT_PULLUP);


    Jogador jogador = Jogador(0);

    // Como é feita a comunicação direta entre as placas, não existe esse objeto.
    // Jogador jogador2 = Jogador(1);

}

void loop()
{
    //PLAYER 1 JOGA
    //Printa que é a vez do minino 1
    do
    {
        
        btn = move();
        switch (btn)
        {
        case UP:
            if(plx > 0) plx--;
            break;
        
        case RIGHT:
            if(ply < 9) ply++;
            break;

        case DOWN:
            if(plx < 9) plx++;

        case LEFT:
            if(ply > 0) ply--;
            
        case CRIVAR:
            btn = 0;
        default:
            break;
        }
    } while (btn);
    //Função de atacar com as coordenadas pl1y(sendo a coluna) e pl1x(sendo a linha)
    //Função de cadastro... qualquer porra que precise de coordena
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

    } else{
        return -1;
    }
}