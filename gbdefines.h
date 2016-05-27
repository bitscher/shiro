#pragma once

#include <cstdint>

enum : unsigned int {
	SCREENBUFFER_WIDTH	= 160,
	SCREENBUFFER_HEIGHT	= 144
};

enum : uint16_t {
	TILE_MAP1_OFT	= 0x9800,
	TILE_MAP2_OFT	= 0x9C00,
	TILE_DATA1_OFT	= 0x8800,
	TILE_DATA2_OFT	= 0x8000,
	SPRITE_DATA_OFT	= 0x8000,

	OAM_OFT			= 0xFE00,	//Object Attribute Memory 40 * 4 Bytes
	DMA_OFT			= 0xFF46,
	LCDC_OFT		= 0xFF40,
	STAT_OFT		= 0xFF41,

	SCY_OFT			= 0xFF42,
	SCX_OFT			= 0xFF43,

	LCDC_Y_OFT		= 0xFF44,
	LCDC_CY_OFT		= 0xFF45,

	BGPAL_OFT 		= 0xFF47,	//Background Palette
	OBJ0PAL_OFT 	= 0xFF48,	//Sprite Palette #0
	OBJ1PAL_OFT 	= 0xFF49,	//Sprite Palette #1

	WY_OFT			= 0xFF4A,	//Window Y Position
	WX_OFT			= 0xFF4B,	//Window X Position minus 7

	TIMA_OFT		= 0xFF05,	//Timer counter
	TMA_OFT			= 0xFF06,	//Timer Modulo
	TAC_OFT			= 0xFF07,	//Timer Control

	IF_OFT			= 0xFF0F,
	IE_OFT			= 0xFFFF,

	P1_OFT			= 0xFF00
};