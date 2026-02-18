#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Endereço I2C: No Tinkercad geralmente é 32 (0x20) ou 39 (0x27).
// Tente 32 primeiro. Se não funcionar, mude para 0x27.
// LiquidCrystal_I2C lcd(0x20, 16, 2); 

byte explosao_tela[8] = {
  B10001,
  B01010,
  B00100, // Centro
  B11111, // Explosão
  B00100,
  B01010,
  B10001,
  B00000
};

byte agua_tela[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10101 // Ondinhas
};

byte missil_tela[8] = {
  B00000,
  B00000,
  B00110, // Corpo do missil
  B11111,
  B00110,
  B00000,
  B00000,
  B00000
};


void inicializarTela() {
  // Inicializa o LCD
  lcd.init(); 

  // Liga a luz de fundo
  lcd.backlight();
  
  // Registrar os desenhos na memória do LCD (Slots 0 a 4)
  lcd.createChar(2, explosao_tela);
  lcd.createChar(3, agua_tela);
  lcd.createChar(4, missil_tela);
}

// ---------------------------------------------
// Catalogo de Cenas

void cenaTitulo() {
    lcd.clear();
    
    lcd.setCursor(1, 0);
    lcd.print("BATALHA  NAVAL");
    
    // Desenha o mar
    for(int i=0; i<16; i++) {
        lcd.setCursor(i, 1);
        lcd.write(3); // Caractere da agua
    }
    
    cenaAtaque();
    
    delay(500);
    lcd.clear();
}

void cenaAtaque() {
  // Anima o míssil indo da esquerda para a direita
  for(int i=0; i<17; i++) {
    // Apaga o rastro do missil (recoloca agua)
    if(i > 0) {
      lcd.setCursor(i-1, 1);
      lcd.write(3); 
    }
    
    // Desenha o missil
    lcd.setCursor(i, 1);
    lcd.write(4);
    
    delay(150); // Velocidade do tiro
  }
}

void cenaPontos() {
  lcd.clear();
  
  lcd.setCursor(1, 0);
  lcd.print("Frota  Aliada");
  lcd.setCursor(7, 1);
  lcd.print(lerRecordes(0));
  
  delay(3500);
    
  lcd.clear();
  
  lcd.setCursor(1, 0);
  lcd.print("Frota  Inimiga");
  lcd.setCursor(7, 1);
  lcd.print(lerRecordes(1));

  delay(3500);

  lcd.clear();
}

void cenaXY(int linha, int coluna) {
  lcd.setCursor(1, 0);
  lcd.print("Coord. do Tiro");
  
  int posInicial{6};
  
  lcd.setCursor(posInicial, 1);
  lcd.print("(");
  
  lcd.setCursor(posInicial + 1, 1);
  lcd.print(linha);
  
  lcd.setCursor(posInicial + 2, 1);
  lcd.print(",");
  
  lcd.setCursor(posInicial + 3, 1);
  lcd.print(coluna);
  
  lcd.setCursor(posInicial + 4, 1);
  lcd.print(")");
}

void cenaAcertou() {
  lcd.clear();
  
  // Anima o míssil indo da esquerda para a direita
  for(int i=0; i<12; i++) {
    // Apaga o rastro do missil (recoloca agua)
    if(i > 0) {
      lcd.setCursor(i-1, 1);
      lcd.write(3); 
    }
    
    // Desenha o missil
    lcd.setCursor(i, 1);
    lcd.write(4);
    
    delay(150); // Velocidade do tiro
  }
  
  // O míssil atingiu (posição 11)
  
  // Efeito piscando (BOOM)
  for(int k=0; k<3; k++) {
    lcd.setCursor(11, 1);
    lcd.write(2); // Desenha explosão
    lcd.setCursor(12, 1);
    lcd.write(2); // Desenha explosão
    
    lcd.setCursor(4, 0);
    lcd.print("   KABOOM!!   ");
    
    delay(100);
    lcd.noBacklight(); // Apaga a luz da tela
    delay(100);
    lcd.backlight();   // Acende a luz da tela
  }
  
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("ALVO  ABATIDO");
  delay(1000);
  lcd.clear();
}

void cenaErrou() {
  lcd.clear();
  
  // Anima o míssil indo da esquerda para a direita
  for(int i=0; i<17; i++) {
    // Apaga o rastro do missil (recoloca agua)
    if(i > 0) {
      lcd.setCursor(i-1, 1);
      lcd.write(3); 
    }
    
    // Desenha o missil
    lcd.setCursor(i, 1);
    lcd.write(4);
    
    delay(150); // Velocidade do tiro
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ERROU MEU CHEFE!");
  delay(1000);
  lcd.clear();
}

// ---------------------------------------------

// void loop()
// {
//   cenaTitulo();
//   cenaPontos();
// }
