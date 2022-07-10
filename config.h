#pragma once

#ifdef _DEBUG
	#define DEBUG_ONLY(...) do { __VA_ARGS__ } while (false)
#else
	#define DEBUG_ONLY(...) do {} while (false)
#endif

#include <chrono>

class Config
{
public:
	inline static Config& getInstance()
	{
		static Config instance;
		return instance;
	}

	bool s_playBootRom;
	bool s_showSpriteDebug;
	std::chrono::microseconds s_updatePeriod; // default 60Hz = 16667us

	Config(Config const&) = delete;
	void operator=(Config const&) = delete;

	void setSpeed(uint8_t speed)
	{
		s_updatePeriod = std::chrono::microseconds(1000000/(60*speed));
	}

private:
	Config() {
		/* Default conf */
		s_playBootRom = false;
		s_showSpriteDebug = false;
		setSpeed(1);
	}
};
