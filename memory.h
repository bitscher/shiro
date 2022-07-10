#pragma once

#include "gbdefines.h"
#include "cartridge.h"

#include <cstdint>
#include <memory>

class Memory
{
public:
	Memory(std::ifstream &romFile, std::ifstream &savFile) noexcept(false);
	~Memory() = default;

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);
	void requestInterrupt(enum InteruptType interruptType);

	// Returns a reference to the byte at the given address, bypasses cartridge and bank switching, USE WITH CAUTION
	// Only intended for direct access to VRAM and IO/Internal RAM
	uint8_t& operator[](uint16_t address) { return m_memory[address]; }

	bool isBootROMEnabled() { return m_bootROMEnabled;  }
	void saveRAMToFile(const std::string& romPath) { m_cartridge->saveRAMToFile(romPath); }

	uint8_t& getGamePadKeyMap() { return m_gamePadKeyMap; }
private :
	uint8_t m_memory[0x10000];
	uint8_t m_bootROM[0x100];
	uint8_t m_gamePadKeyMap;

	bool m_bootROMEnabled;

	std::unique_ptr<Cartridge> m_cartridge;

	void initialize();
};
