#include "Cadastro.h"

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
int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]; // tabuleiro de teste pra validar codigo

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
            printS("Navio ");
            Serial.print(i);
            printS(" -> Tamanho = ");
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

void mostrarTabuleiro() {

    Serial.println("   0 1 2 3 4 5 6 7 8 9");

    for (int i = 0; i < TAM; i++) {

        Serial.print(i);
        Serial.print("  ");

        for (int j = 0; j < TAM; j++) {

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

void mostrarTabuleiro() {

    Serial.println("   0 1 2 3 4 5 6 7 8 9");

    for (int i = 0; i < TAM; i++) {

        Serial.print(i);
        Serial.print("  ");

        for (int j = 0; j < TAM; j++) {

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

