/*

  u8g_pb8h1.c
  
  8bit height monochrom (1 bit) page buffer
  byte has horizontal orientation

  Universal 8bit Graphics Library
  
  Copyright (c) 2011, olikraus@gmail.com
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


  total buffer size is limited to 256 bytes because of the calculation inside the set pixel procedure

  23. Sep 2012: Bug with down procedure, see FPS 1st page --&gt; fixed (bug located in u8g_clip.c)

*/

#include &quot;u8g.h&quot;
#include &lt;string.h&gt;

#ifdef __unix__
#include &lt;assert.h&gt;
#endif

/* NEW_CODE disabled, because the performance increase was too slow and not worth compared */
/* to the increase of code size */
/* #define NEW_CODE */

#ifdef __unix__
void *u8g_buf_lower_limit;
void *u8g_buf_upper_limit;
#endif

void u8g_pb8h1_Init(u8g_pb_t *b, void *buf, u8g_uint_t width) U8G_NOINLINE;
void u8g_pb8h1_set_pixel(u8g_pb_t *b, u8g_uint_t x, u8g_uint_t y, uint8_t color_index) U8G_NOINLINE;
void u8g_pb8h1_SetPixel(u8g_pb_t *b, const u8g_dev_arg_pixel_t * const arg_pixel) U8G_NOINLINE ;
void u8g_pb8h1_Set8PixelStd(u8g_pb_t *b, u8g_dev_arg_pixel_t *arg_pixel) U8G_NOINLINE;
uint8_t u8g_dev_pb8h1_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);


#ifdef NEW_CODE
struct u8g_pb_h1_struct
{
  u8g_uint_t x;
  u8g_uint_t y;
  uint8_t *ptr;
  uint8_t mask;
  uint8_t line_byte_len;
  uint8_t cnt;
};

static uint8_t u8g_pb8h1_bitmask[8] = { 0x080, 0x040, 0x020, 0x010, 0x008, 0x004, 0x002, 0x001 };

static void u8g_pb8h1_state_right(struct u8g_pb_h1_struct *s)  U8G_NOINLINE;
static void u8g_pb8h1_state_right(struct u8g_pb_h1_struct *s)
{
  register u8g_uint_t x;
  x = s-&gt;x;
  x++;
  s-&gt;x = x;
  x &amp;= 7;
  s-&gt;mask = u8g_pb8h1_bitmask[x];
  if ( x == 0 )
    s-&gt;ptr++;
}

static void u8g_pb8h1_state_left(struct u8g_pb_h1_struct *s)
{
  register u8g_uint_t x;
  x = s-&gt;x;
  x--;
  s-&gt;x = x;
  x &amp;= 7;
  s-&gt;mask = u8g_pb8h1_bitmask[x];
  if ( x == 7 )
    s-&gt;ptr--;
}

static void u8g_pb8h1_state_down(struct u8g_pb_h1_struct *s)
{
  s-&gt;y++;
  s-&gt;ptr += s-&gt;line_byte_len;
}

static void u8g_pb8h1_state_up(struct u8g_pb_h1_struct *s)
{
  s-&gt;y--;
  s-&gt;ptr -= s-&gt;line_byte_len;
}

static void u8g_pb8h1_state_init(struct u8g_pb_h1_struct *s, u8g_pb_t *b, u8g_uint_t x, u8g_uint_t y) U8G_NOINLINE;
static void u8g_pb8h1_state_init(struct u8g_pb_h1_struct *s, u8g_pb_t *b, u8g_uint_t x, u8g_uint_t y)
{
  u8g_uint_t tmp;
  
  uint8_t *ptr = b-&gt;buf;
  
  s-&gt;x = x;  
  s-&gt;y = y;
  
  y -= b-&gt;p.page_y0;
  
  tmp = b-&gt;width;
  tmp &gt;&gt;= 3;
  s-&gt;line_byte_len = tmp;
  
  /* assume negative y values, can be down to -7, subtract this from the pointer and add correction of 8 to y */
  ptr -= tmp*8;
  y+=8;
  /* it is important that the result of tmp*y can be 16 bit value also for 8 bit mode */
  ptr += tmp*y;
  
  s-&gt;mask = u8g_pb8h1_bitmask[x &amp; 7];
  
  /* assume negative x values (to -7), subtract 8 pixel from the pointer and add 8 to x */
  ptr--;
  x += 8;
  x &gt;&gt;= 3;
  ptr += x;
  s-&gt;ptr = ptr;
}

