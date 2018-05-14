/*
 * Natalia Isabel Hernandez Naveros
 * Leidy Yuliana Quintero Jaramillo
 * Jhon Anderson Sanchez Buritica 
 ARDUINO   integrado L293D(Puente H)       
pins 5          10
     6          15
     7          7
     8         2
    5V         1, 9, 16
    GND        4, 5, 12, 13

 El motor 1 pines 3 y 6 del Puente H
 El motor 2  pines 11 y 14 del Puente H

 La fuente de alimentación de los Motores se conecta a tierra y
 el positivo al pin 8 del puente H.

 Conexión del Modulo Bluetooth HC-06 y el Arduino
 ARDUINO    Bluetooth HC-06
 0 (RX)       TX
 1 (TX)       RX
 5V           VCC
 GND          GND
 !!Cuidado!! Las conexiones de TX y RX al modulo Bluetooth deben estar desconectadas
 en el momento que se realiza la carga del código (Sketch) al Arduino.

 Conexion Sensor Ultrasonido HC-SR04
 ARDUINO    Ultrasonido HC-SR04
 2            Echo
 3            Trig
 5V           VCC
 GND          Gnd
 */

#include <Servo.h>  ///// recuerden que al incluir la librería servo se desactivan los PWM de los pins                                            9 y 10 del arduino uno
Servo servo1;

int AngSerDer = 15;  ////angulo para la derecha
int AngSerCen = 90; //////angulo para el centro
int AngSerIzq = 165;    /////angulo para la izquierda

int izqA = 5;      //// pines 5,6,7 y 8 al puente H
int izqB = 6;
int derA = 7;
int derB = 8;
int vel = 180;            // Velocidad de los motores se puede variar (0-255)
int estado = '1';         // inicia en espera

int pecho = 2;            // define el pin 2 como (pecho) para el Ultrasonido
int ptrig = 3;            // define el pin 3 como (ptrig) para el Ultrasonido
int duracion, distancia;  // para Calcular distancia

int DisDer;
int DisIzq;
int DisEcoDer;
int DisEcoIzq;
int DisFrenteEco;

void setup() 
{
  Serial.begin(9600);    // inicia el puerto serial para comunicacion con el Bluetooth
  servo1.attach(11);   //// servo1 se conecta al pin 11, no usar 9 ni 10
  pinMode(derA, OUTPUT);
  pinMode(derB, OUTPUT);
  pinMode(izqA, OUTPUT);
  pinMode(izqB, OUTPUT);

  pinMode(pecho, INPUT);   // define el pin 2 como entrada (pecho)
  pinMode(ptrig,OUTPUT);   // define el pin 3 como salida  (ptrig)
  pinMode(13,OUTPUT);

}

void loop() 

