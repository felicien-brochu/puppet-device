#ifndef CommandReader_h
#define CommandReader_h

#include "Command.h"

#include <stdlib.h>
#include <stdint.h>

#define CMD_BUF_MAX_SIZE 1024

class IPositionConsumer {
public:

	virtual void consumeCommand(PositionCommand *command) = 0;
};

class CommandReader {
public:

	CommandReader(IPositionConsumer *consumer);
	void run();

private:

	bool     read();
	Command* consumeCommand();
	Command* getCommand();
	bool     isFull();
	void     clear();
	int      getCommandSize();
	bool     hasCommand();

private:

	char _buffer[CMD_BUF_MAX_SIZE];
	int _size;
	IPositionConsumer *_consumer;
};

#endif // ifndef CommandReader_h
