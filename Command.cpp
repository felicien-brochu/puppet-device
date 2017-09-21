#include "Command.h"

#include "error.h"

#include <stdlib.h>
#include <errno.h>


Command::Command(uint8_t type) : type(type) {}

PositionCommand::PositionCommand(uint8_t type, int servoId, long p) : Command(type), servoId(servoId), p(p) {}

PositionCommand::PositionCommand(int servoId, long p)
	: PositionCommand(Command::POSITION, servoId, p) {}

PositionCommand * PositionCommand::fromString(char *string) {
	errno = 0;

	if (string[0] != CMD_CODE_POSITION) {
		return 0;
	}
	char *paramStart = string + 1;
	char *nextParam  = paramStart;

	int servoId = strtol(paramStart, &nextParam, 10);

	if ((nextParam == paramStart) || (errno == ERANGE)) {
		errno = EFORMAT;
		return 0;
	}
	paramStart = nextParam + 1;
	long p = strtol(paramStart, &nextParam, 10);

	if ((nextParam == paramStart) || (errno == ERANGE)) {
		errno = EFORMAT;
		return 0;
	}

	if (nextParam[0] != CMD_BREAK) {
		errno = ETOOBIG;
		return 0;
	}

	PositionCommand *command = new PositionCommand(servoId, p);

	return command;
}
