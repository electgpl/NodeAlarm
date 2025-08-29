#include <SPI.h>
#include <RH_RF69.h>

#define RFM69_SCK   3
#define RFM69_MISO  20
#define RFM69_MOSI  21
#define RFM69_CS    10
#define RFM69_IRQ   0
#define RFM69_RST   1
#define RF69_FREQ   915.0
#define PIR_PIN     2
#define PIR_ID      1

RH_RF69 rf69(RFM69_CS, RFM69_IRQ);

int lastPIRState = -1; // estado anterior del PIR

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(PIR_PIN, INPUT);

  initializeRFM69();
  rf69.sleep(); // arrancar dormido

  Serial.println("Iniciando prueba PIR sin deep sleep...");
}

void loop() {
  int pirState = digitalRead(PIR_PIN);

  if (pirState != lastPIRState) {
    // Hubo cambio de estado
    Serial.print("Cambio detectado: ");
    Serial.println(pirState ? "HIGH" : "LOW");

    rf69.setModeIdle(); // despertar RF69
    sendPIRState(pirState);
    rf69.sleep();       // volver a dormir RF69

    lastPIRState = pirState;
  }

  delay(10); // pequeña pausa para evitar rebotes
}

void initializeRFM69() {
  SPI.begin(RFM69_SCK, RFM69_MISO, RFM69_MOSI);
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);
  delay(100);
  digitalWrite(RFM69_RST, HIGH);
  delay(100);
  digitalWrite(RFM69_RST, LOW);
  delay(100);

  if (!rf69.init()) {
    Serial.println("Fallo init RF69");
    while (1);
  }

  uint8_t version = rf69.spiRead(0x10);
  if (version != 0x24) {
    Serial.println("Version RF69 incorrecta");
    while (1);
  }

  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("Fallo setFrequency");
    while (1);
  }

  rf69.setTxPower(20, true);
  rf69.spiWrite(0x02, 0x00);
  rf69.setModeRx();
}

void sendPIRState(int state) {
  char message[8];
  snprintf(message, sizeof(message), "P%d,%d", PIR_ID, state);

  if (rf69.send((uint8_t*)message, strlen(message))) {
    rf69.waitPacketSent();
    Serial.print("Enviado: ");
    Serial.println(message);
  }
}
