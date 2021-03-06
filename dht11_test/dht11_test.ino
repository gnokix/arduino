// 
//   FILE:  temp_hum_dew.ino
// PURPOSE: test for dht11 library and temp-hummidty sensor
//

// KY-015 layout is (from L to R)
// S (data signal)
// V (power 5v)
// G (ground -): 

// Pin DIGITAL BOLUDO!!! 2 al Pin Izquierdo (S)
// Pin 5v al medio
// Pin GND al Pin Derecho (-)

// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
	double Td = (b * temp) / (a - temp);
	return Td;
}


#include <dht11.h>

dht11 DHT11;

#define DHT11PIN 2

void setup()
{
  Serial.begin(115200);
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
}

void loop()
{
  Serial.println("\n");

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
		Serial.println("OK"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.println("Checksum error"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.println("Time out error"); 
		break;
    default: 
		Serial.println("Unknown error"); 
		break;
  }

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (°C): ");
  Serial.println((float)DHT11.temperature, 2);

//  Serial.print("Temperature (°F): ");
//  Serial.println(Fahrenheit(DHT11.temperature), 2);

//  Serial.print("Temperature (°K): ");
//  Serial.println(Kelvin(DHT11.temperature), 2);

//  Serial.print("Dew Point (°C): ");
//  Serial.println(dewPoint(DHT11.temperature, DHT11.humidity));

  Serial.print("Dew PointFast (°C): ");
  Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));

  delay(2000);
}
//
// END OF FILE
//