static void u8g_pb8h1_state_set_pixel(struct u8g_pb_h1_struct *s, uint8_t color_index) U8G_NOINLINE;
static void u8g_pb8h1_state_set_pixel(struct u8g_pb_h1_struct *s, uint8_t color_index)
{
  
#ifdef __unix__
  assert( s-&gt;ptr &gt;= u8g_buf_lower_limit );
  assert( s-&gt;ptr &lt; u8g_buf_upper_limit );
#endif
  
  if ( color_index )
  {
    *s-&gt;ptr |= s-&gt;mask;
  }
  else
  {
    uint8_t mask = s-&gt;mask;
    mask ^=0xff;
    *s-&gt;ptr &amp;= mask;
  }  
}
#endif


void u8g_pb8h1_Init(u8g_pb_t *b, void *buf, u8g_uint_t width)
{
  b-&gt;buf = buf;
  b-&gt;width = width;
  u8g_pb_Clear(b);
}

/* limitation: total buffer must not exceed 256 bytes */
void u8g_pb8h1_set_pixel(u8g_pb_t *b, u8g_uint_t x, u8g_uint_t y, uint8_t color_index)
{
#ifdef NEW_CODE
  struct u8g_pb_h1_struct s;
  u8g_pb8h1_state_init(&amp;s, b, x, y);
  u8g_pb8h1_state_set_pixel(&amp;s, color_index);

//  u8g_pb8h1_state_up(&amp;s);
//  if ( s.y &gt; b-&gt;p.page_y1 )
//    return;
//  if ( s.x &gt; b-&gt;width )
//    return;
//  u8g_pb8h1_state_set_pixel(&amp;s, color_index);
#else
  register uint8_t mask;
  u8g_uint_t tmp;
  uint8_t *ptr = b-&gt;buf;
  
  y -= b-&gt;p.page_y0;
  tmp = b-&gt;width;
  tmp &gt;&gt;= 3;
  tmp *= (uint8_t)y;
  ptr += tmp;
  
  mask = 0x080;
  mask &gt;&gt;= x &amp; 7;
  x &gt;&gt;= 3;
  ptr += x;
  if ( color_index )
  {
    *ptr |= mask;
  }
  else
  {
    mask ^=0xff;
    *ptr &amp;= mask;
  }
#endif
}


void u8g_pb8h1_SetPixel(u8g_pb_t *b, const u8g_dev_arg_pixel_t * const arg_pixel)
{
  if ( arg_pixel-&gt;y &lt; b-&gt;p.page_y0 )
    return;
  if ( arg_pixel-&gt;y &gt; b-&gt;p.page_y1 )
    return;
  if ( arg_pixel-&gt;x &gt;= b-&gt;width )
    return;
  u8g_pb8h1_set_pixel(b, arg_pixel-&gt;x, arg_pixel-&gt;y, arg_pixel-&gt;color);
}

void u8g_pb8h1_Set8PixelStd(u8g_pb_t *b, u8g_dev_arg_pixel_t *arg_pixel)
{
  register uint8_t pixel = arg_pixel-&gt;pixel;
  do
  {
    if ( pixel &amp; 128 )
    {
      u8g_pb8h1_SetPixel(b, arg_pixel);
    }
    switch( arg_pixel-&gt;dir )
    {
      case 0: arg_pixel-&gt;x++; break;
      case 1: arg_pixel-&gt;y++; break;
      case 2: arg_pixel-&gt;x--; break;
      case 3: arg_pixel-&gt;y--; break;
    }
    pixel &lt;&lt;= 1;
  } while( pixel != 0  );
}

void u8g_pb8h1_Set8PixelOpt2(u8g_pb_t *b, u8g_dev_arg_pixel_t *arg_pixel)
{
  register uint8_t pixel = arg_pixel-&gt;pixel;
  u8g_uint_t dx = 0;
  u8g_uint_t dy = 0;
  
  switch( arg_pixel-&gt;dir )
  {
    case 0: dx++; break;
    case 1: dy++; break;
    case 2: dx--; break;
    case 3: dy--; break;
  }
  
  do
  {
    if ( pixel &amp; 128 )
      u8g_pb8h1_SetPixel(b, arg_pixel);
    arg_pixel-&gt;x += dx;
    arg_pixel-&gt;y += dy;
    pixel &lt;&lt;= 1;
  } while( pixel != 0  );  
}

