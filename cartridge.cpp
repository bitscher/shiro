#include "cartridge.h"
#include "config.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(*(arr)))

enum MBCType {
	UNKNOWN,
	NO_MBC,
	MBC1,
	MBC2,
};

static const struct CartridgeDesc {
	const std::string name;
	enum MBCType mbc_type;
	bool has_ram;
	bool has_battery;
} CartDescriptions[] = {
	/* 0 */ {"ROM_ONLY",          NO_MBC,  false, false},
	/* 1 */ {"ROM_MBC1",          MBC1,    false, false},
	/* 2 */ {"ROM_MBC1_RAM",      MBC1,    true,  false},
	/* 3 */ {"ROM_MBC1_RAM_BATT", MBC1,    true,  true},
	/* 4 */ {"UNKNOWN",           UNKNOWN, false, false},
	/* 5 */ {"ROM_MBC2",          MBC2,    true,  false},
	/* 6 */ {"ROM_MBC2_BATT",     MBC2,    true,  true},
	/* 7 */ {"UNKNOWN",           UNKNOWN, false, false},
	/* 8 */ {"ROM_RAM",           NO_MBC,  true,  false},
	/* 9 */ {"ROM_RAM_BATT",      NO_MBC,  true,  true},
};

Cartridge* Cartridge::load(std::ifstream &romFile, std::ifstream &savFile) noexcept(false)
{
	uint8_t ROMBanksCount = 1;
	uint8_t RAMBanksCount = 0;
	uint8_t cartridgeType;
	std::vector<Cartridge::MemoryBank<0x4000>> ROMBanks;
	std::vector<Cartridge::MemoryBank<0x2000>> RAMBanks;

	/* Load first ROM bank */
	ROMBanks.emplace_back(Cartridge::MemoryBank<0x4000>());
	if (!romFile.read(reinterpret_cast<char*>(ROMBanks[0].bytes), sizeof(ROMBanks[0].bytes)))
		throw std::runtime_error("Invalid ROM file");

	cartridgeType =   ROMBanks[0].bytes[0x147];
	uint8_t romSize = ROMBanks[0].bytes[0x148];
	uint8_t ramSize = ROMBanks[0].bytes[0x149];

	if (cartridgeType >= ARRAY_SIZE(CartDescriptions) || CartDescriptions[cartridgeType].mbc_type == UNKNOWN)
		throw std::runtime_error("Error: Unknown cartridge type: " + cartridgeType);

	if (romSize <= 6)
		ROMBanksCount = static_cast<unsigned int>(std::pow(2, romSize + 1));
	else if (romSize == 0x52)
		ROMBanksCount = 72;
	else if (romSize == 0x53)
		ROMBanksCount = 80;
	else if (romSize == 0x54)
		ROMBanksCount = 96;
	else
		throw std::runtime_error("Unknown ROM Size (0x148) = " + romSize);

	switch (ramSize)
	{
	case 1: /* Only first 2KB of 1 bank */
	case 2: RAMBanksCount = 1; break;
	case 3: RAMBanksCount = 4; break;
	case 4: RAMBanksCount = 16; break;
	}

	if (CartDescriptions[cartridgeType].mbc_type == MBC2)
		RAMBanksCount = 1;

	std::cout << "ROM size: " << static_cast<uint32_t>(ROMBanksCount) << " ROM banks" << std::endl;
	std::cout << "RAM size: " << static_cast<uint32_t>(RAMBanksCount) << " RAM banks" << std::endl;

	ROMBanks.resize(ROMBanksCount, Cartridge::MemoryBank<0x4000>());
	//Read the rest of the ROM banks
	for (unsigned int i = 1; i < ROMBanksCount && romFile; ++i)
	{
		if (!romFile.read(reinterpret_cast<char*>(ROMBanks[i].bytes), sizeof(ROMBanks[i].bytes)))
			throw std::runtime_error("File too small, bytes read: " + romFile.gcount());
	}

	std::cout << "Read " << static_cast<int32_t>(romFile.gcount()) << " bytes from rom" << std::endl;
	std::cout << "Cartridge Type: (" << static_cast<uint32_t>(cartridgeType) << ") " << CartDescriptions[cartridgeType].name << std::endl;

	if (RAMBanksCount > 0)
		RAMBanks.resize(RAMBanksCount, Cartridge::MemoryBank<0x2000>());

	if (CartDescriptions[cartridgeType].has_ram && savFile.is_open())
	{
		for (unsigned int i = 0; i < RAMBanks.size() && !savFile.eof(); ++i)
			savFile.read(reinterpret_cast<char*>(RAMBanks[i].bytes), sizeof(RAMBanks[i].bytes));
	}

	switch(CartDescriptions[cartridgeType].mbc_type)
	{
	case NO_MBC:
		return new CartridgeNoMBC(ROMBanks, RAMBanks, CartDescriptions[cartridgeType].has_battery);
	case MBC1:
		return new CartridgeMBC1(ROMBanks, RAMBanks, CartDescriptions[cartridgeType].has_battery);
	case MBC2:
		return new CartridgeMBC2(ROMBanks, RAMBanks, CartDescriptions[cartridgeType].has_battery);
	case UNKNOWN:
	default:
		throw std::runtime_error("Unknown Cartridge type");
	}

}

