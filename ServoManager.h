#ifndef ServoManager_h
#define ServoManager_h

#include "CommandReader.h"
#include "PWMServoDriver.h"

class ServoManager : public IPositionConsumer {
public:

	ServoManager();
	void consumeCommand(PositionCommand *command);

private:

	PWMServoDriver _driver;
};

#endif // ifndef ServoManager_h
