/*

  u8g_rot.c

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
  
  
*/

#include &quot;u8g.h&quot;

uint8_t u8g_dev_rot90_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);
uint8_t u8g_dev_rot180_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);
uint8_t u8g_dev_rot270_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

u8g_dev_t u8g_dev_rot = { u8g_dev_rot90_fn, NULL, NULL };


void u8g_UndoRotation(u8g_t *u8g)
{
  if ( u8g-&gt;dev != &amp;u8g_dev_rot )
    return;
  u8g-&gt;dev = u8g_dev_rot.dev_mem;
  u8g_UpdateDimension(u8g);
}

void u8g_SetRot90(u8g_t *u8g)
{
  if ( u8g-&gt;dev != &amp;u8g_dev_rot )
  {
    u8g_dev_rot.dev_mem = u8g-&gt;dev;
    u8g-&gt;dev = &amp;u8g_dev_rot;
  }
  u8g_dev_rot.dev_fn = u8g_dev_rot90_fn;
  u8g_UpdateDimension(u8g);
}

void u8g_SetRot180(u8g_t *u8g)
{
  if ( u8g-&gt;dev != &amp;u8g_dev_rot )
  {
    u8g_dev_rot.dev_mem = u8g-&gt;dev;
    u8g-&gt;dev = &amp;u8g_dev_rot;
  }
  u8g_dev_rot.dev_fn = u8g_dev_rot180_fn;
  u8g_UpdateDimension(u8g);
}

void u8g_SetRot270(u8g_t *u8g)
{
  if ( u8g-&gt;dev != &amp;u8g_dev_rot )
  {
    u8g_dev_rot.dev_mem = u8g-&gt;dev;
    u8g-&gt;dev = &amp;u8g_dev_rot;
  }
  u8g_dev_rot.dev_fn = u8g_dev_rot270_fn;
  u8g_UpdateDimension(u8g);
}

