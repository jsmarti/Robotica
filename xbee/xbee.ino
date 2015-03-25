#include <SoftwareSerial.h>
SoftwareSerial xbee(0, 1);                                    // RX, TX
char on = 'i';                                                // Carácter para prender el led
char off = 'o';                                               // Carácter para apagar el led
int lectura = 0; 
int led = 7;                                                                     
void setup()  {
        Serial.begin(9600);                                   // Baud rate monitor serial
        Serial.println( "XBees conectados" );
        pinMode(led, OUTPUT);                        // Led como salida
        xbee.begin(9600);                            // Baud rate Xbee
}
void loop()  {
        lectura = xbee.read();                       // Lectura del xbee
        if( lectura == on){
              digitalWrite(led, HIGH);               // Prender el led
              Serial.println( lectura );             // Imprimir lectura en el monitor serial
              Serial.println( "LED ON" );
        }
        if(lectura == off)
        {
            digitalWrite(led, LOW);                   // Apagar el led
            Serial.println( lectura );                // Imprimir en el monitor serial
            Serial.println( "LED OFF" );
        }
        delay( 100);
}
