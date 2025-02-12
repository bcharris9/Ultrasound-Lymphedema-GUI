#include <ArduinoBLE.h>

BLEService ledService("180A"); // Custom LED service
BLEByteCharacteristic ledCharacteristic("2A56", BLEWrite);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    BLE.begin();

    BLE.setLocalName("XIAO_BLE");
    BLE.setAdvertisedService(ledService);
    ledService.addCharacteristic(ledCharacteristic);
    BLE.addService(ledService);
    BLE.advertise();
}

void loop() {
    BLEDevice central = BLE.central();
    if (central) {
        while (central.connected()) {
            if (ledCharacteristic.written()) {
                if (ledCharacteristic.value() == 1) {
                    digitalWrite(LED_BUILTIN, HIGH);
                } else {
                    digitalWrite(LED_BUILTIN, LOW);
                }
            }
        }
    }
}
