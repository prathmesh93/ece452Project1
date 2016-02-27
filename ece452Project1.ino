/*
 * ECE 452: Project 1
 * Project 1
 * 1. Start on accelerometer tap
 * 2. Traverse fully on given path or until 30" is hit
 */

#include <RedBot.h>
#include <RedBotSoftwareSerial.h>
#define SPEED 60
#define LINETHRESHOLD 800
// Variable Declarations
RedBotMotors motors;

RedBotAccel accelerometer;
int accelBaseX;
int accelBaseY;
int accelBaseZ;
bool accelerometerActive = false;
RedBotSensor left = RedBotSensor(A3);   // initialize a left sensor object on A3
RedBotSensor center = RedBotSensor(A6); // initialize a center sensor object on A6
RedBotSensor right = RedBotSensor(A7);// initialize a right sensor object on A7

int leftSpeed;   // variable used to store the leftMotor speed
int rightSpeed;  // variable used to store the rightMotor 
RedBotEncoder encoder = RedBotEncoder(A2, 10);
int countsPerRev = 192; //

float wheelDiam = 2.56;
float wheelCirc = PI*wheelDiam;

void setup() {
  Serial.begin(9600);

  // Get base accelerometer for baseline
  delay(1000);
  accelerometer.read();
  accelBaseX = accelerometer.x;
  accelBaseY = accelerometer.y;
  accelBaseZ = accelerometer.z;
  
}

void loop() {
  // Continue this loop until accelerometer is activiated
  while (accelerometerActive == false) {
    accelerometer.read(); // Update the x, y, and z axis readings on the accelerometer
    Serial.println("---------- BASE ----------");
    Serial.println("X\tY\tZ");
    Serial.print(accelBaseX); Serial.print("\t");
    Serial.print(accelBaseY); Serial.print("\t");
    Serial.print(accelBaseZ); Serial.println();

    Serial.println("---------- READINGS ----------");
    Serial.println("X\tY\tZ");
    Serial.print(accelerometer.x); Serial.print("\t");
    Serial.print(accelerometer.y); Serial.print("\t");
    Serial.print(accelerometer.z); Serial.println("\n\n\n\n");
    
    
    if((abs(accelerometer.x - accelBaseX)) > 800 || 
        (abs(accelerometer.y - accelBaseY)) > 800 ||
        (abs(accelerometer.z - accelBaseZ)) > 800) {
          accelerometerActive = true;
          Serial.println("ACTIVE");
        }
        delay(10);
  }

  robotMove();

}

void robotMove() {
  if(center.read() > LINETHRESHOLD)
  {
    leftSpeed = -SPEED; 
    rightSpeed = SPEED;
     driveDistance(30, 150);
  }
  
  // if the line is under the right sensor, adjust relative speeds to turn to the right
  else if(right.read() > LINETHRESHOLD)
  {
    leftSpeed = -(SPEED + 50);
    rightSpeed = SPEED - 50;
  }

  // if the line is under the left sensor, adjust relative speeds to turn to the left
  else if(left.read() > LINETHRESHOLD)
  {
    leftSpeed = -(SPEED - 50);
    rightSpeed = SPEED + 50;
  }
  if((left.read() > LINETHRESHOLD) && (center.read() > LINETHRESHOLD) && (right.read() > LINETHRESHOLD) )
  {
    motors.stop();
  }
  else {
    motors.drive(100);
    Serial.println("DRIVING");
    delay(100);
    while (true){motors.drive(150);}
}
}
void driveDistance(float distance, int motorPower)
{
  long lCount = 0;
  long rCount = 0;
  float numRev;
  // debug
  Serial.print("driveDistance() ");
  Serial.print(distance);
  Serial.print(" inches at ");
  Serial.print(motorPower);
  Serial.println(" power.");

  numRev = (float) distance / wheelCirc;
  Serial.println(numRev, 3);
  encoder.clearEnc(BOTH);  // clear the encoder count
  motors.drive(motorPower);

  while (rCount < numRev*countsPerRev)
  {
    // while the left encoder is less than the target count -- debug print 
    // the encoder values and wait -- this is a holding loop.
    lCount = encoder.getTicks(LEFT);
    rCount = encoder.getTicks(RIGHT);
    Serial.print(lCount);
    Serial.print("\t");
    Serial.print(rCount);
    Serial.print("\t");
    Serial.println(numRev*countsPerRev);
  }
  // now apply "brakes" to stop the motors.
  motors.brake();
}


