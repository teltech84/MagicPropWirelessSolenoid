#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// UUIDs for the service and characteristic
#define SERVICE_UUID        "12345678-1234-1234-1234-123456789012"
#define CHARACTERISTIC_UUID "12345678-1234-1234-1234-123456789013"

// Pin definitions
#define BUTTON_PIN 21
#define LED_PIN 0

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// Callback for connection events
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      digitalWrite(LED_PIN, HIGH);
      Serial.println("Device connected");
      // Update connection parameters for stability
      pServer->updateConnParams(pServer->getConnId(), 0x06, 0x0C, 0, 400);
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      digitalWrite(LED_PIN, LOW);
      Serial.println("Device disconnected");
      delay(500); // Give some time before restarting advertising
      pServer->startAdvertising();
      Serial.println("Restarting advertising...");
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Transmitter...");

  // Initialize pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Create the BLE Device
  BLEDevice::init("ESP32_Transmitter");
  
  // Set power to maximum for better range
  BLEDevice::setPower(ESP_PWR_LVL_P9);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // Add descriptor for notifications
  pCharacteristic->addDescriptor(new BLE2902());
  
  // Set initial value
  uint8_t initialValue = 0;
  pCharacteristic->setValue(&initialValue, 1);

  // Start the service
  pService->start();

  // Configure advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMaxPreferred(0x12);
  
  // Start advertising
  BLEDevice::startAdvertising();
  
  Serial.println("Transmitter ready and advertising");
}

void loop() {
  // Read button with debouncing
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // Button pressed (LOW because of pull-up)
      if (buttonState == LOW && deviceConnected) {
        Serial.println("Button pressed - Sending signal");
        uint8_t value = 1;
        pCharacteristic->setValue(&value, 1);
        pCharacteristic->notify();
        delay(100); // Small delay to ensure notification is sent
      }
    }
  }

  lastButtonState = reading;

  delay(20); // Slightly longer delay for stability
}
