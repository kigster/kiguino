

const int motorPin = 9;
const int knobPin = 1;
const int button = 2;

boolean lastButton = LOW;
boolean currentButton = LOW;
boolean motorOn = false;
int runningSpeed = 0; // out of 0-255
int counter = 0;

void setup()
{
  // Set up the motor pin to be an output:

  pinMode(motorPin, OUTPUT);
  pinMode(button, INPUT);

  // Set up the serial port:

  Serial.begin(9600);
}

void loop()
{  
  counter++;
  /*
  int val = analogRead(knobPin);
  Serial.println(val);
  delay(500);
  */
  currentButton = debounce(lastButton);
  if (lastButton == LOW && currentButton == HIGH) { 
    motorOn = !motorOn;
    motorOn ? log("motor ON") : log("motor OFF");
    log("speed set to", runningSpeed);
  }
  lastButton = currentButton;   
  
  int knobSpeed = readKnob(0, 255);
  if (knobSpeed != runningSpeed) { 
    runningSpeed = knobSpeed;
  }
  
  if (counter % 50 == 0) { 
    log("speed is set to", runningSpeed);
  }
  
  if (motorOn) { 
    runMotor(runningSpeed);
  } else {
    stopMotor();
  }
}

void log(char *message) {
  Serial.println(message); 
}

void log(char *message, int value) {
  Serial.print(message); 
  Serial.print(": "); 
  Serial.println(value); 
}


boolean debounce(boolean last) 
{
  boolean current = digitalRead(button);
  if (last != current) { 
    delay(5);
    current = digitalRead(button);
  }

  return current;     
}

void runMotor(int motorSpeed)
{
  int delayTime = 20;
  analogWrite(motorPin, motorSpeed);  
  delay(delayTime);
}

void stopMotor() {
  runMotor(0);
}

int readKnob(int from, int to) {
  int value = analogRead(knobPin);
  value = map(value, 0, 1023, from, to);
  return value;
}




