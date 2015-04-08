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
#include <PID_v1.h>

//Constantes 
const double pi = 3.1416;
const int limInf = 300;
const int limSup = 500;
double pwm = 200;
double Kp = 0.205223358690436;
double Ki = 0.39641582244666;
double Kd = 0.0121427569545024;
//Variable temporal!!! Caracterizar la velocidad:
const double KA = 5.509;
const double KB = 7.345;

//Movimiento de Xbee
char w = 'w';
char s = 's';
char d = 'd';
char a = 'a';
char p = 'p';
char r = 'r';
char b = 'b';
char u = 'u';

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
double cuentaA = 0;
double velocidadA = 0;
double cuentaB = 0;
double velocidadB = 0;
SoftwareSerial xbee(rx, tx); 
SimpleTimer timer;
char movimiento = 0;
char tA=0;
char tB=0;
double encoder1=0;
double encoder2=0;
double pwmMotor1=0;
double pwmMotor2=0;
PID controlMotor1(&encoder1,&pwmMotor1,&pwm,Kp,Ki,Kd,DIRECT);
PID controlMotor2(&encoder2,&pwmMotor2,&pwm,Kp,Ki,Kd,DIRECT);

//Inicializacion de elementos

void setup(){
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT);
    pinMode(trigger,OUTPUT);
    pinMode(sensorDerecha,INPUT);
    pinMode(sensorIzquierda,INPUT);
    pinMode(sensorTrasero,INPUT);
    timer.setInterval(500,calcularVelocidad); 
    xbee.begin(9600);
    servoA.attach(9);
    servoB.attach(10);
    controlMotor1.SetMode(AUTOMATIC);
    controlMotor2.SetMode(AUTOMATIC);
    Serial.begin(9600);
}


//Ciclo de ejecucion

void loop(){
  
  /*
  De acuerdo a la operacion del robot, se ha definido un esquema en el que se
  revisan los sensores, se reciben comandos, se ejecutan, se toman acciones de correccion y se envian los datos
  */
  
  //Verificiacion de sensores
  
  //Velocidad
  timer.run();
  contarVueltasA();
  contarVueltasB();
  
  
  char rx = recepcionRX();
    if(rx==w){
    //Movimiento hacia adelante
    conversionVelocidad();
    moverAdelante();
    }
    else if(rx==d){
      //Movimiento hacia la derecha
      conversionVelocidad();
      moverDerecha();
    }
    else if(rx==a){
      //Movimiento hacia la izquierda
      conversionVelocidad();
      moverIzquierda();
    }
    else if(rx==s){
      //Movimiento hacia atras
      conversionVelocidad();
      moverAtras();
    }
    else if(rx==p){
    analogWrite(E1,0);
    analogWrite(E2,0);
    movimiento = 0;
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
  
}

//Calcula la velocidad angular
void calcularVelocidad(){
  
  velocidadA = pi*cuentaA/0.5;
  cuentaA=0;
  
  velocidadB = pi*cuentaB/0.5;
  cuentaB=0;
  
  
}

//Calcula la corriente sensada segun la linealizacion
//y calibracion

double leerCorriente(){
    //Lectura analoga
   double voltajeMedido = analogRead(sensorCorriente);
   double conversion = voltajeMedido*5/1023;
   
   //Estimacionc y envio
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
  controlMotor1.Compute();
  controlMotor2.Compute();
  analogWrite(E1,pwmMotor1);
  analogWrite(E2,pwmMotor2);
  movimiento = 1;

}

void moverDerecha(){
  lecturaUSDerecha = leerSensorDerecho();
  if(lecturaUSDerecha <10){
    Serial.print("Obstaculo a la derecha");
    analogWrite(E1,0);
    analogWrite(E2,0);
  }
  digitalWrite(M1,LOW);
  digitalWrite(M2,HIGH);
  controlMotor1.Compute();
  controlMotor2.Compute();
  analogWrite(E1,0);
  analogWrite(E2,pwmMotor2);
  movimiento = 1;

}

void moverIzquierda(){
  lecturaUSIzquierda = leerSensorIzquierdo();
  if(lecturaUSIzquierda <10){
    Serial.print("Obstaculo a la izquierda");
    analogWrite(E1,0);
    analogWrite(E2,0);
  }
  digitalWrite(M1,HIGH);
  digitalWrite(M2,LOW);
  controlMotor1.Compute();
  controlMotor2.Compute();
  analogWrite(E1,pwmMotor1);
  analogWrite(E2,0);
  movimiento = 1;

}

void moverAtras(){
  lecturaUSAtras = leerSensorTrasero();
  if(lecturaUSAtras <10){
    Serial.print("Le va a dar!!");
    analogWrite(E1,0);
    analogWrite(E2,0);
  }
  else{
  digitalWrite(M1,LOW);
  digitalWrite(M2,LOW);
  controlMotor1.Compute();
  controlMotor2.Compute();
  analogWrite(E1,pwmMotor1);
  analogWrite(E2,pwmMotor2);
  movimiento = 1;
  }

}

void recoger(){
  int lecturaServoA = servoA.read();
   xbee.print("Recoge");
   if(!movimiento){
     if(lecturaServoA==0)
       servoA.write(180);
      else
      servoA.write(0);
   }
   
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

//Cuenta las vueltas de la rueda A
void contarVueltasA(){
  int val = analogRead(sensorInfrarrojoA);
  if(val>limSup && tA){
  cuentaA++;
  tA = 0;
  }
              
  if(val<limInf){
    tA=1;
  }
  
}

//Cuenta las vueltas de la rueda B
void contarVueltasB(){
  int val = analogRead(sensorInfrarrojoB);
  if(val>limSup && tB){
  cuentaB++;
  tB = 0;
  }
              
  if(val<limInf){
    tB=1;
  }
  
}

//Realiza la conversion de radianes por segundo a pwm para el control de velocidad
//Se hace para ambos motores
void conversionVelocidad(){
  encoder2 = KA*velocidadA;
  encoder1 = KA*velocidadB;

}

void posicionarPala(){
  int lecturaServo = servoB.read();
   xbee.print("Deposita");
   if(!movimiento){
     if(lecturaServo==0)
       servoB.write(25);
      else
      servoB.write(0);
   }
}






