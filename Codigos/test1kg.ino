/*
  Código para leer un sensor de peso (celda de carga) de 1 kg con el amplificador HX711
  usando una placa ESP8266 y la librería de Bogdan Necula.
  Versión final de medición con salida en gramos y precisión de 0.1g.
*/

#include "HX711.h"

// Pines de conexión para el HX711
const int DOUT_PIN = 4; // Pin de DATOS (DT).
const int SCK_PIN = 5;  // Pin de RELOJ (SCK).

// Instancia del objeto HX711
HX711 scale;


// Con este valor, la función scale.get_units() devuelve el peso en gramos.
float calibration_factor = -708.0; 

void setup() {
  // Iniciar la comunicación serie
  Serial.begin(9600);
  Serial.println("Inicializando balanza...");

  // Inicializar el sensor
  scale.begin(DOUT_PIN, SCK_PIN);
  
  Serial.println("Modo Medicion (Gramos con precision de 0.1g).");
  scale.set_scale(calibration_factor); // Ajustar el factor de calibración
  scale.tare(20);                      // Poner la balanza a cero con una tara más precisa
  
  Serial.println("Tara completada. La balanza esta lista para usar.");
}

void loop() {
  if (scale.is_ready()) {
    // Lectura en gramos (la calibración se hizo para que get_units devuelva gramos)
    float peso_en_gramos = scale.get_units(10); // Promediar 10 lecturas para mayor estabilidad

    // Imprimir el valor en el Monitor Serie
    Serial.print("Peso: ");

    // Si la lectura es muy cercana a cero (menor a 0.05g), la mostramos como 0.0
    // Esto evita mostrar pequeños valores negativos como "-0.0 g" cuando no hay peso.
    if (abs(peso_en_gramos) < 0.05) { 
        peso_en_gramos = 0.0;
    }
    
    Serial.print(peso_en_gramos, 1); // Imprime el valor con 1 decimal de precisión
    Serial.println(" g");

  } else {
    Serial.println("HX711 no encontrado. Verifique las conexiones.");
  }
  delay(500); // Espera medio segundo entre lecturas
}