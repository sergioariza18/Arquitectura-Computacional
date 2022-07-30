#include "Comunes.h"

/*  SEGURIDAD  */
char pin[4] ="0123";
char pinIngresado[4];
int contador = 0;
int numeroIntentos = 0;
/*F***********************************************
* NAME: mostrarSeguridad
*-------------------------------------------------
*PARAMS: none
*return: none
*-------------------------------------------------
*PURPOSE: Mostrar a traves del LCD  el mensaje
*inicial del sistema de seguridad
*e inicializar las variables del led RGB
*-------------------------------------------------
*NOTE:
*
**************************************************/
void mostrarSeguridad()
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Digite la clave:");
  delay(2000);
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_BLUE,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
}
/*F***********************************************
* NAME: mostrarEnDisplay
*-------------------------------------------------
*PARAMS: String texto
*return: none
*-------------------------------------------------
*PURPOSE: Mostrar mensajes en el LCD
*-------------------------------------------------
*NOTE:
*
**************************************************/
void mostrarEnDisplay(String texto)
{
  lcd.setCursor(0, 1);
  lcd.print(texto);
}
/*F***********************************************
* NAME: ComprobarPin
*-------------------------------------------------
*PARAMS:
*return: int
*-------------------------------------------------
*PURPOSE: Verificar que la constraseña ingresada
*sea de maximo 4 caracteres
*-------------------------------------------------
*NOTE:
*
**************************************************/
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
/*F***********************************************
* NAME: RGB
*-------------------------------------------------
*PARAMS: int R, int G, int B
*return: none
*-------------------------------------------------
*PURPOSE: Encender el led RGB
*-------------------------------------------------
*NOTE:
*
**************************************************/
void RGB(int R,int G,int B)
{
  analogWrite(LED_RED, R);
  analogWrite(LED_GREEN, G);
  analogWrite(LED_BLUE, B);
}
/*F***********************************************
* NAME: comprobarPass
*-------------------------------------------------
*PARAMS:
*return: bool
*-------------------------------------------------
*PURPOSE: Verifica que el pin ingresado sea correcto
*-------------------------------------------------
*NOTE:
**************************************************/
bool comprobarPass(){
  char key = keypad.getKey();
  bool band = false;
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
      RGB(0,255,0);
      mostrarEnDisplay("Clave correcta");
      delay(2000);
      RGB(0,255,0);
      delay(700);
      RGB(0,0,255);
      delay(500);
      RGB(255,0,0);
      delay(500);
      RGB(0,255,0);
      delay(500);
      RGB(0,0,255);
      delay(500);
      RGB(255,0,0);
      delay(500);
      RGB(0,0,0);
      mostrarEnDisplay("                ");
      band = true;
   }
   else
   {
    //Si es incorrecto el pin, led azúl
    RGB(0,0,255);
    mostrarEnDisplay("Clave incorrecta");
    delay(3000);  
    mostrarEnDisplay("                ");
    RGB(0,0,0);
    numeroIntentos++;
   }
 }
  if(numeroIntentos == 3)
  {
      //Bloquear sistema por 5s
      RGB(255,0,0);
      mostrarEnDisplay("SISTEMA BLOQUEADO");
      delay(5000);
      numeroIntentos = 0;
      mostrarEnDisplay("                ");
      RGB(0,0,0);
  }
  return band;
}
/*  MENÚ  */
//fotosensor
int lightValue = 0;
int luz;
//temperatura
float temp_value = 0.0;
byte temp_high;
byte temp_low;
//Microfono
byte sonido;
//Encoder
int encoder(int encoderValue, int rango);
//Opciones
int opcion;
String arrayMenu[] = {"Umbral TempHIGH", "Umbral TempLOW", "Umbral Light", "Umbral Sound", "Reset", "Guardar"};
int bandera = 0;
int valor=-1;
//charPersonalizaado
byte caracter[8] = 
{
  B10000,
  B11000,
  B11100,
  B11110,
  B11110,
  B11100,
  B11000,
  B10000
};
//Variables para EEPROM
const int dir_temp_high=0;
const int dir_temp_low=1;
const int dir_luz = 2;
const int dir_sonido = 6;
//Tareas asíncronas
AsyncTask asyncTaskEncoder(500, true, encoder);
//Funciones para EEPROM
/*F***********************************************
* NAME: cargarValores
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Carga los valores de los sensores 
*en la EEPROM
*-------------------------------------------------
*NOTE:
**************************************************/
void cargarValores()
{
  temp_high = EEPROM.read(dir_temp_high);
  temp_low = EEPROM.read(dir_temp_low);
  luz = EEPROM.get(dir_luz, luz);
  sonido = EEPROM.read(dir_sonido);
}
/*F***********************************************
* NAME: reiniciarValores
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Reinicia a los valores por defecto de los
*sensores en la EEPROM
*-------------------------------------------------
*NOTE:
**************************************************/
void reiniciarValores()
{
  temp_high=25;
  EEPROM.write(dir_temp_high,temp_high);
  temp_low=18;
  EEPROM.write(dir_temp_low,temp_low);
  luz = 300;
  EEPROM.put(dir_luz,luz);
  sonido = 30;
  EEPROM.write(dir_sonido,sonido);
}
/*F***********************************************
* NAME: mostrar_caracter
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Muestra en el LCD el caracter personalizado
*-------------------------------------------------
*NOTE:
*
**************************************************/
void mostrar_caracter(int pos) 
{
  lcd.createChar(1, caracter);
  lcd.setCursor(0, pos);
  lcd.write((byte)1);
}
/*F***********************************************
* NAME: btn_encoder
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Detecta si el boton del encoder ha sido
*pulsado
*-------------------------------------------------
*NOTE:
**************************************************/
void btn_encoder() {
  if (digitalRead(sw) == LOW) 
  {
    if (bandera == 0) 
    {
      bandera = 1;
    } 
    else 
    {
      bandera = 0;
    }
  }
}
/*F***********************************************
* NAME: clear_up
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: limpia el nivel superior de la pantalla
*-------------------------------------------------
*NOTE:
*
**************************************************/
void clear_up() 
{
  lcd.setCursor(0, 0);
  lcd.print("                 ");
}
/*F***********************************************
* NAME: clear_down
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Limpia la pantalla en el nivel inferior
*-------------------------------------------------
*NOTE:
*
**************************************************/
void clear_down() 
{
  lcd.setCursor(0, 1);
  lcd.print("                 ");
}
/*F***********************************************
* NAME: temp_High
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Permite establecer el valor maximo 
*para la temperatura y escribirlo en la EEPROM
*-------------------------------------------------
*NOTE:
*
**************************************************/
void temp_High()
{
        valor = temp_high;
        valor = encoder(valor,0);
        lcd.setCursor(12, 0);
        lcd.print( valor );
        lcd.setCursor(14, 1);
        lcd.print(" C");
        temp_high = valor;
        EEPROM.write(dir_temp_high, temp_high );
}
/*F***********************************************
* NAME: temp_Low
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Permite establecer el valor minimo 
*para la temperatura y escribirlo en la EEPROM
*-------------------------------------------------
*NOTE:
*
**************************************************/
void temp_Low()
{
        lcd.setCursor(0, 0);
        mostrar_caracter(0);
        lcd.print(" Temp LOW ");
        valor = temp_low;
        valor = encoder(valor,1);
        lcd.setCursor(12, 0);
        lcd.print( valor );
        lcd.setCursor(14, 0);
        lcd.print(" C");
        temp_low = valor;
        EEPROM.write(dir_temp_low, temp_low);
}
/*F***********************************************
* NAME: Luz
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Permite establecer el valor de umbral 
*para la luz y escribirlo en la EEPROM
*-------------------------------------------------
*NOTE:
**************************************************/
void Luz()
{
        valor = luz;
        valor = encoder(valor,2);
        lcd.setCursor(6, 0);
        lcd.print( valor );
        lcd.setCursor(9, 0);
        lcd.print(" lm");
        luz = valor;
        EEPROM.put(dir_luz, luz);
}
/*F***********************************************
* NAME: Luz
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Permite establecer el valor de umbral 
*para el sonido y escribirlo en la EEPROM
*-------------------------------------------------
*NOTE:
**************************************************/
void Sonido()
{
        valor = sonido;
        valor = encoder(valor,3);
        lcd.setCursor(9, 0);
        lcd.print( valor );
        lcd.setCursor(11, 0);
        lcd.print(" dB");
        sonido = valor;
        EEPROM.write(dir_sonido, sonido);
}
/*F***********************************************
* NAME: getEncoderTurn
*-------------------------------------------------
*PARAMS:
*return: int
*-------------------------------------------------
*PURPOSE: Retorna el valor obtenido al rotar la
*perilla del encoder
*-------------------------------------------------
*NOTE:
*
**************************************************/
int getEncoderTurn(void)
{
  static int oldA = HIGH;
  static int oldB = HIGH;
  int result = 0;
  int newA = digitalRead(clk);
  int newB = digitalRead(dt);
  if (newA != oldA || newB != oldB)
  {
    // something has changed
    if (oldA == HIGH && newA == LOW)
    {
      result = (oldB * 2 - 1);
    }
  }
  oldA = newA;
  oldB = newB;
  return result;
}
/*F***********************************************
* NAME: encoder
*-------------------------------------------------
*PARAMS:
*return: int
*-------------------------------------------------
*PURPOSE: Limita el rango minimo maixmo en menu
*y para cada una de sus opciones
*-------------------------------------------------
*NOTE:
**************************************************/
int encoder(int encoderValue, int rango) 
{
  int change = getEncoderTurn();
  encoderValue = encoderValue + change;
  if (rango == -1) 
  {
    if (encoderValue >= 9) 
    {
      encoderValue = 8;
    }
    else if (encoderValue <= -1)
    {
      encoderValue = 0;
    }
   
  }
  else if(rango == 0)
  { // TEMP HIGH
    if (encoderValue >= 51)
    {
      encoderValue = 50;
    }
    else if (encoderValue <= 24) 
    {
        encoderValue = 25;      
    }
  }
  else if(rango == 1)
  { // TEMP LOW
    if (encoderValue >= 19) 
    {
      encoderValue = 18;
    }
    else if (encoderValue <= -1) 
    {
      encoderValue = 0;
    }
  }
  else if(rango == 2)
  { // LUZ
    if (encoderValue >= 301)
    {
      encoderValue = 300;
    } 
    else if (encoderValue <= -1) 
    {
      encoderValue = 0;
    }
  }
  else if(rango == 3)
  { // SONIDO
    if (encoderValue >= 31)
    {
      encoderValue = 30;
    } 
    else if (encoderValue <= -1) 
    {
      encoderValue = 0;
    }
  }
  return encoderValue;
}
/*F***********************************************
* NAME: pantalla0
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Muestra en el LCD la opcion 0 y 1 del 
*menu
*-------------------------------------------------
*NOTE:
**************************************************/
void pantalla0()
{
  if (bandera == 1) 
  { // seleccionado
      if (opcion == 0)
      {
        clear_down();
        temp_High();
      }
      else if (opcion == 1) 
      {
        clear_up();
        temp_Low();
      }
      clear_down();
    } 
    else 
    { // NO seleccionado bandera = 0
      clear_up();
      if (opcion == 0) 
      {
        lcd.setCursor(0, 2);
        lcd.print(" ");
        mostrar_caracter(0);
      }
      else if (opcion == 1) 
      {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        mostrar_caracter(2);
      }
      lcd.setCursor(1, 0);
      lcd.print(arrayMenu[0]);
      lcd.setCursor(1, 1);
      lcd.print(arrayMenu[1]);
    }
}
/*F***********************************************
* NAME: pantalla1
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Muestra en el LCD la opcion 1 y 2 del 
*menu
*-------------------------------------------------
*NOTE:
**************************************************/
void pantalla1()
{
  if (bandera == 1)
  { // seleccionado
      if (opcion == 2) 
      {
        clear_down();
        temp_Low();
      } 
      else if (opcion == 3) 
      {
        clear_up();
        lcd.setCursor(0, 0);
        mostrar_caracter(0);
        lcd.print(" Luz");
        clear_down();
        Luz();
      }

    } 
    else 
    { // NO seleccionado b = 0
      lcd.setCursor(0, 0); //arriba
      lcd.print("                ");
      if (opcion == 2) 
      {
        lcd.setCursor(0, 2);
        lcd.print(" ");
        mostrar_caracter(0);
      } else if (opcion == 3)
      {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        mostrar_caracter(2);
      }
      lcd.setCursor(1, 0);
      lcd.print(arrayMenu[1]);
      lcd.setCursor(1, 1);
      lcd.print("                ");
      lcd.setCursor(1, 1);
      lcd.print(arrayMenu[2]);
    }
}
/*F***********************************************
* NAME: pantalla2
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Muestra en el LCD la opcion 2 y 3 del 
*menu
*-------------------------------------------------
*NOTE:
**************************************************/
void pantalla2()
{
  if (bandera == 1)
  { // seleccionado
      if (opcion == 4)
      {
        clear_down();
        Luz();
      } 
      else if (opcion == 5)
      {
        clear_up();
        lcd.setCursor(0, 0);
        mostrar_caracter(0);
        lcd.print("Sonido ");
        clear_down();
        Sonido();
      }
    }
    else 
    { // NO seleccionado b = 0
      clear_up();
      if (opcion == 4)
      {
        lcd.setCursor(0, 2);
        lcd.print(" ");
        mostrar_caracter(0);
      } 
      else if (opcion == 5)
      {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        mostrar_caracter(2);
      }
      lcd.setCursor(1, 0);
      lcd.print(arrayMenu[2]);
      lcd.setCursor(1, 1);
      lcd.print("                ");
      lcd.setCursor(1, 1);
      lcd.print(arrayMenu[3]);
    }
}
/*F***********************************************
* NAME: pantalla3
*-------------------------------------------------
*PARAMS:
*return: int
*-------------------------------------------------
*PURPOSE: Muestra en el LCD la opcion 3 y 4 del 
*menu
*-------------------------------------------------
*NOTE:
*
**************************************************/
void pantalla3()
{
  if (bandera == 1) 
  { // seleccionado
      if (opcion == 6) 
      {
        clear_down();
        Sonido();

      } 
      else if (opcion == 7) 
      {
        clear_up();
        lcd.setCursor(0, 0);
        mostrar_caracter(0);
        lcd.print("Reset");
        lcd.setCursor(0, 1);
        lcd.print("       ");
        lcd.setCursor(0, 1);
        lcd.print("   HECHO!");
        reiniciarValores();
      }
    }
    else 
    { // NO seleccionado bandera = 0
      clear_up();
      if (opcion == 6)
      {
        lcd.setCursor(0, 2);
        lcd.print(" ");
        mostrar_caracter(0);
      } 
      else if (opcion == 7) 
      {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        mostrar_caracter(2);
      }
      lcd.setCursor(1, 0);
      lcd.print(arrayMenu[3]);
      lcd.setCursor(1, 1);
      lcd.print("                ");
      lcd.setCursor(1, 1);
      lcd.print(arrayMenu[4]);
    }
}
/*F***********************************************
* NAME: menu
*-------------------------------------------------
*PARAMS:
*return: int
*-------------------------------------------------
*PURPOSE: imprime en el lcd las opciones del menu
*
*-------------------------------------------------
*NOTE:
*
**************************************************/
int pantalla4()
{
  if (bandera == 1) 
  { // seleccionado
      if (opcion == 8) 
      {
        clear_up();
        lcd.setCursor(0, 0);
        mostrar_caracter(0);
        lcd.print("Guardar");
        return 1;
      }
  }else 
    { // NO seleccionado bandera = 0
      clear_up();
      if (opcion == 7)
      {
        lcd.setCursor(0, 2);
        lcd.print(" ");
        mostrar_caracter(0);
      } 
      lcd.setCursor(1, 0);
      lcd.print(arrayMenu[4]);
      lcd.setCursor(1, 1);
      lcd.print("                ");
      lcd.setCursor(1, 1);
      lcd.print(arrayMenu[5]);
    }
    return 0;
}
int menu()
{
  if (bandera == 0) 
  {
    opcion = encoder(opcion,-1);
  }
  btn_encoder();
  if (opcion == 0 || opcion == 1)
  {
    pantalla0();
  }
  else if (opcion == 2 || opcion == 3) 
  {//cambia a pantalla1
    pantalla1();
  } 
  else if (opcion == 4 || opcion == 5)
  {//cambia a pantalla2
    pantalla2();
  }
  else if (opcion == 6 || opcion == 7)
  {//cambia a pantalla3
    pantalla3();
  }
  else if(opcion == 7 || opcion == 8)
  {
    return pantalla4();
  }
  return 0;
}
AsyncTask asyncTaskMenu(10, true, menu);
/*F***********************************************
* NAME: mostrarMenu
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Inicializa el encoder y carga los valores
*de los sensores
*-------------------------------------------------
*NOTE:
*
**************************************************/
void mostrarMenu()
{
  lcd.begin(16, 2);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  pinMode(sw, INPUT);
  digitalWrite(sw, HIGH);
  pinMode(sw, INPUT_PULLUP); 
  cargarValores();
}
/*F***********************************************
* NAME: upMenu
*-------------------------------------------------
*PARAMS:
*return: int
*-------------------------------------------------
*PURPOSE: Inicializa y controla la tarea asincrona
*del menu 
*-------------------------------------------------
*NOTE:
*
**************************************************/
int upMenu()
{
  asyncTaskMenu.Start();
  do{
    asyncTaskMenu.Update(asyncTaskMenu);
  }while(menu()==0);
  asyncTaskMenu.Stop();
  return 1;
}
//------------------------------TAREAS ASINCRONAS--------------------------
void ATemperatura(void);
void ALuz(void);
void AMicrofono(void);
AsyncTask asyncTaskTemp(4000,true,ATemperatura);
AsyncTask asyncTaskLuz(5000,true,ALuz);
AsyncTask asyncTaskMicro(6000,true,AMicrofono);
/*F***********************************************
* NAME:TareasAsincronas
*-------------------------------------------------
*PARAMS:
*return: int
*-------------------------------------------------
*PURPOSE: Actualiza las tareas asincronas mientras
*el valor de la temperatura este por debajo
*del umbral maximo
*-------------------------------------------------
*NOTE:
*
**************************************************/
int TareasAsincronas()
{ 
  do{
    asyncTaskTemp.Update(asyncTaskTemp);
    asyncTaskLuz.Update(asyncTaskLuz);
    asyncTaskMicro.Update(asyncTaskMicro);
    delay(1000);
  }while(tempC<temp_high);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(tempC);
  return 1;
}
/*F***********************************************
* NAME: ATemperatura
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Lee el valor de temperatura actual a 
*traves del sensor e imprime el valor obtenido
*-------------------------------------------------
*NOTE:
**************************************************/
void ATemperatura(void)
{
  lcd.setCursor(0, 0);
  lcd.print("Temperatura                ");
  long a = 1023 - analogRead(A0);
  tempC = beta /(log((1025.0*10/a-10) / 10)+beta/298.0)-273.0;
  lcd.setCursor(0, 1);
      lcd.print(tempC);
      lcd.println(" C              ");
    if(tempC>temp_high)
    {
      RGB(255,0,0);
    }
    else if(tempC>temp_low)
    {
      RGB(0,0,255);
    }
    else
    {
      RGB(0,255,0);
    }
}
/*F***********************************************
* NAME: ATLuz
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Lee el valor de luz actual a 
*traves del sensor e imprime el valor obtenido
*-------------------------------------------------
*NOTE:
**************************************************/
void ALuz(void)
{
  lcd.setCursor(0, 0);
  lcd.print("Luz                             ");
  int valorluz = analogRead(A1);
  lcd.setCursor(0, 1);
  lcd.print(valorluz);
  lcd.println(" lm           ");
  if(valorluz > luz)
    RGB(0,255,0);
  else
    RGB(0,0,255);
}

/*F***********************************************
* NAME: AMicrofono
*-------------------------------------------------
*PARAMS:
*return: none
*-------------------------------------------------
*PURPOSE: Lee el valor de sonido actual a 
*traves del sensor e imprime el valor obtenido
*-------------------------------------------------
*NOTE:
*
**************************************************/
void AMicrofono(void)
{
  lcd.setCursor(0, 0);
  lcd.print("Sonido                             ");
  int value = analogRead(A2);
  lcd.setCursor(0, 1);
  lcd.print(value);
  lcd.println(" dB           ");
  if(value > sonido)
  {
    RGB(125,33,129);
    delay(500);
  }
  else
    RGB(0,0,0);
}
