            #include <SimpleTimer.h>
            const double pi = 3.1416;
            //Entrada de sensor
            int inAnalogo = 0;
            int inAnalogo2 = 1;
            //Conteo de giros
            double count =0;
            double count2=0;
            //valor leido por le pin analogo
            int val = 0;
            int val2=0;
            //Variable para realizar el cambio de estado
            char t=0;
            char t2=0;
            //Velocidad
            double velocidad=0;
            double velocidad2=0;
            //Timer
            SimpleTimer timer;
            //Motores

            //Motor Derecho
            int E1 = 5;  
            int M1 = 4; 
            //Motor Izquierdo
            int E2 = 6;                      
            int M2 = 7;
            
            void setup(){
              timer.setInterval(500,calcularVelocidad);
              pinMode(M1, OUTPUT);   
              pinMode(M2, OUTPUT);
              Serial.begin(9600); 
            }
            
            
            void loop(){
              timer.run();
              digitalWrite(M1,HIGH);
              digitalWrite(M2,HIGH);
              analogWrite(E1,200);
              analogWrite(E2,200);
              val = analogRead(inAnalogo);
              val2 = analogRead(inAnalogo2);
              if(val>900 && t){
              count++;
              t = 0;
              }
              
              if(val<200){
                t=1;
              }
              
              if(val2>900 && t2){
              count2++;
              t2 = 0;
              }
              
              if(val2<200){
                t2=1;
              }
            
            }
            
            void calcularVelocidad(){
              velocidad = pi*count/0.5;
              velocidad2 = pi*count2/0.5;
              Serial.println("Velocidad1:");
              Serial.println(velocidad);
              Serial.println("Velocidad2:");
              Serial.println(velocidad2);
              count=0;
              count2=0;
              
            }
