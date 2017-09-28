#define SOUNDPIN 7
#define BLUEPIN 4
#define REDPIN 3
#define GREENPIN 2

int blueNow; int blueNew;
int redNow; int redNew;
int greenNow; int greenNew;

void setup()

{

  pinMode(SOUNDPIN,INPUT); //SIG of the Parallax Sound Impact Sensor connected to Digital Pin 7
  pinMode(BLUEPIN, OUTPUT); // BLUE
  pinMode(REDPIN, OUTPUT); // RED
  pinMode(GREENPIN, OUTPUT); // GREEN
  
  blueNow = random(255);
  redNow = random(255);
  greenNow = random(255);

  blueNew = blueNow;
  redNew = redNow;
  greenNew = greenNow;

}

#define fade(x,y) if (x>y) x--; else if (x<y) x++;

void loop() {

      Serial.begin(9600);

  boolean soundstate = digitalRead(SOUNDPIN);

  if (soundstate == 1) {

    analogWrite(BLUEPIN, blueNow);
    analogWrite(REDPIN, redNow);
    analogWrite(GREENPIN, greenNow);

    redNew = random(255);
    blueNew = random(255);
    greenNow = random(255);

    // fade to new colors

    while ((blueNow != blueNew) || (redNow != redNew) ||  (greenNow != greenNew))

    {

      fade(blueNow,blueNew)
      //Serial.print ("BLUE-Now :"); Serial.print (blueNow); Serial.print (" | BLUE-New :"); Serial.println (blueNew);
      fade(redNow,redNew);  
      //Serial.print ("RED-Now :"); Serial.print (redNow); Serial.print (" | RED-New :"); Serial.println (redNew);    
      fade (greenNow,greenNew);
      //Serial.print ("GREEN-Now :"); Serial.print (greenNow); Serial.print (" | GREEN-New :"); Serial.println (greenNew);
      //Serial.print ("SoundState :"); Serial.println (soundstate);
      //Serial.println ("------------------------------------------------------------------------------");
      
       
      analogWrite(BLUEPIN, blueNow);
      analogWrite(REDPIN, redNow);
  
      delay(1);

    }

  }

  else{

    digitalWrite(REDPIN,0);

    digitalWrite(GREENPIN,0);

    digitalWrite(BLUEPIN,0);

  }

}
