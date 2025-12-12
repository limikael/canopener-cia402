#pragma once
#include "trapezoidal_motion.h"
#include "canopener.h"

class MotorController {
public:
	MotorController(canopener::Device& device);
	void begin();
	void setPulPin(int pulPin_) { pulPin=pulPin_; }
	void setDirPin(int dirPin_) { dirPin=dirPin_; }
	void setEnaPin(int enaPin_) { enaPin=enaPin_; }
	void setBaseIndex(uint16_t baseIndex_) { baseIndex=baseIndex_; }
	void setBaseSubIndex(uint8_t baseSubIndex_) { baseSubIndex=baseSubIndex_; }
	void loop();

private:
	unsigned long previousMillis;
	int pulPin,dirPin,enaPin;
	trapezoidal_motion_t motion;
	canopener::Device& device;
	uint16_t baseIndex;
	uint8_t baseSubIndex;
};
