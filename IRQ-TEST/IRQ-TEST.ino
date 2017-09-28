volatile int ParpadAR=300;
#define LED 9
volatile byte state = LOW;

void setup() {
  Serial.begin(9600); 
  pinMode(LED, OUTPUT);
  attachInterrupt(0, toggle, FALLING); 
}

void loop() { 
  digitalWrite(LED, HIGH);
 delay(ParpadAR); 
  digitalWrite(LED,LOW);
 delay(ParpadAR);
  Serial.println(ParpadAR);
}

void toggle() { 
state = !state;
 Serial.println(state); 
  if ( state = HIGH ) {
    ParpadAR=100; // do stuff if the condition is true

    Serial.println("estado igual a 0");
   } else {
    ParpadAR=800; // do stuff if the condition is false

    Serial.println("estado igual a 1");
   }
}
