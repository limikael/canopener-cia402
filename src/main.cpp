#include <Arduino.h>
#include "canopener.h"
#include "MotorController.h"
using namespace canopener;

SerialBus serialBus(Serial);
EspBus espBus(5,4); // tx, rx
BridgeBus bus(serialBus,espBus);
Device dev(bus);
MotorController motorController(dev);

#define PIN_PUL 3
#define PIN_DIR 10
#define PIN_ENA 1

void setup() {
    Serial.begin(115200);
    dev.setNodeId(5);
    motorController.setPulPin(PIN_PUL);
    motorController.setDirPin(PIN_DIR);
    motorController.setEnaPin(PIN_ENA);
    motorController.setBaseIndex(0x6000);
    motorController.setBaseSubIndex(0x00);
    motorController.begin();
}

void loop() {
    dev.loop();
    motorController.loop();
}