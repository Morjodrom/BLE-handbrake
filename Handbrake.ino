#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// BLE UUIDs
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Analog pin for potentiometer
const int potPin = A0;

// BLE objects
BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;

// Potentiometer range and thresholds
const int potMin = 0;
const int potMax = 4095; // ESP32-C3 has 12-bit ADC
const int edgeThreshold = 100; // Threshold for edge detection

// Discrete values for edge positions
const int edgeMinValue = 0;
const int edgeMaxValue = 255;

// Callback for BLE connection events
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        Serial.println("Device connected");
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        Serial.println("Device disconnected");
        // Restart advertising to allow reconnection
        pServer->startAdvertising();
    }
};

void setup() {
    Serial.begin(115200);

    // Initialize BLE
    BLEDevice::init("ESP32-C3 Game Controller");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create BLE service
    BLEService* pService = pServer->createService(SERVICE_UUID);

    // Create BLE characteristic for Y-axis and discrete values
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );
    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    pServer->getAdvertising()->start();
    Serial.println("Waiting for a client to connect...");
}

void loop() {
    if (deviceConnected) {
        // Read potentiometer value
        int potValue = analogRead(potPin);

        // Map potentiometer value to Y-axis range (0-255)
        int yAxisValue = map(potValue, potMin, potMax, 0, 255);

        // Send Y-axis value via BLE
        uint8_t yAxisData = static_cast<uint8_t>(yAxisValue);
        pCharacteristic->setValue(&yAxisData, 1);
        pCharacteristic->notify();

        // Check for edge positions and send discrete values
        if (potValue <= edgeThreshold) {
            uint8_t edgeData = static_cast<uint8_t>(edgeMinValue);
            pCharacteristic->setValue(&edgeData, 1);
            pCharacteristic->notify();
            Serial.println("Edge Min Position Detected");
        } else if (potValue >= (potMax - edgeThreshold)) {
            uint8_t edgeData = static_cast<uint8_t>(edgeMaxValue);
            pCharacteristic->setValue(&edgeData, 1);
            pCharacteristic->notify();
            Serial.println("Edge Max Position Detected");
        }

        // Small delay to avoid flooding BLE with data
        delay(100);
    }
}