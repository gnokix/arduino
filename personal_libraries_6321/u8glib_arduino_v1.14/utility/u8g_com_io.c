/*
  
  u8g_com_io.c
  
  abstraction layer for low level i/o 

  Universal 8bit Graphics Library
  
  Copyright (c) 2012, olikraus@gmail.com
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

  Update for ATOMIC operation done (01 Jun 2013)
    U8G_ATOMIC_OR(ptr, val)
    U8G_ATOMIC_AND(ptr, val)
    U8G_ATOMIC_START();
    U8G_ATOMIC_END();

  uint8_t u8g_Pin(uint8_t port, uint8_t bitpos)						Convert to internal number: AVR: port*8+bitpos, ARM: port*16+bitpos
  void u8g_SetPinOutput(uint8_t internal_pin_number)
  void u8g_SetPinInput(uint8_t internal_pin_number)
  void u8g_SetPinLevel(uint8_t internal_pin_number, uint8_t level)
  uint8_t u8g_GetPinLevel(uint8_t internal_pin_number)


*/

#include &quot;u8g.h&quot;

#if defined(__AVR__)

#include &lt;avr/interrupt.h&gt;
#include &lt;avr/io.h&gt;

typedef volatile uint8_t * IO_PTR;

/* create internal pin number */
uint8_t u8g_Pin(uint8_t port, uint8_t bitpos)
{
  port &lt;&lt;= 3;
  port += bitpos;
  return port;
}

const IO_PTR u8g_avr_ddr_P[] PROGMEM = {
#ifdef DDRA
  &amp;DDRA,
#else
  0,
#endif
  &amp;DDRB,
#ifdef DDRC
  &amp;DDRC,
#ifdef DDRD
  &amp;DDRD,
#ifdef DDRE
  &amp;DDRE,
#ifdef DDRF
  &amp;DDRF,
#ifdef DDRG
  &amp;DDRG,
#ifdef DDRH
  &amp;DDRH,
#endif
#endif
#endif
#endif
#endif
#endif
};


const IO_PTR u8g_avr_port_P[] PROGMEM = {
#ifdef PORTA
  &amp;PORTA,
#else
  0,
#endif
  &amp;PORTB,
#ifdef PORTC
  &amp;PORTC,
#ifdef PORTD
  &amp;PORTD,
#ifdef PORTE
  &amp;PORTE,
#ifdef PORTF
  &amp;PORTF,
#ifdef PORTG
  &amp;PORTG,
#ifdef PORTH
  &amp;PORTH,
#endif
#endif
#endif
#endif
#endif
#endif
};

const IO_PTR u8g_avr_pin_P[] PROGMEM = {
#ifdef PINA
  &amp;PINA,
#else
  0,
#endif
  &amp;PINB,
#ifdef PINC
  &amp;PINC,
#ifdef PIND
  &amp;PIND,
#ifdef PINE
  &amp;PINE,
#ifdef PINF
  &amp;PINF,
#ifdef PING
  &amp;PING,
#ifdef PINH
  &amp;PINH,
#endif
#endif
#endif
#endif
#endif
#endif
};

static volatile uint8_t *u8g_get_avr_io_ptr(const IO_PTR *base, uint8_t offset)
{
  volatile uint8_t * tmp;
  base += offset;
  memcpy_P(&amp;tmp, base, sizeof(volatile uint8_t * PROGMEM));
  return tmp; 
}

/* set direction to output of the specified pin (internal pin number) */
void u8g_SetPinOutput(uint8_t internal_pin_number)
{
  *u8g_get_avr_io_ptr(u8g_avr_ddr_P, internal_pin_number&gt;&gt;3) |= _BV(internal_pin_number&amp;7);
}

void u8g_SetPinInput(uint8_t internal_pin_number)
{
  *u8g_get_avr_io_ptr(u8g_avr_ddr_P, internal_pin_number&gt;&gt;3) &amp;= ~_BV(internal_pin_number&amp;7);
}

void u8g_SetPinLevel(uint8_t internal_pin_number, uint8_t level)
{
  volatile uint8_t * tmp = u8g_get_avr_io_ptr(u8g_avr_port_P, internal_pin_number&gt;&gt;3);
  
  if ( level == 0 )
  {
    U8G_ATOMIC_AND(tmp, ~_BV(internal_pin_number&amp;7));
   // *tmp &amp;= ~_BV(internal_pin_number&amp;7);
  }
  else
  {
    U8G_ATOMIC_OR(tmp, _BV(internal_pin_number&amp;7));
    //*tmp |= _BV(internal_pin_number&amp;7);
  }
  
}

uint8_t u8g_GetPinLevel(uint8_t internal_pin_number)
{
  volatile uint8_t * tmp = u8g_get_avr_io_ptr(u8g_avr_pin_P, internal_pin_number&gt;&gt;3);
  if ( ((*tmp) &amp; _BV(internal_pin_number&amp;7))  != 0 )
    return 1;
  return 0;
}

#else

/* convert &quot;port&quot; and &quot;bitpos&quot; to internal pin number */
uint8_t u8g_Pin(uint8_t port, uint8_t bitpos)
{
  port &lt;&lt;= 3;
  port += bitpos;
  return port;
}

void u8g_SetPinOutput(uint8_t internal_pin_number)
{
}

void u8g_SetPinInput(uint8_t internal_pin_number)
{
}

void u8g_SetPinLevel(uint8_t internal_pin_number, uint8_t level)
{
}

uint8_t u8g_GetPinLevel(uint8_t internal_pin_number)
{
  return 0;
}

#endif


#if defined(U8G_WITH_PINLIST)

void u8g_SetPIOutput(u8g_t *u8g, uint8_t pi)
{
  uint8_t pin;
  pin = u8g-&gt;pin_list[pi];
  if ( pin != U8G_PIN_NONE )
    u8g_SetPinOutput(pin);
}

void u8g_SetPILevel(u8g_t *u8g, uint8_t pi, uint8_t level)
{
  uint8_t pin;
  pin = u8g-&gt;pin_list[pi];
  if ( pin != U8G_PIN_NONE )
    u8g_SetPinLevel(pin, level);
}

#else  /* defined(U8G_WITH_PINLIST) */
void u8g_SetPIOutput(u8g_t *u8g, uint8_t pi)
{
}

void u8g_SetPILevel(u8g_t *u8g, uint8_t pi, uint8_t level)
{
}

#endif /* defined(U8G_WITH_PINLIST) */
