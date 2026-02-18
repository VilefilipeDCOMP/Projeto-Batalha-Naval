#include "Comunicacao.h"
#include <Arduino.h>

SoftwareSerial serialPlaca(PIN_RX, PIN_TX); // RX=7, TX=8

bool meuTurno = false;

void conectarPlacas() {
    serialPlaca.begin(9600);
    
    Serial.println("Aguardando conexao com a outra placa...");
    
    bool conectado = false;
    bool jaEnvieiReady = false;
    unsigned long ultimoEnvio = 0;
    
    while (!conectado) {
        unsigned long agora = millis();
        if (agora - ultimoEnvio >= 500) {
            serialPlaca.println("READY");
            ultimoEnvio = agora;
            if (!jaEnvieiReady) {
                jaEnvieiReady = true;
            }
        }
        if (serialPlaca.available()) {
            String msg = serialPlaca.readStringUntil('\n');
            msg.trim();
            
            if (msg == "READY") {
                conectado = true;
                if (jaEnvieiReady) {
                    meuTurno = true;
                    Serial.println("Conectado! Voce ataca primeiro.");
                } else {
                    meuTurno = false;
                    Serial.println("Conectado! Adversario ataca primeiro.");
                }
                serialPlaca.println("READY");
            }
        }
    }
    
    delay(200);
    
    while (serialPlaca.available()) {
        serialPlaca.read();
    }
    
    Serial.println("=== JOGO INICIADO ===");
}

void enviarTiro(int x, int y) {
    String mensagem = "TIRO:" + String(x) + "," + String(y);
    serialPlaca.println(mensagem);
    
    Serial.print("Tiro enviado -> (");
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.println(")");
}

int receberStatusDoTiro() {
    Serial.println("Aguardando resposta do tiro...");
    
    unsigned long inicio = millis();
    while (!serialPlaca.available()) {
        if (millis() - inicio > 30000) {
            Serial.println("TIMEOUT! Sem resposta da outra placa.");
            return -1;
        }
    }
    
    String resposta = serialPlaca.readStringUntil('\n');
    resposta.trim();
    
    if (resposta == "HIT") {
        Serial.println("Status: ACERTOU!");
        return 6;
    } 
    else if (resposta == "MISS") {
        Serial.println("Status: ERROU!");
        return 5;
    } 
    else if (resposta == "WIN") {
        Serial.println("Status: VOCE VENCEU! Todos os navios inimigos foram afundados!");
        return -1;
    }
    
    Serial.print("Resposta desconhecida: ");
    Serial.println(resposta);
    return 0;
}

bool receberTiroAdversario(int &x, int &y) {
    Serial.println("Aguardando tiro do adversario...");

    unsigned long inicio = millis();
    while (!serialPlaca.available()) {
        if (millis() - inicio > 60000) {
            Serial.println("TIMEOUT! Adversario demorou demais.");
            return false;
        }
    }
    
    String msg = serialPlaca.readStringUntil('\n');
    msg.trim();
    
    if (msg.startsWith("TIRO:")) {
        String coords = msg.substring(5);
        
        int virgula = coords.indexOf(',');
        if (virgula > 0) {
            x = coords.substring(0, virgula).toInt();
            y = coords.substring(virgula + 1).toInt();
            
            Serial.print("Tiro recebido -> (");
            Serial.print(x);
            Serial.print(",");
            Serial.print(y);
            Serial.println(")");
            
            return true;
        }
    }
    
    Serial.print("Mensagem invalida recebida: ");
    Serial.println(msg);
    return false;
}

void enviarStatusDoTiro(int resultado) {
    if (resultado == 6) {
        if (todosNaviosAfundados()) {
            serialPlaca.println("WIN");
            Serial.println("Enviado: WIN - Todos os seus navios foram afundados!");
        } else {
            serialPlaca.println("HIT");
            Serial.println("Enviado: HIT");
        }
    } 
    else if (resultado == 5) {
        serialPlaca.println("MISS");
        Serial.println("Enviado: MISS");
    }
    else {
        serialPlaca.println("MISS");
        Serial.println("Enviado: MISS (tiro invalido/repetido)");
    }
}
