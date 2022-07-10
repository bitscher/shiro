#pragma once

#include "gamepad.h"
#include "display.h"

#include <SDL.h>

/*
* This class is handling the input on the host machine with SDL
* Its implementation can be changed easily to use another library
* Note: Current implementation assumes SDL has been initialized in
* the Display class
*/

class Controls
{
public:
	Controls(GamepadHandler &gamepadHdlr, Display &display);

	void UpdateEvents(bool &quit);

private:
	GamepadHandler &m_GamePadHandler;
	Display &m_Display;
};