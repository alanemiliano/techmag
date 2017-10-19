/*

-SISTEMA DE ALARMA Y MONITOREO
-ALUMNOS: 7MO 2DA INFORMATICA
-PROFESOR:  IVAN NOMI
-ESCUELA TECNICA N-1 DE PINAMAR

*/

//INCLUIMOS LAS LIBRERIAS NECESARIAS

#include <Password.h>   //Incluimos la libreria de Password
#include <Keypad.h>   //Incluimos la libreria  del Keypad
#include <LiquidCrystal.h>    //Incluimos la libreria del display


// Definimos los pines necesarios

int sirenaPin = 12;   //sirena de auto o moto
int ct = 15;    // Tiempo de calibracion para el sensor PIR
int Estadodesensor = 0;   // variable que lee el estado del sensor de apertura
int valor;    //variable que lee el estado del sensor PIR
int largocontrasena = 4;    //Largo de la contraseña para el teclado
int clicks;    //Numero de Clicks en el teclado
int estadoanterior = 0;   //el estado de la alarma si se activo o no
int intentos = 3;   //numero de intentos para ingresar la contraseña bien
int tiempoespera = 10000;    //tiempo de espera para desactivar la alarma 

#define ledverde 13   //led verde para indicar alarma sin movimiento
#define ledrojo 11   //led rojo para indicar alarma detecta movimiento
#define buzzer 8    //buzzer para el sonido del teclado
#define piro 9    //sensor PIR
#define sensor 10   //sensor/es de apertura


// definimos los pines del display

LiquidCrystal lcd(53, 49, 39, 37, 35, 33);
// PROTOTIPO 1 (5, 4, 11, 10, 9, 8)
//PROTOTIPO 2 (53, 49, 39, 37, 35, 33)


//Definimos la dimesión del teclado

const byte ROWS = 4;    //Cuatro Filas
const byte COLS = 4;    //Cuatro Columnas


// Definimos el Keymap o distribucion de las teclas en el teclado

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};


//Definimos donde van a ir conectados los pines del teclado

byte rowPins[ROWS] = {22, 24, 26, 28};// Conectar los keypads ROW1, ROW2, ROW3 y ROW4 a esos Pines de Arduino.
byte colPins[COLS] = {30, 32, 34, 36};// Conectar los keypads COL1, COL2, COL3 y COL4 a esos Pines de Arduino.
 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


//Definimos la contraseña de nuestra alarma
Password password = Password("12AB");


void setup() {


    Serial.begin(9600); //incializamos el puerto serial en 9600 baudios


    //añadimos una instrucción para acceder a una serie de eventos
   keypad.addEventListener(keypadEvent);  
   

    //configuramos los pines para que sean de salida o de entrada

    pinMode(ledverde, OUTPUT);   //inicializamos el led verde como una salida
    pinMode(ledrojo, OUTPUT);   //inicializamos el led rojo como una salida
    pinMode(piro, INPUT);   //inicializamos el pin del PIR como entrada 
    pinMode(sensor, INPUT);   //inicializamos el pin del sensor de apertura como entrada 
    pinMode(buzzer, OUTPUT);    //inicializamos el pin del buzzer como una salida
    pinMode(sirenaPin, OUTPUT);   //inicializamos el pin de la sirena como una salida


    //LCD
    
    lcd.begin(16, 2);   //inicializamos el display con el numero de filas y columnas
    lcd.clear();    //borramos el contenido que tenga el display en el momento


    //configuracion de los pines necesarios
    
    //digitalWrite(sirenaPin, LOW);   //apagamos la sirena
    //digitalWrite(ledrojo, LOW);   //apagamos el led rojo
    //digitalWrite(ledverde, LOW);   //apagamos el led verde  


    //Calibracion del sensor PIR

    lcd.setCursor(0, 0);    //configuramos la fila y la columna donde empiece a escribir
    lcd.print(" Calibrando.. ");    //escribimos el mensaje en el display

    Serial.println("Espere, se esta calibrando el sensor PIR....");   //escribimos por el monior serie
    
    for(int i = 0; i <= ct; i++)    //inicializamos un ciclo para que el sensor pueda calibrarse
    {
       
      Serial.print(((i*100)/ct));   //escribimos por el monior serie
      Serial.print("% ");   //escribimos por el monior serie
      Serial.println("COMPLETADO.....");   //escribimos por el monior serie
      delay(1000);   //dejamos un tiempo de espera
      
      }
      

    Serial.println("Calibracion Completada Satisfactoriamente.");   //escribimos por el monior serie
    Serial.println("** SENSORES ACTIVOS **");   //escribimos por el monior serie

    lcd.clear();    //borramos el contenido que tenga el display en el momento

    lcd.setCursor(0,0);    //configuramos la fila y la columna donde empiece a escribir
    lcd.print("  *Bienvenido*");    //escribimos el mensaje en el display
    lcd.setCursor(0,1);    //configuramos la fila y la columna donde empiece a escribir
    lcd.print("Ingrese la Clave");    //escribimos el mensaje en el display
     
    delay(50);   //dejamos un tiempo de espera

     
}


