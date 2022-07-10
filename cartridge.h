#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <string>

class Cartridge
{

public:
	template<unsigned int size>
	struct MemoryBank { uint8_t bytes[size]; };

	Cartridge(std::vector<MemoryBank<0x4000>> &ROMBanks, std::vector<MemoryBank<0x2000>> &RAMBanks, bool has_battery);
	virtual ~Cartridge() = default;

	virtual uint8_t read(uint16_t address) = 0;
	virtual void write(uint16_t address, uint8_t data) = 0;

	void saveRAMToFile(const std::string& romPath);

	static Cartridge* load(std::ifstream &romFile, std::ifstream &savFile) noexcept(false);

protected:
	const std::vector<MemoryBank<0x4000>> m_ROMBanks;
	std::vector<MemoryBank<0x2000>> m_RAMBanks;

	uint8_t m_currentROMBank;
	uint8_t m_currentRAMBank;

	bool m_RAMWriteEnabled;

	const bool m_hasBattery;
};


class CartridgeNoMBC : public Cartridge
{
public:
	CartridgeNoMBC(std::vector<Cartridge::MemoryBank<0x4000>> &ROMBanks, std::vector<Cartridge::MemoryBank<0x2000>> &RAMBanks, bool has_battery);
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);
};

class CartridgeMBC1 : public Cartridge
{
private:
	uint8_t m_MBC1Mode;
	uint8_t m_ROMBankSelector;

public:
	CartridgeMBC1(std::vector<Cartridge::MemoryBank<0x4000>> &ROMBanks, std::vector<Cartridge::MemoryBank<0x2000>> &RAMBanks, bool has_battery);
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);
};

class CartridgeMBC2 : public Cartridge
{
public:
	CartridgeMBC2(std::vector<Cartridge::MemoryBank<0x4000>> &ROMBanks, std::vector<Cartridge::MemoryBank<0x2000>> &RAMBanks, bool has_battery);
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);
};
