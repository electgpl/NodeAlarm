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
#define RF69_FREQ 915.0
#define PIR_PIN 2       // Pin digital del PIR (GPIO2)
#define PIR_ID 1        // ID del sensor PIR (ej: 1 para "P1")

// Configuración de deep sleep
#define SLEEP_TIME_US 30000000  // 30 segundos en microsegundos (opcional, como backup)

RH_RF69 rf69(RFM69_CS, RFM69_IRQ);

void setup() {
  Serial.begin(115200);
  delay(2000); // Dar tiempo para que se abra el monitor serie
  
  Serial.println("=== INICIANDO SISTEMA PIR ===");
  Serial.flush();
  
  // Verificar causa del despertar
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  
  Serial.print("Causa del despertar: ");
  switch(wakeup_reason) {
    case ESP_SLEEP_WAKEUP_GPIO: 
      Serial.println("PIR - Movimiento detectado"); 
      break;
    case ESP_SLEEP_WAKEUP_TIMER: 
      Serial.println("Timer"); 
      break;
    default: 
      Serial.println("Power ON o Reset"); 
      break;
  }
  Serial.flush();
  
  // Inicializar RFM69
  Serial.println("Paso 1: Inicializando RFM69...");
  Serial.flush();
  initializeRFM69();
  Serial.println("RFM69 inicializado correctamente");
  Serial.flush();
  
  // Configurar GPIO2 como entrada - usar pinMode como en tu código que funciona
  Serial.println("Paso 2: Configurando GPIO2...");
  Serial.flush();
  
  pinMode(PIR_PIN, INPUT);
  delay(500); // Dar tiempo para que se estabilice
  
  // Leer estado actual del PIR varias veces
  Serial.println("Leyendo PIR múltiples veces:");
  for(int i = 0; i < 5; i++) {
    int pirState = digitalRead(PIR_PIN);
    Serial.print("  Lectura ");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.println(pirState);
    delay(200);
  }
  
  int pirState = digitalRead(PIR_PIN);
  Serial.print("Estado PIR final: ");
  Serial.println(pirState);
  Serial.flush();
  
  // Si despertó por PIR o es power-on, enviar estado
  if (wakeup_reason == ESP_SLEEP_WAKEUP_GPIO || wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED) {
    Serial.println("Paso 3: Enviando estado PIR...");
    Serial.flush();
    sendPIRState(pirState);
  }
  
  // Para debug: mostrar qué GPIOs causaron el wake-up
  if (wakeup_reason == ESP_SLEEP_WAKEUP_GPIO) {
    uint64_t gpio_wakeup_status = esp_sleep_get_gpio_wakeup_status();
    Serial.print("GPIO que causó wake-up: ");
    Serial.println(gpio_wakeup_status, BIN);
    Serial.flush();
  }
  
  // Habilitar wake-up por GPIO2 - probar con flanco ascendente en lugar de nivel
  Serial.println("Paso 4: Configurando wake-up...");
  Serial.flush();
  
  esp_sleep_enable_gpio_wakeup();
  gpio_wakeup_enable(GPIO_NUM_2, GPIO_INTR_POSEDGE); // Cambiar a flanco ascendente
  
  Serial.print("GPIO2 configurado para wake-up. Estado antes de sleep: ");
  Serial.println(gpio_get_level(GPIO_NUM_2));
  Serial.flush();
  
  // Opcional: Timer de backup para despertar cada 30 segundos
  // esp_sleep_enable_timer_wakeup(SLEEP_TIME_US);
  
  // Poner RFM69 en sleep antes del deep sleep del ESP32
  putRFM69ToSleep();
  
  // TEMPORAL: Para debug, usar modo normal en lugar de deep sleep
  // Comentar las siguientes 3 líneas para activar deep sleep
  Serial.println("ACTIVANDO MODO DEBUG - SIN DEEP SLEEP");
  Serial.flush();
  debugMode(); // Función de debug - REMOVER después
  return; // No llegar al deep sleep
  
  /*
  Serial.println("Entrando en deep sleep...");
  Serial.flush(); // Asegurar que se envíe todo por serial
  delay(100);
  
  // Entrar en deep sleep
  esp_deep_sleep_start();
  */
}

void loop() {
  // Este loop nunca se ejecutará debido al deep sleep
  // Todo el procesamiento se hace en setup()
}

void initializeRFM69() {
  SPI.begin(RFM69_SCK, RFM69_MISO, RFM69_MOSI);
  
  // Reset del RFM69
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
    // En lugar de while(1), mejor reiniciar
    ESP.restart();
  }
  
  // Verificar comunicación SPI
  uint8_t version = rf69.spiRead(0x10);
  Serial.print("RegVersion (0x10) = 0x");
  Serial.println(version, HEX);
  
  if (version != 0x24) {
    Serial.println("Error en comunicación SPI");
    ESP.restart();
  }
  
  // Configurar frecuencia y potencia
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("Fallo al configurar frecuencia");
    ESP.restart();
  }
  
  rf69.setTxPower(20, true);
  rf69.spiWrite(0x02, 0x00); // Configuración adicional si es necesaria
}

void sendPIRState(int state) {
  char message[8];
  snprintf(message, sizeof(message), "P%d,%d", PIR_ID, state);
  
  Serial.print("Enviando estado PIR: ");
  Serial.println(message);
  Serial.flush();
  
  // Usar exactamente la misma lógica que tu código que funciona
  if (rf69.send((uint8_t*)message, strlen(message))) {
    rf69.waitPacketSent();
    Serial.println("Mensaje enviado correctamente.");
  } else {
    Serial.println("ERROR: Fallo al enviar mensaje.");
  }
  
  Serial.flush();
}

void putRFM69ToSleep() {
  // Poner el RFM69 en modo sleep para minimizar consumo
  rf69.sleep();
  
  // Alternativamente, puedes usar el comando directo:
  // rf69.spiWrite(0x01, 0x00); // RegOpMode = Sleep mode
  
  Serial.println("RFM69 en modo sleep");
}

// Función de debug temporal - REMOVER después de que funcione
void debugMode() {
  Serial.println("=== MODO DEBUG - SIN DEEP SLEEP ===");
  Serial.println("Monitoreando PIR cada 2 segundos...");
  Serial.flush();
  
  int lastState = -1;
  int counter = 0;
  
  while(true) {
    counter++;
    int currentState = digitalRead(PIR_PIN); // Usar digitalRead como en tu código
    
    Serial.print("[");
    Serial.print(counter);
    Serial.print("] PIR Estado: ");
    Serial.print(currentState);
    
    if (currentState != lastState) {
      Serial.print(" -> CAMBIO DETECTADO!");
      lastState = currentState;
      Serial.println();
      Serial.flush();
      
      Serial.println("*** ENVIANDO POR CAMBIO DE ESTADO ***");
      sendPIRState(currentState);
      Serial.println("*** TRANSMISIÓN COMPLETADA ***");
      Serial.flush();
    } else {
      Serial.println(" (sin cambio)");
    }
    
    Serial.flush();
    delay(2000);
  }
}