Cartridge::Cartridge(std::vector<MemoryBank<0x4000>> &ROMBanks, std::vector<MemoryBank<0x2000>> &RAMBanks, bool has_battery) :
	m_ROMBanks(std::move(ROMBanks)),
	m_RAMBanks(std::move(RAMBanks)),
	m_currentRAMBank(0),
	m_RAMWriteEnabled(false),
	m_hasBattery(has_battery)
{
	m_currentROMBank = std::min(static_cast<size_t>(1), m_ROMBanks.size() - 1);
}

void Cartridge::saveRAMToFile(const std::string& romPath)
{
	if (!m_hasBattery)
		return;

	std::ofstream savFile;

	savFile.open(romPath + ".sav", std::ios_base::out | std::ios_base::binary);
	if (!savFile.is_open())
	{
		fprintf(stderr, "Could not create sav file at %s.sav\n", romPath.c_str());
		return;
	}

	for (const auto& bank : m_RAMBanks)
		savFile.write(reinterpret_cast<const char*>(bank.bytes), sizeof(bank.bytes));

	savFile.close();
}

CartridgeNoMBC::CartridgeNoMBC(std::vector<MemoryBank<0x4000>> &ROMBanks, std::vector<MemoryBank<0x2000>> &RAMBanks, bool has_battery) :
	Cartridge(ROMBanks, RAMBanks, has_battery)
{}

uint8_t CartridgeNoMBC::read(uint16_t address)
{
	if (address < 0x4000)
		return m_ROMBanks[0].bytes[address];
	else if (address < 0x8000)
		return m_ROMBanks[m_currentROMBank].bytes[address - 0x4000];
	else if (address >= 0xA000 && address < 0xC000 && m_RAMBanks.size() > 0) // Read in switchable ram bank
		return m_RAMBanks[m_currentRAMBank].bytes[address - 0xA000];

	DEBUG_ONLY(fprintf(stderr, "Illegal read in Catridge at address 0x%04X\n", address););
	return 0xFF;
}

void CartridgeNoMBC::write(uint16_t address, uint8_t value)
{
	(void) address;
	(void) value;
}

CartridgeMBC1::CartridgeMBC1(std::vector<Cartridge::MemoryBank<0x4000>> &ROMBanks, std::vector<Cartridge::MemoryBank<0x2000>> &RAMBanks, bool has_battery) :
	Cartridge(ROMBanks, RAMBanks, has_battery),
	m_MBC1Mode(0),
	m_ROMBankSelector(0)
{}

uint8_t CartridgeMBC1::read(uint16_t address)
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

	DEBUG_ONLY(fprintf(stderr, "Illegal read in Catridge at address 0x%04X\n", address););
	return 0xFF;
}

void CartridgeMBC1::write(uint16_t address, uint8_t value)
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

CartridgeMBC2::CartridgeMBC2(std::vector<Cartridge::MemoryBank<0x4000>> &ROMBanks, std::vector<Cartridge::MemoryBank<0x2000>> &RAMBanks, bool has_battery) :
	Cartridge(ROMBanks, RAMBanks, has_battery)
{}

uint8_t CartridgeMBC2::read(uint16_t address)
{
	if (address < 0x4000)
		return m_ROMBanks[0].bytes[address];
	else if (address < 0x8000) // Read in switchable bank
		return m_ROMBanks[m_currentROMBank].bytes[address - 0x4000];
	else if (address >= 0xA000 && address < 0xA200)
		return m_RAMBanks[0].bytes[address - 0xA000] & 0xF;

	DEBUG_ONLY(fprintf(stderr, "Illegal read in Catridge at address 0x%04X\n", address););
	return 0xFF;
}

void CartridgeMBC2::write(uint16_t address, uint8_t value)
{
	if (address < 0x2000 && (address & 0x100) == 0) // RAM Enabling
		m_RAMWriteEnabled = ((value & 0xF) == 0x0A);
	else if (address >= 0x2000 && address < 0x4000 && (address & 0x100) == 0x100) // Switch ROM Bank
		m_currentROMBank = static_cast<uint8_t>(std::max(value & 0x0F, 1));
	else if (address >= 0xA000 && address < 0xA200 && m_RAMWriteEnabled)
		m_RAMBanks[0].bytes[address - 0xA000] = (value & 0xF);
}
