t#include <DHT.h>
#define DHTPIN 9
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

float temp;
float hum;
int t=0;
int h=0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  Mirf.cePin = 7;
  Mirf.csnPin = 8;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"c0001");
  Mirf.payload = sizeof(unsigned long);
  // To change channel: Mirf.channel = 10;
  Mirf.config();
  Serial.println("Beginning ... "); 
}

void loop() {
  unsigned long time = millis();
  Mirf.setTADDR((byte *)"c0001");
  //------------------------------------------------------
  
  float converted = 0.00;
  temp=dht.readTemperature();
  hum=dht.readHumidity();
  
  if(temp!=(0||"nan")&&hum!=(0||"nan")){
     Serial.print("temperatura (C): ");
     Serial.print(temp);
     Serial.println("°C");
     Serial.print("umidità: ");
     Serial.print(hum);
     Serial.println("%");
  }
  delay (500);
  
  //-------------------------------------------------------
  
  Mirf.send((byte *)&time);
  
  while(Mirf.isSending()){}
  Serial.println("Finished sending");
  delay(10);
  
  while(!Mirf.dataReady()){
    //Serial.println("Waiting");
    if ( ( millis() - time ) > 1000 ) {
      Serial.println("Timeout on response from server!");
      return;
    }
  }
  Mirf.getData((byte *) &temp);
  Mirf.getData((byte *) &hum);
  Serial.print("Ping: ");
  Serial.println((millis() - time));
  delay(500);
}
