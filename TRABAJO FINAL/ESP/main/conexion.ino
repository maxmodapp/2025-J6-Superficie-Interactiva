//#include <ESP8266WiFi.h>
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
//#include <ArduinoJson.h>

// #include <ESP8266mDNS.h>

//CONFIG
const char* WIFI_SSID = "Pupi";
const char* WIFI_PASS = "44487105az";
const uint16_t HTTP_PORT = 80;
const char* WS_PATH = "/ws";

unsigned long PUSH_MS = 100;   // 100 ms = 10 Hz

//GLOBAL
AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws(WS_PATH);

//definidas en scales.ino
extern float g_x;
extern float g_y;
extern float g_W;

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
      msg.reserve(len + 1);                    
      msg.concat((const char*)data, len);        

      Serial.printf("[WS] RX: %s\n", msg.c_str());
      if (msg == "ping") client->text("{\"type\":\"pong\"}");

    }
  }
}
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.printf("Conectando a %s", WIFI_SSID);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 80) {
    delay(250);
    Serial.print(".");
    attempts++;
    yield(); 
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi OK. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("No se pudo conectar al WiFi (revisá SSID/clave).");
  }
}
void initWebSocket() {
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  //prueba
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
    req->send(200, "text/plain", "WS en /ws (testing X,Y,peso)");
  });

  server.begin();
  Serial.println("Servidor WS iniciado");
}


void sendPositionWS() {
  unsigned long now = millis();
  if (now - lastPush < PUSH_MS) return;  
  lastPush = now;

  //usamos las variables calculadas en scales.ino:
  //g_x, g_y, g_W (definidas ahí como globales)
  StaticJsonDocument<128> doc;
  doc["type"] = "pos";
  doc["x"] = g_x;
  doc["y"] = g_y;
  doc["w"] = g_W;

  char out[128];
  size_t n = serializeJson(doc, out, sizeof(out));
  ws.textAll(out, n);
}

/*
void setup() {
  Serial.begin(115200);
  delay(100);

  initWiFi();
  initWebSocket();
  Serial.println("Setup completo");
}


void loop() {
  unsigned long now = millis();
  if (now - lastPush >= PUSH_MS) {
    lastPush = now;

    //simulador de datos
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

    yield();
  }
}

*/