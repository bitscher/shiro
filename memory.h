#pragma once

#include "gbdefines.h"
#include "cartridge.h"

#include <cstdint>

class Memory
{
public:
	Memory();
	~Memory();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);

	void switchBank(uint8_t data);
	Cartridge& getCartridge() { return m_cartridge; }

	// Returns a reference to the byte at the given address, bypasses cartridge and bank switching, USE WITH CAUTION
	// Only intended for direct access to VRAM and IO/Internal RAM
	uint8_t& operator[](uint16_t address) { return m_memory[address]; }

	bool isBootROMEnabled() { return m_bootROMEnabled;  }

	uint8_t& getGamePadKeyMap() { return m_gamePadKeyMap; }
private :
	uint8_t m_memory[0x10000];
	uint8_t m_bootROM[0x100];
	uint8_t m_gamePadKeyMap;

	bool m_bootROMEnabled;

	Cartridge m_cartridge;

	void initialize();
};