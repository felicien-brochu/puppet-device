#ifndef Command_h
#define Command_h

#include <stdint.h>

#define CMD_BREAK         0x0D // Enter key \r
#define CMD_PARAM_BREAK   0x3B // ;
#define CMD_CODE_POSITION 0x50 // P

class Command {
public:

	typedef enum {
		NONE     = 0,
		POSITION = 1
	} Type;

	Command(uint8_t type = Type::NONE);

	uint8_t type;
};

/**
 * PositionCommand example: "P1;310\r"
 *
 */
class PositionCommand : public Command {
public:

	PositionCommand(int  servoId,
	                long p);
	static PositionCommand* fromString(char *string);

	uint8_t servoId;
	long p;

private:

	PositionCommand(uint8_t type,
	                int     servoId,
	                long    p);
};

#endif // ifndef CommandReader_h
