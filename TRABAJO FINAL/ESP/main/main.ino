// main.ino
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <dummy.h>
#include <HX711.h>

void setup() {
  Serial.begin(115200);
  delay(100);

  initWiFi();        // conexion.ino
  initWebSocket();   // conexion.ino
  initScales();      // scales.ino

  Serial.println("Setup completo");
}

void loop() {
  //leer galgas y actualizar g_f1,g_f2,g_f3,g_W,g_x,g_y
  updateScales();

  //enviar posición/peso por WebSocket
  sendPositionWS();

  //por las dudas
  yield();
}
