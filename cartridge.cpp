#include "cartridge.h"
#include "config.h"

#include <fstream>
#include <iostream>
#include <algorithm>

const char* Cartridge::CartridgeTypeDesc[] = {
	"ROM_ONLY",
	"ROM_MBC1", "ROM_MBC1_RAM", "ROM_MBC1_RAM_BATT",
	"UNKNOWN",
	"ROM_MBC2", "ROM_MBC2_BATT",
	"UNKNOWN",
	"ROM_RAM", "ROM_RAM_BATT",
	//"ROM_MMM01", "ROM_MMM01_SRAM", "ROM_MMM01_SRAM_BATT"
};

Cartridge::Cartridge() :
	m_cartridgeType(ROM_ONLY),
	m_currentROMBank(1),
	m_currentRAMBank(0),
	m_RAMWriteEnabled(false),
	m_ROMBankSelector(0),
	m_MBC1Mode(0),
	m_writeFunc(&Cartridge::writeROMOnly),
	m_readFunc(&Cartridge::readMBC1)
{
	m_MBC2InternalRAM = {};
}

void Cartridge::loadRom(const char * path) {
	std::ifstream romFile;

	romFile.open(path, std::ios_base::in | std::ios_base::binary);

	if (!romFile.is_open())
	{
		fprintf(stderr, "Could not open rom %s\n", path);
		return;
	}

	unsigned int ROMBanksCount = 1;

	romFile.seekg(0x148, std::ios_base::beg);
	int romSize = romFile.peek();
	if (romSize >= 0 && romSize <= 6)
		ROMBanksCount = static_cast<unsigned int>(std::pow(2, romSize + 1));
	else if (romSize == 0x52)
		ROMBanksCount = 72;
	else if (romSize == 0x53)
		ROMBanksCount = 80;
	else if (romSize == 0x54)
		ROMBanksCount = 96;
	else
	{
		fprintf(stderr, "Unknown ROM Size (0x148) = %u\n", romSize);
		exit(-1);
	}

	printf("ROM size: %u ROM banks\n", ROMBanksCount);

	romFile.seekg(std::ios_base::beg);

	m_ROMBanks.reserve(ROMBanksCount);
	//Read the ROM banks
	for (unsigned int bankIdx = 0; bankIdx < ROMBanksCount && romFile; ++bankIdx)
	{
		m_ROMBanks.emplace_back(MemoryBank<0x4000>());
		romFile.read(reinterpret_cast<char*>(m_ROMBanks[bankIdx].bytes), 0x4000);
	}

	m_currentROMBank = ROMBanksCount >= 2 ? 1 : 0;
	m_cartridgeType = static_cast<CartridgeType>(m_ROMBanks[0].bytes[0x147]);

	if (m_cartridgeType >= MAX_SUPPORTED_TYPE)
	{
		fprintf(stderr, "Error: Unknown cartridge type: %u\n", m_cartridgeType);
	}
	else if (romFile)
	{
		printf("Read %ld bytes from rom\n", static_cast<long int>(romFile.gcount()));
		printf("Cartridge Type: (%u) %s\n", m_cartridgeType, CartridgeTypeDesc[m_cartridgeType]);
	}
	else
		fprintf(stderr, "Error: Read only %ld bytes from rom\n", static_cast<long int>(romFile.gcount()));

	romFile.close();

	uint8_t RAMBanksCount = 0;

	switch (m_ROMBanks[0].bytes[0x149]) // RAM size
	{
	case 1:
	case 2: RAMBanksCount = 1; break;
	case 3: RAMBanksCount = 4; break;
	case 4: RAMBanksCount = 16; break;
	}
	printf("RAM size: %u RAM banks\n", RAMBanksCount);

	if (RAMBanksCount > 0)
	{
		m_RAMBanks.reserve(RAMBanksCount);
		for (unsigned int i = 0; i < RAMBanksCount; ++i)
			m_RAMBanks.emplace_back(MemoryBank<0x2000>());

		m_currentRAMBank = 0;
	}
	
	switch (m_cartridgeType)
	{
	case ROM_ONLY:
	case ROM_RAM:
	case ROM_RAM_BATT:
		m_readFunc = &Cartridge::readMBC1;
		m_writeFunc = &Cartridge::writeROMOnly;
		break;
	case ROM_MBC1:
	case ROM_MBC1_RAM:
	case ROM_MBC1_RAM_BATT:
		m_readFunc = &Cartridge::readMBC1;
		m_writeFunc = &Cartridge::writeMBC1;
		break;
	case ROM_MBC2:
	case ROM_MBC2_BATT:
		m_readFunc = &Cartridge::readMBC2;
		m_writeFunc = &Cartridge::writeMBC2;
		break;
	default:
		break;
	}

}

