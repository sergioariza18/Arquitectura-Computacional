#include <Keypad.h>
#include <LiquidCrystal.h>
/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 31
 * LCD D5 pin to digital pin 32
 * LCD D6 pin to digital pin 33 
 * LCD D7 pin to digital pin 34
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

#define LED_RED 14
#define LED_GREEN 15
#define LED_BLUE 16

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad
char pin[4] ="0123";
char pinIngresado[4];

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
const int rs = 12, en = 11, d4 = 31, d5 = 32, d6 = 33, d7 = 34;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int contador = 0;
int numeroIntentos = 0;

void setup() 
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Digite la clave:");
  delay(2000);
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_BLUE,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
}

void loop() 
{
  char key = keypad.getKey();
  pinIngresado[contador] = key;
  if (key)
  {
    lcd.setCursor(contador++, 1);
    lcd.println("*_               ");
  }
  if(contador == 4)
  {
    contador = 0;
    if(comprobarPin()==1)
    {
      //Si es correcto el pin, led verde
      LED_RGB(0,255,0);
      mostrarEnDisplay("Clave correcta");
      delay(2000);
      LED_RGB(0,255,0);
      delay(700);
      LED_RGB(0,0,255);
      delay(500);
      LED_RGB(255,0,0);
      delay(500);
      LED_RGB(0,255,0);
      delay(500);
      LED_RGB(0,0,255);
      delay(500);
      LED_RGB(255,0,0);
      delay(500);
      LED_RGB(0,0,0);
      mostrarEnDisplay("                ");
   }
   else
   {
    //Si es incorrecto el pin, led azúl
    LED_RGB(0,0,255);
    mostrarEnDisplay("Clave incorrecta");
    delay(3000);  
    mostrarEnDisplay("                ");
    LED_RGB(0,0,0);
    numeroIntentos++;
   }
 }
  if(numeroIntentos == 3)
  {
      //Bloquar sistema por 5s
      LED_RGB(255,0,0);
      mostrarEnDisplay("SISTEMA BLOQUEADO");
      delay(5000);
      numeroIntentos = 0;
      mostrarEnDisplay("                ");
      LED_RGB(0,0,0);
  }
}

void mostrarEnDisplay(String texto)
{
  lcd.setCursor(0, 1);
  lcd.print(texto);
}

int comprobarPin()
{
  int cont = 0;
  for(int i = 0; i < 4; i++)
  {
     if(pinIngresado[i] == pin[i])    
        cont++;
  }
  if(cont == 4)
  {
    return 1;
  }
   return 0;
}

void LED_RGB(int R,int G,int B)
{
  analogWrite(LED_RED, R);
  analogWrite(LED_GREEN, G);
  analogWrite(LED_BLUE, B);
}
