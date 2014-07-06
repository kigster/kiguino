


const int knobs[] = {0,1};
const int motors[] = {9, 10};

int speed[] = {0,0};
int speedRunning[] = {0,0};

const int number_of_motors = sizeof(motors)/sizeof(motors[0]);

static int counter = 0;

void setup()
{
  for (int m = 0; m < number_of_motors; m++) {
    pinMode (motors[m], OUTPUT);
  }
  Serial.begin(9600);
}

void loop()
{  
  counter++;
  readSpeed();
  setMotorSpeed();
  
  if (counter % 50 == 0) { 
    logSpeeds();
  }
  
  delay(50);
}

void log(char *message) {
  Serial.println(message); 
}

void log(char *message, int value) {
  Serial.print(message); 
  Serial.print(": "); 
  Serial.println(value); 
}

//-------------------------------------------------------------------
int readKnob(int pin, int from, int to) {
  int value = analogRead(pin);
  value = map(value, 0, 1023, from, to);
  return value;
}

//-------------------------------------------------------------------
void stopMotors() {
  for (int m = 0; m < number_of_motors; m++) {
    analogWrite(motors[m], 0);
  }
  delay(10);
}

void setMotorSpeed() {
  for (int m = 0; m < number_of_motors; m++) {
     analogWrite(motors[m], speedRunning[m]);
  }
  delay(10);
}

void readSpeed() {
  for (int m = 0; m < number_of_motors; m++) {
    int knobSpeed = readKnob(knobs[m], 0, 255);
    if (knobSpeed != speedRunning[m]) { 
      speedRunning[m] = knobSpeed;
    }
  }
}

void logSpeeds() {
  for (int m = 0; m < number_of_motors; m++) {
    Serial.print("speed of motor ");
    Serial.print(m);  
    log("is set to", speedRunning[m]);
  }
}


