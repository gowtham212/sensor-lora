float readpres(void);
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);
float data;
void setup()
{
  unsigned long startMillis;
  unsigned long currentMillis;
  unsigned long period;
  unsigned long startMillis1;
  unsigned long currentMillis1;
  unsigned long period1;
  int k = 0;
  pinMode(3, OUTPUT);//relay vcc to 7 and 8
  Serial.begin(9600);
  mySerial.begin(9600);

  mySerial.print("Enter number: ");
  while (mySerial.available() == 0)
  {

  }
  data = mySerial.parseFloat();
  mySerial.println(data);
  int a = readpres();
  if (a < 2) {
    digitalWrite(3, LOW);
    startMillis = millis();
  }
  for (int i = 0; i <= 50; i++)
  {
   
    while (readpres() < data )
    {

    }
    currentMillis = millis();
    digitalWrite(3, HIGH);

    period = (currentMillis - startMillis);

    if (period != 0)
    { mySerial.println ("period");
      mySerial.println (period);
    }
//    delay(3000);
  }

}
void loop()
{


}
float readpres()
{ int sensorVal = analogRead(A1);
  float voltage = (sensorVal * 5.0) / 1024.0;
  float pressure_pascal = (3.0 * ((float)voltage - 0.47)) * 1000000.0;
  float pressure_bar = (pressure_pascal / 10e5)*10;
  mySerial.println(pressure_bar);
  return pressure_bar;
}
