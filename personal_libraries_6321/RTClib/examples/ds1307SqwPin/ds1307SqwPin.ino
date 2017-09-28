// SQW/OUT pin mode using a DS1307 RTC connected via I2C.
//
// According to the data sheet (http://datasheets.maxim-ic.com/en/ds/DS1307.pdf), the
// DS1307's SQW/OUT pin can be set to low, high, 1Hz, 4.096kHz, 8.192kHz, or 32.768kHz.
//
// This sketch reads the state of the pin, then iterates through the possible values at
// 5 second intervals.
//

// NOTE:
// You must connect a pull up resistor (~10kohm) from the SQW pin up to VCC.  Without
// this pull up the wave output will not work!

#include &lt;Wire.h&gt;
#include &quot;RTClib.h&quot;

RTC_DS1307 rtc;

int mode_index = 0;

Ds1307SqwPinMode modes[] = {OFF, ON, SquareWave1HZ, SquareWave4kHz, SquareWave8kHz, SquareWave32kHz};


void print_mode() {
  Ds1307SqwPinMode mode = rtc.readSqwPinMode();
  
  Serial.print(&quot;Sqw Pin Mode: &quot;);
  switch(mode) {
  case OFF:             Serial.println(&quot;OFF&quot;);       break;
  case ON:              Serial.println(&quot;ON&quot;);        break;
  case SquareWave1HZ:   Serial.println(&quot;1Hz&quot;);       break;
  case SquareWave4kHz:  Serial.println(&quot;4.096kHz&quot;);  break;
  case SquareWave8kHz:  Serial.println(&quot;8.192kHz&quot;);  break;
  case SquareWave32kHz: Serial.println(&quot;32.768kHz&quot;); break;
  default:              Serial.println(&quot;UNKNOWN&quot;);   break;
  }
}

void setup () {
  Serial.begin(57600);
  Wire.begin();
  rtc.begin();

  print_mode();
}

void loop () {
  rtc.writeSqwPinMode(modes[mode_index++]);
  print_mode();

  if (mode_index &gt; 5) {
    mode_index = 0;
  }

  delay(5000);
}
