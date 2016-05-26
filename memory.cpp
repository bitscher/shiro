#include "bitwiseop.h"
#include "config.h"
#include "memory.h"
#include <cstring>
#include <fstream>
#include <iostream>

Memory::Memory()
{
	initialize();
}

Memory::~Memory()
{
}

void Memory::initialize()
{
	// NOTE: It seems the real hardware has random data in memory at startup but let's clear it anyway
	std::memset(m_memory, 0, sizeof(m_memory));

	m_gamePadKeyMap = 0;

	m_memory[0xFF05] = 0x00; // TIMA
	m_memory[0xFF06] = 0x00; // TMA
	m_memory[0xFF07] = 0x00; // TAC
	m_memory[0xFF10] = 0x80; // NR10
	m_memory[0xFF11] = 0xBF; // NR11
	m_memory[0xFF12] = 0xF3; // NR12
	m_memory[0xFF14] = 0xBF; // NR14
	m_memory[0xFF16] = 0x3F; // NR21
	m_memory[0xFF17] = 0x00; // NR22
	m_memory[0xFF19] = 0xBF; // NR24
	m_memory[0xFF1A] = 0x7F; // NR30
	m_memory[0xFF1B] = 0xFF; // NR31
	m_memory[0xFF1C] = 0x9F; // NR32
	m_memory[0xFF1E] = 0xBF; // NR33
	m_memory[0xFF20] = 0xFF; // NR41
	m_memory[0xFF21] = 0x00; // NR42
	m_memory[0xFF22] = 0x00; // NR43
	m_memory[0xFF23] = 0xBF; // NR30
	m_memory[0xFF24] = 0x77; // NR50
	m_memory[0xFF25] = 0xF3; // NR51
	m_memory[0xFF26] = 0xF1; // NR52
	m_memory[0xFF40] = 0x91; // LCDC
	m_memory[0xFF41] = 0x80; // STAT
	m_memory[0xFF42] = 0x00; // SCY
	m_memory[0xFF43] = 0x00; // SCX
	m_memory[0xFF45] = 0x00; // LYC
	m_memory[0xFF47] = 0xFC; // BGP
	m_memory[0xFF48] = 0xFF; // OBP0
	m_memory[0xFF49] = 0xFF; // OBP1
	m_memory[0xFF4A] = 0x00; // WY
	m_memory[0xFF4B] = 0x00; // WX
	m_memory[0xFFFF] = 0x00; // IE

	m_bootROMEnabled = false;

	if (Config::getInstance().s_playBootRom)
	{
		std::ifstream bootROM;
		bootROM.open("bios.bin", std::ios_base::in | std::ios_base::binary);

		if (bootROM.good())
		{
			bootROM.read((char*)&m_bootROM, 256);
			m_bootROMEnabled = true;
		}
	}
}

uint8_t Memory::read(uint16_t address)
{
	if (m_bootROMEnabled && address < 0x100)
	{
		return m_bootROM[address];
	}
	if (address < 0x8000 || (address >= 0xA000 && address < 0xC000))
	{
		return m_cartridge.read(address);
	}
	else if (address >= 0xE000 && address < 0xFE00) // 8kB internal RAM echo
	{
		return m_memory[address - 0x2000];
	}
	else
	{
		return m_memory[address];
	}
}

void Memory::write(uint16_t address, uint8_t data)
{
	if (address == 0xFF02 && data == 0x81)
		m_memory[0xFF0F] = SET_BIT_N(m_memory[0xFF0F], 3);
	else if (address == 0xFF50 && data == 0x01)
	{
		std::cout << "Disabling boot rom" << std::endl;
		m_bootROMEnabled = false;
	}
	else if (address == DMA_OFT)
	{
		//TODO Emulate better timing
		memcpy(&m_memory[OAM_OFT], &m_memory[data << 8], 0xA0);
	}
	else if (address < 0x8000 || (address >= 0xA000 && address < 0xC000))
	{
		// Cartridge-mapped address ranges
		m_cartridge.write(address, data);
	}
	else if (address == P1_OFT)
	{
		if ((data & 0x10) != 0) // Selected P15
		{
			m_memory[address] = (data & 0xF0) | (~m_gamePadKeyMap & 0x0F);
		}
		else if ((data & 0x20) != 0) // Selected P14
		{
			m_memory[address] = (data & 0xF0) | ((~m_gamePadKeyMap >> 4) & 0x0F);
		}
	}
	else
		m_memory[address] = data;
}