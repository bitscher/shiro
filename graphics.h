#pragma once

#include <cstdint>

#include "utils.h"

#include "memory.h"

class Graphics
{
public:
	static const uint8_t s_greyShades[4];

	enum class lcd_mode_t: uint8_t {
		LCD_MODE0,	//Mode 0 - H-Blank
		LCD_MODE1,	//Mode 1 - V-Blank
		LCD_MODE2,	//Mode 2 - Reading OAM memory
		LCD_MODE3	//Mode 3 - Reading OAM and VRAM
	};

	using OAM_Entry = struct {
		uint8_t yPos;
		uint8_t xPos;
		uint8_t tileIdx;
		uint8_t attributeFlags;

		bool getPriority() { return AND_BIT_N(attributeFlags, 7) != 0; }
		bool isYFlipped() { return AND_BIT_N(attributeFlags, 6) != 0; }
		bool isXFlipped() { return AND_BIT_N(attributeFlags, 5) != 0; }
		bool getPalette() { return AND_BIT_N(attributeFlags, 4) != 0; }
	};

	Graphics(Memory & memory);

	void setLCDOperationMode(lcd_mode_t mode);
	void incrementY();

	bool shouldStartVBlank() { return m_memory[LCDC_Y_OFT] >= 143; }
	inline bool isLCDEnabled() { return AND_BIT_N(m_memory[LCDC_OFT], 7) != 0; }

	void* getScreenBuffer() { return static_cast<void*>(m_screenBuffer); }
	uint32_t getScreenBufferSize() { return sizeof(m_screenBuffer); }

	void* getSpriteDebugBuffer() { return static_cast<void*>(m_spriteDebugBuffer); }
	uint32_t getSpriteDebugBufferSize() { return sizeof(m_spriteDebugBuffer); }
	void fillSpriteDebugBuffer();

private:
	uint8_t m_screenBuffer[SCREENBUFFER_HEIGHT][SCREENBUFFER_WIDTH][3];
	uint8_t m_spriteDebugBuffer[16][8*40][3];
	Memory & m_memory;

	lcd_mode_t m_currentLCDMode;

// --------------------------
// Drawing methods
// --------------------------
	void drawLine();
	void drawBG(uint8_t currentLine);
	void drawWin(uint8_t currentLine);
	void drawObj(uint8_t currentLine);

// --------------------------
// LCD Position and Scrolling
// --------------------------
	inline uint8_t getYScroll() { return m_memory[SCY_OFT]; }
	inline uint8_t getXScroll() { return m_memory[SCX_OFT]; }

	inline uint8_t getYCoordinate() { return m_memory[LCDC_Y_OFT]; }
	inline uint8_t getYCompareCoord() { return m_memory[LCDC_CY_OFT]; }

	inline uint8_t getWindowYPos() { return m_memory[WY_OFT]; }
	inline uint8_t getWindowXPos() { return m_memory[WX_OFT]; }

// --------------------------
// LCD Monochrome Palettes
// --------------------------
	inline uint8_t getBGPalette() { return m_memory[BGPAL_OFT]; }

// LCD Control IO access (0xFF40)
	inline uint16_t getWinTileMapAddress() { return AND_BIT_N(m_memory[LCDC_OFT], 6) ? TILE_MAP2_OFT : TILE_MAP1_OFT; }
	inline bool isWindowDisplayEnabled() { return AND_BIT_N(m_memory[LCDC_OFT], 5) != 0; }
	inline uint16_t getTileDataAddress() { return AND_BIT_N(m_memory[LCDC_OFT], 4) ? TILE_DATA2_OFT : TILE_DATA1_OFT; }
	inline uint16_t getBGTileMapAddress() { return AND_BIT_N(m_memory[LCDC_OFT], 3) ? TILE_MAP2_OFT : TILE_MAP1_OFT; }
	inline bool use8x16Sprites() { return AND_BIT_N(m_memory[LCDC_OFT], 2) != 0; }
	inline bool spritesEnabled() { return AND_BIT_N(m_memory[LCDC_OFT], 1) != 0; }
	inline bool displayBGEnabled() { return AND_BIT_N(m_memory[LCDC_OFT], 0) != 0; }

// LCD Status Register
	inline bool isCoincidenceInterrupt() { return AND_BIT_N(m_memory[STAT_OFT], 6) != 0; }
	inline bool isOAMInterrupt() { return AND_BIT_N(m_memory[STAT_OFT], 5) != 0; }
	inline bool isInVBlank() { return AND_BIT_N(m_memory[STAT_OFT], 4) != 0; }
	inline bool isInHBlank() { return AND_BIT_N(m_memory[STAT_OFT], 3) != 0; }
	inline bool getCoincidenceFlag() { return AND_BIT_N(m_memory[STAT_OFT], 2) != 0; }
	inline uint8_t getLCDOperationMode() { return ((m_memory[STAT_OFT]) & 0x3); }
};
