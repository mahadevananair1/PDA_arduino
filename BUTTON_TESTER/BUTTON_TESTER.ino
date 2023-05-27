#define BUTTON_PIN_1 3

void setup()
{
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
 
}
void loop()
{
  Serial.println(digitalRead(3));
  
  Serial.println("7777777777777777777777");
  delay(1000);
}
