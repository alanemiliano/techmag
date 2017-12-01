/*

  -SISTEMA DE ALARMA Y MONITOREO
  -CURSO: 7MO 2DA INFORMATICA
  -PROFESOR:  IVAN NOMI
  -ESCUELA TECNICA N-1 DE PINAMAR
  -2016/2017
  -ALUMNOS: Alan Quispe Torrico, Matias Olazo, Gisela Fernandez, Jorge Neira


*/

//INCLUIMOS LAS LIBRERIAS NECESARIAS
#include <SPI.h>    //Iniciamos el Bus SPI
#include <MFRC522.h>    // Iniciamos  el MFRC522
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
boolean releCam = 5;    //pin del rele de la camara IP
boolean releSiren = 4;    //pin del rele de la sirena

#define ledverde 13   //led verde para indicar alarma sin movimiento
#define ledrojo 11   //led rojo para indicar alarma detecta movimiento
#define sensor 10   //sensor/es de apertura
#define piro 9    //sensor PIR
#define buzzer 8    //buzzer para el sonido del teclado
#define RST_PIN  7    //Pin 9 para el reset del RC522 //pin 7 para  arduimo mega
#define SS_PIN  6   //Pin 10 para el SS (SDA) del RC522 //pin 6 para  arduimo mega
MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522


// definimos los pines del display

LiquidCrystal lcd(53, 49, 39, 37, 35, 33);
// PROTOTIPO 1 (5, 4, 11, 10, 9, 8)
//PROTOTIPO 2 (53, 49, 39, 37, 35, 33)


//Definimos la dimesión del teclado

const byte ROWS = 4;    //Cuatro Filas
const byte COLS = 4;    //Cuatro Columnas


// Definimos el Keymap o distribucion de las teclas en el teclado

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


//Definimos donde van a ir conectados los pines del teclado

byte rowPins[ROWS] = {22, 24, 26, 28};// Conectar los keypads ROW1, ROW2, ROW3 y ROW4 a esos Pines de Arduino.
byte colPins[COLS] = {30, 32, 34, 36};// Conectar los keypads COL1, COL2, COL3 y COL4 a esos Pines de Arduino.

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


//Definimos la contraseña de nuestra alarma
Password password = Password("12AB");


void setup() {

  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
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
  pinMode(releCam, OUTPUT);   //inicializamos el pin del rele de la camara IP
  pinMode(releSiren, OUTPUT);    //inicializamos el pin del rele de la sirena

  //LCD

  lcd.begin(16, 2);   //inicializamos el display con el numero de filas y columnas
  lcd.clear();    //borramos el contenido que tenga el display en el momento


  //configuracion de los pines necesarios

  digitalWrite(sirenaPin, LOW);   //apagamos la sirena
  digitalWrite(releCam, false);   //apagamos el rele de la camara IP
  digitalWrite(releSiren, false);   //apagamos el rele de la sirena
  digitalWrite(ledrojo, HIGH);   //Encendemos el led rojo
  digitalWrite(ledverde, HIGH);   //Encendemos el led verde


  //Calibracion del sensor PIR

  lcd.setCursor(0, 0);    //configuramos la fila y la columna donde empiece a escribir
  lcd.print(" Calibrando.. ");    //escribimos el mensaje en el display

  Serial.println("Espere, se esta calibrando el sensor PIR....");   //escribimos por el monior serie

  for (int i = 0; i <= ct; i++)   //inicializamos un ciclo para que el sensor pueda calibrarse
  {

    Serial.print(((i * 100) / ct)); //escribimos por el monior serie
    Serial.print("% ");   //escribimos por el monior serie
    Serial.println("COMPLETADO.....");   //escribimos por el monior serie
    delay(1000);   //dejamos un tiempo de espera

  }

  Serial.println("Calibracion Completada Satisfactoriamente.");   //escribimos por el monior serie
  Serial.println("** SENSORES ACTIVOS **");   //escribimos por el monior serie

  lcd.clear();    //borramos el contenido que tenga el display en el momento

  lcd.setCursor(0, 0);   //configuramos la fila y la columna donde empiece a escribir
  lcd.print("  *Bienvenido*");    //escribimos el mensaje en el display
  lcd.setCursor(0, 1);   //configuramos la fila y la columna donde empiece a escribir
  lcd.print("Ingrese la Clave");    //escribimos el mensaje en el display

  delay(50);   //dejamos un tiempo de espera

}

byte ActualUID[4]; //almacenará el código del Tag leído
byte Alumno[4] = {0xA6, 0xA7, 0x62, 0xAC} ; //código del usuario 1 Alan
byte AlumnoB[4] = {0x72, 0x65, 0xC6, 0x57} ; //código del usuario 2 Bruno
byte Docente[4] = {0xA7, 0x29, 0xD7, 0x2B} ; //código del usuario 3

