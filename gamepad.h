#pragma once

#include "memory.h"

class GamepadHandler
{
public:
	typedef enum {
		BUTTON_A = 0x01,
		BUTTON_B = 0x02,
		BUTTON_SELECT = 0x04,
		BUTTON_START = 0x08,
		BUTTON_RIGHT = 0x10,
		BUTTON_LEFT = 0x20,
		BUTTON_UP = 0x40,
		BUTTON_DOWN = 0x80
	} GamePadButton_t;

	GamepadHandler(Memory &memory);

	void buttonPressed(GamePadButton_t button);
	void buttonReleased(GamePadButton_t button);
private:
	Memory& m_memory;
};