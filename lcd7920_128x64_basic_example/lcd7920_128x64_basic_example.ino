/*
LCD ST7920 12864 basic example
*/


#include "U8glib.h"

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported
// devices with all constructor calls is here: http://code.google.com/p/u8glib/wiki/device
U8GLIB_ST7920_128X64 u8g(53, 51, 49, U8G_PIN_NONE);	// SPI Com: SCK=en--> 53 ,MOSI=rw-->51 ,CS=rs-->49

void draw(void)
{
	// graphic commands to redraw the complete screen should be placed here
	// a complete list of fonts is here: https://code.google.com/p/u8glib/wiki/fontsize
	//u8g.setFont(u8g_font_unifont);
	//u8g.setFont(u8g_font_osb21);
	//u8g.setFont(u8g_font_tpss);
	//u8g.setFont(u8g_font_p01type);
	//u8g.setFont(u8g_font_m2icon_9);
	u8g.setFont(u8g_font_04b_03);
	//u8g.drawStr( 0, 22, "Hello World!");
	int h;
	int w;

	u8g.setFontRefHeightExtendedText();
	u8g.setFontPosTop();

	h = u8g.getFontAscent() - u8g.getFontDescent();
	w = u8g.getStrWidth("u8g");
	u8g.drawStr(3, 10, "u8g");
	u8g.drawFrame(3 - 2, 10 - 1, w + 3, h + 3) ;
	u8g.setPrintPos( 0, 22);
	u8g.print("ColorIndex = ");
	u8g.print(u8g.getColorIndex());
}

void setup(void)
{

	// flip screen, if required
	// u8g.setRot180();

	// set SPI backup if required
	//u8g.setHardwareBackup(u8g_backup_avr_spi);

	// assign default color value
	if ( u8g.getMode() == U8G_MODE_R3G3B2 )
	{
		u8g.setColorIndex(255);     // white

	}
	else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
	{
		u8g.setColorIndex(3);         // max intensity

	}
	else if ( u8g.getMode() == U8G_MODE_BW )
	{
		u8g.setColorIndex(1);         // pixel on

	}
	else if ( u8g.getMode() == U8G_MODE_HICOLOR )
	{
		u8g.setHiColorByRGB(255, 255, 255);

	}
}

void loop(void)
{
	// picture loop
	u8g.firstPage();
	do
	{
		draw();
	}
	while( u8g.nextPage() );

	// rebuild the picture after some delay
	delay(500);
}


