//int tabuleiro[10][10];
const int navio = 1;
const int hagata = 0;

void iniciarMapaVazio(){
    for(int i = 0;i < 10; i++){
        for(int j = 0;j < 10; j++){
            tabuleiro[i][j] = hagata;
        }
    }
    lcd.print("Mapa Pronto!");
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
