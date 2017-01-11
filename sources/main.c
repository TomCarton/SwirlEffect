// -- Swirl Sample --
// Designed and written by Thomas Carton

// this sample is provided such as it is (with no optimization)
// you can use it freely inside your own project please just
// add my name in your credits screen ;)

// main.c


// -- includes --
#include <Agb.h>


// -- datas --
// - the 4 pictures used
// - with a unique palette for the sample
// - that allows to swirl from one image to another one
#include "celest_1.h"
#include "celest_2.h"
#include "celest_3.h"
#include "celest_4.h"

// - the 3 swirl tables
// - made from 3 240x160x256 bitmaps
// - each pixel indicates when it would be blitted to the screen 
#include "swirlref_1.h"
#include "swirlref_2.h"
#include "swirlref_3.h"

const u8 * pictures[] = { celest_1, celest_2, celest_3, celest_4 };
const u8 * swirls[] = { swirlref_1, swirlref_2, swirlref_3 };


// -- define --
#define SWIRL_SPEED_DEC		3


// -- typedef --
typedef void (*IntrFuncp)(void);


// -- extern --
extern void intr_main(void);


// -- global --
u32	IntrMainBuf[0x200];

u16 whitePalette[256];

u16 vblCount;


//-----------------------------------------------------------------------------
void IntrDummy(void)
//-----------------------------------------------------------------------------
{

}

//-----------------------------------------------------------------------------
void VBlankIntr(void)
//-----------------------------------------------------------------------------
{
	vblCount++;
	*(u16 *)INTR_CHECK_BUF = V_BLANK_INTR_FLAG;	// Set V Blank interrupt check
}

// -- interrupt handler table --
const IntrFuncp IntrTable[13] = {
	VBlankIntr,		// V Blank interrupt
	IntrDummy,		// H Blank interrupt
	IntrDummy,		// V Counter interrupt
	IntrDummy,		// Timer 0 interrupt
	IntrDummy,		// Timer 1 interrupt
	IntrDummy,		// Timer 2 interrupt
	IntrDummy,		// Timer 3 interrupt
	IntrDummy,		// Serial communication interrupt
	IntrDummy,		// DMA 0 interrupt
	IntrDummy,		// DMA 1 interrupt
	IntrDummy,		// DMA 2 interrupt
	IntrDummy,		// DMA 3 interrupt
	IntrDummy,		// Key interrupt
};

//-----------------------------------------------------------------------------
void Fade_To_Palette(const u16 * new_palette)
//-----------------------------------------------------------------------------
{
	u16 i;
	u8 active;
	u16 * palette;
	const u16 * new_pal;
	u16 colorA, colorB;
	u16 rA, gA, bA, rB, gB, bB;
			
	do
	{
		VBlankIntrWait();

		active = 0;

		new_pal = new_palette;
		palette = (u16*)PLTT;
	
		for (i=0; i<256; ++i)
		{
			colorA = *new_pal++;
			colorB = *palette;

			if (colorA!=colorB)
			{
				rA = colorA & 0x7C00;
				gA = colorA & 0x03E0;
				bA = colorA & 0x001F;

				rB = colorB & 0xFC00;
				gB = colorB & 0x03E0;
				bB = colorB & 0x001F;
				
				if (rA>rB)
					rB += 0x0400;
				else if (rA<rB)
					rB -= 0x0400;
					
				if (gA>gB)
					gB += 0x0020;
				else if (gA<gB)
					gB -= 0x0020;

				if (bA>bB)
					bB += 0x0001;
				else if (bA<bB)
					bB -= 0x0001;

				*palette++ = rB | gB | bB;
				
				active = 1;
			}
			else
				palette++;
		}
				
	} while (active);
}

//-----------------------------------------------------------------------------
void Swirl_Screen_To_Image(const u8 * new_image, const u8 * effect)
//-----------------------------------------------------------------------------
{
	s16 i, j;
	u16 * screen, * image;
	const u8 * table;

	for (j=0; j<(256>>SWIRL_SPEED_DEC); ++j)	
	{
		VBlankIntrWait();

		screen = (u16 *)VRAM;
		image = (u16 *) new_image;
		table = effect;
		
		for (i=0; i<120*160; ++i)
		{
			if (((*table)>>SWIRL_SPEED_DEC)==j)
				*screen = *image;
			
			table+=2;
			image++;
			screen++;
		}
	}
}

//-----------------------------------------------------------------------------
void AgbMain(void)
//-----------------------------------------------------------------------------
{
	u16 i;
	u16 pic_index, fx_index;
	u16 theKey;
	
	*(vu16 *)REG_WAITCNT = CST_ROM0_1ST_3WAIT | CST_ROM0_2ND_1WAIT | CST_PREFETCH_ENABLE;

	DmaClear(3, 0, EX_WRAM,  EX_WRAM_SIZE,         32);	// Clear external CPU work RAM
	DmaClear(3, 0, CPU_WRAM, CPU_WRAM_SIZE - 0x200,32);	// Clear internal CPU work RAM

    DmaCopy(3, intr_main, IntrMainBuf, sizeof(IntrMainBuf), 32); // Interrupt main routine set
    *(vu32 *)INTR_VECTOR_BUF = (vu32 )IntrMainBuf;

	*(vu16 *)REG_IME  = 1;										// Set IME
	*(vu16 *)REG_IE   = V_BLANK_INTR_FLAG | CASSETTE_INTR_FLAG;	// Permit V blank & cassette interrupts
	*(vu16 *)REG_STAT = STAT_V_BLANK_IF_ENABLE;

	// -- create a white palette
	for (i=0; i<256; ++i)
		whitePalette[i] = 0x7FFF;

	// -- clear screen data
	DmaClear(3, 0, VRAM, VRAM_SIZE, 16);
	// -- set the current palette to white
	DmaCopy(3, whitePalette, PLTT, 512, 32);
	// -- init the screen to the mode 4
	*(vu16*)REG_DISPCNT = DISP_MODE_4 | DISP_BG2_ON;

	// -- set the current palette with the pictures palette
	DmaCopy(3, celest_1Palette, PLTT, 512, 32);
	
	vblCount = 0;	
	pic_index = 0;
	fx_index = 0;
	Swirl_Screen_To_Image(pictures[pic_index], swirls[fx_index]);

	while(1)
	{
		theKey = *(vu16*)REG_KEYINPUT ^ ALL_KEY_MASK;
		
		if (theKey & A_BUTTON)
		{
			pic_index = (pic_index + 1) % 4;
			fx_index = (fx_index + ((vblCount >> 1) & 0x1)) % 3;
			Swirl_Screen_To_Image(pictures[pic_index], swirls[fx_index]);
		}
		
		if (theKey & B_BUTTON)
		{
			pic_index = (pic_index + 1) % 4;
			Fade_To_Palette(whitePalette);
			DmaCopy(3, pictures[pic_index], VRAM, 240*160, 32);
			Fade_To_Palette(celest_1Palette);
		}
		
		VBlankIntrWait();
	}
}
