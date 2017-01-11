#pragma once

#include <cstdint>
#include <functional>
#include <vector>

class Cartridge
{
	enum CartridgeType {
		ROM_ONLY,
		ROM_MBC1, ROM_MBC1_RAM, ROM_MBC1_RAM_BATT,
		UNKNOWN_04,
		ROM_MBC2, ROM_MBC2_BATT,
		UNKNOWN_07,
		ROM_RAM, ROM_RAM_BATT,
		//ROM_MMM01, ROM_MMM01_SRAM, ROM_MMM01_SRAM_BATT,
		MAX_SUPPORTED_TYPE
	};
	
	static const char* CartridgeTypeDesc[];

	template<unsigned int size>
	struct MemoryBank { uint8_t bytes[size]; };

public:
	Cartridge();
	~Cartridge() = default;

	void loadRom(const char * path);

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);

private:
	CartridgeType m_cartridgeType;
	
	std::vector<MemoryBank<0x4000>> m_ROMBanks;
	std::vector<MemoryBank<0x2000>> m_RAMBanks;

	MemoryBank<512> m_MBC2InternalRAM;

	uint8_t m_currentROMBank;
	uint8_t m_currentRAMBank;

	bool m_RAMWriteEnabled;
	uint8_t m_ROMBankSelector;

	uint8_t m_MBC1Mode;

	std::function<void(Cartridge&, uint16_t, uint8_t)> m_writeFunc;
	std::function<uint8_t(Cartridge&, uint16_t)> m_readFunc;

	void writeROMOnly(uint16_t address, uint8_t value);

	uint8_t readMBC1(uint16_t address);
	void writeMBC1(uint16_t address, uint8_t value);

	uint8_t readMBC2(uint16_t address);
	void writeMBC2(uint16_t address, uint8_t value);
};