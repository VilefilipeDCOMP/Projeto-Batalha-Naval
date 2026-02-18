#ifndef RECORDES_H
#define RECORDES_H

class Jogador {
    public:
        int id{-1};
        int vitorias{0};
        Jogador(int num);
};

int lerRecordes();
int salvarNovaVitoria();
int zerarRecordes();

#endif