void loop() {
    
  
   keypad.getKey();


   if (estadoanterior==1){    //si activan la alarma...

    
    //delay(100000);    //se le da un minuto a la persona para cerrar la puerta

    // Lee y guarda el estado del sensor de apertura en la varibale Estadodesensor
    Estadodesensor = digitalRead(sensor);
      
    //lee y guarda el estado del sensor PIR en la variable valor
    valor = digitalRead(piro);
      
      
    if (valor == 1 or Estadodesensor == 1){   //si el sensor PIR o el sensor de apertura fueron activados...

      //
      //delay(10000);    //se le da un minuto a la persona para ingresar la clave
      digitalWrite(sirenaPin, HIGH);    //Se prende la Sirena!!!!!!!!!!!!!!!
      digitalWrite(ledverde, LOW);    //se apaga el led verde        
      digitalWrite(ledrojo, HIGH);    //se enciende el led rojo que indica advertencia
  
      // Sensor de apertura y PIR
      if (Estadodesensor == 1)    //si el sensor de apertura fue abierto...

      
      {
        //
        //digitalWrite(ledverde, LOW);
        //digitalWrite(ledverde, HIGH);
        //digitalWrite(ledrojo, HIGH);

        Serial.println(" fue activado el sensor de apertura ");   //escribimos por el monior serie
        digitalWrite(buzzer, HIGH);

        while(tiempoespera>0 and tiempoespera<1){

             keypad.getKey();
          tiempoespera = tiempoespera - 1;

          
          }

          Serial.println("Sali del ciclo");

          

        
       }   //final de if (Estadodesensor == 1)...
        
        else    //si no se activo el sensor de apertura
        
        {
                    digitalWrite(buzzer, LOW);  

          
          //
          if( valor == 1 )   //si el sensor PIR fue activado..
          {
            //
            //digitalWrite(ledverde, LOW);
            //digitalWrite(ledrojo, HIGH);
            
            Serial.println("Se activo el sensor PIR ");   //escribimos por el monior serie
            Serial.println(valor);
                        
          }    //final if( valor == HIGH )...


        }    //final else de de if (Estadodesensor == 1)...


      }   //final de if (valor == HIGH or Estadodesensor == HIGH)


      
    }   //final if (estadoanterior==1)......
    
      
    else{   //si ponen la contraseña devuelta quieren desactivarla

      //
      digitalWrite(ledverde, HIGH);   //se enciende el led verde
      digitalWrite(ledrojo, LOW);    //se apaga el led rojo que indica advertencia
      
      estadoanterior=0;   //ponemos el estado anterior en 0
    
    }

    if (intentos==0){   //si los intentos estan en 0...

          digitalWrite(ledverde, LOW);    //se apaga el led verde
          digitalWrite(ledrojo, HIGH);    //se enciende el led rojo que indica advertencia
      
    }

    if(estadoanterior>=1 and intentos==0){    //si el estado anterior es >=1 y los intentos son = a 0
 
      intentos=3;   //renovamos los intentos
  
    }


    while(intentos<3){    //mientras los intentos sean menor a 3...

      digitalWrite(ledverde, LOW);    //se apaga el led verde
      digitalWrite(ledrojo, HIGH);    //se enciende el led rojo que indica advertencia
      
      keypad.getKey();    //si no pones esto no te anda el teclado 

    }

  

}   //final void loop
//-------------------HASTA ACA ESTA TODO ACOMODADO------------------------------------



