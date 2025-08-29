#include <SPI.h>
#include <RH_RF69.h>
#include "esp_sleep.h"
#include "driver/gpio.h"

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

void setup() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  Serial.print("DESPERTAR: ");
  switch(wakeup_reason) {
    case ESP_SLEEP_WAKEUP_GPIO: 
      Serial.println("GPIO - PIR"); 
      break;
    case ESP_SLEEP_WAKEUP_TIMER: 
      Serial.println("TIMER"); 
      break;
    default: 
      Serial.println("POWER ON"); 
      break;
  }
  initializeRFM69();
  pinMode(PIR_PIN, INPUT);
  delay(100);
  int pirState = digitalRead(PIR_PIN);
  sendPIRState(pirState);
  setupGPIOWakeup();
  rf69.sleep();
  esp_sleep_enable_timer_wakeup(10000000);
  esp_sleep_enable_gpio_wakeup();
  int currentPIR = digitalRead(PIR_PIN);
  if (currentPIR == HIGH) {
    gpio_wakeup_enable(GPIO_NUM_2, GPIO_INTR_NEGEDGE);
  } else {
    gpio_wakeup_enable(GPIO_NUM_2, GPIO_INTR_POSEDGE);
  }
  delay(100);
  esp_light_sleep_start();
  delay(1000);
  ESP.restart();
}
void loop() {
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
    ESP.restart();
  }
  uint8_t version = rf69.spiRead(0x10);
  if (version != 0x24) {
    ESP.restart();
  }
  if (!rf69.setFrequency(RF69_FREQ)) {
    ESP.restart();
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
  }
}
void setupGPIOWakeup() {
  int currentState = digitalRead(PIR_PIN);
  esp_sleep_enable_gpio_wakeup();
  if (currentState == HIGH) {
    gpio_wakeup_enable(GPIO_NUM_2, GPIO_INTR_NEGEDGE);
  } else {
    gpio_wakeup_enable(GPIO_NUM_2, GPIO_INTR_POSEDGE);
  }
}