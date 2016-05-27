#include "graphics.h"

#include <cstring>
#include <iostream>

const uint8_t Graphics::s_greyShades[] = { 255, 170, 85, 0 };

Graphics::Graphics(Memory & memory): m_memory(memory)
{
	std::memset(m_screenBuffer, 0xFF, sizeof(m_screenBuffer));
	m_currentLCDMode = LCD_MODE2;
}

void Graphics::drawLine() {
	uint8_t currentLine = getYCoordinate();

	drawBG(currentLine);
	drawWin(currentLine);
	drawObj(currentLine);
}

void Graphics::drawBG(uint8_t currentLine)
{
	if(displayBGEnabled())
	{
		uint16_t bgTileMapAddress = getBGTileMapAddress();
		uint16_t tileDataAddress = getTileDataAddress();

		uint8_t xScroll = getXScroll();
		uint8_t yScroll = getYScroll();

		uint16_t bgCoordY = yScroll + currentLine;
		uint8_t tileCoordY = (bgCoordY / 8) % 32;
		uint8_t spriteCoordY = (bgCoordY % 8);
		uint16_t lineOffset = spriteCoordY * 2;

		uint8_t palette = getBGPalette();

		for (uint8_t x = 0; x < 160;)
		{
			uint16_t bgCoordX = xScroll + x;

			uint8_t tileCoordX = (bgCoordX / 8) % 32;

			uint8_t tileIdx = m_memory[bgTileMapAddress + tileCoordY * 32 + tileCoordX];

			if (tileDataAddress == TILE_DATA1_OFT)
				tileIdx = 128 + static_cast<int8_t>(tileIdx);

			//Fetch the right tile byte for this tile index
			uint8_t tileDataLower = m_memory[tileDataAddress + tileIdx * 16 + lineOffset];
			uint8_t tileDataUpper = m_memory[tileDataAddress + tileIdx * 16 + lineOffset + 1];

			for (uint8_t pix = (bgCoordX % 8); pix < 8 && x < 160; ++pix, ++x)
			{
				uint8_t colorIdx = (((tileDataUpper >> (7-pix)) & 0x1) << 1) | ((tileDataLower >> (7-pix)) & 0x1);
				// Fetch color in palette
				uint8_t color = (palette >> (2*colorIdx)) & 0x3;
				uint8_t greyLevel = s_greyShades[color];

				m_screenBuffer[currentLine][x][0] = greyLevel;
				m_screenBuffer[currentLine][x][1] = greyLevel;
				m_screenBuffer[currentLine][x][2] = greyLevel;
			}
		}
	}
	else
	{
		std::memset(m_screenBuffer[currentLine], 0xFF, 160*3);
	}
}

void Graphics::drawWin(uint8_t currentLine)
{
	if (isWindowDisplayEnabled())
	{
		uint8_t winXPos = getWindowXPos() - 7;
		uint8_t winYPos = getWindowYPos();

		if (winXPos < 160 && winYPos <= currentLine)
		{
			uint16_t winTileMapAddress = getWinTileMapAddress();
			uint16_t tileDataAddress = getTileDataAddress();

			uint8_t visiblePixels = 160 - winXPos;

			uint16_t winCoordY = currentLine - winYPos;
			uint8_t tileCoordY = (winCoordY / 8) % 32;
			uint16_t lineOffset = (winCoordY % 8) * 2;

			uint8_t palette = getBGPalette();

			for (uint8_t x = 0; x < visiblePixels;)
			{
				uint8_t tileCoordX = (x / 8) % 32;

				uint8_t tileIdx = m_memory[winTileMapAddress + tileCoordY * 32 + tileCoordX];

				if (tileDataAddress == TILE_DATA1_OFT)
					tileIdx = 128 + static_cast<int8_t>(tileIdx);

				//Fetch the right tile byte for this tile index
				uint8_t tileDataLower = m_memory[tileDataAddress + tileIdx * 16 + lineOffset];
				uint8_t tileDataUpper = m_memory[tileDataAddress + tileIdx * 16 + lineOffset + 1];

				for (uint8_t pix = (x % 8); pix < 8 && x < visiblePixels; ++pix, ++x)
				{
					uint8_t colorIdx = (((tileDataUpper >> (7-pix)) & 0x1) << 1) | ((tileDataLower >> (7-pix)) & 0x1);
					// Fetch color in palette
					uint8_t color = (palette >> (2*colorIdx)) & 0x3;

					uint8_t greyLevel = s_greyShades[color];

					m_screenBuffer[currentLine][x + winXPos][0] = greyLevel;
					m_screenBuffer[currentLine][x + winXPos][1] = greyLevel;
					m_screenBuffer[currentLine][x + winXPos][2] = greyLevel;
				}
			}
		}
	}
}

