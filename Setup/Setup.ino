
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// struct
struct Navios {
    int tamanho;
    bool vivo;
    int linha;
    int coluna;
    char orientacao;
};


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


void setup() {
    Serial.begin(9600);
    iniciarMapaVazio();
    CadastroCompletao();
    // funcao de sidnei
    registrarTiro(5, 5); // OS parametros vao vir da função de sidnei e isso vai pro loop dps
    
    //mostrarTabuleiro(); 
}

void loop() {
    // A lógica de turnos virá aqui depois
}