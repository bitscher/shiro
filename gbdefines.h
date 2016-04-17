#define COL_WHITE  0x0
#define COL_LTGREY 0x1
#define COL_DKGREY 0x2
#define COL_BLACK  0x3

#define SCREENBUFFER_WIDTH 160
#define SCREENBUFFER_HEIGHT 144

#define TILE_MAP1_OFT	0x9800
#define TILE_MAP2_OFT	0x9C00
#define TILE_DATA1_OFT	0x8800
#define TILE_DATA2_OFT	0x8000
#define SPRITE_DATA_OFT 0x8000

#define OAM_OFT			0xFE00	//Object Attribute Memory 40 * 4 Bytes
#define DMA_OFT			0xFF46
#define LCDC_OFT		0xFF40
#define STAT_OFT		0xFF41

#define SCY_OFT			0xFF42
#define SCX_OFT			0xFF43

#define LCDC_Y_OFT		0xFF44
#define LCDC_CY_OFT		0xFF45

#define BGPAL_OFT 		0xFF47	//Background Palette
#define OBJ0PAL_OFT 	0xFF48	//Sprite Palette #0
#define OBJ1PAL_OFT 	0xFF49	//Sprite Palette #1

#define WY_OFT			0xFF4A	//Window Y Position
#define WX_OFT			0xFF4B	//Window X Position minus 7

#define TIMA_OFT		0xFF05	//Timer counter
#define TMA_OFT			0xFF06	//Timer Modulo
#define TAC_OFT			0xFF07	//Timer Control

#define IF_OFT			0xFF0F
#define IE_OFT			0xFFFF

#define P1_OFT			0xFF00