//----------------------------------------------------------------------FUNCIONES EXTRA
void keypadEvent(KeypadEvent eKey)
{
  
  switch (keypad.getState())
  {
    
    case PRESSED:

    //   ESTO SIRVE PARA UN SPEAKER
    int inicio;
    
    for( inicio = 1; inicio <= 1; inicio++ )

    {
      
      digitalWrite(buzzer, HIGH);  
      delay(200);            
      digitalWrite(buzzer, LOW);  
      delay(100);
      
    }

    
    Serial.print("Presionado: ");
    Serial.println(eKey);
 
    switch (eKey)
    
    {
      
      /*
      case '#':
      break;
       
      case '*':
      break;
      */
       
      default:
      clicks=clicks+1;
      password.append(eKey);

    }

    //Serial.println(ilosc);
    
    if(clicks == 1)
    {
      
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("   < PIN >");
      lcd.setCursor(0,1);
      lcd.print("*_");
    
    }
    if(clicks == 2)
    {
      
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("   < PIN >");
      lcd.setCursor(0,1);
      lcd.print("**_");
    
    }//---------------------------------------HASTA ACA ESTA TODO ACOMODADO.---------------------
    if(clicks == 3)
    {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("   < PIN >");
    lcd.setCursor(0,1);
    lcd.print("***_");
    }
    if(clicks == 4)
    {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("   < PIN >");
    lcd.setCursor(0,1);
    lcd.print("****_");
    }
    if(clicks == 5)
    {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("   < PIN >");
    lcd.setCursor(0,1);
    lcd.print("*****_");
    }
    if(clicks == 6)
    {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("   < PIN >");
    lcd.setCursor(0,1);
    lcd.print("******_");
    }
    if(clicks == 7)
    {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("   < PIN >");
    lcd.setCursor(0,1);
    lcd.print("*******_");
    }
    if(clicks == 8)
    {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("   < PIN >");
    lcd.setCursor(0,1);
    lcd.print("********");
    }
     
    if(clicks == largocontrasena)
    {
    delay(250);
    checkPassword();
    clicks = 0;
    }
  }
}
 
void checkPassword()
{
  if (password.evaluate())
  {
int inicio;
for( inicio = 1; inicio <= 3; inicio++ )
{
  digitalWrite(buzzer, HIGH);  
  delay(120);            
  digitalWrite(buzzer, LOW);  
  delay(70);      
}    
    clicks = 0;
    password.reset();
    
    Serial.println("Correcto");    
 
    //digitalWrite(ledrojo, HIGH);
    //digitalWrite(ledverde, LOW);
 
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("<<PIN CORRECTO>>");
    delay(1000);
    lcd.clear();
    lcd.print("     Alarma");
    lcd.setCursor(0,1);
    lcd.print("    Activada"); 
 
     estadoanterior=estadoanterior+1;


    if(estadoanterior>1){ //si ingresan devuelta la contraseña

       lcd.clear();
       lcd.print("  *Bienvenido*");
       
       digitalWrite(ledrojo, LOW);
       digitalWrite(ledverde, HIGH);
       
       
      }

    
/* 
    delay(2000);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, HIGH);    
       
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  *Bienvenido*");
    lcd.setCursor(0,1);
    lcd.print("FAVOR ENTRE PIN");   
 */

 
  }  
  else  
  {
int inicio;
for( inicio = 1; inicio <= 1; inicio++ )
{
  

  digitalWrite(buzzer, HIGH);  
  delay(300);            
  digitalWrite(buzzer, LOW);  
  delay(100);


        
}  
   // ilosc = 0;  
    password.reset();
 
    Serial.println("Error");
 
    digitalWrite(ledverde, LOW);
    digitalWrite(ledrojo, HIGH);    
             
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("<<PIN ERRONEO>>");
    delay(2000);
    
    intentos=intentos-1;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    *Error*");
    lcd.setCursor(0,1);
    lcd.print("  Intentos: ");
    lcd.print(intentos);

    digitalWrite(buzzer, HIGH);


      if(intentos==0){
        
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("    *Alerta*");
          //digitalWrite(buzzer, HIGH);  

        
        }


  }
}

