#include "Comunicacao.h"

SoftwareSerial serialPlaca(PIN_RX, PIN_TX); // RX=7, TX=8

bool meuTurno = false;

void conectarPlacas() {
    serialPlaca.begin(4800);   // 4800 baud: mais confiável no SoftwareSerial do Tinkercad
    serialPlaca.listen();       // Garante que esta porta SoftwareSerial está ativa

    Serial.println(F("Aguardando conexao com a outra placa..."));

    bool conectado = false;
    unsigned long ultimoEnvio = 0;
    unsigned int semente = analogRead(A5);  // pino flutuante para aleatoriedade

    const bool isMaster = (PLAYER_ID == 1);

    // PROTOCOLO SIMPLIFICADO:
    //   'R' = READY    'A' = ACK
    // Apenas 1 byte cada, lido byte a byte — sem readStringUntil.

    while (!conectado) {
        // --- RECEPÇÃO (byte a byte, sem bloqueio, sem prints) ---
        if (serialPlaca.available()) {
            char c = serialPlaca.read();

            if (c == 'R') {
                delay(100);
                serialPlaca.write('A');
                delay(50);
                conectado = true;
                meuTurno = false;
            }
            else if (c == 'A') {
                conectado = true;
                meuTurno = true;
            }
            // qualquer outro byte é ignorado silenciosamente
        }

        // --- ENVIO periódico com intervalo aleatório ---
        unsigned long agora = millis();
        unsigned long intervalo = 1000 + (semente % 500);
        if (!conectado && (agora - ultimoEnvio >= intervalo)) {
            serialPlaca.write('R');
            ultimoEnvio = agora;
            delay(200);  // janela de escuta obrigatória após envio
        }
    }

    delay(300);

    // Limpa lixo remanescente
    while (serialPlaca.available()) {
        serialPlaca.read();
    }

    // Prints só DEPOIS de sair do loop
    if (meuTurno) {
        Serial.println(F("Conectado! Voce ataca primeiro."));
    } else {
        Serial.println(F("Conectado! Adversario ataca primeiro."));
    }
    Serial.println(F("=== JOGO INICIADO ==="));
}


void enviarTiro(int x, int y) {
    String mensagem = "TIRO:" + String(x) + "," + String(y);
    serialPlaca.println(mensagem);
    
    Serial.print("Tiro enviado -> (");
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    printS(")");
}

int receberStatusDoTiro() {
    printS("Aguardando resposta do tiro...");
    
    unsigned long inicio = millis();
    while (!serialPlaca.available()) {
        if (millis() - inicio > 30000) {
            printS("TIMEOUT! Sem resposta da outra placa.");
            return -1;
        }
    }
    
    String resposta = serialPlaca.readStringUntil('\n');
    resposta.trim();
    
    if (resposta == "HIT") {
        printS("Status: ACERTOU!");
        return 6;
    } 
    else if (resposta == "MISS") {
        printS("Status: ERROU!");
        return 5;
    } 
    else if (resposta == "WIN") {
        printS("Status: VOCE VENCEU! Todos os navios inimigos foram afundados!");
        return -1;
    }
    
    Serial.print("Resposta desconhecida: ");
    Serial.print(resposta);
    return 0;
}

bool receberTiroAdversario(int &x, int &y) {
    printS("Aguardando tiro do adversario...");

    unsigned long inicio = millis();
    while (!serialPlaca.available()) {
        if (millis() - inicio > 60000) {
            printS("TIMEOUT! Adversario demorou demais.");
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
            printS(")");
            
            return true;
        }
    }
    
    Serial.print("Mensagem invalida recebida: ");
    Serial.print(msg);
    return false;
}

void enviarStatusDoTiro(int resultado) {
    if (resultado == 6) {
        if (todosNaviosAfundados()) {
            serialPlaca.println("WIN");
            printS("Enviado: WIN - Todos os seus navios foram afundados!");
        } else {
            serialPlaca.println("HIT");
            printS("Enviado: HIT");
        }
    } 
    else if (resultado == 5) {
        serialPlaca.println("MISS");
        printS("Enviado: MISS");
    }
    else {
        serialPlaca.println("MISS");
        printS("Enviado: MISS (tiro invalido/repetido)");
    }
}
