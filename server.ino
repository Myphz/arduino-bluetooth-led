#include <ArduinoBLE.h>
#define PERIPHERAL_UUID "19b10000-e8f2-537e-4f6c-d104768a1214"
#define LED_SERVICE_UUID "19b10001-e8f2-537e-4f6c-d104768a1214"

#define LED_START_RANGE 22
#define LED_END_RANGE 25

void setup() {
  Serial.begin(9600);
  BLE.begin();

  BLE.scanForUuid(PERIPHERAL_UUID);
  
  randomSeed(analogRead(0));
}

void loop() {
  BLEDevice peripheral = BLE.available();

  if (!peripheral) {
    Serial.println("Device not found! Retrying in 1 second...");
    delay(1000);
    return;
  }

  BLE.stopScan();
  controlPeripheral(peripheral);

  // Peripheral disconnected, restart search
  BLE.scanForUuid(PERIPHERAL_UUID);
}

void controlPeripheral(BLEDevice peripheral) {
  Serial.println("Connecting to device...");
  if (!peripheral.connect()) {
    Serial.println("Failed to connect!");
    return;
  }
  // Retrieve attributes
  if (!peripheral.discoverAttributes()) {
    Serial.println("No attributes discovered!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic ledCharacteristic = peripheral.characteristic(LED_SERVICE_UUID);
  if (!ledCharacteristic) {
    Serial.println("Device doesn't have ledCharacteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) {
    int randLed = random(LED_START_RANGE, LED_END_RANGE+1);
    delay(1000);
    Serial.println("Turning LED " + randLed);
    ledCharacteristic.writeValue((byte)randLed);
  }

  Serial.println("Peripheral disconnected from server");
}