void loop() {

  keypad.getKey();

  LeoTarjeta();  //llamo a la funcion para que compare mis tarjetas

  if (estadoanterior == 1) {   //si activan la alarma...


    //delay(100000);    //se le da un minuto a la persona para cerrar la puerta

    // Lee y guarda el estado del sensor de apertura en la varibale Estadodesensor
    Estadodesensor = digitalRead(sensor);

    //lee y guarda el estado del sensor PIR en la variable valor
    valor = digitalRead(piro);


    if (valor == 1 or Estadodesensor == 1) {  //si el sensor PIR o el sensor de apertura fueron activados...


      //
      //delay(10000);    //se le da un minuto a la persona para ingresar la clave
      digitalWrite(sirenaPin, HIGH);    //Se prende la Sirena!!!!!!!!!!!!!!!
      digitalWrite(releSiren, true);    //enciende el rele de la sirena
      digitalWrite(ledverde, LOW);    //se apaga el led verde
      digitalWrite(ledrojo, HIGH);    //se enciende el led rojo que indica advertencia

      // Sensor de apertura y PIR
      if (Estadodesensor == 1)    //si el sensor de apertura fue abierto...


      {
        //digitalWrite(ledverde, LOW);
        //digitalWrite(ledverde, HIGH);
        //digitalWrite(ledrojo, HIGH);

        Serial.println(" fue activado el sensor de apertura ");   //escribimos por el monior serie
        delay(100);
        digitalWrite(buzzer, HIGH);

        while (tiempoespera > 0 and tiempoespera < 1) {

          keypad.getKey();
          tiempoespera = tiempoespera - 1;


        }



      }   //final de if (Estadodesensor == 1)...

      else    //si no se activo el sensor de apertura

      {
        digitalWrite(buzzer, LOW);


        //
        if ( valor == 1 )  //si el sensor PIR fue activado..
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


  else {  //si ponen la contraseña devuelta quieren desactivarla

    //
    //digitalWrite(ledverde, HIGH);   //se enciende el led verde
    //digitalWrite(ledrojo, LOW);    //se apaga el led rojo que indica advertencia
    digitalWrite(sirenaPin, LOW);    //Se prende la Sirena!!!!!!!!!!!!!!!
    digitalWrite(releSiren, false);   //apagamos el rele de la sirena

    estadoanterior = 0;   //ponemos el estado anterior en 0

  }

  if (intentos < 3) { //si los intentos estan en 0...

    digitalWrite(ledverde, LOW);    //se apaga el led verde
    digitalWrite(ledrojo, HIGH);    //se enciende el led rojo que indica advertencia
    digitalWrite(buzzer, HIGH);   //Se enciende el buzzer

    if (estadoanterior >= 1 and intentos <= 0) { //si el estado anterior es >=1 y los intentos son = a 0

      keypad.getKey();    //si no pones esto no te anda el teclado

    }

  }

  if (intentos <= 0) {
    delay(100000);    //Tiempo para que no se pueda acceder ninguna clave
  }

  /*
      while(intentos<3){    //mientras los intentos sean menor a 3...

        digitalWrite(ledverde, LOW);    //se apaga el led verde
        digitalWrite(ledrojo, HIGH);    //se enciende el led rojo que indica advertencia

        keypad.getKey();    //si no pones esto no te anda el teclado
        LeoTarjeta();  //llamo a la funcion para que compare mis tarjetas


      }
  */



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

      for ( inicio = 1; inicio <= 1; inicio++ )

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
          clicks = clicks + 1;
          password.append(eKey);

      }

      //Serial.println(ilosc);

      if (clicks == 1)
      {

        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("   < PIN >");
        lcd.setCursor(0, 1);
        lcd.print("*_");

      }
      if (clicks == 2)
      {

        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("   < PIN >");
        lcd.setCursor(0, 1);
        lcd.print("**_");

      }//---------------------------------------HASTA ACA ESTA TODO ACOMODADO.---------------------
      if (clicks == 3)
      {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("   < PIN >");
        lcd.setCursor(0, 1);
        lcd.print("***_");
      }
      if (clicks == 4)
      {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("   < PIN >");
        lcd.setCursor(0, 1);
        lcd.print("****_");
      }
      if (clicks == 5)
      {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("   < PIN >");
        lcd.setCursor(0, 1);
        lcd.print("*****_");
      }
      if (clicks == 6)
      {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("   < PIN >");
        lcd.setCursor(0, 1);
        lcd.print("******_");
      }
      if (clicks == 7)
      {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("   < PIN >");
        lcd.setCursor(0, 1);
        lcd.print("*******_");
      }
      if (clicks == 8)
      {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("   < PIN >");
        lcd.setCursor(0, 1);
        lcd.print("********");
      }

      if (clicks == largocontrasena)
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
    TodoOk();   //hace sonar el buzzer, renueva los intentos y pone el estado anterior en 1
    LedOk();    //aoaga el led rojo y prende el verde

    clicks = 0;
    password.reset();

    Serial.println("Correcto");

    //digitalWrite(ledrojo, HIGH);
    //digitalWrite(ledverde, LOW);

    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("<<PIN CORRECTO>>");
    delay(1000);
    lcd.clear();
    lcd.print("     Alarma");
    lcd.setCursor(0, 1);
    lcd.print("    Activada");

    if (estadoanterior > 1) { //si ingresan devuelta la contraseña

      lcd.clear();
      lcd.print("  *Bienvenido*");

      //digitalWrite(ledrojo, LOW);
      //digitalWrite(ledverde, HIGH);


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
    BuzzerX();    //hace sonar el buzzer de manera negativa
    LedX();   //apaga el led verde y enciende el rojo
    // ilosc = 0;
    password.reset();

    Serial.println("Error");

    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("<<PIN ERRONEO>>");
    delay(2000);

    Error();    //resta los intentos y muestra por pantalla que esta pasando

  }
}


void LeoTarjeta() {

  if ( mfrc522.PICC_IsNewCardPresent())    // Revisamos si hay nuevas tarjetas  presentes
  {

    //Seleccionamos una tarjeta
    if ( mfrc522.PICC_ReadCardSerial())
    {
      // Enviamos serialemente su UID
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        ActualUID[i] = mfrc522.uid.uidByte[i];
      }
      //comparamos los UID para determinar si es uno de nuestros usuarios
      if (compareArray(ActualUID, Alumno)) {

        LedOk();    //aoaga el led rojo y prende el verde
        TodoOk();   //hace sonar el buzzer, renueva los intentos y pone el estado anterior en 1

        if (estadoanterior > 1) { //si ingresan devuelta la contraseña

          lcd.clear();
          Serial.println("Bienvenido");
          lcd.print("  *Bienvenido*");
          lcd.setCursor(0, 1);
          lcd.print("     Alan");


        } else {
          Serial.println("Alarma Activada");
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("<<TARGETA CORRECTO>>");
          delay(1000);
          lcd.clear();
          lcd.print("     Alarma");
          lcd.setCursor(0, 1);
          lcd.print("    Activada");
        }

      }
      else if (compareArray(ActualUID, Docente)) {

        LedOk();    //aoaga el led rojo y prende el verde
        TodoOk();   //hace sonar el buzzer, renueva los intentos y pone el estado anterior en 1

        if (estadoanterior > 1) { //si ingresan devuelta la contraseña

          lcd.clear();
          Serial.println("Bienvenido");
          lcd.print("  *Bienvenido*");
          lcd.setCursor(0, 1);
          lcd.print("  Profesor");
        } else {
          Serial.println("Alarma Activada");
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("<<TARGETA CORRECTO>>");
          delay(1000);
          lcd.clear();
          lcd.print("     Alarma");
          lcd.setCursor(0, 1);
          lcd.print("    Activada");
        }
      } else if (compareArray(ActualUID, AlumnoB)) {

        LedOk();    //aoaga el led rojo y prende el verde
        TodoOk();   //hace sonar el buzzer, renueva los intentos y pone el estado anterior en 1

        if (estadoanterior > 1) { //si ingresan devuelta la contraseña

          lcd.clear();
          Serial.println("Bienvenido");
          lcd.print("  *Bienvenido*");
          lcd.setCursor(0, 1);
          lcd.print("    Bruno");
        } else {
          Serial.println("Alarma Activada");
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("<<TARGETA CORRECTO>>");
          delay(1000);
          lcd.clear();
          lcd.print("     Alarma");
          lcd.setCursor(0, 1);
          lcd.print("    Activada");
        }
      }
      else {

        Serial.println("Acceso denegado...");
        LedX();   //apaga el led verde y enciende el rojo
        BuzzerX();    //hace sonar el buzzer de manera negativa
        Error();    //resta los intentos y muestra por pantalla que esta pasando
      }

      // Terminamos la lectura de la tarjeta tarjeta  actual
      mfrc522.PICC_HaltA();

    }

  }

}



//Función para comparar dos vectores
boolean compareArray(byte array1[], byte array2[]) {

  if (array1[0] != array2[0])return (false);
  if (array1[1] != array2[1])return (false);
  if (array1[2] != array2[2])return (false);
  if (array1[3] != array2[3])return (false);
  return (true);

}

void TodoOk() {
  int inicio;
  for ( inicio = 1; inicio <= 3; inicio++ )
  {
    digitalWrite(buzzer, HIGH);
    delay(120);
    digitalWrite(buzzer, LOW);
    delay(70);
  }

  intentos = 3;   //Renuevo los intentos intentos
  estadoanterior = estadoanterior + 1;    //ponemos el estado anterior en 1

}

void BuzzerX() {
  int inicio;
  for ( inicio = 1; inicio <= 1; inicio++ )
  {

    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    delay(100);

  }
}

void Error() {

  intentos = intentos - 1;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    *Error*");
  lcd.setCursor(0, 1);
  lcd.print("  Intentos: ");
  Serial.println(intentos);
  lcd.print(intentos);

  digitalWrite(buzzer, HIGH);

  if (intentos == 0) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("    *Alerta*");

  }
}

void LedOk() {
  digitalWrite(ledrojo, LOW);
  digitalWrite(ledverde, HIGH);
}
void LedX() {
  digitalWrite(ledverde, LOW);
  digitalWrite(ledrojo, HIGH);
}

