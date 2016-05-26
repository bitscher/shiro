#pragma once

#include <cstdint>
#include <functional>

class Cartridge
{
	enum CartridgeType {
		ROM_ONLY,
		ROM_MBC1, ROM_MBC1_RAM, ROM_MBC1_RAM_BATT,
		ROM_MBC2, ROM_MBC2_BATT,
		ROM_RAM, ROM_RAM_BATT,
		//ROM_MMM01, ROM_MMM01_SRAM, ROM_MMM01_SRAM_BATT,
		MAX_SUPPORTED_TYPE
	};
	
	static const char* CartridgeTypeDesc[];

public:
	Cartridge();
	~Cartridge();

	void loadRom(const char * path);

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);

private:
	CartridgeType m_cartridgeType;

	uint8_t* m_currentROMBank;
	uint8_t* m_currentRAMBank;

	unsigned int m_ROMBanksCount;
	unsigned int m_RAMBanksCount;
	
	uint8_t** m_ROMBanks;
	uint8_t** m_RAMBanks;

	bool m_RAMWriteEnabled;
	uint8_t m_ROMBankSelector;

	uint8_t m_MBC1Mode;

	std::function<void(Cartridge&, uint16_t, uint8_t)> m_writeFunc;

	void writeROMOnly(uint16_t address, uint8_t value);
	void writeMBC1(uint16_t address, uint8_t value);
	void writeMBC2(uint16_t address, uint8_t value);
};