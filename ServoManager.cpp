#include "ServoManager.h"

ServoManager::ServoManager() {
	_driver.begin();
	_driver.setPWMFreq(50); // 50Hz servos
	yield();
}

void ServoManager::consumeCommand(PositionCommand *command) {
	_driver.setPWM(command->servoId, 0, command->p);
	delete command;
}
