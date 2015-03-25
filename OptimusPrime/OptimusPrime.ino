/*
Puente H --> 4,5,6,7
Servos --> 9,10
Xbee--> 0,1
Trigger-->2
3 US --> 3,12,13
Infrarrojos --> A0,A1
Corriente -->A2
*/

#include <SimpleTimer.h>
#include <SoftwareSerial.h>
#include <Servo.h> 

//Constantes 
const double pi = 3.1416;
const int limInf = 200;
const int limSup = 900;
const int pwm = 200;

//Movimiento de Xbee
char w = 'w';
char s = 's';
char d = 'd';
char a = 'a';
char p = 'p';
char r = 'r';
char b = 'b';
//Nombramiento de pines

//Motores

//Motor Derecho
int E1 = 5;  
int M1 = 4; 
//Motor Izquierdo
int E2 = 6;                      
int M2 = 7;

//Servos

//Escoba
Servo servoA;
//Recogedor
Servo servoB;


//Xbee
int rx = 0;
int tx = 1;

//Ultrasonido
int trigger = 2;
int sensorTrasero = 3;
int sensorDerecha = 12;
int sensorIzquierda = 13;

//Infrarrojos
int sensorInfrarrojoA=0;
int sensorInfrarrojoB=1;

//Sensor de corriente
int sensorCorriente = 2;

//Variables globales
double lecturaCorriente = 0;
double lecturaUS1 = 0;
double lecturaUS2 = 0;
double lecturaUS3 = 0;
int lecturaInf1 = 0;
int lecturaInf2 = 0;
double cuentaA = 0;
double velocidadA = 0;
double cuentaB = 0;
double velocidadB = 0;
SoftwareSerial xbee(rx, tx); 
SimpleTimer timer;
char movimiento = 0;

//Inicializacion de elementos

void setup(){
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT);
    pinMode(trigger,OUTPUT);
    pinMode(sensorDerecha,INPUT);
    pinMode(sensorIzquierda,INPUT);
    pinMode(sensorTrasero,INPUT);
    timer.setInterval(100,calcularVelocidad); 
    xbee.begin(9600);
    servoA.attach(9);
    servoB.attach(10);
}


//Ciclo de ejecucion

void loop(){
  
  /*
  De acuerdo a la operacion del robot, se ha definido un esquema en el que se reciben comandos, se ejecutan, se
  revisan los sensores, se toman acciones de correccion y se envian los datos
  */
  
  char rx = recepcionRX();
  
  if(rx==w){
    //Movimiento hacia adelante
    moverAdelante();
  }
  else if(rx==d){
    //Movimiento hacia la derecha
    moverDerecha();
  }
  else if(rx==a){
    //Movimiento hacia la izquierda
    moverIzquierda();
  }
  else if(rx==s){
    //Movimiento hacia atras
    moverAtras();
  }
  else if(rx==p){
  analogWrite(E1,0);
  analogWrite(E2,0);
  movimiento = 0;
  }
  
  else if(rx==b){
    //Recoger
    depositar();
  }
  /*
  else if(rx==0x06){
    //Botar
    depositar();
  }
  */
  //Verificiacion de sensores
}

//Calcula la velocidad angular
void calcularVelocidad(){
  velocidadA = 2*pi*cuentaA/0.1;
  cuentaA=0;
  
  velocidadB = 2*pi*cuentaB/0.1;
  cuentaB=0;
}

//Calcula la corriente sensada segun la linealizacion
//y calibracion

double leerCorriente(){
    //Lectura analoga
   double voltajeMedido = analogRead(sensorCorriente);
   double conversion = voltajeMedido*5/1023;
   
   //Estimacion y envio
   return (conversion + 0.9997)/1.9821;
}

//Calcula la distancia sensada por el sensor trasero,
//segun linealizacion

double leerSensorTrasero(){
  //Activa el trigger para el envio de la onda ultrasonica
  digitalWrite(trigger,HIGH);
  delay(5);
  digitalWrite(trigger,LOW);
  
  //Lectura del sensor y medicion de ancho de pulso
  double distancia = pulseIn(sensorTrasero,HIGH);
  
  //Estimacion y envio
  return 0.0169*distancia - 0.9637;
}

//Calcula la distancia sensada por el sensor derecho,
//segun linealizacion

double leerSensorDerecho(){
  //Activa el trigger para el envio de la onda ultrasonica
  digitalWrite(trigger,HIGH);
  delay(5);
  digitalWrite(trigger,LOW);
  
  //Lectura del sensor y medicion de ancho de pulso
  double distancia = pulseIn(sensorDerecha,HIGH);
  
  //Estimacion y envio
  return 0.0169*distancia - 0.9637;
}

//Calcula la distancia sensada por el sensor izquierdo,
//segun linealizacion

double leerSensorIzquierdo(){
  //Activa el trigger para el envio de la onda ultrasonica
  digitalWrite(trigger,HIGH);
  delay(5);
  digitalWrite(trigger,LOW);
  
  //Lectura del sensor y medicion de ancho de pulso
  double distancia = pulseIn(sensorIzquierda,HIGH);
  
  //Estimacion y envio
  return 0.0169*distancia - 0.9637;
}

char recepcionRX(){
  return xbee.read();
}

void moverAdelante(){
  digitalWrite(M1,HIGH);
  digitalWrite(M2,HIGH);
  //Valor de PWM por determinar
  analogWrite(E1,pwm);
  analogWrite(E2,pwm);
  movimiento = 1;

}

void moverDerecha(){
  digitalWrite(M1,LOW);
  digitalWrite(M2,HIGH);
  analogWrite(E1,pwm);
  analogWrite(E2,pwm);
  movimiento = 1;

}

void moverIzquierda(){
  digitalWrite(M1,HIGH);
  digitalWrite(M2,LOW);
  analogWrite(E1,pwm);
  analogWrite(E2,pwm);
  movimiento = 1;

}

void moverAtras(){
  digitalWrite(M1,LOW);
  digitalWrite(M2,LOW);
  analogWrite(E1,pwm);
  analogWrite(E2,pwm);
  movimiento = 1;

}

void recoger(){
   
}

void depositar(){
   int lecturaServo = servoB.read();
   xbee.print("Deposita");
   if(!movimiento){
     if(lecturaServo==0)
       servoB.write(180);
      else
      servoB.write(0);
   }
}






