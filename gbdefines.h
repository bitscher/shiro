#pragma once

#include <cstdint>

constexpr unsigned int COL_WHITE  = 0x0;
constexpr unsigned int COL_LTGREY = 0x1;
constexpr unsigned int COL_DKGREY = 0x2;
constexpr unsigned int COL_BLACK  = 0x3;

constexpr unsigned int SCREENBUFFER_WIDTH	= 160;
constexpr unsigned int SCREENBUFFER_HEIGHT	= 144;

constexpr uint16_t TILE_MAP1_OFT	= 0x9800;
constexpr uint16_t TILE_MAP2_OFT	= 0x9C00;
constexpr uint16_t TILE_DATA1_OFT	= 0x8800;
constexpr uint16_t TILE_DATA2_OFT	= 0x8000;
constexpr uint16_t SPRITE_DATA_OFT	= 0x8000;

constexpr uint16_t OAM_OFT			= 0xFE00;	//Object Attribute Memory 40 * 4 Bytes
constexpr uint16_t DMA_OFT			= 0xFF46;
constexpr uint16_t LCDC_OFT			= 0xFF40;
constexpr uint16_t STAT_OFT			= 0xFF41;

constexpr uint16_t SCY_OFT			= 0xFF42;
constexpr uint16_t SCX_OFT			= 0xFF43;

constexpr uint16_t LCDC_Y_OFT		= 0xFF44;
constexpr uint16_t LCDC_CY_OFT		= 0xFF45;

constexpr uint16_t BGPAL_OFT 		= 0xFF47;	//Background Palette
constexpr uint16_t OBJ0PAL_OFT 		= 0xFF48;	//Sprite Palette #0
constexpr uint16_t OBJ1PAL_OFT 		= 0xFF49;	//Sprite Palette #1

constexpr uint16_t WY_OFT			= 0xFF4A;	//Window Y Position
constexpr uint16_t WX_OFT			= 0xFF4B;	//Window X Position minus 7

constexpr uint16_t TIMA_OFT			= 0xFF05;	//Timer counter
constexpr uint16_t TMA_OFT			= 0xFF06;	//Timer Modulo
constexpr uint16_t TAC_OFT			= 0xFF07;	//Timer Control

constexpr uint16_t IF_OFT			= 0xFF0F;
constexpr uint16_t IE_OFT			= 0xFFFF;

constexpr uint16_t P1_OFT			= 0xFF00;