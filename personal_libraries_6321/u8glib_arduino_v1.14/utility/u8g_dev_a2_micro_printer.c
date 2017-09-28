/*

  u8g_dev_a2_micro_printer_ds.c

  Use DC2 bitmap command of the A2 Micro panel termal printer
  double stroke

  Universal 8bit Graphics Library
  
  Copyright (c) 2013, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS &quot;AS IS&quot; AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
  
*/

#include &quot;u8g.h&quot;

#define WIDTH 240
#define HEIGHT 128
#define PAGE_HEIGHT 8

#define LINE_DELAY 40


uint8_t u8g_dev_a2_micro_printer_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_NONE);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        uint8_t y, i, j;
        uint8_t *ptr;
        u8g_pb_t *pb = (u8g_pb_t *)(dev-&gt;dev_mem);
        
        y = pb-&gt;p.page_y0;
        ptr = pb-&gt;buf;

	u8g_WriteByte(u8g, dev, 27);      /* ESC */
	u8g_WriteByte(u8g, dev, 55 );      /* parameter command */
	u8g_WriteByte(u8g, dev, 7);      /* Max printing dots,Unit(8dots),Default:7(64 dots) 8*(x+1)*/
	u8g_WriteByte(u8g, dev, 160);      /* 3-255 Heating time,Unit(10us),Default:80(800us) */
	u8g_WriteByte(u8g, dev, 20);      /* 0-255 Heating interval,Unit(10us),Default:2(20us)*/
	
	u8g_WriteByte(u8g, dev, 18);      /* DC2 */
	u8g_WriteByte(u8g, dev, 42 );      /* *  */
	u8g_WriteByte(u8g, dev, pb-&gt;p.page_height ); 
	u8g_WriteByte(u8g, dev, pb-&gt;width/8 ); 
	
        for( i = 0; i &lt; pb-&gt;p.page_height; i ++ )
        {
	  for( j = 0; j &lt; pb-&gt;width/8; j++ )
	  {
	    u8g_WriteByte(u8g, dev, *ptr);
	    ptr++;
	  }
	  u8g_Delay(LINE_DELAY);
          y++;
        }

	/* set parameters back to their default values */
	u8g_WriteByte(u8g, dev, 27);      /* ESC */
	u8g_WriteByte(u8g, dev, 55 );      /* parameter command */
	u8g_WriteByte(u8g, dev, 7);      /* Max printing dots,Unit(8dots),Default:7(64 dots) 8*(x+1)*/
	u8g_WriteByte(u8g, dev, 80);      /* 3-255 Heating time,Unit(10us),Default:80(800us) */
	u8g_WriteByte(u8g, dev, 2);      /* 0-255 Heating interval,Unit(10us),Default:2(20us)*/
	
      }
      break;
  }
  return u8g_dev_pb8h1_base_fn(u8g, dev, msg, arg);
}

static uint8_t u8g_dev_expand4(uint8_t val)
{
  uint8_t a,b,c,d;
  a = val&amp;1;
  b = (val&amp;2)&lt;&lt;1;
  c = (val&amp;4)&lt;&lt;2;
  d = (val&amp;8)&lt;&lt;3;
  a |=b;
  a |=c;
  a |=d;
  a |= a&lt;&lt;1;
  return a;
}

uint8_t u8g_dev_a2_micro_printer_double_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_NONE);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_FIRST:
      {
        //u8g_pb_t *pb = (u8g_pb_t *)(dev-&gt;dev_mem);
	//u8g_WriteByte(u8g, dev, 18);      /* DC2 */
	//u8g_WriteByte(u8g, dev, 42 );      /* *  */
	//u8g_WriteByte(u8g, dev, pb-&gt;p.total_height*2 ); 
	//u8g_WriteByte(u8g, dev, pb-&gt;width/8*2 ); 
      }
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        uint8_t y, i, j;
        uint8_t *ptr;
        uint8_t *p2;
        u8g_pb_t *pb = (u8g_pb_t *)(dev-&gt;dev_mem);
        
        y = pb-&gt;p.page_y0;
        ptr = pb-&gt;buf;
	//u8g_WriteByte(u8g, dev, 18);      /* DC2 */
	//u8g_WriteByte(u8g, dev, 35 );      /* #  */
	//u8g_WriteByte(u8g, dev, 0x0ff );      /* max  */

	u8g_WriteByte(u8g, dev, 27);      /* ESC */
	u8g_WriteByte(u8g, dev, 55 );      /* parameter command */
	u8g_WriteByte(u8g, dev, 7);      /* Max printing dots,Unit(8dots),Default:7(64 dots) 8*(x+1)*/
	u8g_WriteByte(u8g, dev, 160);      /* 3-255 Heating time,Unit(10us),Default:80(800us) */
	u8g_WriteByte(u8g, dev, 20);      /* 0-255 Heating interval,Unit(10us),Default:2(20us)*/
	
	u8g_WriteByte(u8g, dev, 18);      /* DC2 */
	u8g_WriteByte(u8g, dev, 42 );      /* *  */
	u8g_WriteByte(u8g, dev, pb-&gt;p.page_height*2 ); 
	u8g_WriteByte(u8g, dev, pb-&gt;width/8*2 ); 
	
        for( i = 0; i &lt; pb-&gt;p.page_height; i ++ )
        {
	  p2 = ptr;
	  for( j = 0; j &lt; pb-&gt;width/8; j++ )
	  {
	    u8g_WriteByte(u8g, dev, u8g_dev_expand4(*p2 &gt;&gt; 4));
	    u8g_WriteByte(u8g, dev, u8g_dev_expand4(*p2 &amp; 15));
	    p2++;
	  }
	  u8g_Delay(LINE_DELAY);
	  p2 = ptr;
	  for( j = 0; j &lt; pb-&gt;width/8; j++ )
	  {
	    u8g_WriteByte(u8g, dev, u8g_dev_expand4(*p2 &gt;&gt; 4));
	    u8g_WriteByte(u8g, dev, u8g_dev_expand4(*p2 &amp; 15));
	    p2++;
	  }
	  u8g_Delay(LINE_DELAY);
	  ptr += pb-&gt;width/8;
          y++;
        }
	
	/* set parameters back to their default values */
	u8g_WriteByte(u8g, dev, 27);      /* ESC */
	u8g_WriteByte(u8g, dev, 55 );      /* parameter command */
	u8g_WriteByte(u8g, dev, 7);      /* Max printing dots,Unit(8dots),Default:7(64 dots) 8*(x+1)*/
	u8g_WriteByte(u8g, dev, 80);      /* 3-255 Heating time,Unit(10us),Default:80(800us) */
	u8g_WriteByte(u8g, dev, 2);      /* 0-255 Heating interval,Unit(10us),Default:2(20us)*/
	
      }
      break;
  }
  return u8g_dev_pb8h1_base_fn(u8g, dev, msg, arg);
}

#if defined(U8G_16BIT)
U8G_PB_DEV(u8g_dev_a2_micro_printer_384x240, 384, 240, 8, u8g_dev_a2_micro_printer_fn, u8g_com_null_fn);
#else
U8G_PB_DEV(u8g_dev_a2_micro_printer_384x240, 240, 240, 8, u8g_dev_a2_micro_printer_fn, u8g_com_null_fn);
#endif

U8G_PB_DEV(u8g_dev_a2_micro_printer_192x120_ds, 192, 120, 8, u8g_dev_a2_micro_printer_double_fn, u8g_com_null_fn);