#include "CommandReader.h"
#include "ServoManager.h"

#include <Arduino.h>
#include <Wire.h>
#define SERVOMIN  110 // this is the 'minimum' pulse length count (out of 4096)cw
#define SERVOMAX  470 // this is the 'maximum' pulse length count (out of 4096)ccw

ServoManager  *servoManager;
CommandReader *commandReader;

void setup() {
	Serial.begin(115200);

	servoManager  = new ServoManager();
	commandReader = new CommandReader(servoManager);

	Serial.print("Servo API is Ready. Write \"P[servoId];[position]\\r\" to change position of servo\n\n");
}

void loop() {
	// long a1 = micros();

	commandReader->run();

	// long a2 = micros() - a1;
	//
	// if (a2 > 15) {
	//      Serial.println(a2);
	// }
}
