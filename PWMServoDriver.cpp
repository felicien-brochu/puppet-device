#include "PWMServoDriver.h"

#include <Wire.h>
#if defined(ARDUINO_SAM_DUE)
 # define WIRE Wire1
#else // if defined(ARDUINO_SAM_DUE)
 # define WIRE Wire
#endif // if defined(ARDUINO_SAM_DUE)


// Set to true to print some debug messages, or false to disable them.
#define ENABLE_DEBUG_OUTPUT false

PWMServoDriver::PWMServoDriver(uint8_t addr) {
	_i2cAddr = addr;
}

void PWMServoDriver::begin(void) {
	WIRE.begin();
	reset();
}

void PWMServoDriver::reset(void) {
	write8(PCA9685_MODE1, 0x0);
}

void PWMServoDriver::setPWMFreq(float freq) {
	freq *= 0.9; // Correct for overshoot in the frequency setting (see issue #11).
	float prescaleval = 25000000;
	prescaleval /= 4096;
	prescaleval /= freq;
	prescaleval -= 1;

	uint8_t prescale = floor(prescaleval + 0.5);

	uint8_t oldmode = read8(PCA9685_MODE1);
	uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
	write8(PCA9685_MODE1,    newmode);         // go to sleep
	write8(PCA9685_PRESCALE, prescale);        // set the prescaler
	write8(PCA9685_MODE1,    oldmode);
	delay(5);
	write8(PCA9685_MODE1, oldmode | 0xa1);     //  This sets the MODE1 register to turn on auto increment.
	                                           // This is why the beginTransmission below was not working.
}

void PWMServoDriver::setPWM(uint8_t num, uint16_t on, uint16_t off) {
	WIRE.beginTransmission(_i2cAddr);
	WIRE.write(LED0_ON_L + 4 * num);
	WIRE.write(on);
	WIRE.write(on >> 8);
	WIRE.write(off);
	WIRE.write(off >> 8);
	WIRE.endTransmission();
}

// Sets pin without having to deal with on/off tick placement and properly handles
// a zero value as completely off.  Optional invert parameter supports inverting
// the pulse for sinking to ground.  Val should be a value from 0 to 4095 inclusive.
void PWMServoDriver::setPin(uint8_t num, uint16_t val, bool invert) {
	// Clamp value between 0 and 4095 inclusive.
	val = min(val, 4095);

	if (invert) {
		if (val == 0) {
			// Special value for signal fully on.
			setPWM(num, 4096, 0);
		} else if (val == 4095) {
			// Special value for signal fully off.
			setPWM(num, 0, 4096);
		} else {
			setPWM(num, 0, 4095 - val);
		}
	} else {
		if (val == 4095) {
			// Special value for signal fully on.
			setPWM(num, 4096, 0);
		} else if (val == 0) {
			// Special value for signal fully off.
			setPWM(num, 0, 4096);
		} else {
			setPWM(num, 0, val);
		}
	}
}

uint8_t PWMServoDriver::read8(uint8_t addr) {
	WIRE.beginTransmission(_i2cAddr);
	WIRE.write(addr);
	WIRE.endTransmission();

	WIRE.requestFrom((uint8_t)_i2cAddr, (uint8_t)1);
	return WIRE.read();
}

void PWMServoDriver::write8(uint8_t addr, uint8_t data) {
	WIRE.beginTransmission(_i2cAddr);
	WIRE.write(addr);
	WIRE.write(data);
	WIRE.endTransmission();
}