uint8_t Cartridge::read(uint16_t address)
{
	return m_readFunc(*this, address);
}

uint8_t Cartridge::readMBC1(uint16_t address)
{
	if (address < 0x4000)
		return m_ROMBanks[0].bytes[address];
	else if (address < 0x8000) // Read in switchable bank
	{
		if (m_currentROMBank == 0x20 || m_currentROMBank == 0x40 || m_currentROMBank == 0x60)
			++m_currentROMBank;

		return m_ROMBanks[m_currentROMBank].bytes[address - 0x4000];
	}
	else if (address >= 0xA000 && address < 0xC000 && m_RAMBanks.size() > 0) // Read in switchable ram bank
		return m_RAMBanks[m_currentRAMBank].bytes[address - 0xA000];
	else
	{
		DEBUG_ONLY(fprintf(stderr, "Illegal read in Catridge at address 0x%04X\n", address););
		return 0xFF;
	}
}

void Cartridge::write(uint16_t address, uint8_t data)
{
	m_writeFunc(*this, address, data);
}

void Cartridge::writeROMOnly(uint16_t address, uint8_t value)
{
	(void) address;
	(void) value;
}

void Cartridge::writeMBC1(uint16_t address, uint8_t value)
{
	if (address >= 0x6000 && address <= 0x8000)
		m_MBC1Mode = value & 0x1;
	else if (address >= 0x2000 && address < 0x4000)
	{
		// Switch ROM Bank
		m_ROMBankSelector = (m_ROMBankSelector & 0xE0) | (value & 0x1F);
		if (m_ROMBankSelector < m_ROMBanks.size())
			m_currentROMBank = std::max(static_cast<uint8_t>(1), m_ROMBankSelector);
	}
	else if (address < 0x2000)
	{
		m_RAMWriteEnabled = (value & 0x0F) == 0x0A;
	}
	else if (address >= 0x4000 && address < 0x6000)
	{
		if (m_MBC1Mode == 0) // 16/8 Mode
		{
			//Two most significant bits of the ROM bank selection
			m_ROMBankSelector = (m_ROMBankSelector & 0x1F) | ((value & 0x3) << 5);
			if (m_ROMBankSelector < m_ROMBanks.size())
				m_currentROMBank = std::max(static_cast<uint8_t>(1), m_ROMBankSelector);
		}
		else // 4/32 Mode
		{
			// Switch RAM bank
			unsigned int bankNumber = value & 0x3;
			if (bankNumber < m_RAMBanks.size())
				m_currentRAMBank = bankNumber;
		}
	}
	else if (address >= 0xA000 && address < 0xC000 && m_RAMWriteEnabled)
	{
		m_RAMBanks[m_currentRAMBank].bytes[address - 0xA000] = value;
	}
}

uint8_t Cartridge::readMBC2(uint16_t address)
{
	if (address < 0x4000)
		return m_ROMBanks[0].bytes[address];
	else if (address < 0x8000) // Read in switchable bank
		return m_ROMBanks[m_currentROMBank].bytes[address - 0x4000];
	else if (address >= 0xA000 && address < 0xA200) // Read in MBC internal RAM
		return m_MBC2InternalRAM.bytes[address - 0xA000] & 0xF;
	else
	{
		DEBUG_ONLY(fprintf(stderr, "Illegal read in Catridge at address 0x%04X\n", address););
		return 0xFF;
	}
}

void Cartridge::writeMBC2(uint16_t address, uint8_t value)
{
	if (address < 0x2000 && (address & 0x100) == 0)
	{
		// RAM Enabling
		m_RAMWriteEnabled = ((value & 0xF) == 0x0A);
	}
	else if (address >= 0x2000 && address < 0x4000)
	{
		// Switch ROM Bank
		if ((address & 0x100) == 0x100)
		{
			m_currentROMBank = static_cast<uint8_t>(std::max(value & 0x0F, 1));
		}
	}
	else if (address >= 0xA000 && address < 0xA200 && m_RAMWriteEnabled)
	{
		m_MBC2InternalRAM.bytes[address - 0xA000] = (value & 0xF);
	}
	else
	{
		DEBUG_ONLY(fprintf(stderr, "Illegal write in Catridge at address 0x%04X\n", address););
	}
}