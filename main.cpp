#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>

#include "config.h"
#include "display.h"
#include "memory.h"
#include "gamepad.h"
#include "lr35902_cpu.h"

#include "graphics.h"
#include "controls.h"

using namespace std::chrono;

int main( int argc, char * argv[] ) {

	if (argc < 2)
		return -1;

	char * romPath = NULL;

	Config &conf = Config::getInstance();

	if (!strcmp(argv[1], "-b"))
	{
		conf.s_playBootRom = true;
		romPath = argv[2];
	}
	else
		romPath = argv[1];

	// Emulation classes
	Memory mem;
	lr35902_cpu cpu(mem);
	Graphics graphics(mem);
	GamepadHandler gamepad(mem);

	// Program interface classes (currently SDL)
	Display renderer(graphics);
	Controls controls(gamepad);

	mem.getCartridge().loadRom(romPath);

	bool mainLoop = true;

	while (mainLoop)
	{
		auto startTime = high_resolution_clock::now();

		controls.UpdateEvents();

		graphics.setLCDOperationMode(Graphics::lcd_mode_t::LCD_MODE2);
		cpu.run(80);
		graphics.setLCDOperationMode(Graphics::lcd_mode_t::LCD_MODE3);
		cpu.run(172);
		graphics.setLCDOperationMode(Graphics::lcd_mode_t::LCD_MODE0); /* HBLANK */
		cpu.run(204);
		if (graphics.shouldStartVBlank())
		{
			if (graphics.isLCDEnabled())
			{
				// Output it on SDL
				// TODO should be on a different thread.
				renderer.render();
			}

			if (conf.s_showSpriteDebug)
			{
				graphics.fillSpriteDebugBuffer();
				renderer.renderSpriteDebug();
			}

			graphics.setLCDOperationMode(Graphics::lcd_mode_t::LCD_MODE1); /* VBLANK */
			for (uint8_t vBlankLines = 0; vBlankLines < 10; ++vBlankLines)
			{
				cpu.run(456);
				graphics.incrementY();
			}

			microseconds executionTime = duration_cast<microseconds>(high_resolution_clock::now() - startTime);

			if (executionTime < conf.s_updatePeriod)
			{
				std::this_thread::sleep_for(conf.s_updatePeriod - executionTime);
			}
		}
	}

	return 0;

}
