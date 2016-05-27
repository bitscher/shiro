#pragma once

#include "gamepad.h"

#ifdef _MSC_VER
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

/*
* This class is handling the input on the host machine with SDL
* Its implementation can be changed easily to use another library
* Note: Current implementation assumes SDL has been initialized in
* the Display class
*/

class Controls
{
public:
	Controls(GamepadHandler& gamepadHdlr);

	void UpdateEvents(bool &quit);

private:
	GamepadHandler& m_GamePadHandler;
};