uint8_t u8g_dev_rot90_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  u8g_dev_t *rotation_chain = (u8g_dev_t *)(dev-&gt;dev_mem);
  switch(msg)
  {
    default:
    /*
    case U8G_DEV_MSG_INIT:
    case U8G_DEV_MSG_STOP:
    case U8G_DEV_MSG_PAGE_FIRST:
    case U8G_DEV_MSG_PAGE_NEXT:
    case U8G_DEV_MSG_SET_COLOR_ENTRY:
    case U8G_DEV_MSG_SET_XY_CB:
    */
      return u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
#ifdef U8G_DEV_MSG_IS_BBX_INTERSECTION
    case U8G_DEV_MSG_IS_BBX_INTERSECTION:
      {
        u8g_dev_arg_bbx_t *bbx = (u8g_dev_arg_bbx_t *)arg;
        u8g_uint_t x, y, tmp;
        
        /* transform the reference point */
        y = bbx-&gt;x;
        x = u8g-&gt;height;
        /* x = u8g_GetWidthLL(u8g, rotation_chain); */
        x -= bbx-&gt;y; 
        x--;
        
        /* adjust point to be the uppler left corner again */
        x -= bbx-&gt;h;
        x++;
        
        /* swap box dimensions */        
        tmp = bbx-&gt;w;
        bbx-&gt;w = bbx-&gt;h;
        bbx-&gt;h = tmp;
        
        /* store x,y */
        bbx-&gt;x = x;
        bbx-&gt;y = y;
      }
      return u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
#endif /* U8G_DEV_MSG_IS_BBX_INTERSECTION */
    case U8G_DEV_MSG_GET_PAGE_BOX:
      /* get page size from next device in the chain */
      u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
      //printf(&quot;pre x: %3d..%3d y: %3d..%3d   &quot;, ((u8g_box_t *)arg)-&gt;x0, ((u8g_box_t *)arg)-&gt;x1, ((u8g_box_t *)arg)-&gt;y0, ((u8g_box_t *)arg)-&gt;y1);
      {
	u8g_box_t new_box;
	//new_box.x0 = u8g_GetHeightLL(u8g,rotation_chain) - ((u8g_box_t *)arg)-&gt;y1 - 1;
	//new_box.x1 = u8g_GetHeightLL(u8g,rotation_chain) - ((u8g_box_t *)arg)-&gt;y0 - 1;
	
	new_box.x0 = ((u8g_box_t *)arg)-&gt;y0;
	new_box.x1 = ((u8g_box_t *)arg)-&gt;y1;
	new_box.y0 = ((u8g_box_t *)arg)-&gt;x0;
	new_box.y1 = ((u8g_box_t *)arg)-&gt;x1;
	*((u8g_box_t *)arg) = new_box;
	//printf(&quot;post x: %3d..%3d y: %3d..%3d\n&quot;, ((u8g_box_t *)arg)-&gt;x0, ((u8g_box_t *)arg)-&gt;x1, ((u8g_box_t *)arg)-&gt;y0, ((u8g_box_t *)arg)-&gt;y1);
      }
      break;
    case U8G_DEV_MSG_GET_WIDTH:
      *((u8g_uint_t *)arg) = u8g_GetHeightLL(u8g,rotation_chain);
      break;
    case U8G_DEV_MSG_GET_HEIGHT:
      *((u8g_uint_t *)arg) = u8g_GetWidthLL(u8g, rotation_chain);
      break;
    case U8G_DEV_MSG_SET_PIXEL:
    case U8G_DEV_MSG_SET_TPIXEL:
      {
        u8g_uint_t x, y;
        y = ((u8g_dev_arg_pixel_t *)arg)-&gt;x;
        x = u8g_GetWidthLL(u8g, rotation_chain);
        x -= ((u8g_dev_arg_pixel_t *)arg)-&gt;y; 
        x--;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;x = x;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;y = y;
      }
      u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
      break;
    case U8G_DEV_MSG_SET_8PIXEL:
    case U8G_DEV_MSG_SET_4TPIXEL:
      {
        u8g_uint_t x, y;
	//uint16_t x,y;
        y = ((u8g_dev_arg_pixel_t *)arg)-&gt;x;
        x = u8g_GetWidthLL(u8g, rotation_chain);
        x -= ((u8g_dev_arg_pixel_t *)arg)-&gt;y; 
        x--;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;x = x;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;y = y;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;dir+=1;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;dir &amp;= 3;
      }
      u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
      break;
  }
  return 1;
}

