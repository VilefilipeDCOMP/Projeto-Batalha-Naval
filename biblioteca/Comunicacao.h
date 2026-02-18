#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include <SoftwareSerial.h>

// TX da placa conecta no RX da outra
#define PIN_RX 7
#define PIN_TX 8

// principais
void conectarPlacas();
void enviarTiro(int x, int y);
int  receberStatusDoTiro();

// adversário
bool receberTiroAdversario(int &x, int &y);
void enviarStatusDoTiro(int resultado);

extern bool meuTurno;
extern SoftwareSerial serialPlaca;

#endif