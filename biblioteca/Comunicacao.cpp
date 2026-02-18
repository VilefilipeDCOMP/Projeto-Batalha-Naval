#include "Comunicacao.h"

SoftwareSerial serialPlaca(PIN_RX, PIN_TX); // RX=7, TX=8

bool meuTurno = false;

// void conectarPlacas() {
//     serialPlaca.begin(9600);
//     serialPlaca.setTimeout(50);
    
//     printS("Aguardando conexao com a outra placa...");

//     String msg;
//     msg.reserve(64);
    
//     bool conectado = false;
//     bool jaEnvieiReady = false;
//     unsigned long ultimoEnvio = 0;
    
//     while (!conectado) {
//         // printS("Aguardando conexão...\n");
//         unsigned long agora = millis();

//         // O que envia
//         if (agora - ultimoEnvio >= 500) {
//             printS("[DEBUG] LOOP de ENVIO\n");
//             serialPlaca.println("READY");
//             ultimoEnvio = agora;
//             if (!jaEnvieiReady) {
//                 jaEnvieiReady = true;
//             }
//         }

//         // O que recebe
//         if (serialPlaca.available()) {
//             printS("[DEBUG] Loop de RECEBENDO\n");

//             msg = serialPlaca.readStringUntil('\n');
//             msg.trim();

//             Serial.println(msg);
            
//             if (msg == "READY") {
//                 printS("[DEBUG] READY");
//                 conectado = true;
//                 if (jaEnvieiReady) {
//                     meuTurno = true;
//                     printS("Conectado! Voce ataca primeiro.");
//                 } else {
//                     meuTurno = false;
//                     printS("Conectado! Adversario ataca primeiro.");
//                 }
//                 serialPlaca.println("READY");
//             }
//         }
//     }
    
//     delay(200);
    
//     while (serialPlaca.available()) {
//         serialPlaca.read();
//     }
    
//     printS("=== JOGO INICIADO ===");
// }

void conectarPlacas() {
    serialPlaca.begin(9600);
    
    printS("Aguardando conexao com a outra placa...");
    
    bool conectado = false;
    unsigned long ultimoEnvio = 0;
    
    while (!conectado) {
        // Primeiro: SEMPRE tenta ler o que chegou (prioridade para receber)
        printS("[DEBUG] RECEBENDO");
        if (serialPlaca.available()) {
            String msg = serialPlaca.readStringUntil('\n');
            msg.trim();

            Serial.println(msg);
            
            if (msg == "READY") {
                // Recebi READY do outro — respondo com ACK e conecto
                delay(50); // Pequeno delay para o outro parar de transmitir
                serialPlaca.println("ACK");
                conectado = true;
                meuTurno = false; // Quem recebe READY primeiro joga segundo
                printS("Conectado! Adversario ataca primeiro.");
            } 
            else if (msg == "ACK") {
                // Recebi ACK — o outro confirmou, estamos conectados
                conectado = true;
                meuTurno = true; // Quem enviou READY primeiro joga primeiro
                printS("Conectado! Voce ataca primeiro.");
            }
        }
        
        // Segundo: Envia READY periodicamente, mas com intervalo ALEATÓRIO
        // para evitar colisão constante
        printS("[DEBUG] ENVIANDO");
        unsigned long agora = millis();
        unsigned long intervalo = 800 + (millis() % 400); // 800-1200ms aleatório
        if (!conectado && (agora - ultimoEnvio >= intervalo)) {
            serialPlaca.println("READY");
            ultimoEnvio = agora;
        }
    }
    
    delay(200);
    
    // Limpa o buffer serial
    while (serialPlaca.available()) {
        serialPlaca.read();
    }
    
    printS("=== JOGO INICIADO ===");
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
