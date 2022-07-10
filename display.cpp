#include "config.h"
#include "display.h"

#include <cstring>

#define SCREEN_SCALING 2

Display::Display(Graphics &graphics) : m_graphics(graphics) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(160 * SCREEN_SCALING, 144 * SCREEN_SCALING, 0, &m_sdlWindow, &m_sdlRenderer);
	SDL_SetWindowTitle(m_sdlWindow, "Shiro");

	m_sdlTexture = SDL_CreateTexture(m_sdlRenderer,SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 160, 144);

	render();

	if (Config::getInstance().s_showSpriteDebug)
	{
		SDL_CreateWindowAndRenderer(8 * 40, 16, 0, &m_sdlWindowSpriteDebug, &m_sdlRendererSpriteDebug);
		SDL_SetWindowTitle(m_sdlWindowSpriteDebug, "DEBUG - OAM");
		SDL_SetWindowPosition(m_sdlWindowSpriteDebug, 20, 40);
		m_sdlTextureSpriteDebug = SDL_CreateTexture(m_sdlRendererSpriteDebug, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 8 * 40, 16);

		renderSpriteDebug();
	}
}

Display::~Display() {
	SDL_DestroyTexture(m_sdlTexture);
	SDL_DestroyRenderer(m_sdlRenderer);
	SDL_DestroyWindow(m_sdlWindow);
	if (Config::getInstance().s_showSpriteDebug) {
		SDL_DestroyTexture(m_sdlTextureSpriteDebug);
		SDL_DestroyRenderer(m_sdlRendererSpriteDebug);
		SDL_DestroyWindow(m_sdlWindowSpriteDebug);
	}
	SDL_Quit();
}


void Display::render() {

	int pitch;
	void * screenTexture;
	SDL_LockTexture(m_sdlTexture, NULL, static_cast<void**>(&screenTexture), &pitch);

	std::memcpy(screenTexture, m_graphics.getScreenBuffer(), m_graphics.getScreenBufferSize());

	//SDL_RenderClear(m_sdlRenderer);
	SDL_RenderCopy(m_sdlRenderer, m_sdlTexture, NULL, NULL);
	SDL_RenderPresent(m_sdlRenderer);

	SDL_UnlockTexture(m_sdlTexture);
}

void Display::renderSpriteDebug() {

	int pitch;
	void * spriteDebugTexture;
	Uint32 window_flags = SDL_GetWindowFlags(m_sdlWindowSpriteDebug);

	if (window_flags & SDL_WINDOW_HIDDEN)
		return;

	SDL_LockTexture(m_sdlTextureSpriteDebug, NULL, static_cast<void**>(&spriteDebugTexture), &pitch);

	std::memcpy(spriteDebugTexture, m_graphics.getSpriteDebugBuffer(), m_graphics.getSpriteDebugBufferSize());

	SDL_RenderCopy(m_sdlRendererSpriteDebug, m_sdlTextureSpriteDebug, NULL, NULL);
	SDL_RenderPresent(m_sdlRendererSpriteDebug);

	SDL_UnlockTexture(m_sdlTextureSpriteDebug);
}

void Display::toggleOAMWindow()
{
	Uint32 window_flags = SDL_GetWindowFlags(m_sdlWindowSpriteDebug);

	if (window_flags & SDL_WINDOW_HIDDEN)
		SDL_ShowWindow(m_sdlWindowSpriteDebug);
	else
		SDL_HideWindow(m_sdlWindowSpriteDebug);
}