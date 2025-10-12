//proximos test para las 3 celdas de cargas
  #include <Arduino.h>
  #include "HX711.h"
  #include <CoM_Scale.h>

  // Celda 1
  #define HX1_DOUT D0
  #define HX1_SCK  D1
  // Celda 2
  #define HX2_DOUT D2
  #define HX2_SCK  D3
  // Celda 3
  #define HX3_DOUT D4
  #define HX3_SCK  D5


  static const unsigned SAMPLE_MS = 100;

  CoM_Scale balanza(HX1_SCK, HX1_DOUT,
                    HX2_SCK, HX2_DOUT,
                    HX3_SCK, HX3_DOUT);

  // Imprime mm con 1 dec y cm con 2 dec
  static void print_mm_and_cm(float mm) {
    const float cm = mm / 10.0f;
    Serial.print(mm, 1);
    Serial.print(F(" mm ("));
    Serial.print(cm, 2);
    Serial.print(F(" cm)"));
  }

  void setupWebPage() {
    // Placeholder, se unira luego con la parte de la web
  }

  void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.setDebugOutput(false);

    balanza.begin();
    // (Opcional) para ajuste manual
    // balanza.setGains(1.00f, 1.00f, 1.00f);

    setupWebPage();
  }

  static unsigned long t0 = 0;

  void loop() {
    if (millis() - t0 < SAMPLE_MS) return;
    t0 = millis();

    balanza.update();
    const Resultado &res = balanza.getResultado();

    Serial.print(F("CoM: x="));
    print_mm_and_cm(res.x_mm);
    Serial.print(F("  y="));
    print_mm_and_cm(res.y_mm);

    Serial.print(F("  ||  Total_comp~ "));
    Serial.print(res.total_comp_g, 1);
    Serial.print(F(" g  Total_signed~ "));
    Serial.print(res.total_signed_g, 1);
    Serial.print(F(" g"));

    if (!res.valido)      Serial.print(F("  || (Inestable)"));
    if (res.extrapolando) Serial.print(F("  || ⚠ Extrapolando"));

    Serial.println();
  }
