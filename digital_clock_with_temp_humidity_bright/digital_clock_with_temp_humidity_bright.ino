
// RTC Module - Mega2560 PinOut = 20 (SDA), 21 (SCL)
#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 RTC; // Instanciado del objeto RTC
//Definimos variables para usar con RTC
int hour, minute, day, dayOfWeek, month, year;
int second; // mejorar para que segundos tenga siempre 2 digitos

char DOW[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char MON[][16] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
String date;
String time;

// DHTC11 Module from Left to right (Signal,5v+,GND)
// * Remeber signal goes on DIGITAL PIN
#include <dht11.h>
dht11 DHT11; // Instanciamos el objeto DHT11
#define DHT11PIN 2 //Definimos el Pin de entrada de datos
int humidity, temperature, dewpoint;


// u8gLib
// * Remeber signal goes on DIGITAL PIN
#include "U8glib.h"
// SPI Communication: SCK=en--> 53 ,MOSI=rw-->51 ,CS=rs-->49
U8GLIB_ST7920_128X64 u8g(53, 51, 49, U8G_PIN_NONE); //Instanciamos el objeto u8g


void setup()
{

	Serial.begin(57600);

// Sartup RTC
	Wire.begin();
	RTC.begin();
	if (! RTC.isrunning())
	{
		Serial.println("RTC is NOT running! Trying to start");
		RTC.adjust(DateTime(__DATE__, __TIME__));
	}
// End RTC

// Satartup  u8gLib
	u8g.setColorIndex(1);         // pixel on
//End u8gLib

}

void getTemp()
{

	int chk = DHT11.read(DHT11PIN);
	if (chk ==  DHTLIB_OK)
	{
		temperature = DHT11.temperature;
		humidity = DHT11.humidity;
		double a = 17.271;
		double b = 237.7;
		double temp = (a * temperature) / (b + temperature) + log(humidity * 0.01);
		dewpoint = (b * temp) / (a - temp);
	}

}

void getDate()
{

	DateTime now = RTC.now();

	String WeekDayName = DOW[now.dayOfWeek()];
	String MonthName = MON[now.month()];

	day = now.day();
	month = now.month();
	year = now.year();

	date = WeekDayName + ", " + day + " " + MonthName + " " + year;
}

void getTime()
{

	DateTime now = RTC.now();
	second = now.second();
	minute = now.minute();
	hour = now.hour();

	String Null = ""; // la variable de tipo string no puede inicar con un valor no string, despues se puede concatenar con lo que quieras
	time = Null + hour + ":" + minute + ":" + second;

}

void draw(void)
{
	u8g.setFont(u8g_font_04b_03);
	u8g.setPrintPos( 26, 10);
	getDate();
	u8g.print(date);

	u8g.setFont(u8g_font_10x20);
	u8g.setPrintPos( 28, 28);
	getTime();
	u8g.print(time);

	getTemp();

	u8g.setFont(u8g_font_7x14B);

	u8g.setPrintPos( 10, 44);
	u8g.print("T=");
	u8g.print(temperature);
	u8g.print(" C");


	u8g.setPrintPos( 10, 58);
	u8g.print("H=");
	u8g.print(humidity);
	u8g.print(" %");

	u8g.setPrintPos( 70, 58);
	u8g.print("DP=");
	u8g.print(dewpoint);
	u8g.print(" C");

}

void loop()
{

	u8g.firstPage();
	do
	{
		draw();
	}
	while( u8g.nextPage() );

	delay(1000);
}
