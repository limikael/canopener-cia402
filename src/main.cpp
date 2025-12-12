#include "canopener.h"
#include <Arduino.h>
#include "MotorController.h"
#include "trapezoidal_motion.h"
using namespace canopener;

SerialBus bus(Serial);
Device dev(bus);
MotorController motorController(dev);

void setup() {
    Serial.begin(115200);
    dev.setNodeId(5);
    motorController.setPulPin(8);
    motorController.setDirPin(20);
    motorController.setEnaPin(21);
    motorController.setBaseIndex(0x6000);
    motorController.setBaseSubIndex(0x00);
    motorController.begin();
}

void loop() {
    dev.loop();
    motorController.loop();
}