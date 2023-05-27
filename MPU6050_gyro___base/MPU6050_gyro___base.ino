#include<Wire.h>
#define BUTTON_PIN 3

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
const int MPU_addr=0x68;
int16_t axis_X,axis_Y,axis_Z;
int minVal=265;
int maxVal=402;
double x;
double y;
double z;
int ledPin=6;
int analogPin = A0;   // potentiometer connected to analog pin 3
int val = 0;         // variable to store the read  
int new_val = 0;
int write_value=0;
int rpm = 0;
int fsrPin= A1;
int squeeze_val = 0;
int sq_power=0;
String status_pressure = "Hello String";  
int mode = 1;
int act_value=0;
int flag=0;



void setup(){
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);   // sets the pin as output
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);

if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  
  testdrawcircle();    // Draw circles (outlines)

  testfillcircle();    // Draw circles (filled)
    
 
  // Clear the buffer.



  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.clearDisplay();
display.setCursor(0, 20);
display.setTextSize(2);
display.print("ADAPTIVE");
display.display();
delay(2000);
display.clearDisplay();

  
}
void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  axis_X=Wire.read()<<8|Wire.read();
  axis_Y=Wire.read()<<8|Wire.read();
  axis_Z=Wire.read()<<8|Wire.read();
    int xAng = map(axis_X,minVal,maxVal,-90,90);
    int yAng = map(axis_Y,minVal,maxVal,-90,90);
    int zAng = map(axis_Z,minVal,maxVal,-90,90);
       x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
       y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
       z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
//     Serial.print("Angle of inclination in X axis = ");
//     Serial.print(x);
//     Serial.println((char)176);
//     Serial.print("Angle of inclination in Y axis= ");
//     Serial.print(y);
//     Serial.println((char)176);
     Serial.print("Angle of inclination in Z axis= ");
     Serial.print(z);
     Serial.println((char)176);
     Serial.println("-------------------------------------------");

    if(z>200){
      z= 0;
    }


      

// squeeze detection
squeeze_val = analogRead(fsrPin);

if (squeeze_val < 10) {
    status_pressure = "LOW";
    Serial.println(status_pressure);
    sq_power = 0;
  } else if (squeeze_val < 500) {
    status_pressure = "MED";
    Serial.println(status_pressure);
    sq_power = 40;
  } else if (squeeze_val < 800) {
    status_pressure = "HI";
    Serial.println(status_pressure);
    sq_power = 70;
  } else {
    status_pressure = "LA";
    Serial.println(status_pressure);
    sq_power = 100;
  }
  Serial.print(squeeze_val/10);
  Serial.println("------------------------");
  

// MODE SELECTION
Serial.print("BUTTON: ");
Serial.println(digitalRead(BUTTON_PIN));

if (digitalRead(BUTTON_PIN)==0){
  mode = mode+1;
  flag=1;
}  

if (mode == 1 && flag==1){
display.clearDisplay();
display.setCursor(0, 20);
display.setTextSize(2);
display.print("ADAPTIVE");
display.display();
act_value = 50;
delay(2000);
flag =0;
}

if (mode == 2 && flag==1){
display.clearDisplay();
display.setCursor(0, 20);
display.setTextSize(2);
display.print("SPORT");
display.display();
delay(2000);
act_value=150;
flag=0;   
}

if (mode == 3 && flag==1){
display.clearDisplay();
display.setCursor(0, 20);
display.setTextSize(2);
display.print("RE-GEN");
display.display();
delay(2000);
act_value = -5000;
flag=0;
}

if(mode>=4)
{
  mode =1;
}
  
  rpm = map(write_value, -110, -4, 15, 60) *2;
     
  
      
     delay(1000);
     val = analogRead(analogPin);
     new_val = val-360 + 4*z-squeeze_val/10 + act_value;
     write_value = new_val/4;
  
  



     Serial.print("write_value=");
     Serial.println(write_value);
     Serial.print("val=");
     Serial.println(val);
     Serial.println("RPM:");
     Serial.println(rpm);
     if(mode==3){
      rpm =0;
      return;
     }
      analogWrite(ledPin, write_value);
     
//     analogWrite(ledPin,val/4);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  display.clearDisplay();
//   display.setTextSize(1);
//  display.setCursor(0, 0);
//
// 
//
//  display.print("RPM:  ");
//  display.println(rpm);
//  
//
// 
//
//  display.println("Pedal Pressure:");
//  display.println(status_pressure);
//  
//  
//  
//  
//  display.print("Angle:  ");
//   
//  display.print(z);
    display.setTextSize(2);
  display.setCursor(0,0);
 
  display.print("RPM: ");
  display.setTextSize(3);
  display.setCursor(65,5);
   if (rpm==0){
    display.print("LOW");
    }else{
      display.print(rpm);
      }
  
  display.print(" ");
  display.display();

display.setTextSize(1);
display.setCursor(0, 35);
display.print("Pressure: ");
display.setTextSize(2);
display.setCursor(0, 45);
display.print(status_pressure);
display.print(" ");
display.display();

display.setTextSize(1);
display.setCursor(65, 35);
display.print("Angle:");
display.setTextSize(2);
display.setCursor(65, 45);
display.print(z);
display.display();






     
}

void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillcircle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(2000);
}
