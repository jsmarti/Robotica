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
int pwm = 200;

//Movimiento de Xbee
char w = 'w';
char s = 's';
char d = 'd';
char a = 'a';
char p = 'p';
char r = 'r';
char b = 'b';
char u = 'u';
char z = 'z';
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
double lecturaUSDerecha = 0;
double lecturaUSIzquierda = 0;
double lecturaUSAtras = 0;
int lecturaInf1 = 0;
int lecturaInf2 = 0;
double cuentaA = 0;
double velocidadA = 0;
double cuentaB = 0;
double velocidadB = 0;
SoftwareSerial xbee(rx, tx); 
SimpleTimer timer;
char movimiento = 0;
char atras = 0;
char tA=0;
char tB=0;


//Inicializacion de elementos

void setup(){
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT);
    pinMode(trigger,OUTPUT);
    pinMode(sensorDerecha,INPUT);
    pinMode(sensorIzquierda,INPUT);
    pinMode(sensorTrasero,INPUT);
    timer.setInterval(3000,calcularVelocidad); 
    xbee.begin(9600);
    servoA.attach(9);
    servoB.attach(10);
    servoA.write(0);
    servoB.write(0);
}


//Ciclo de ejecucion

void loop(){
  
  /*
  De acuerdo a la operacion del robot, se ha definido un esquema en el que se
  revisan los sensores, se reciben comandos, se ejecutan, se toman acciones de correccion y se envian los datos
  */
  
  //Velocidad
  timer.run();
  contarVueltasA();
  contarVueltasB();
  
  char rx = recepcionRX();    
    if(rx==p){
      analogWrite(E1,0);
      analogWrite(E2,0);
      movimiento = 0;
      xbee.println(String(lecturaCorriente) + ":" + "P"+":"+String((velocidadA)/2));
    }

  lecturaCorriente = leerCorriente()/2;  
  if(lecturaCorriente <= 3){
    
    if(rx=='1'){
      pwm=100;
      xbee.println(String(lecturaCorriente) + ":" + "1"+":"+String((velocidadA)/2));
    }
    else if(rx == '2'){
      pwm=150;
      xbee.println(String(lecturaCorriente) + ":" + "2"+":"+String((velocidadA)/2));
    }
    else if(rx == '3'){
      pwm=200;
      xbee.println(String(lecturaCorriente) + ":" + "3"+":"+String((velocidadA)/2));
    }
    else if(rx==w){
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
    else if(rx==b){
      //Depositoar
      depositar();
    }
    else if(rx==r){
      //recoger
      recoger();
    }
    else if(rx==u){
      //Pone la pala en posicion de recoger
      posicionarPala();
    }
    else if(rx == z){
      procesarSensores();
    }
    else{
      if(movimiento){
        if(atras){
          verificarAtras();
        }
      }
    }
    
  }
  
  else{
    xbee.println(String(lecturaCorriente) + ":" + "SC"+":"+String((velocidadA)/2));
    analogWrite(E1,0);
    analogWrite(E2,0);
  }
  
  
}

//Calcula la velocidad angular
void calcularVelocidad(){
  
  velocidadA = 2*pi*cuentaA/3;
  cuentaA=0;
  
  velocidadB = 2*pi*cuentaB/3;
  cuentaB=0;
  
  
}

//Calcula la corriente sensada segun la linealizacion
//y calibracion

double leerCorriente(){
    //Lectura analoga
   double voltajeMedido = analogRead(sensorCorriente);
   double conversion = voltajeMedido*5/1023;
   
   //Estimacion y envio
   return (conversion - 0.2277)/0.9918;
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

  int lecturaServoA = servoA.read();
  int lecturaServoB = servoB.read();
  
  if(lecturaServoA==0 && lecturaServoB==0 ){
  analogWrite(E1,pwm);
  analogWrite(E2,pwm);
  movimiento = 1;
  xbee.println(String(lecturaCorriente) + ":" + "F"+":"+String((velocidadA)/2));
  }
  else{
  xbee.println(String(lecturaCorriente) + ":" + "PPE"+":"+String((velocidadA)/2));
  }

}

void moverDerecha(){
  lecturaUSDerecha = leerSensorDerecho();
  if(lecturaUSDerecha <10){
    analogWrite(E1,0);
    analogWrite(E2,0);
    xbee.println(String(lecturaCorriente) + ":" + "OD"+":"+String((velocidadA)/2));
  }
  else{
  digitalWrite(M1,LOW);
  digitalWrite(M2,HIGH);
  int lecturaServoA = servoA.read();
  int lecturaServoB = servoB.read();
  if(lecturaServoA==0 && lecturaServoB==0 ){
  analogWrite(E1,pwm);
  analogWrite(E2,pwm+55);
  movimiento = 1;

  xbee.println(String(lecturaCorriente) + ":" + "D"+":"+String((velocidadA)/2));
  }
  else{
  xbee.println(String(lecturaCorriente) + ":" + "PPE"+":"+String((velocidadA)/2));
  }
  }

}

void moverIzquierda(){
  lecturaUSIzquierda = leerSensorIzquierdo();
  if(lecturaUSIzquierda <10){
    analogWrite(E1,0);
    analogWrite(E2,0);
    xbee.println(String(lecturaCorriente) + ":" + "OI"+":"+String((velocidadA)/2));
  }
  else{
  
  digitalWrite(M1,HIGH);
  digitalWrite(M2,LOW);
  
  int lecturaServoA = servoA.read();
  int lecturaServoB = servoB.read();
  if(lecturaServoA==0 && lecturaServoB==0 ){
  analogWrite(E1,pwm+55);
  analogWrite(E2,pwm);
  movimiento = 1;

  xbee.println(String(lecturaCorriente) + ":" + "I"+":"+String((velocidadA)/2));
  }
  else{
  xbee.println(String(lecturaCorriente) + ":" + "PPE"+":"+String((velocidadA)/2));
  }
  }

}

void moverAtras(){
  lecturaUSAtras = leerSensorTrasero();
  if(lecturaUSAtras <10){
    analogWrite(E1,0);
    analogWrite(E2,0);
    xbee.println(String(lecturaCorriente) + ":" + "OT"+":"+String((velocidadA)/2));
  }
  else{
  digitalWrite(M1,LOW);
  digitalWrite(M2,LOW);
  int lecturaServoA = servoA.read();
  int lecturaServoB = servoB.read();
  if(lecturaServoA==0 && lecturaServoB==0 ){
  analogWrite(E1,pwm);
  analogWrite(E2,pwm);
  movimiento = 1;
  atras = 1;
  xbee.println(String(lecturaCorriente) + ":" + "B"+":"+String((velocidadA)/2));
  }
  else{
  xbee.println(String(lecturaCorriente) + ":" + "PPE"+":"+String((velocidadA)/2));
  }
  }

}

void recoger(){
   int lecturaServo = servoA.read();
   if(!movimiento){
     if(lecturaServo==0)
     {
       servoA.write(180);
       xbee.println(String(lecturaCorriente) + ":" + "R"+":"+String((velocidadA)/2));
     }
      else
      {
      servoA.write(0);
      xbee.println(String(lecturaCorriente) + ":" + "MR"+":"+String((velocidadA)/2));
      }
   }
}

void depositar(){
   int lecturaServo = servoB.read();
   if(!movimiento){
     if(lecturaServo==0)
     {
       servoB.write(180);
       xbee.println(String(lecturaCorriente) + ":" + "DP"+":"+String((velocidadA)/2));
     }
      else
      {
      servoB.write(0);
      xbee.println(String(lecturaCorriente) + ":" + "MB"+":"+String((velocidadA)/2));
      }
   }
}

void contarVueltasA(){
  int val = analogRead(sensorInfrarrojoA);
  if(val>400 && tA){
  cuentaA++;
  tA = 0;
  }
              
  if(val<250){
    tA=1;
  }
  
}

void contarVueltasB(){
  int val = analogRead(sensorInfrarrojoB);
  if(val>400 && tB){
  cuentaB++;
  tB = 0;
  }
              
  if(val<250){
    tB=1;
  }
  
}

void posicionarPala(){
  int lecturaServo = servoB.read();
   if(!movimiento){
     if(lecturaServo==0)
     {
       servoB.write(25);
       xbee.println(String(lecturaCorriente) + ":" + "U"+":"+String((velocidadA)/2));
     }
      else
      {
      servoB.write(0);
      xbee.println(String(lecturaCorriente) + ":" + "MU"+":"+String((velocidadA)/2));
      }
   }
}

void procesarSensores(){
  lecturaCorriente = leerCorriente();
  lecturaUSIzquierda = leerSensorIzquierdo();
  lecturaUSDerecha = leerSensorDerecho();
  lecturaUSAtras = leerSensorTrasero();
  String respuesta = String(lecturaCorriente)+":Z:"; 
  if(lecturaUSDerecha <10){
    respuesta += "O:";
  }
  else{
    respuesta += "L:";
  }

  if(lecturaUSAtras <10){
    respuesta += "O:";
  }
  else{
    respuesta += "L:";
  }
    if(lecturaUSIzquierda <10){
    respuesta += "O";
  }
  else{
     respuesta += "L";
  }
  
  xbee.println(respuesta);

}

void verificarAtras(){
  lecturaUSAtras = leerSensorTrasero();
  if(lecturaUSAtras <10){
    analogWrite(E1,0);
    analogWrite(E2,0);
    atras = 0;
    xbee.println(String(lecturaCorriente) + ":" + "OT"+":"+String((velocidadA)/2));
  }
  else{}
}












