#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <EEPROM.h>
#include <AsyncTaskLib.h>
#include "Integrado.h"
#include "Alarma.h"

enum State
{
  PosicionA,
  PosicionB,
  PosicionC,
  PosicionD
};

// Variables globales
State currentState;
String currentInput;
int bandMenu = 0;

// Acciones de los estados y condiciones de transiciones
void stateA()
{
  /*lcd.clear();
  lcd.println("ESTADO A        ");
  delay(1500);*/
  if (comprobarPass() == true)
  {
     changeState(State::PosicionB);
  }  
}
void stateB()
{
  lcd.clear();
  lcd.println("ESTADO B        ");
  delay(2000);
  if(upMenu() == 1)
  {
    changeState(State::PosicionC);
  }
}
void stateC()
{
  lcd.clear();
  lcd.println("ESTADO C        ");
  delay(2000);
  if(TareasAsincronas() == 1)
  {
    changeState(State::PosicionD);
  }
}
void stateD()
{
 Alarm();
 delay(5000);
 changeState(State::PosicionB);
}

// Salidas asociadas a las transiciones
void outputA()
{
}
void outputB()
{
}
void outputC()
{
 lcd.clear();
}
void outputD()
{
 lcd.clear();
 lcd.println("TEMP MUY ALTA       ");
}
/*F***********************************************
* NAME: setup
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Inicializa el programa
*
*-------------------------------------------------
*NOTE:
*
**************************************************/
void setup()
{
  asyncTaskTemp.Start();
  asyncTaskLuz.Start();
  asyncTaskMicro.Start(); 
  mostrarMenu();
  mostrarSeguridad();
  currentState = PosicionA;
  outputA();
}
/*F***********************************************
* NAME: loop
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Ejecuta y actualiza el programa
*
*-------------------------------------------------
*NOTE:
*
**************************************************/
void loop()
{
  updateStateMachine();
}
/*F***********************************************
* NAME: UpdateStateMachine
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Actualiza el estado de la maquina
*
*-------------------------------------------------
*NOTE:
*
**************************************************/

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
