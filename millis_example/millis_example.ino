int seconds;
int seconds_last;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  seconds = millis() / 1000;
  seconds =  (int) seconds;
  seconds_last = seconds;
}

void loop() {
  // put your main code here, to run repeatedly:
  seconds =  millis() / 1000;
  seconds = (int) seconds;
  
  if (seconds > seconds_last) {
    seconds_last = seconds;
    print_seconds();
  } else {
    do_nothing();
  }
}

void do_nothing() {
  //Do Nothing!
}

void print_seconds() {
  Serial.print("Seconds since restart: ");
  Serial.println(seconds);
  Serial.println("");
}

