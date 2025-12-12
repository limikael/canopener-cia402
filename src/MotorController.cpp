#include "MotorController.h"
#include <Arduino.h>

using namespace canopener;

#define DOFFS_TARGET_POSITION 0x7a
#define DOFFS_PROFILE_MAX_VELOCITY 0x81
#define DOFFS_PROFILE_MAX_ACCEL 0x83
#define DOFFS_PROFILE_MAX_DECEL 0x84
#define DOFFS_ACTUAL_POSITION 0x64
#define DOFFS_CONTROL 0x40

MotorController::MotorController(Device& dev)
		: device(dev) {
	pulPin=-1;
	dirPin=-1;

    baseIndex=0x0000;
    baseSubIndex=0x00;

    motion.current_pos=0;
    motion.current_vel=0;
}

void MotorController::begin() {
    device.insert(baseIndex+DOFFS_TARGET_POSITION,baseSubIndex).setType(Entry::INT32).set<int32_t>(0);
	device.insert(baseIndex+DOFFS_PROFILE_MAX_VELOCITY,baseSubIndex).setType(Entry::INT32).set<int32_t>(1000);
	device.insert(baseIndex+DOFFS_PROFILE_MAX_ACCEL,baseSubIndex).setType(Entry::INT32).set<int32_t>(1000);
	device.insert(baseIndex+DOFFS_PROFILE_MAX_DECEL,baseSubIndex).setType(Entry::INT32).set<int32_t>(1000);
	device.insert(baseIndex+DOFFS_CONTROL,baseSubIndex).setType(Entry::UINT16).set<uint16_t>(0);
	device.insert(baseIndex+DOFFS_ACTUAL_POSITION,baseSubIndex).setType(Entry::INT32);

    pinMode(pulPin,OUTPUT);
    pinMode(dirPin,OUTPUT);
    pinMode(enaPin,OUTPUT);
    digitalWrite(dirPin,LOW);
    digitalWrite(enaPin,HIGH);
    previousMillis=millis();
}

void MotorController::loop() {
	uint16_t control=device.at(baseIndex+DOFFS_CONTROL,baseSubIndex).get<uint16_t>();
	if ((control&0x0f)==0x0f)
	    digitalWrite(enaPin,LOW);

	else
	    digitalWrite(enaPin,HIGH);

    unsigned long currentMillis=millis();
    unsigned long deltaTime=currentMillis-previousMillis;
    if (deltaTime>10) {
	    previousMillis=currentMillis;

	    motion.target_pos=device.at(baseIndex+DOFFS_TARGET_POSITION,baseSubIndex).get<int32_t>();
	    motion.max_vel=device.at(baseIndex+DOFFS_PROFILE_MAX_VELOCITY,baseSubIndex).get<int32_t>();
	    motion.max_accel=device.at(baseIndex+DOFFS_PROFILE_MAX_ACCEL,baseSubIndex).get<int32_t>();
	    motion.max_decel=device.at(baseIndex+DOFFS_PROFILE_MAX_DECEL,baseSubIndex).get<int32_t>();

		//if (deltaTime>0)
	    update_trapezoidal_motion(&motion,deltaTime/1000.0);
	    device.at(baseIndex+DOFFS_ACTUAL_POSITION,baseSubIndex).set<int32_t>(motion.current_pos);

	    if (motion.current_vel>=0) {
		    digitalWrite(dirPin,LOW);
	    	tone(pulPin,motion.current_vel);
	    }

	    else {
		    digitalWrite(dirPin,HIGH);
	    	tone(pulPin,-motion.current_vel);
	    }

    }

    //delay(10);
}