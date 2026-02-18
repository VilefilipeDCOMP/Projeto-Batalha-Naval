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