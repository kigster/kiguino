

const int motorPin = 9;
const int button = 2;

boolean lastButton = LOW;
boolean currentButton = LOW;
boolean motorOn = false;
int runningSpeed = 150; // out of 0-255

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
  currentButton = debounce(lastButton);
  if (lastButton == LOW && currentButton == HIGH) { 
    motorOn = !motorOn;
    motorOn ? log("motor ON") : log("motor OFF");
  }
  
  runningSpeed = constrain(runningSpeed, 0, 255);
  
  lastButton = currentButton;   
   
  if (motorOn) { 
    runMotor(runningSpeed);
  } else {
    stopMotor();
  }
}

void log(char *message) {
  Serial.println(message); 
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


// This function will let you type a speed into the serial
// monitor window. Open the serial monitor using the magnifying-
// glass icon at the top right of the Arduino window. Then
// type your desired speed into the small text entry bar at the
// top of the window and click "Send" or press return. The motor
// will then operate at that speed. The valid range is 0 to 255.

void serialSpeed()
{
  int speed;
  
  Serial.println("Type a speed (0-255) into the box above,");
  Serial.println("then click [send] or press [return]");
  Serial.println();  // Print a blank line

  // In order to type out the above message only once,
  // we'll run the rest of this function in an infinite loop:

  while(true)  // "true" is always true, so this will loop forever.
  {
    // First we check to see if incoming data is available:
  
    while (Serial.available() > 0)
    {
      // If it is, we'll use parseInt() to pull out any numbers:
      
      speed = Serial.parseInt();
  
      // Because analogWrite() only works with numbers from
      // 0 to 255, we'll be sure the input is in that range:
  
      speed = constrain(speed, 0, 255);
      
      // We'll print out a message to let you know that the
      // number was received:
      
      Serial.print("Setting speed to ");
      Serial.println(speed);
  
      // And finally, we'll set the speed of the motor!
      
      analogWrite(motorPin, speed);
    }
  }
}



