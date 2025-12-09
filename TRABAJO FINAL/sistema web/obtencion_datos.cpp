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

const float x1= 0.0;
const float y1_1= 4.85;

const float x2= -2.425;
const float y2= -4.20;

const float x3=  2.425;
const float y3= -4.20;

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

  scale1.set_scale(-2056.29 );//   galga 1            (-332590+173653)/84.25=-1886.49                 tare= -173653 | 40918 | 116279 | 
  scale2.set_scale(-1384.04);//       galga 2        (-112600-40918)/84.25=-1822.17
  scale3.set_scale(837.73);//          galga 3      (176473-116279)/84.25=714.46

//28.08333333
//29.964 | 21.087 | 31.771 | 


//calculo para scale en centro
//-57747.000 | -38868.000 | 23526.000 | diviidido por 28.083
//scales centro: -2056.29 | -1384.04 | 837.73


//en un extremo 62.968 + 9.855 + 7.844 = 80.667

//cerca del centro 29.956 + 26.282 + 27.601 = 83.839, muy preciso
}
float f1=0;
float f2=0;
float f3=0;
void loop() {
  long v=0;


  if (!scale1.is_ready()) {
    Serial.print("NO READY | ");
  } else {
    f1 = scale1.get_units(5);
    Serial.print(f1,3);
    Serial.print(" | ");
  }

    if (!scale2.is_ready()) {
    Serial.print("NO READY | ");
  } else {
    f2 = scale2.get_units(5);
    Serial.print(f2,3);
    Serial.print(" | ");
  }

    if (!scale3.is_ready()) {
    Serial.println("NO READY | ");
  } else {
    f3 = scale3.get_units(5);
    Serial.print(f3,3);
    Serial.print(" | ");
  }

  float W = f1 + f2 + f3;

  //CALCULO DE POSICION X,Y
  float x = 0;
  float y = 0;

  if (W != 0) {  //evitar división por cero
    x = (f1 * x1 + f2 * x2 + f3 * x3) / W;
    y = (f1 * y1_1 + f2 * y2 + f3 * y3) / W;
  }

  // ---- PRINT FINAL ----
  Serial.print("SUM=");
  Serial.print(W, 3);
  Serial.print(" | X=");
  Serial.print(x, 3);
  Serial.print(" | Y=");
  Serial.println(y, 3);


  delay(500);
  yield();
}
