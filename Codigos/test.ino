/*
  Código para leer un sensor de peso (celda de carga) con el amplificador HX711
  usando una placa ESP8266.
  
  Conexiones para ESP8266 (NodeMCU):
  ---------------------------------------------------------
  HX711 DT  -> ESP8266 Pin D2 (GPIO4)
  HX711 SCK -> ESP8266 Pin D1 (GPIO5)
  HX711 VCC -> 3.3V del ESP8266 luego se usara la fuente entregada
  HX711 GND -> GND del ESP8266
  ---------------------------------------------------------
*/

#include "HX711.h"


const int DOUT_PIN = 4; // Pin de DATOS (DT).
const int SCK_PIN = 5;  // Pin de RELOJ (SCK).


HX711 scale;

// --------------------------------------------------------------------
float calibration_factor = -22050.0;

void setup() {
  // Iniciar la comunicación serie
  Serial.begin(9600);
  
  // Inicializar el sensor
  scale.begin(DOUT_PIN, SCK_PIN);
  Serial.println("Realizando tara automática...");

  scale.set_scale(calibration_factor); 

  scale.tare(); // Poner la balanza a cero

  Serial.println("Tara completada");
}

void loop() {
  if (scale.is_ready()) {
    // lectura en kg
    float reading = scale.get_units(10);
    
    // Imprimir el valor en el Monitor Serie
    Serial.print("Peso: ");
    Serial.print(reading, 3); // Imprime con 3 decimales
    Serial.println(" kg");
    
  } else {
    Serial.println("HX711 no encontrado. Verifique las conexiones.");
  }
  
  delay(500);
}
