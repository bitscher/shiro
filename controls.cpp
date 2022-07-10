#include "controls.h"

Controls::Controls(GamepadHandler &gamepadHdlr, Display &display) :
	m_GamePadHandler(gamepadHdlr),
	m_Display(display)
{}


void Controls::UpdateEvents(bool &quit)
{
	SDL_Event e;
	const struct {
		SDL_Keycode sym;
		GamepadHandler::GamePadButton_t button;
	} mapping[] = {
		{ SDLK_n,           GamepadHandler::BUTTON_A},
		{ SDLK_b,           GamepadHandler::BUTTON_B},
		{ SDLK_RETURN,      GamepadHandler::BUTTON_START},
		{ SDLK_BACKSPACE,   GamepadHandler::BUTTON_SELECT},
		{ SDLK_UP,          GamepadHandler::BUTTON_UP},
		{ SDLK_DOWN,        GamepadHandler::BUTTON_DOWN},
		{ SDLK_LEFT,        GamepadHandler::BUTTON_LEFT},
		{ SDLK_RIGHT,       GamepadHandler::BUTTON_RIGHT},
	};

	if (!SDL_PollEvent(&e))
		return;

	switch (e.type)
	{
	case SDL_QUIT:
		quit = true;
		break;
	case SDL_WINDOWEVENT:
		if (e.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			if (e.window.windowID == 1)
				quit = true;
			else
				m_Display.toggleOAMWindow();
		}
		break;
	case SDL_KEYUP:
		if (e.key.keysym.sym == SDLK_F1)
		{
			m_Display.toggleOAMWindow();
			break;
		}
		/* Fallthrough */
	case SDL_KEYDOWN:
		for (const auto& m : mapping)
		{
			if (e.key.keysym.sym == m.sym)
			{
				if (e.type == SDL_KEYUP)
					m_GamePadHandler.buttonReleased(m.button);
				else
					m_GamePadHandler.buttonPressed(m.button);
				break;
			}
		}
		break;
	default:
		break;
	}
}