uint8_t u8g_dev_rot180_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  u8g_dev_t *rotation_chain = (u8g_dev_t *)(dev-&gt;dev_mem);
  switch(msg)
  {
    default:
    /*
    case U8G_DEV_MSG_INIT:
    case U8G_DEV_MSG_STOP:
    case U8G_DEV_MSG_PAGE_FIRST:
    case U8G_DEV_MSG_PAGE_NEXT:
    case U8G_DEV_MSG_SET_COLOR_ENTRY:
    case U8G_DEV_MSG_SET_XY_CB:
    */
      return u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
#ifdef U8G_DEV_MSG_IS_BBX_INTERSECTION
    case U8G_DEV_MSG_IS_BBX_INTERSECTION:
      {
        u8g_dev_arg_bbx_t *bbx = (u8g_dev_arg_bbx_t *)arg;
        u8g_uint_t x, y;
        
        /* transform the reference point */
        //y = u8g_GetHeightLL(u8g, rotation_chain);
        y = u8g-&gt;height;
        y -= bbx-&gt;y; 
        y--;
        
        //x = u8g_GetWidthLL(u8g, rotation_chain);
        x = u8g-&gt;width;
        x -= bbx-&gt;x; 
        x--;
        
        /* adjust point to be the uppler left corner again */
        y -= bbx-&gt;h;
        y++;
        
        x -= bbx-&gt;w;
        x++;
                
        /* store x,y */
        bbx-&gt;x = x;
        bbx-&gt;y = y;
      }
      return u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
#endif /* U8G_DEV_MSG_IS_BBX_INTERSECTION */
    case U8G_DEV_MSG_GET_PAGE_BOX:
      /* get page size from next device in the chain */
      u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
      //printf(&quot;pre x: %3d..%3d y: %3d..%3d   &quot;, ((u8g_box_t *)arg)-&gt;x0, ((u8g_box_t *)arg)-&gt;x1, ((u8g_box_t *)arg)-&gt;y0, ((u8g_box_t *)arg)-&gt;y1);
      {
	u8g_box_t new_box;
	
	new_box.x0 = u8g_GetWidthLL(u8g,rotation_chain) - ((u8g_box_t *)arg)-&gt;x1 - 1;
	new_box.x1 = u8g_GetWidthLL(u8g,rotation_chain) - ((u8g_box_t *)arg)-&gt;x0 - 1;
	new_box.y0 = u8g_GetHeightLL(u8g,rotation_chain) - ((u8g_box_t *)arg)-&gt;y1 - 1;
	new_box.y1 = u8g_GetHeightLL(u8g,rotation_chain) - ((u8g_box_t *)arg)-&gt;y0 - 1;
	*((u8g_box_t *)arg) = new_box;
	//printf(&quot;post x: %3d..%3d y: %3d..%3d\n&quot;, ((u8g_box_t *)arg)-&gt;x0, ((u8g_box_t *)arg)-&gt;x1, ((u8g_box_t *)arg)-&gt;y0, ((u8g_box_t *)arg)-&gt;y1);
      }
      break;
    case U8G_DEV_MSG_GET_WIDTH:
      *((u8g_uint_t *)arg) = u8g_GetWidthLL(u8g,rotation_chain);
      break;
    case U8G_DEV_MSG_GET_HEIGHT:
      *((u8g_uint_t *)arg) = u8g_GetHeightLL(u8g, rotation_chain);
      break;
    case U8G_DEV_MSG_SET_PIXEL:
    case U8G_DEV_MSG_SET_TPIXEL:
      {
        u8g_uint_t x, y;

        y = u8g_GetHeightLL(u8g, rotation_chain);
        y -= ((u8g_dev_arg_pixel_t *)arg)-&gt;y; 
        y--;
        
        x = u8g_GetWidthLL(u8g, rotation_chain);
        x -= ((u8g_dev_arg_pixel_t *)arg)-&gt;x; 
        x--;
        
        ((u8g_dev_arg_pixel_t *)arg)-&gt;x = x;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;y = y;
      }
      u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
      break;
    case U8G_DEV_MSG_SET_8PIXEL:
    case U8G_DEV_MSG_SET_4TPIXEL:
      {
        u8g_uint_t x, y;
        
        y = u8g_GetHeightLL(u8g, rotation_chain);
        y -= ((u8g_dev_arg_pixel_t *)arg)-&gt;y; 
        y--;
        
        x = u8g_GetWidthLL(u8g, rotation_chain);
        x -= ((u8g_dev_arg_pixel_t *)arg)-&gt;x; 
        x--;
        
        ((u8g_dev_arg_pixel_t *)arg)-&gt;x = x;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;y = y;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;dir+=2;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;dir &amp;= 3;
      }
      u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
      break;
  }
  return 1;
}

