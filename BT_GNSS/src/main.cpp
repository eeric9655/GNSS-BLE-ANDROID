#include "head.h"
/*
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
*/

void setup() {
  Serial.begin(115200);
  pinMode(GPS_INDICATOR,OUTPUT);
  //SerialBT.begin("ESP32 SAR2"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  delay(1000); // give me time to bring up serial monitor
  Serial2.begin(9600);
  BLE_Setup();
  for(int i=0; i<20;i++){
    state_indicate=!state_indicate;
    digitalWrite(GPS_INDICATOR,(int)state_indicate);
    delay(100);
  }
}

void loop() { 
  /*
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  /*if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }*/
   while (Serial2.available() > 0)
      if (gps.encode(Serial2.read()))
        displayInfo();
      if (millis() > 5000 && gps.charsProcessed() < 10)
      {
        Serial.println(F("No GPS detected: check wiring."));
        //while (true);
      }
}

void displayInfo()
{
  if(millis()-pre_time>=delay_time){        
    if (gps.location.isValid()){
      bool loadinng = false;
      //gps_data = "@Lat:"+String(gps.location.lat(),6)+"@Lng:"+String(gps.location.lng(),6);   
      gps_data = "@"+String(gps.location.lat(),6)+"@"+String(gps.location.lng(),6);   
      state_indicate=!state_indicate;
      digitalWrite(GPS_INDICATOR,(int)state_indicate);
      Serial.print("gps: ");
      Serial.println(gps_data); 
      //@Lat:36.624853@Lng:127.458248
      //SerialBT.print(gps_data);      
    }  
    else
    {
      Serial.print(F("INVALID"));
      //SerialBT.print("INVALID\n");
      gps_data = "INVALED";
    }
    BLE_SendData(gps_data);
    pre_time = millis();
  }
}
void BLE_Setup(){
  BLEDevice::init("BLE_GNSS_87");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE|
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                       );

  pCharacteristic->setValue("Hello World111111111111111120000");
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}
void BLE_SendData(String data){
  if (deviceConnected) {
    pCharacteristic->setValue(data.c_str()); // Send 4-byte value
    pCharacteristic->notify();
    delay(50);
  }
}