#include <Wire.h>
#include <RTClib.h>

// Mega2560 PinOut = 20 (SDA), 21 (SCL)

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

void printDate(){
  
  DateTime now = RTC.now();
  
  char DOW[][4]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
  char MON[][16]={"January","February","March","April","May","June","July","August","September","October","November","December"};
  
  String WeekDayName = DOW[now.dayOfWeek()];
  String MonthName = MON[now.month()];
  
  int day = now.day();
  int month = now.month();
  int year = now.year();
  
  String date = WeekDayName + ", " + day + " " + MonthName + " " + year;

  Serial.print (date);
}

void printTime(){
  
  DateTime now = RTC.now();
  
  int second = now.second();
  int minute = now.minute();
  int hour = now.hour();

  String Null = ""; // la variable de tipo string no puede inicar con un valor no string, despues se puede concatenar con lo que quieras
  String time = Null + hour + ":" + minute + ":" + second;
  
  Serial.print(time);

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
          printDate();
          Serial.print(" - ");
          printTime();
          Serial.println();
	}
	delay(1000);
}
