/*
2
	  http://www.bajdi.com
3
	  SD card datalogger for Arduino Mega (this sketch will ONLY work with Arduino Mega)
4
	  
5
	 This example shows how to log data from three analog sensors
6
	 to an SD card using the SD library.
7
	     
8
	 The circuit:
9
	 * analog sensors on analog ins 0, 1, and 2
10
	 * SD card attached to SPI bus as follows:
11
	 ** MOSI - pin 51
12
	 ** MISO - pin 50
13
	 ** CLK - pin 52
14
	 ** CS - pin 53
15
	  
16
	 created  24 Nov 2010
17
	 updated 2 Dec 2010
18
	 by Tom Igoe
19
	 updated 22 Jan 2012 for Arduino Mega
20
	 by Bajdi.com
21
	  
22
	 This example code is in the public domain.
23
	      
24
	 */
25
	 
26
	#include <SD.h>
27
	 
28
	// On the Ethernet Shield, CS is pin 4. Note that even if it's not
29
	// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
30
	// 53 on the Mega) must be left as an output or the SD library
31
	// functions will not work.
32
	const int chipSelect = 53;
33
	 
34
	void setup()
35
	{
36
	  Serial.begin(9600);
37
	  Serial.print("Initializing SD card...");
38
	  // make sure that the default chip select pin is set to
39
	  // output, even if you don't use it:
40
	  pinMode(53, OUTPUT);
41
	   
42
	  // see if the card is present and can be initialized:
43
	  if (!SD.begin(chipSelect)) {
44
	    Serial.println("Card failed, or not present");
45
	    // don't do anything more:
46
	    return;
47
	  }
48
	  Serial.println("card initialized.");
49
	}
50
	 
51
	void loop()
52
	{
53
	  // make a string for assembling the data to log:
54
	  String dataString = "";
55
	 
56
	  // read three sensors and append to the string:
57
	  for (int analogPin = 0; analogPin < 3; analogPin++) {
58
	    int sensor = analogRead(analogPin);
59
	    dataString += String(sensor);
60
	    if (analogPin < 2) {
61
	      dataString += ",";
62
	    }
63
	  }
64
	 
65
	  // open the file. note that only one file can be open at a time,
66
	  // so you have to close this one before opening another.
67
	  File dataFile = SD.open("datalog.txt", FILE_WRITE);
68
	 
69
	  // if the file is available, write to it:
70
	  if (dataFile) {
71
	    dataFile.println(dataString);
72
	    dataFile.close();
73
	    // print to the serial port too:
74
	    Serial.println(dataString);
75
	  } 
76
	  // if the file isn't open, pop up an error:
77
	  else {
78
	    Serial.println("error opening datalog.txt");
79
	  }
80
	}
