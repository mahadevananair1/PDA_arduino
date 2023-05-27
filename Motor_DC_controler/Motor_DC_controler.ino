
#include "Wire.h"       
#include "I2Cdev.h"     
#include "MPU6050.h"    

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

struct MyData {
  byte X;
  byte Y;
  byte Z;
};

MyData data;

// Define pins
const int potPin = A0; // Potentiometer input
const int inclPin = A1; // Inclination sensor input
const int motorPin = 9; // Motor output

// Define variables
int potValue = 0; // Potentiometer value (0-1023)
int inclValue = 0; // Inclination sensor value (0-1023)
int motorSpeed = 0; // Motor speed (0-255)

void setup() {
  // Set motor pin as output
  pinMode(motorPin, OUTPUT);
  // Initialize serial communication
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
}

void loop() {

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  data.X = map(ax, -17000, 17000, 0, 255 ); // X axis data
  data.Y = map(ay, -17000, 17000, 0, 255);  // Y axis data
  data.Z = map(az, -17000, 17000, 0, 255);  // Z axis data
  Serial.print("Axis X = ");
  Serial.print(data.X);
//  Serial.print("  ");
//  Serial.print("Axis Y = ");
//  Serial.print(data.Y);
//  Serial.print("  ");
//  Serial.print("Axis Z = ");
//  Serial.println(data.Z);
//  delay(500);



  
  // Read potentiometer value
  potValue = analogRead(potPin);
  // Convert potentiometer value to motor speed (0-255)
  motorSpeed = map(potValue, 0, 1023, 0, 255);
  // Read inclination sensor value
  inclValue = analogRead(inclPin);

  // If the device is not level, stop the motor
  if (inclValue > 550 || inclValue < 470) {
    motorSpeed = 0;
  }

  // Set motor speed
  analogWrite(motorPin, motorSpeed);


  // Wait for 100 milliseconds
  delay(100);
}
