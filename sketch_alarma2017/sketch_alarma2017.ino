

// Definiciones

int sirenaPin = 12;

int ct = 15;    // Tiempo de calibracion 


/*  
 boolean abierto = false;
int cambios;
*/



#define ledverde 13
#define ledrojo 7
#define piro 2


const int sensor = 6;     // En el pin 6 esta el sensor de apertura
// # Sensor al Digital 6 y a GND
int Estadodesensor = 0;         // variable que lee el estado del sensor de apertura



int valor;


// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(5, 4, 11, 10, 9, 8);





/*
// Rutina de Interrupción
void interrupcion()
{
  abierto = true;
//  Serial.print("cambio ");
  if (cambios == 0)
    cambios ++;
  else {
    cambios = 0;
    abierto = false;
  }
  //Serial.println(cambios);
}
*/




void setup() {



// Sensor magentico de apertura!!!!!!!!!!!!!!!!!



  // -inicializamos el sensor como una entrada:
pinMode(sensor, INPUT);

  


//LCD!!!!!!!!!!!!!!!!!



        // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);




  
  // put your setup code here, to run once:
  Serial.begin(9600);



    lcd.clear();


  
    
 // Interruptor D3 Interrupcion 1
  /*
  digitalWrite(6, HIGH);
  attachInterrupt(1,interrupcion,HIGH); // Attaching the ISR to INT0 
*/


  

  pinMode(ledverde, OUTPUT);
  pinMode(ledrojo, OUTPUT);
  pinMode(piro, INPUT);


// Sirena!!!!!!!!!!!!!!!



  pinMode(sirenaPin, OUTPUT);

  digitalWrite(sirenaPin, LOW);




  

  //Calibracion del sensor PIR



    Serial.println("Espere, se estan calibrando los sensores....");

// mensaje en display

              lcd.setCursor(1, 0);

                            // Print a message to the LCD.
              lcd.print(" Calibrando.. ");
    
// mensaje en display


           // (note: line 1 is the second row, since counting begins with 0):
        lcd.setCursor(9, 0);


      for(int i = 0; i <= ct; i++){
        Serial.print(((i*100)/ct));      
        Serial.print("% ");
        Serial.println("COMPLETADO.....");
        delay(1000);
        }
        
                    lcd.clear();



      //lcd.print(" Calibracion completada ");
      lcd.print("        Alarma Activada ");
      Serial.println("** SENSORES ACTIVOS **");

      //lcd.println("** SENSOR ACTIVO **");

      

                  // scroll 16 positions (string length) to the left
    // to move it offscreen left:
    for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(500);
        }


                    lcd.clear();

      
      delay(50);




  
      
}

void loop() {

        digitalWrite(ledrojo, LOW);



 // Lee el estado del sensor de apertura:
Estadodesensor = digitalRead(sensor);


  /* delay (1000);*/
  
  // put your main code here, to run repeatedly:
    valor=digitalRead(piro);

  if (valor==HIGH or Estadodesensor == HIGH){
  
  
  // Sirena!!!!!!!!!!!!!!!!!!






          digitalWrite(sirenaPin, HIGH);






  
                  
         // (note: line 1 is the second row, since counting begins with 0):
        lcd.setCursor(9, 0);
  
  
   /* 
      digitalWrite(ledverde, LOW);
      digitalWrite(ledrojo, HIGH);

      */
  
      // Sensor de apertura y PIR
          if (Estadodesensor == HIGH)
        {


        digitalWrite(ledverde, LOW);
      digitalWrite(ledrojo, HIGH);
  
  
    
          Serial.println(" fue activado el sensor de apertura "); 
  
  
          //PROBANDO MOVER TEXTO
  
    
  
                    // Print a message to the LCD.
          lcd.print(" Se abrio la puerta ");
  
  
  
            // scroll 16 positions (string length) to the left
    // to move it offscreen left:
    for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(300);
        }
  
  
        
        }
        else
        
        {
                        if( valor == HIGH ){

                                  digitalWrite(ledverde, LOW);
                                  digitalWrite(ledrojo, HIGH);
          
            
                        Serial.println("Se activo el sensor PIR ");
                
                
                
                        //   LCD!!!!!!!
                
                                  // Print a message to the LCD.
                        lcd.print("Se activo el sensor PIR ");
                
                
                
                          // scroll 16 positions (string length) to the left
                  // to move it offscreen left:
                  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
                    // scroll one position left:
                    lcd.scrollDisplayLeft();
                    // wait a bit:
                    delay(400);
                      }            
            
                    
                    }
/*    
      Serial.println("** MOVIMIENTO DETECTADO **");
      delay(1000);
      
      */



        }
  


  
    }
    
/*else{
    
      lcd.clear(); 
      digitalWrite(ledrojo, LOW);
      digitalWrite(sirenaPin, LOW);
      digitalWrite(ledverde, HIGH);
      Serial.println("-- Z A R A S A  --");  
      delay(2000);
      
    }
    */
              lcd.clear();

        digitalWrite(sirenaPin, LOW);


      digitalWrite(ledrojo, LOW);
      digitalWrite(ledverde, HIGH);





}

