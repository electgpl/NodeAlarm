#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RH_RF69.h>

// Pines RFM69 (ajustar si usás otros)
#define RFM69_IRQ  14
#define RFM69_RST  18
#define RFM69_CS   10
#define RFM69_SCK  12
#define RFM69_MISO 13
#define RFM69_MOSI 11

#define RF69_FREQ 915.0

// OLED 128x64 por I2C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RH_RF69 rf69(RFM69_CS, RFM69_IRQ);

void setup() {
  Serial.begin(115200);
  SPI.begin(RFM69_SCK, RFM69_MISO, RFM69_MOSI);

  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);
  delay(100);
  digitalWrite(RFM69_RST, HIGH);
  delay(100);
  digitalWrite(RFM69_RST, LOW);
  delay(100);

  Serial.println("Inicializando RFM69...");
  if (!rf69.init()) {
    Serial.println("Fallo al inicializar RFM69");
    while (1);
  }

  uint8_t version = rf69.spiRead(0x10);
  Serial.print("RegVersion (0x10) = 0x");
  Serial.println(version, HEX);
  if (version != 0x24) {
    Serial.println("Error en comunicación SPI");
    while (1);
  }

  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("Fallo al configurar frecuencia");
    while (1);
  }

  rf69.setTxPower(20, true);
  rf69.setModeRx();  // Modo recepción continua

  Serial.println("Inicializando OLED...");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("No se encontró OLED");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Esperando...");
  display.display();
}

void loop() {
  if (rf69.available()) {
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf69.recv(buf, &len)) {
      buf[len] = '\0';  // Aseguramos terminación en null
      Serial.print("Recibido: ");
      Serial.println((char*)buf);

      // Mostrar en pantalla
      display.clearDisplay();
      display.setTextSize(3);
      display.setCursor(0, 0);
      display.print((char*)buf);
      display.display();
    } else {
      Serial.println("Error en recepción");
    }
  }
}