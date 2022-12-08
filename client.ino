#include <ArduinoBLE.h>
#define PERIPHERAL_UUID "19b10000-e8f2-537e-4f6c-d104768a1214"
#define LED_SERVICE_UUID "19b10001-e8f2-537e-4f6c-d104768a1214"

#define LED_START_RANGE 22
#define LED_END_RANGE 25

BLEService ledService(PERIPHERAL_UUID);
// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, writable by central
BLEByteCharacteristic switchCharacteristic(LED_SERVICE_UUID, BLEWrite);

void setup() {
  Serial.begin(9600);
  for (int i = LED_START_RANGE; i <= LED_END_RANGE; i++) pinMode(i, OUTPUT);
  
  BLE.begin();

  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);
  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();
  if (!central) {
    Serial.println("No central found!");
    delay(1000);
    return;
  }

  int oldValue = 0;

  while (central.connected()) {
    if (!switchCharacteristic.written()) continue;

    int newValue = (int)switchCharacteristic.value();
    Serial.println("Received value " + newValue);

    if (oldValue) digitalWrite(oldValue, LOW);
    digitalWrite(newValue, HIGH);
    oldValue = newValue;
  }

  Serial.println("Disconnected from server");
}
