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