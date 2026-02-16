#include <Wire.h>
#include "rgb_lcd.h"
#include <Recordes.h>

#define UP 2
#define RIGHT 3
#define DOWN 4
#define LEFT 5
#define CRIVAR 6


void setup()
{
    pinMode(UP, INPUT_PULLUP);
  	pinMode(RIGHT, INPUT_PULLUP);
  	pinMode(DOWN, INPUT_PULLUP);
  	pinMode(LEFT, INPUT_PULLUP);
  	pinMode(CRIVAR, INPUT_PULLUP);

    int pl1X = 0, pl1Y = 0;
    int pl2X = 0, pl2Y = 0;
    int btn = 0;
    Jogador jogador1 = Jogador(0);

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
            if(pl1y < 9) pl1Y++;
            break;
        
        case RIGHT:
            if(pl1y < 9) pl
        default:
            break;
        }
    } while (1);
    

    //PLAYER 2 JOGA
    
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