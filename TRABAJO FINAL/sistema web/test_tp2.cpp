#include <dummy.h>

#include <HX711.h>

#define DT1  12        // D6
#define SCK1 14        // D5

#define DT2  4        // D2
#define SCK2 5        // D1

#define DT3  13        // D7
#define SCK3 15        // D0 D8

HX711 scale1;
HX711 scale2;
HX711 scale3;

void setup() {
  
  Serial.begin(115200);

  pinMode(SCK1, OUTPUT);
  pinMode(SCK2, OUTPUT);
  pinMode(SCK3, OUTPUT);
  scale1.begin(DT1, SCK1);
  scale2.begin(DT2, SCK2);
  scale3.begin(DT3, SCK3);
  digitalWrite(SCK1, LOW);  

  Serial.println("START");

  while (!scale1.is_ready()) yield();
  scale1.tare(10);

  while (!scale2.is_ready()) yield();
  scale2.tare(10);

  while (!scale3.is_ready()) yield();
  scale3.tare(10);

  scale1.set_scale(-1886.49 );//   galga 1            (-332590+173653)/84.25=-1886.49                 tare= -173653 | 40918 | 116279 | 
  scale2.set_scale(-1822.17);//       galga 2        (-112600-40918)/84.25=-1822.17
  scale3.set_scale(714.46);//          galga 3      (176473-116279)/84.25=714.46

}

void loop() {
  long v=0;



  if (!scale1.is_ready()) {
    Serial.print("NO READY | ");
  } else {
    v = scale1.get_value(5);
    Serial.print(v);
    Serial.print(" | ");
  }

    if (!scale2.is_ready()) {
    Serial.print("NO READY | ");
  } else {
    v = scale2.get_value(5);
    Serial.print(v);
    Serial.print(" | ");
  }

    if (!scale3.is_ready()) {
    Serial.println("NO READY | ");
  } else {
    v = scale3.get_value(5);
    Serial.print(v);
    Serial.println(" | ");
  }

  delay(500);
  yield();
}