static const unsigned char BitReverseTable256[256] =
{
	#define R2(n) n, n + 2*64, n + 1*64, n + 3*64
	#define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
	#define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )
	R6(0), R6(2), R6(1), R6(3)
};

void Graphics::drawObj(uint8_t currentLine)
{
	if (spritesEnabled())
	{
		uint8_t spriteHeight;
		uint8_t spriteSizeBytes;
		if (use8x16Sprites())
		{
			spriteHeight = 16;
			spriteSizeBytes = 2 * 16;
		}
		else
		{
			spriteHeight = 8;
			spriteSizeBytes = 2 * 8;
		}
		OAM_Entry *oam = reinterpret_cast<OAM_Entry*>(&m_memory[OAM_OFT]);
		

		uint8_t curSpriteXPosForPix[160]; // Keep the index
		int16_t rowPixelsColors[160];
		memset(curSpriteXPosForPix, 255, sizeof(curSpriteXPosForPix));
		memset(rowPixelsColors, 0xFF, sizeof(rowPixelsColors));

		// Hardware limit to 10 sprites per scanline
		for(uint8_t sprite = 0, spritesShown = 0; sprite < 40 && spritesShown < 10; ++sprite)
		{
			OAM_Entry &curSprite = oam[sprite];
			if(curSprite.yPos > 0 && curSprite.yPos < 160)
			{   // Sprite is visible

				int16_t upperY = curSprite.yPos - 16;
				if (currentLine < upperY || (currentLine - upperY) >= spriteHeight)
					continue; // Sprite will not show on this scanline

				uint16_t paletteOffset = AND_BIT_N(curSprite.attributeFlags, 4) ? OBJ1PAL_OFT : OBJ0PAL_OFT;
				uint8_t palette = m_memory[paletteOffset];
				bool drawOnTop = AND_BIT_N(curSprite.attributeFlags, 7) == 0;

				// Fetch the correct tile data
				uint8_t tileDataLower = m_memory[SPRITE_DATA_OFT + spriteSizeBytes * curSprite.tileIdx + (currentLine - upperY) * 2];
				uint8_t tileDataUpper = m_memory[SPRITE_DATA_OFT + spriteSizeBytes * curSprite.tileIdx + (currentLine - upperY) * 2 + 1];

				if (AND_BIT_N(curSprite.attributeFlags, 5) != 0) // X Flip
				{
					//uint8_t tmp = tileDataLower;
					tileDataLower = BitReverseTable256[tileDataLower];
					tileDataUpper = BitReverseTable256[tileDataUpper];
				}

				bool spriteDrawn = false;
				for(uint8_t pix = 0; pix < 8; ++pix)
				{
					int16_t screenPix = curSprite.xPos + pix - 8;
					if (screenPix > 0 && screenPix < 160 && curSprite.xPos <= curSpriteXPosForPix[screenPix])
					{
						uint8_t colorIdx = (((tileDataUpper >> (7-pix)) & 0x1) << 1) | ((tileDataLower >> (7-pix)) & 0x1);

						curSpriteXPosForPix[screenPix] = curSprite.xPos;

						if (colorIdx != 0 && (drawOnTop || m_screenBuffer[currentLine][screenPix][0] == s_greyShades[0]))
						{
							// Fetch color in palette
							uint8_t color = (palette >> (2*colorIdx)) & 0x3;

							// Replace the color for one from this sprite
							rowPixelsColors[screenPix] = color;
						}

						spriteDrawn = true;
					}
				}

				if (spriteDrawn)
					++spritesShown;
			}
		}

		for (uint8_t rowPix = 0; rowPix < 160; ++rowPix)
		{
			if (rowPixelsColors[rowPix] >= 0)
			{
				uint8_t greyLevel = s_greyShades[rowPixelsColors[rowPix]];

				m_screenBuffer[currentLine][rowPix][0] = greyLevel;
				m_screenBuffer[currentLine][rowPix][1] = greyLevel;
				m_screenBuffer[currentLine][rowPix][2] = greyLevel;
			}
		}
	}
}


