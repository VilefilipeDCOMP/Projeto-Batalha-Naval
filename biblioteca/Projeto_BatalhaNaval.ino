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
    //PLAYER 1 JOGA
    //Printa que é a vez do minino 1
    do
    {
        
        btn = move();
        switch (btn)
        {
        case UP:
            if(plx > 0) plx--;
            break;
        
        case RIGHT:
            if(ply < 9) ply++;
            break;

        case DOWN:
            if(plx < 9) plx++;

        case LEFT:
            if(ply > 0) ply--;
            
        case CRIVAR:
            btn = 0;
        default:
            break;
        }
    } while (btn);
    //Função de atacar com as coordenadas pl1y(sendo a coluna) e pl1x(sendo a linha)
    //Função de cadastro... qualquer porra que precise de coordena
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

    } else{
        return -1;
    }
}