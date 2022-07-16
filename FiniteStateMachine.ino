   #include "AsyncTaskLib.h"
  #include <LiquidCrystal.h>
  // initialize the library with the numbers of the interface pins
  const int rs = 12, en = 11, d4 = 31, d5 = 32, d6 = 33, d7 = 34;
  LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
  
   //Temp
    #define analogPin A0 //the thermistor attach to
    #define beta 4090 //the beta of the thermistor
    #define resistance 10 //the value of the pull-down resistor
    
   //Definimos los pines  de los LED 
    #define LED_RED 14
    #define LED_GREEN 15
    #define LED_BLUE 16

    //Buzzer
    const int buzzerPin = 8;
    int frecuencia;

   // Variables de temperatura
  long a =1023 - analogRead(analogPin);
  float tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;

  //Tarea Async
  float read_temperature();
  AsyncTask asyncTaskTemp(2000,true,read_temperature);



  
enum State
{
  PosicionA,
  PosicionB,
  PosicionC,
  PosicionD
};



  // Variables globales
  State currentState;
  float currentInput;

// Acciones de los estados y condiciones de transiciones
void stateA()
{
  if (currentInput >18)
    changeState(State::PosicionB);
}

void stateB(){
  if (currentInput < 18)
    changeState(State::PosicionA);
  if (currentInput > 25)
    changeState(State::PosicionC);
}

void stateC()
{
  if (currentInput < 25)
    changeState(State::PosicionB);
  if (currentInput > 40)
    changeState(State::PosicionD);
}

void stateD()
{
  Alarma();
  changeState(State::PosicionB);
}

// Salidas asociadas a las transiciones
void outputA()
{
  lcd.print("ESTADO A");
  LED_RGB(0,0,1);
}

void outputB()
{
  lcd.print("ESTADO B");
  LED_RGB(0,1,0);
}

void outputC()
{
  lcd.print("ESTADO C");
  LED_RGB(1,0,0);
}

void outputD()
{
     lcd.print("ESTADO D");
     LED_RGB(1,0,0);
     LED_RGB(0,0,1);
}

void setup()
{
  pinMode(buzzerPin,OUTPUT);
  asyncTaskTemp.Start();
  Serial.begin(9600);
  currentState = PosicionA;
  outputA();
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() 
{
  readInput(asyncTaskTemp.Update());
  updateStateMachine();
  
  lcd.print("Temp: ");
  // Print a centigrade temperature to the LCD.
  lcd.print(currentInput);
  // Print the unit of the centigrade temperature to the LCD.
   lcd.print(" C");
   lcd.clear();
}

// Actualiza el estado de la maquina
void updateStateMachine()
{
  switch (currentState)
  {
    case PosicionA: stateA(); break;
    case PosicionB: stateB(); break;
    case PosicionC: stateC(); break;
    case PosicionD: stateD(); break;
  }
}

// Lee la entrada por puerto serie
void readInput(float Temp)
{
  currentInput = Temp;
}

// Funcion que cambia el estado y dispara las transiciones
void changeState(int newState)
{
  currentState = newState;

  switch (currentState)
  {
    case State::PosicionA: outputA();   break;
    case State::PosicionB: outputB();   break;
    case State::PosicionC: outputC();   break;
    case State::PosicionD: outputD();   break;
    default: break;
  }
}

float read_temperature()
{
  //read thermistor value
  long a =1023.0 - analogRead(analogPin);
  //the calculating formula of temperature
  float tempC = beta /(log((1025.0 * 10.0 / a - 10.0) / 10.0) + beta / 298.0) - 273.0;
  return tempC;
}


void LED_RGB(int R,int G,int B)
{
  analogWrite(LED_RED, R);
  analogWrite(LED_GREEN, G);
  analogWrite(LED_BLUE, B);
}

void Alarma()
{
  for(int i = 200;i <= 800;i++) 
{
tone(3,i); 
delay(5); 
}
delay(5000); 
}
