#include <Arduino.h>
#include "BluetoothSerial.h"
#include <TinyGPSPlus.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic = nullptr;
BLEService *pService = nullptr;
BLEServer *pServer = nullptr;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      
      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);
          
        Serial.println();
        Serial.println("*********");
      }
    }
};
bool deviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Device connected");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Device disconnected");
    }
};

int received;// received value will be stored in this variable
char receivedChar;// received value will be stored as CHAR in this variable
unsigned long int pre_time=0, delay_time=5000;
bool state_indicate = false;  
#define GPS_INDICATOR 2

String gps_data;
TinyGPSPlus gps;
BluetoothSerial SerialBT;
void displayInfo();
void BLE_Setup();
void BLE_SendData(String data);