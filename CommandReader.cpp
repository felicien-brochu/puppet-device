#include "CommandReader.h"

#include <Arduino.h>
#include <errno.h>


CommandReader::CommandReader(IPositionConsumer *consumer) : _consumer(consumer), _size(0) {}

void CommandReader::run() {
	if (read()) {
		if (hasCommand()) {
			Serial.println("");
			Command *command = consumeCommand();

			if (command) {
				Serial.println("ok");

				if ((command->type == Command::POSITION)) {
					PositionCommand *positionCommand = static_cast<PositionCommand *>(command);
					_consumer->consumeCommand(positionCommand);
				}
			} else {
				Serial.print("invalid command");

				if (errno) {
					Serial.print(" errno=");
					Serial.println(errno);
					errno = 0;
				}

				Serial.print("\n");
			}
		}
	}
}

bool CommandReader::read() {
	if (Serial.available() > 0) {
		if (isFull()) {
			clear();
		}
		_buffer[_size] = Serial.read();
		Serial.print(_buffer[_size]);
		_size++;
		return true;
	}
	return false;
}

Command * CommandReader::consumeCommand() {
	int commandSize = getCommandSize();

	if (commandSize == 0) { // No command to consume
		return 0;
	}

	Command *command = getCommand();

	// Remove command from buffer
	for (int i = 0; i < _size - commandSize; i++) {
		_buffer[i] = _buffer[commandSize + i];
	}
	_size -= commandSize;

	return command;
}

Command * CommandReader::getCommand() {
	Command *command;

	switch (_buffer[0]) {
	case CMD_CODE_POSITION:
		command = PositionCommand::fromString(_buffer);
		break;
	}

	return command;
}

bool CommandReader::isFull() {
	return _size >= CMD_BUF_MAX_SIZE;
}

void CommandReader::clear() {
	_size = 0;
}

int CommandReader::getCommandSize() {
	for (int i = 0; i < _size; i++) {
		if (_buffer[i] == CMD_BREAK) {
			return i + 1;
		}
	}
	return 0;
}

bool CommandReader::hasCommand() {
	return getCommandSize() > 0;
}
