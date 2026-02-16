#define UP 2
#define RIGHT 3
#define DOWN 4
#define LEFT 5
#define CRIVAR 6
#define LED 13



void setup()
{
    pinMode(UP, INPUT_PULLUP);
  	pinMode(RIGHT, INPUT_PULLUP);
  	pinMode(DOWN, INPUT_PULLUP);
  	pinMode(LEFT, INPUT_PULLUP);
  	pinMode(CRIVAR, INPUT_PULLUP);
}

void loop()
{
    //PLAYER 1 JOGA

    //PLAYER 2 JOGA
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
}

