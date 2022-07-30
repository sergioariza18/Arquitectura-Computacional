/*  LED RGB  */
#define LED_RED 14
#define LED_GREEN 15
#define LED_BLUE 16
/*  KEYPAD   */
const byte ROWS = 4; 
const byte COLS = 3;
char keys[ROWS][COLS] = 
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; 
byte colPins[COLS] = {8, 7, 6}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
/*  LCD   */
const int rs = 12, en = 11, d4 = 31, d5 = 32, d6 = 33, d7 = 34;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
/*  TEMPSENSOR  */
#define analogPin A0 //temperatura
#define analogPin A1 //luz
#define analogPin A2 //microfono
#define beta 4090 
#define resistance 10 
/*  ENCODER */
#define clk 17
#define dt 18
#define sw 19 
//tareas 
#define bt A8
float tempC = 0;
