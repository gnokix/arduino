#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 RTC;

void setup ()
{

	Serial.println("rtc init");

	Serial.begin(57600);

	Wire.begin();

	RTC.begin();

	if (! RTC.isrunning())
	{
		Serial.println("RTC is NOT running! Trying to start");
		RTC.adjust(DateTime(__DATE__, __TIME__));
	}
}

void loop ()
{

	DateTime now = RTC.now();

	if (! RTC.isrunning())
	{
		Serial.println("RTC is NOT running!");
	}
	else
	{
		Serial.print(now.year(), DEC);
		Serial.print('/');
		Serial.print(now.month(), DEC);
		Serial.print('/');
		Serial.print(now.day(), DEC);
		Serial.print(' ');
		Serial.print(now.hour(), DEC);
		Serial.print(':');
		Serial.print(now.minute(), DEC);
		Serial.print(':');
		Serial.print(now.second(), DEC);
		Serial.println();
	}
	delay(1000);
}