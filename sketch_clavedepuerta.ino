#include <Keypad.h> //incluye lal ibreria que usamos para el teclado
#include <Servo.h>  //incluye la libreria del Servo
#include <Wire.h>		// incluye libreria para interfaz I2C
#include <RTClib.h>		// incluye libreria para el manejo del modulo RTC
#include <LiquidCrystal_I2C.h> //incluye la libreria de la LCD

Servo cito; //variable para poder usar el servomotor

RTC_DS3231 rtc;			// crea objeto del tipo RTC_DS3231

#define direccion_lcd 0x27 //configuracion del LCD
#define filas 2
#define columnas 16
LiquidCrystal_I2C lcd(direccion_lcd, columnas, filas);

const int FILAS = 4;/* configuracion del teclado*/
const int COLUMNAS= 4;
char keys[FILAS][COLUMNAS]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinesFila[FILAS]={9,8,7,6};
byte pinesColumna[COLUMNAS]={5,4,3,2};/**/

Keypad teclado=Keypad(makeKeymap(keys), pinesFila, pinesColumna,FILAS,COLUMNAS);/*creamos la configuracion de la contraseña*/

char teclaPrecionada;

char pass[5]="2205"; //Poner en el arreglo 1 posicion mas a la que tendra la contraseña correcta
char ingresaPass[5]; // tiene que tener el mismo tamaño del arreglo de la contraseña correcta
int indice=0;/**/

void setup()
{
  Serial.begin(9600);		// inicializa comunicacion serie a 9600 bps

  rtc.begin();	//Inicializa el modulo RTC

 rtc.adjust(DateTime(__DATE__, __TIME__));	// funcion que permite establecer fecha y horario

 lcd.init();       //iniciamos la lcd
 lcd.backlight();

 cito.attach(10);//activamos el pin del servo
}

void loop()
{
  DateTime tiempo = rtc.now();			// funcion que obtiene la hora de la fecha completa
  Serial.print(tiempo.hour());     /*muestra las horas, minutos y segundos en el monitor serial*/
  Serial.print(":");
  Serial.print(tiempo.minute());
  Serial.print(":");
  Serial.println(tiempo.second()); /**/
  lcd.setCursor(12,1);
  lcd.print(tiempo.hour());        /*muestra las horas y minutos en el LCD*/
  lcd.print(":");				
  lcd.print(tiempo.minute());      /**/
  
  delay(100);
  
/*recordar probar quitar if para ver si asi se muestra toda la contrasena, mostrar solo el teclaPrecionada*/

  teclaPrecionada=teclado.getKey(); //detecta que en el teclado se pulso un boton
  if(teclaPrecionada){
    ingresaPass[indice]=teclaPrecionada; // le asigna lo que ingresamos a el arreglo que va a ser comparado
    indice++; //cambiara la posicion del arreglo para poder asignar los caracteres
    lcd.setCursor(0,0); //pone el cursor en la posicion 0,0
    lcd.print(teclaPrecionada);
    delay(10);
    
  }
  if(indice==4){ //cuando se llegue al limite del arreglo verificara si esta correcta o no la contraseña
    if(strcmp(pass,ingresaPass)==0){ //Verifica que la contraseña esta correcta
      lcd.setCursor(0,0); //pone el cursor en la posicion 0,0
      lcd.clear(); //pone el cursor en blanco
      lcd.print(" Acceso aceptado"); //pone en el lcd que se accedio correctamente
      lcd.setCursor(0,1); //pone en el lcd en pocision 0,1
      lcd.print("abierto "); //indica que esta abierto
      cito.write(180); //mueve el servo a 180°
      delay(1500);
      lcd.clear();  //limpia el lcd
      lcd.setCursor(0, 1);
      lcd.print("abierto"); //deja la indicacion que esta abierto
    }
    else{
      lcd.setCursor(0,0);//pone el cursor en la posicion 0,0
      lcd.clear();//limpia el lcd
      lcd.print(" Acceso denegado");//pone en el lcd que no se accedio
      lcd.setCursor(0,1);//pone el cursor en la posicion 0,1
      lcd.print("cerrado ");//indica que esta cerrado
      cito.write(0);//mueve el servo a 0°
      delay(1500);
      lcd.clear();//limpia el lcd
      lcd.setCursor(0, 1);//pone el cursor en la posicion 0,1
      lcd.print("cerrado");//deja la indicacion que esta cerrado
    }
    indice=0;
  }
}
