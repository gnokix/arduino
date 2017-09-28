/*
LCD ST7920 12864 basic example
*/

#include "U8glib.h"

int potPin = 15;
int potVal = 0;


// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported 
// devices with all constructor calls is here: http://code.google.com/p/u8glib/wiki/device
U8GLIB_ST7920_128X64 u8g(53, 51, 49, U8G_PIN_NONE);	// SPI Com: SCK=en--> 53 ,MOSI=rw-->51 ,CS=rs-->49 

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  // a complete list of fonts is here: https://code.google.com/p/u8glib/wiki/fontsize
  u8g.setFont(u8g_font_04b_03);
  u8g.setPrintPos( 0, 22);
  u8g.print("Bright: ");
  u8g.print( potVal );
  //u8g.print( "%" );
}

void setup(void) {

  //Serial.begin(9600);

  // assign default color value
    u8g.setColorIndex(1);         // pixel on

}

void loop(void) {
  // Pot Reading
  potVal = analogRead(potPin);    // read the value from the sensor
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  // Serial.println(potVal);
  delay(10);        // delay in between reads for stability
  // delay(500);
}