void Graphics::fillSpriteDebugBuffer()
{
	uint8_t spriteHeight = use8x16Sprites() ? 16 : 8;
	uint8_t spriteSizeBytes = 2 * spriteHeight;

	for (uint8_t line = 0; line < 16; ++line)
		for (uint16_t col = 0; col < 320; ++col)
		{
			m_spriteDebugBuffer[line][col][0] = 0x00;
			m_spriteDebugBuffer[line][col][1] = 0xA0;
			m_spriteDebugBuffer[line][col][2] = 0x70;
		}

	OAM_Entry *oam = reinterpret_cast<OAM_Entry*>(&m_memory[OAM_OFT]);

	// Hardware limit to 10 sprites per scanline
	for (uint8_t sprite = 0; sprite < 40; ++sprite)
	{
		OAM_Entry &curSprite = oam[sprite];

		for (unsigned int line = 0; line < spriteHeight; ++line)
		{
			uint16_t paletteOffset = AND_BIT_N(curSprite.attributeFlags, 4) ? OBJ1PAL_OFT : OBJ0PAL_OFT;
			uint8_t palette = m_memory[paletteOffset];

			// Fetch the correct tile data
			uint8_t tileIdx = use8x16Sprites() ? curSprite.tileIdx & 0xFE : curSprite.tileIdx;
			uint8_t tileDataLower = m_memory[SPRITE_DATA_OFT + spriteSizeBytes * tileIdx + line * 2];
			uint8_t tileDataUpper = m_memory[SPRITE_DATA_OFT + spriteSizeBytes * tileIdx + line * 2 + 1];

			if (AND_BIT_N(curSprite.attributeFlags, 5) != 0) // X Flip
			{
				//uint8_t tmp = tileDataLower;
				tileDataLower = BitReverseTable256[tileDataLower];
				tileDataUpper = BitReverseTable256[tileDataUpper];
			}

			for (uint8_t pix = 0; pix < 8; ++pix)
			{
				uint8_t colorIdx = (((tileDataUpper >> (7 - pix)) & 0x1) << 1) | ((tileDataLower >> (7 - pix)) & 0x1);

				if (colorIdx != 0)
				{
					// Fetch color in palette
					uint8_t color = (palette >> (2 * colorIdx)) & 0x3;

					uint8_t greyLevel = s_greyShades[color];

					m_spriteDebugBuffer[line][8 * sprite + pix][0] = greyLevel;
					m_spriteDebugBuffer[line][8 * sprite + pix][1] = greyLevel;
					m_spriteDebugBuffer[line][8 * sprite + pix][2] = greyLevel;
				}
			}
		}
	}
}

void Graphics::setLCDOperationMode(lcd_mode_t mode)
{
	//if (!isLCDEnabled())
	//    return;

	m_memory[STAT_OFT] &= 0xC7; // Clear bits 5, 4 & 3

	switch(mode)
	{
		case LCD_MODE0:     //H-Blank
			// Set Interrupt flag in LCD STAT (0xFF41.3)
			m_memory[STAT_OFT] = SET_BIT_N(m_memory[STAT_OFT], 3);

			// If enable set interrupt flag for LCD STAT (0xFF0F.1)
			if (AND_BIT_N(m_memory[IE_OFT], 1))
				m_memory[IF_OFT] = SET_BIT_N(m_memory[IF_OFT], 1);
			break;
		case LCD_MODE1:     //V-Blank
			// Set Interrupt flag in LCD STAT (0xFF41.4)
			m_memory[STAT_OFT] = SET_BIT_N(m_memory[STAT_OFT], 4);

			// Set current line to 144
			m_memory[LCDC_Y_OFT] = 144;

			// If enable set interrupt flag for V-BLANK (0xFF0F.0)
			if (AND_BIT_N(m_memory[IE_OFT], 0))
				m_memory[IF_OFT] = SET_BIT_N(m_memory[IF_OFT], 0);
			break;
		case LCD_MODE2:     //Reading OAM memory
			// Update Y register (0xFF44), reset to zero if we just finished V-Blank
			m_memory[LCDC_Y_OFT] = (m_currentLCDMode == LCD_MODE1) ? 0 : getYCoordinate() + 1 ;
			// Set mode 2 interrupt flag in LCD STAT
			m_memory[STAT_OFT] = SET_BIT_N(m_memory[STAT_OFT], 5);
			// Check if LY=LYC
			if (getYCoordinate() == getYCompareCoord())
				m_memory[STAT_OFT] = SET_BIT_N(m_memory[STAT_OFT], 6);

			// If enabled, set interrupt flag for LCD STAT (0xFF0F.1)
			if (AND_BIT_N(m_memory[IE_OFT], 1))
				m_memory[IF_OFT] = SET_BIT_N(m_memory[IF_OFT], 1);
			break;
		case LCD_MODE3:     //Reading OAM and VRAM
			// Draw the current line
			drawLine();
			break;
	}

	m_currentLCDMode = mode;
	m_memory[STAT_OFT] = (m_memory[STAT_OFT] & 0xFC) | static_cast<uint8_t>(mode);
}

void Graphics::incrementY()
{
	m_memory[LCDC_Y_OFT] = getYCoordinate() + 1;
}
