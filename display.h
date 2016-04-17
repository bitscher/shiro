#pragma once

/*
 * This class is doing the actual on screen rendering with SDL
 * Its implementation can be changed easily to use another library
 */

#include <cstdint>

#ifdef _MSC_VER
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "graphics.h"

class Display
{
public:
	Display(Graphics &graphics);
	~Display();

	void render();
	void renderSpriteDebug();

private:
	Graphics &m_graphics;

	SDL_Window * m_sdlWindow;
	SDL_Renderer * m_sdlRenderer;
	SDL_Texture * m_sdlTexture;

	SDL_Window * m_sdlWindowSpriteDebug;
	SDL_Renderer * m_sdlRendererSpriteDebug;
	SDL_Texture * m_sdlTextureSpriteDebug;
};
