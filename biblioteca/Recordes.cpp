#include <EEPROM.h>
#include "Recordes.h"

Jogador::Jogador(int num) {
    id = num;
    vitorias = lerRecordes(num);
}

int lerRecordes(int jogador_id) {
    int valor_lido{0};
    int enderenco_inicial{sizeof(int) * jogador_id};
    
    EEPROM.get(enderenco_inicial, valor_lido);
    
    Serial.println(valor_lido);

    return valor_lido;
}

int salvarNovaVitoria(int jogador_id) {
    // vitorias_jogador = lerRecordes(jogador); 
    // EEPROM[jogador_id] += 1;

    int valor_atual{lerRecordes(jogador_id)};
    int enderenco_inicial{sizeof(int) * jogador_id};

    EEPROM.put(enderenco_inicial, valor_atual + 1);
}

int zerarRecordes() {
    for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
    }
}