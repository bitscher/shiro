#include "gamepad.h"
#include "gbdefines.h"
#include "bitwiseop.h"

GamepadHandler::GamepadHandler(Memory &memory) : m_memory(memory)
{}

void GamepadHandler::buttonPressed(GamePadButton_t button)
{
	uint8_t &keyMap = m_memory.getGamePadKeyMap();
	keyMap &= ~static_cast<uint8_t>(button);

	m_memory[IF_OFT] = SET_BIT_N(m_memory[IF_OFT], 4);
}

void GamepadHandler::buttonReleased(GamePadButton_t button)
{
	uint8_t &keyMap = m_memory.getGamePadKeyMap();
	keyMap |= static_cast<uint8_t>(button);
}
