#include <biblioteca/Recordes.h>
#include <biblioteca/Tela.h>
#include <biblioteca/Cadastro_mapear.h>
#include <biblioteca/Comunicacao.h>

#include <Wire.h>

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

    iniciarJogo();
}

void loop(){
    if (fim) {
        cenaPontos();

        btn = move();

        if (btn == CRAVAR) {
            iniciarJogo();
        }
        
        // return;
    }

    if (meuTurno) {
        Serial.println("--- Seu turno! Escolha as coordenadas ---");
        lcd.clear();

        receberCoord();

        enviarTiro(plx, ply);

        hitou = receberStatusDoTiro();

        if (hitou == 6) {
            cenaAcertou();
        } else if (hitou == 5) {
            cenaErrou();
        } else if (hitou == -1) {
            cenaAcertou();
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

    //Dá o tiro com as coordenadas (plx, ply) e manda essas coordenadas via serial para a outra placa verificar se mamou
    //Estado atual vira espectador
    }
}

void iniciarJogo(){
    hitou = 0;
    fim = false;

    // cenaTitulo();

    Jogador jogador = Jogador(0);

    // 1: Posicionar navios
    iniciarMapaVazio();
    CadastroCompletao();

    // Mostrar na tela "Vez do jogador dois e repetir o processo de cadastro"
    
    // // funcao de sidnei
    // registrarTiro(5, 5); // OS parametros vao vir da função de sidnei e isso vai pro loop dps

    mostrarTabuleiro();

    // // Conecta com a outra placa (handshake)
    conectarPlacas();

    lcd.clear();
    lcd.print(" JOGO  INICIADO ");

    delay(500);
}

void receberCoord() {
    plx = 0; ply = 0;
  	btn = 0;

    //  Coord.
    //      (0,0)
    
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Coord.");

    lcd.setCursor(6, 1);   
    lcd.print("(");           
    lcd.print(0);
    lcd.print(",");
    lcd.print(0);
    lcd.print(")");
  
    do {
        btn = move();
        switch (btn){
            case UP:
                if(plx > 0) plx--;
                break;
            
            case RIGHT:
                if(ply < 9) ply++;
                break;

            case DOWN:
                if(plx < 9) plx++;
                break;

            case LEFT:
                if(ply > 0) ply--;
                break;

            case CRIVAR:
                btn = 0;
                break;

            default:
                break;
            }

        lcd.setCursor(7, 1);
        lcd.print(plx);
        
        lcd.setCursor(9, 1);
        lcd.print(ply);
      
      	delay(25);
    } while (btn);
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
     //return 0; Algum imbecil botou esse return 0 aqui animal estupido (pode ter sido eu)
}