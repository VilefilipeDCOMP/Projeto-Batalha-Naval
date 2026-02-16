#include <biblioteca/Recordes.h>
#include <biblioteca/Tela.h>
#include <biblioteca/Cadastro.h>
#include <biblioteca/MapearNaviosDada.h>
#include <biblioteca/Cadastro.h>

#include <Wire.h>
#include "rgb_lcd.h"

#define UP 2
#define RIGHT 3
#define DOWN 4
#define LEFT 5
#define CRIVAR 6

int plx = 0, ply = 0;
int btn = 0;
int hitou;
bool fim;

void setup()
{
    pinMode(UP, INPUT_PULLUP);
  	pinMode(RIGHT, INPUT_PULLUP);
  	pinMode(DOWN, INPUT_PULLUP);
  	pinMode(LEFT, INPUT_PULLUP);
  	pinMode(CRIVAR, INPUT_PULLUP);


    Jogador jogador = Jogador(0);

    // Como é feita a comunicação direta entre as placas, não existe esse objeto.
    // Jogador jogador2 = Jogador(1);

}

void loop()
{
    //PLAYER JOGA
    hitou = registrarTiro(/*infoX,*/ /*infoY*/); //SE TOMOU RETORNA 6, SE ERROU RETORNA 5
    //As variaveis infox e infoy são recebidas pela placa através do serial(ou algo do tipo)
    //SIDNEI RESOLVE ISSO e PRECISA decidir como dizer quem começa atirando, para não começar registrando um tiro que não foi dado como acontece duas linhas acima
    //SIDNEI usa essa merda desse hitou pra informar a outra placa que o tiro dela pegou em mim
    //cena de acerto ou cena de erro

    //Verifica se alguem ganhou
    if(registrarTiro(infoX, infoY) == 6){
        fim = todosNaviosAfundados();
    };

    if(fim){
        //cena de vitoria 
        while(1);
    }
    
    {
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
        cenaXY(plx, ply);
    } while (btn);
    //Dá o tiro com as coordenadas (plx, ply) e manda essas coordenadas via serial para a outra placa verificar se mamou
    //Estado atual vira espectador
}

int move(){
    if(digitalRead(UP) == LOW){
        return UP;
        
    } else if(digitalRead(RIGHT) == LOW){
        return RIGHT;

    } else if(digitalRead(DOWN) == LOW){
        return DOWN;

    } else if(digitalRead(LEFT) == LOW){
        return LEFT;

    } else if(digitalRead(CRIVAR) == LOW){
        return CRIVAR;

    }
}