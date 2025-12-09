//scales.ino

//#include <dummy.h>
//#include <HX711.h>

//pines de las 3 galgas
#define DT1  12        // D6
#define SCK1 14        // D5

#define DT2  4         // D2
#define SCK2 5         // D1

#define DT3  13        // D7
#define SCK3 15        // D8

HX711 scale1;
HX711 scale2;
HX711 scale3;


//coordenadas de las galgas (triángulo) estan ubicadas a 4.4cm del centro
const float xG1 =0.0;
const float yG1 =4.4; //se posiciono el eje y sobre la galga 1

const float xG2 =-3.8104; //x2​=4.40cos(210°)=4.40(−0.8660)=−3.8104
const float yG2 =-2.2000; //y2​=4.40sin(210°)=4.40(−0.5)=−2.2000

const float xG3 =3.8104; //x3​=4.40cos(−30°)=4.40(0.8660)=3.8104
const float yG3 =-2.2000; //y3​=4.40sin(−30°)=4.40(−0.5)=−2.2000

//variables globales con el último estado medido
float g_f1 = 0;
float g_f2 = 0;
float g_f3 = 0;
float g_W  = 0;   //suma de pesos
float g_x  = 0;   //posición X
float g_y  = 0;   //posición Y

const unsigned long SCALE_MS = 200;

//inicializa las galgas
void initScales() {

  pinMode(SCK1, OUTPUT);
  pinMode(SCK2, OUTPUT);
  pinMode(SCK3, OUTPUT);

  scale1.begin(DT1, SCK1);
  scale2.begin(DT2, SCK2);
  scale3.begin(DT3, SCK3);

  Serial.println("INIT SCALES...");

  while (!scale1.is_ready()) yield();
  scale1.tare(10);

  while (!scale2.is_ready()) yield();
  scale2.tare(10);

  while (!scale3.is_ready()) yield();
  scale3.tare(10);

  //escalas calibradas 114.8 g 
  //G1=-74301.000 | G2=-72548.000 | G3=26387.000 | average-tare con masa en el centro 114.8/3=38.26
  // galga 1 -74301/38.26=-1942.002
  // galga 2 -72548/38.26=-1896.184
  // galga 3 26387/38.26=689.675


  // otra calibracion: -65339.000 | -71075.000 | 30676.000 |
  // galga 1 -65339/38.26=-1707.76
  // galga 2 -71075/38.26=-1857.68
  // galga 3 30676/38.26=801.77
  scale1.set_scale(-1707.76);  // galga 1 -74301/38.26=-1942.002
  scale2.set_scale(-1857.68);  // galga 2 -72548/38.26=-1896.184
  scale3.set_scale(801.77);   // galga 3 26387/38.26=689.675

  Serial.println("SCALES READY");
}

//lee galgas, actualiza g_f1,g_f2,g_f3,g_W,g_x,g_y y printea
void updateScales() {
  static unsigned long lastRead = 0;
  unsigned long now = millis();

  //lectura completa cada SCALE_INTERVAL_MS
  if (now - lastRead < SCALE_MS) {
    return; 
  }
  lastRead = now;


  //lecturas individuales
  if (!scale1.is_ready()) {
    //Serial.print("G1 NO READY | ");
  } else {
    g_f1 = scale1.get_units(1);
    //Serial.print("G1=");
    //Serial.print(g_f1, 3);
    //Serial.print(" | ");
  }

  if (!scale2.is_ready()) {
    //Serial.print("G2 NO READY | ");
  } else {
    g_f2 = scale2.get_units(1);
    //Serial.print("G2=");
    //Serial.print(g_f2, 3);
    //Serial.print(" | ");
  }

  if (!scale3.is_ready()) {
    //Serial.println("G3 NO READY | ");
  } else {
    g_f3 = scale3.get_units(1);
    //Serial.print("G3=");
    //Serial.print(g_f3, 3);
    //Serial.println(" | ");
  }

  //suma total de peso
  g_W = g_f1 + g_f2 + g_f3;

  //calculo de posición X,Y
  if (g_W != 0.0f) {  //evita división por cero
    g_x = (g_f1 * xG1 + g_f2 * xG2 + g_f3 * xG3) / g_W;
    g_y = (g_f1 * yG1 + g_f2 * yG2 + g_f3 * yG3) / g_W;
  }

  //Print final de debug
  //Serial.print("SUM=");
  //Serial.print(g_W, 3);
  //Serial.print(" | X=");
  //Serial.print(g_x, 3);
  //Serial.print(" | Y=");
  //Serial.println(g_y, 3);
}
