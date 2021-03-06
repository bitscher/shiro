#include "controls.h"

Controls::Controls(GamepadHandler &gamepadHdlr) : m_GamePadHandler(gamepadHdlr)
{}


void Controls::UpdateEvents(bool &quit)
{
	SDL_Event e;
	SDL_PollEvent(&e);
	if (e.type == SDL_QUIT)
	{
		quit = true;
	}
	else if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_n:
			m_GamePadHandler.buttonPressed(GamepadHandler::BUTTON_A);
			break;
		case SDLK_b:
			m_GamePadHandler.buttonPressed(GamepadHandler::BUTTON_B);
			break;
		case SDLK_RETURN:
			m_GamePadHandler.buttonPressed(GamepadHandler::BUTTON_START);
			break;
		case SDLK_BACKSPACE:
			m_GamePadHandler.buttonPressed(GamepadHandler::BUTTON_SELECT);
			break;
		case SDLK_UP:
			m_GamePadHandler.buttonPressed(GamepadHandler::BUTTON_UP);
			break;
		case SDLK_DOWN:
			m_GamePadHandler.buttonPressed(GamepadHandler::BUTTON_DOWN);
			break;
		case SDLK_LEFT:
			m_GamePadHandler.buttonPressed(GamepadHandler::BUTTON_LEFT);
			break;
		case SDLK_RIGHT:
			m_GamePadHandler.buttonPressed(GamepadHandler::BUTTON_RIGHT);
			break;
		}
	}
	else if (e.type == SDL_KEYUP)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_n:
			m_GamePadHandler.buttonReleased(GamepadHandler::BUTTON_A);
			break;
		case SDLK_b:
			m_GamePadHandler.buttonReleased(GamepadHandler::BUTTON_B);
			break;
		case SDLK_RETURN:
			m_GamePadHandler.buttonReleased(GamepadHandler::BUTTON_START);
			break;
		case SDLK_BACKSPACE:
			m_GamePadHandler.buttonReleased(GamepadHandler::BUTTON_SELECT);
			break;
		case SDLK_UP:
			m_GamePadHandler.buttonReleased(GamepadHandler::BUTTON_UP);
			break;
		case SDLK_DOWN:
			m_GamePadHandler.buttonReleased(GamepadHandler::BUTTON_DOWN);
			break;
		case SDLK_LEFT:
			m_GamePadHandler.buttonReleased(GamepadHandler::BUTTON_LEFT);
			break;
		case SDLK_RIGHT:
			m_GamePadHandler.buttonReleased(GamepadHandler::BUTTON_RIGHT);
			break;
		}
	}
}
