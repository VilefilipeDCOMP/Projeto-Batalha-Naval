// 2. Sistema de mapear os navios do jogador (O Cartógrafo)
// Esta pessoa cuida do "tabuleiro" e das regras do jogo em si. Ela não se importa de onde vem o tiro, apenas gerencia o que acontece quando um tiro cai no mapa.
//     O que vai programar: A matriz (array bidimensional, ex: int mapa[10][10]) que representa a grade do jogo. Ela precisa criar a lógica para saber onde tem água, onde tem navio intacto e onde tem navio destruído.
//     Principais desafios: Atualizar a matriz corretamente e verificar a condição de vitória ou derrota (ex: "Todos os navios do jogador 1 foram destruídos?").
//     Exemplos de funções que essa pessoa vai criar: iniciarMapaVazio(), registrarTiroRecebido(x, y), verificarVitoria().
#include <EEPROM.h>

struct NaviosCopia {//copiei mermo
    int tamanho; // n vou explicar pq vc n eh broco
    int vida;   //  n vou explicar pq vc n eh broco pt 2
    int linha;  /* linha e coluna definem a posicao do navio */
    int coluna; 
    char orientacao; // se o navio ta na horizontal ou na vertical
    bool vivo; // pra ver se ta vivo
};

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
}

void salvarJoguinho(){
    int endereco
}
