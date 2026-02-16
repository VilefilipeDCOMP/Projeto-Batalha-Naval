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
void mostrarTabuleiro();