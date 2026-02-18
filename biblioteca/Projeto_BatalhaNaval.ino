#include <biblioteca/Recordes.h>
#include <biblioteca/Tela.h>
#include <biblioteca/Cadastro_mapear.h>
#include <biblioteca/Comunicacao.h>

#include <Wire.h>

#define UP 2
#define RIGHT 3
#define DOWN 4
#define LEFT 5
#define CRIVAR 6

int plx = 0, ply = 0;
int btn = 0;
int hitou;
bool fim = false;

void setup()
{
    Serial.begin(9600);

    pinMode(UP, INPUT_PULLUP);
    pinMode(RIGHT, INPUT_PULLUP);
    pinMode(DOWN, INPUT_PULLUP);
    pinMode(LEFT, INPUT_PULLUP);
    pinMode(CRIVAR, INPUT_PULLUP);

    inicializarTela();
    cenaTitulo();

    Jogador jogador = Jogador(0);

    // 1: Posicionar navios
    iniciarMapaVazio();
    CadastroCompletao();
    mostrarTabuleiro();

    // Conecta com a outra placa (handshake)
    conectarPlacas();
}


void loop()
{
    if (fim) {
        return;
    }

    if (meuTurno) {
        Serial.println("--- Seu turno! Escolha as coordenadas ---");
        lcd.clear();
        cenaXY(plx, ply); // mostra inicial

        receberCoord(); 

        enviarTiro(plx, ply);

        hitou = receberStatusDoTiro();

        if (hitou == 6) {
            cenaAcertou();
        } else if (hitou == 5) {
            cenaErrou();
        } else if (hitou == -1) {
            lcd.clear();
            lcd.setCursor(2, 0);
            lcd.print("VOCE  VENCEU!");
            lcd.setCursor(1, 1);
            lcd.print("PARABENS MORAL");
            Serial.println("=== VOCE VENCEU! ===");
            salvarNovaVitoria();
            fim = true;
            return;
        }

        meuTurno = false;

    } else {
        Serial.println("--- Turno do adversario. Aguardando tiro... ---");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Turno  Inimigo");
        lcd.setCursor(0, 1);
        lcd.print("Aguardando...");

        int tiroX, tiroY;
        if (receberTiroAdversario(tiroX, tiroY)) {
            int resultado = registrarTiro(tiroX, tiroY);

            lcd.clear();
            cenaXY(tiroX, tiroY);
            delay(500);

            enviarStatusDoTiro(resultado);

            if (resultado == 6) {
                cenaAcertou();

                if (todosNaviosAfundados()) {
                    lcd.clear();
                    lcd.setCursor(1, 0);
                    lcd.print("VOCE  PERDEU!");
                    lcd.setCursor(2, 1);
                    lcd.print("FIM DE JOGO");
                    Serial.println("=== VOCE PERDEU! ===");
                    fim = true;
                    return;
                }
            } else if (resultado == 5) {
                cenaErrou();
            }

            mostrarTabuleiro();
        }

        meuTurno = true;
    }
}

int move() {
    if (digitalRead(UP) == LOW) {
        return UP;
    } else if (digitalRead(RIGHT) == LOW) {
        return RIGHT;
    } else if (digitalRead(DOWN) == LOW) {
        return DOWN;
    } else if (digitalRead(LEFT) == LOW) {
        return LEFT;
    } else if (digitalRead(CRIVAR) == LOW) {
        return CRIVAR;
    }
    return 0;
}