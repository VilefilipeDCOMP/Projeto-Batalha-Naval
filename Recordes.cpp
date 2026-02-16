#include <EEPROM.h>

class Jogador {
    public:
        int id{-1};
        int vitorias{0};
        int tabuleiro[10][10];
        Jogador(int num) {
            id = num;
        }
};

// Id player, Vitoria, Tabuleiro

int lerRecordes(int jogador_id) {
    // falta devolver a quantidade de vitórias do jogador em si 

    int valor_lido{0};
    int enderenco_inicial{0 + jogador_id};
    
    Serial.print("Read int from EEPROM: ");
    
    EEPROM.get(enderenco_inicial, valor_lido);
    
    Serial.println(valor_lido);

    return valor_lido;
}

int salvarNovaVitoria(int jogador_id) {
    // vitorias_jogador = lerRecordes(jogador); 
    EEPROM[jogador_id] += 1;
}

int zerarRecordes() {
    for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
    }
}

// Player 1 - 4
// Player 2 - 0 