uint8_t u8g_dev_rot270_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  u8g_dev_t *rotation_chain = (u8g_dev_t *)(dev-&gt;dev_mem);
  switch(msg)
  {
    default:
    /*
    case U8G_DEV_MSG_INIT:
    case U8G_DEV_MSG_STOP:
    case U8G_DEV_MSG_PAGE_FIRST:
    case U8G_DEV_MSG_PAGE_NEXT:
    case U8G_DEV_MSG_SET_COLOR_ENTRY:
    case U8G_DEV_MSG_SET_XY_CB:
    */
      return u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
#ifdef U8G_DEV_MSG_IS_BBX_INTERSECTION
    case U8G_DEV_MSG_IS_BBX_INTERSECTION:
      {
        u8g_dev_arg_bbx_t *bbx = (u8g_dev_arg_bbx_t *)arg;
        u8g_uint_t x, y, tmp;
        
        /* transform the reference point */
        x = bbx-&gt;y;
        
        y = u8g-&gt;width;
        /* y = u8g_GetHeightLL(u8g, rotation_chain); */
        y -= bbx-&gt;x; 
        y--;
                
        /* adjust point to be the uppler left corner again */
        y -= bbx-&gt;w;
        y++;
        
        /* swap box dimensions */        
        tmp = bbx-&gt;w;
        bbx-&gt;w = bbx-&gt;h;
        bbx-&gt;h = tmp;
        
        /* store x,y */
        bbx-&gt;x = x;
        bbx-&gt;y = y;
      }
      return u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
#endif /* U8G_DEV_MSG_IS_BBX_INTERSECTION */
    case U8G_DEV_MSG_GET_PAGE_BOX:
      /* get page size from next device in the chain */
      u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
      //printf(&quot;pre x: %3d..%3d y: %3d..%3d   &quot;, ((u8g_box_t *)arg)-&gt;x0, ((u8g_box_t *)arg)-&gt;x1, ((u8g_box_t *)arg)-&gt;y0, ((u8g_box_t *)arg)-&gt;y1);
      {
	u8g_box_t new_box;
	
	new_box.x0 = u8g_GetHeightLL(u8g,rotation_chain) - ((u8g_box_t *)arg)-&gt;y1 - 1;
	new_box.x1 = u8g_GetHeightLL(u8g,rotation_chain) - ((u8g_box_t *)arg)-&gt;y0 - 1;
	new_box.y0 = u8g_GetWidthLL(u8g,rotation_chain) - ((u8g_box_t *)arg)-&gt;x1 - 1;
	new_box.y1 = u8g_GetWidthLL(u8g,rotation_chain) - ((u8g_box_t *)arg)-&gt;x0 - 1;
	*((u8g_box_t *)arg) = new_box;
	//printf(&quot;post x: %3d..%3d y: %3d..%3d\n&quot;, ((u8g_box_t *)arg)-&gt;x0, ((u8g_box_t *)arg)-&gt;x1, ((u8g_box_t *)arg)-&gt;y0, ((u8g_box_t *)arg)-&gt;y1);
      }
      break;
    case U8G_DEV_MSG_GET_WIDTH:
      *((u8g_uint_t *)arg) = u8g_GetHeightLL(u8g,rotation_chain);
      break;
    case U8G_DEV_MSG_GET_HEIGHT:
      *((u8g_uint_t *)arg) = u8g_GetWidthLL(u8g, rotation_chain);
      break;
    case U8G_DEV_MSG_SET_PIXEL:
    case U8G_DEV_MSG_SET_TPIXEL:
      {
        u8g_uint_t x, y;
        x = ((u8g_dev_arg_pixel_t *)arg)-&gt;y;
        
        y = u8g_GetHeightLL(u8g, rotation_chain);
        y -= ((u8g_dev_arg_pixel_t *)arg)-&gt;x; 
        y--;
          
        /*
        x = u8g_GetWidthLL(u8g, rotation_chain);
        x -= ((u8g_dev_arg_pixel_t *)arg)-&gt;y; 
        x--;
        */
        ((u8g_dev_arg_pixel_t *)arg)-&gt;x = x;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;y = y;
      }
      u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
      break;
    case U8G_DEV_MSG_SET_8PIXEL:
    case U8G_DEV_MSG_SET_4TPIXEL:
      {
        u8g_uint_t x, y;
        x = ((u8g_dev_arg_pixel_t *)arg)-&gt;y;
        
        y = u8g_GetHeightLL(u8g, rotation_chain);
        y -= ((u8g_dev_arg_pixel_t *)arg)-&gt;x; 
        y--;
          
        /*
        x = u8g_GetWidthLL(u8g, rotation_chain);
        x -= ((u8g_dev_arg_pixel_t *)arg)-&gt;y; 
        x--;
        */
        ((u8g_dev_arg_pixel_t *)arg)-&gt;x = x;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;y = y;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;dir+=3;
        ((u8g_dev_arg_pixel_t *)arg)-&gt;dir &amp;= 3;
      }
      u8g_call_dev_fn(u8g, rotation_chain, msg, arg);
      break;
  }
  return 1;
}

