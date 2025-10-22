#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
// Opcional mDNS:
// #include <ESP8266mDNS.h>

// ======== CONFIG ========
const char* WIFI_SSID = "Pupi";
const char* WIFI_PASS = "44487105az";
const uint16_t HTTP_PORT = 80;
const char* WS_PATH = "/ws";

unsigned long PUSH_MS = 100;   // 100 ms = ~10 Hz
// Rango para X/Y pensado para tu UI: [-100..100]
const float XR = 100.0f;
const float YR = 100.0f;
// Peso simulado (kg)
const float W_BASE = 2.0f;     // ~2 kg
const float W_AMP  = 1.0f;     // ±1 kg

// ======== GLOBAL ========
AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws(WS_PATH);
unsigned long lastPush = 0;

void onWsEvent(AsyncWebSocket *server,
               AsyncWebSocketClient *client,
               AwsEventType type,
               void *arg,
               uint8_t *data,
               size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("[WS] Cliente %u conectado (%s)\n",
                  client->id(), client->remoteIP().toString().c_str());
    client->text("{\"type\":\"hello\",\"msg\":\"ESP8266 listo (testing)\"}");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("[WS] Cliente %u desconectado\n", client->id());
  } else if (type == WS_EVT_DATA) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
      
      String msg;
      msg.reserve(len + 1);                      // opcional
      msg.concat((const char*)data, len);        // copia EXACTAMENTE 'len' bytes

      Serial.printf("[WS] RX: %s\n", msg.c_str());
      if (msg == "ping") client->text("{\"type\":\"pong\"}");
      // acá podrías parsear JSON si quisieras
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.printf("Conectando a %s", WIFI_SSID);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 80) {
    delay(250);
    Serial.print(".");
    attempts++;
    yield(); // 8266: ceder CPU al WiFi
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi OK. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("No se pudo conectar al WiFi (revisá SSID/clave).");
  }

  // mDNS opcional:
  // if (MDNS.begin("esp8266")) { Serial.println("mDNS: esp8266.local"); }

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  // HTTP simple para probar
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
    req->send(200, "text/plain", "WS en /ws (testing X,Y,peso)");
  });

  server.begin();
  Serial.printf("Servidor iniciado en puerto %u. WS: %s\n", HTTP_PORT, WS_PATH);
}

void loop() {
  unsigned long now = millis();
  if (now - lastPush >= PUSH_MS) {
    lastPush = now;

    // ===== Simulador de datos =====
    static float t = 0.0f;
    t += 0.06f;
    float x = 0.7f * XR * sinf(t);
    float y = 0.9f * YR * sinf(t * 0.6f + 1.2f);
    float w = W_BASE + W_AMP * (0.5f * sinf(t * 0.8f) + 0.5f * cosf(t * 0.3f));

    // JSON: {"type":"pos","x":..,"y":..,"w":..}
    StaticJsonDocument<128> doc;
    doc["type"] = "pos";
    doc["x"] = x;
    doc["y"] = y;
    doc["w"] = w;

    char out[128];
    size_t n = serializeJson(doc, out, sizeof(out));
    ws.textAll(out, n);

    yield(); // 8266: evitar WDT
  }
}

