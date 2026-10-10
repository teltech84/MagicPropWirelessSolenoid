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

int buttonState = LOW;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// Callback for connection events
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      digitalWrite(LED_PIN, HIGH);
      Serial.println("*** CLIENT CONNECTED ***");
      Serial.print("Connection ID: ");
      Serial.println(pServer->getConnId());
      
      // Update connection parameters for stability
      pServer->updateConnParams(pServer->getConnId(), 0x06, 0x0C, 0, 400);
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      digitalWrite(LED_PIN, LOW);
      Serial.println("*** CLIENT DISCONNECTED ***");
      delay(500);
      pServer->startAdvertising();
      Serial.println("Restarting advertising...");
    }
};
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n=== PIN 21 FULL DIAGNOSTIC ===");
  
  // First, try to "reset" the pin by setting it to different modes
  Serial.println("Resetting pin 21...");
  
  pinMode(21, INPUT);           // Plain input first
  delay(100);
  
  pinMode(21, OUTPUT);          // Set as output
  digitalWrite(21, HIGH);       // Drive it HIGH
  delay(100);
  
  pinMode(21, INPUT_PULLUP);    // Now set as input with pullup
  delay(100);
  
  Serial.println("\nPin 21 now configured as INPUT_PULLUP");
  Serial.println("Testing pin state...\n");
}

void loop() {
  int state = digitalRead(21);
  
  Serial.print("Pin 21: ");
  Serial.print(state);
  Serial.print(" | ");
  
  // Try reading multiple times
  Serial.print(digitalRead(21));
  Serial.print(" ");
  Serial.print(digitalRead(21));
  Serial.print(" ");
  Serial.print(digitalRead(21));
  Serial.println();
  
  delay(500);
}