#ifdef NEW_CODE
static void u8g_pb8h1_Set8PixelState(u8g_pb_t *b, u8g_dev_arg_pixel_t *arg_pixel)
{
  register uint8_t pixel = arg_pixel-&gt;pixel;
  struct u8g_pb_h1_struct s;
  uint8_t cnt;
  u8g_pb8h1_state_init(&amp;s, b, arg_pixel-&gt;x, arg_pixel-&gt;y);
  cnt = 8;
  switch( arg_pixel-&gt;dir )
  {
    case 0: 
      do
      {
	if ( s.x &lt; b-&gt;width )
	  if ( pixel &amp; 128 )
	    u8g_pb8h1_state_set_pixel(&amp;s, arg_pixel-&gt;color);
	u8g_pb8h1_state_right(&amp;s); 
	pixel &lt;&lt;= 1;
	cnt--;
      } while( cnt &gt; 0 &amp;&amp; pixel != 0  );
      break;
    case 1: 
      do
      {
	if ( s.y &gt;= b-&gt;p.page_y0 )
	  if ( s.y &lt;= b-&gt;p.page_y1 )
	    if ( pixel &amp; 128 )
	      u8g_pb8h1_state_set_pixel(&amp;s, arg_pixel-&gt;color);
	u8g_pb8h1_state_down(&amp;s); 
	pixel &lt;&lt;= 1;
	cnt--;
      } while( cnt &gt; 0 &amp;&amp; pixel != 0  );
      break;
    case 2: 
      do
      {
	if ( s.x &lt; b-&gt;width )
	  if ( pixel &amp; 128 )
	    u8g_pb8h1_state_set_pixel(&amp;s, arg_pixel-&gt;color);
	u8g_pb8h1_state_left(&amp;s); 
	pixel &lt;&lt;= 1;
	cnt--;
      } while( cnt &gt; 0 &amp;&amp; pixel != 0 );
      break;
    case 3: 	
      do
      {
	if ( s.y &gt;= b-&gt;p.page_y0 )
	  if ( s.y &lt;= b-&gt;p.page_y1 )
	    if ( pixel &amp; 128 )
	      u8g_pb8h1_state_set_pixel(&amp;s, arg_pixel-&gt;color);
	u8g_pb8h1_state_up(&amp;s); 
	pixel &lt;&lt;= 1;
	cnt--;
      } while( cnt &gt; 0 &amp;&amp; pixel != 0  );
      break;
  }
}
#endif

uint8_t u8g_dev_pb8h1_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  u8g_pb_t *pb = (u8g_pb_t *)(dev-&gt;dev_mem);
  switch(msg)
  {
    case U8G_DEV_MSG_SET_8PIXEL:
#ifdef NEW_CODE
      if ( u8g_pb_Is8PixelVisible(pb, (u8g_dev_arg_pixel_t *)arg) )
        u8g_pb8h1_Set8PixelState(pb, (u8g_dev_arg_pixel_t *)arg);
#else
      if ( u8g_pb_Is8PixelVisible(pb, (u8g_dev_arg_pixel_t *)arg) )
        u8g_pb8h1_Set8PixelOpt2(pb, (u8g_dev_arg_pixel_t *)arg);
#endif
      break;
    case U8G_DEV_MSG_SET_PIXEL:
      u8g_pb8h1_SetPixel(pb, (u8g_dev_arg_pixel_t *)arg);
      break;
    case U8G_DEV_MSG_INIT:
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_FIRST:
      u8g_pb_Clear(pb);
      u8g_page_First(&amp;(pb-&gt;p));
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      if ( u8g_page_Next(&amp;(pb-&gt;p)) == 0 )
        return 0;
      u8g_pb_Clear(pb);
      break;
#ifdef U8G_DEV_MSG_IS_BBX_INTERSECTION
    case U8G_DEV_MSG_IS_BBX_INTERSECTION:
      return u8g_pb_IsIntersection(pb, (u8g_dev_arg_bbx_t *)arg);
#endif
    case U8G_DEV_MSG_GET_PAGE_BOX:
      u8g_pb_GetPageBox(pb, (u8g_box_t *)arg);
      break;
    case U8G_DEV_MSG_GET_WIDTH:
      *((u8g_uint_t *)arg) = pb-&gt;width;
      break;
    case U8G_DEV_MSG_GET_HEIGHT:
      *((u8g_uint_t *)arg) = pb-&gt;p.total_height;
      break;
    case U8G_DEV_MSG_SET_COLOR_ENTRY:
      break;
    case U8G_DEV_MSG_SET_XY_CB:
      break;
    case U8G_DEV_MSG_GET_MODE:
      return U8G_MODE_BW;
  }
  return 1;
}
 
  