{

  if(Serial.available()>0){        // lee el bluetooth y almacena en estado
    estado = Serial.read();
  }
  if(estado=='a')
{           // Boton desplazar al Frente
    servo1.write(AngSerCen);
    analogWrite(derB, 0);   
    analogWrite(izqB, 0);
    analogWrite(derA, vel); 
    analogWrite(izqA, vel);
 }
  if(estado=='b')
{          // Boton IZQ
    servo1.write(AngSerIzq);
    analogWrite(derB, vel);   
    analogWrite(izqB, 0);
    analogWrite(derA, 0); 
    analogWrite(izqA, vel);     
  }
  if(estado=='c')
{         // Boton Parar
    servo1.write(AngSerCen);
    analogWrite(derB, 0);   
    analogWrite(izqB, 0);
    analogWrite(derA, 0);   
    analogWrite(izqA, 0);
  }
  if(estado=='d')
{          // Boton DER
    servo1.write(AngSerDer);
    analogWrite(derB, 0);   
    analogWrite(izqB, vel);
    analogWrite(izqA, 0);
    analogWrite(derA, vel); 
  }

  if(estado=='e')
{          // Boton Reversa
    servo1.write(AngSerCen);
    analogWrite(derA, 0);   
    analogWrite(izqA, 0);
    analogWrite(derB, vel); 
    analogWrite(izqB, vel);     
  }

////

  if (estado =='f'){          // Boton ON,  se mueve sensando distancia

     digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
     delay(0.01);
     digitalWrite(ptrig, LOW);
    
     duracion = pulseIn(pecho, HIGH);              // Lee el tiempo del Echo
     distancia = (duracion/2) / 29;            // calcula la distancia en centimetros
     delay(10);
    
     if (distancia <= 15 && distancia >=2)
   {                                        // si la distancia es menor de 10cm
      
        digitalWrite(13,HIGH);                 // Enciende LED

       ////////////// voltear derecha

      servo1.write(AngSerDer);
      analogWrite(derB, 0);   
      analogWrite(izqB, 0);
      analogWrite(derA, 0);   
      analogWrite(izqA, 0);
      delay(700);
      digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
     delay(0.01);
     digitalWrite(ptrig, LOW);
    
     duracion = pulseIn(pecho, HIGH);              // Lee el tiempo del Echo
     DisEcoDer = (duracion/2) / 29;            // calcula la distancia en centimetros
     delay(10);
     DisDer = DisEcoDer;  ///guarda la variable DisEcoDer para luego compararla
     delay(500);

//////////////////voltear izquierda

      servo1.write(AngSerIzq);
      analogWrite(derB, 0);   
      analogWrite(izqB, 0);
      analogWrite(derA, 0);   
      analogWrite(izqA, 0);
      delay(700);
       digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
     delay(0.01);
     digitalWrite(ptrig, LOW);
    
     duracion = pulseIn(pecho, HIGH);              // Lee el tiempo del Echo
     DisEcoIzq = (duracion/2) / 29;            // calcula la distancia en centimetros
     delay(10);
      DisIzq = DisEcoIzq;   ///guarda la variable DisEcoIzq para luego compararla
      delay(500);

      if (DisDer > DisIzq)   /// se comparan las variables y con ello las distancias
      {
    servo1.write(AngSerCen);    /// se detiene un momento voltea, retrocede un poco      
    delay(400);                             ///  y elige su nueva dirección
    analogWrite(derA, 0);   
    analogWrite(izqA, 0);
    analogWrite(derB, vel); 
    analogWrite(izqB, vel);
    delay(300);
    analogWrite(derB, 0);   
    analogWrite(izqB, vel);
    analogWrite(izqA, 0);
    analogWrite(derA, vel); 
    delay(500);
    analogWrite(derB, 0);   
    analogWrite(izqB, 0);
    analogWrite(derA, 0);   
    analogWrite(izqA, 0);
    delay(200);
  
      }

      if (DisDer < DisIzq)
      {
    servo1.write(AngSerCen);
    delay(400);
    analogWrite(derA, 0);   
    analogWrite(izqA, 0);
    analogWrite(derB, vel); 
    analogWrite(izqB, vel);
    delay(300);
    analogWrite(derB, vel);   
    analogWrite(izqB, 0);
    analogWrite(derA, 0); 
    analogWrite(izqA, vel);     
    delay(500);
    analogWrite(derB, 0);   
    analogWrite(izqB, 0);
    analogWrite(derA, 0);   
    analogWrite(izqA, 0);
    delay(200);

      }

    
     }
     else{                   // Si no hay obstaculos se desplaza al frente 
         analogWrite(derB, 0);   
         analogWrite(izqB, 0);
         analogWrite(derA, vel); 
         analogWrite(izqA, vel);
     }
  }
  if(estado=='1'){          // detiene los motores queda en espera
     servo1.write(AngSerCen);
     analogWrite(derB, 0);   
     analogWrite(izqB, 0);
     analogWrite(derA, 0);   
     analogWrite(izqA, 0);
  }
}
