            #include <SimpleTimer.h>
            const double pi = 3.1416;
            //Entrada de sensor
            int inAnalogo = 0;
            //Conteo de giros
            double count =0;
            //valor leido por le pin analogo
            int val = 0;
            //Variable para realizar el cambio de estado
            char t=0;
            //Velocidad
            double velocidad=0;
            //Timer
            SimpleTimer timer;
            
            void setup(){
              timer.setInterval(1000,calcularVelocidad);
              Serial.begin(9600); 
            }
            
            
            void loop(){
              timer.run();
              val = analogRead(inAnalogo);
              if(val>900 && t){
              count++;
              t = 0;
              }
              
              if(val<200){
                t=1;
              }
            
            }
            
            void calcularVelocidad(){
              velocidad = 2*pi*count/1;
              count=0;
              Serial.println(velocidad);
            }
