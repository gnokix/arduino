/*

  u8g_page.c
  
  page helper functions, only called by the dev handler.

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

/*
  setup page count structure
  conditions: page_height &lt;= total_height
*/
void u8g_page_Init(u8g_page_t *p, u8g_uint_t page_height, u8g_uint_t total_height )
{
  p-&gt;page_height = page_height;
  p-&gt;total_height = total_height;
  p-&gt;page = 0;
  u8g_page_First(p);
}

void u8g_page_First(u8g_page_t *p)
{
  p-&gt;page_y0 = 0;
  p-&gt;page_y1 = p-&gt;page_height;
  p-&gt;page_y1--;
  p-&gt;page = 0;
}

uint8_t u8g_page_Next(u8g_page_t * p)
{
  register u8g_uint_t y1;
  p-&gt;page_y0 += p-&gt;page_height;
  if ( p-&gt;page_y0 &gt;= p-&gt;total_height )
    return 0;
  p-&gt;page++;
  y1 = p-&gt;page_y1;
  y1 += p-&gt;page_height;
  if ( y1 &gt;= p-&gt;total_height )
  {
    y1 = p-&gt;total_height;
    y1--;
  }
  p-&gt;page_y1 = y1;
  
  return 1;
}



