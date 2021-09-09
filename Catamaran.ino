





#include <SoftwareSerial.h>

SoftwareSerial BTSerial(7, 8); // CONNECT BT RXD PIN TO ARDUINO 8 PIN | CONNECT BT TXN PIN TO ARDUINO 7 PIN

int intangle = 0;
int intstrength = 0;
int intbutton = 0;
float pi = 3.1416;
bool rightbackward = 0;
bool leftbackward = 0;
bool lower = 0;


void setup()
{
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);

  BTSerial.begin(9600);  // HC-05 default speed in AT command more
}

void loop()
{
  if (BTSerial.available() > 0)
  {
    String value =
      BTSerial.readStringUntil('#');
    if (value.length() == 7)
    {
      String angle =
        value.substring(0, 3);
      intangle = angle.toInt();
      String strength =
        value.substring(3, 6);
      intstrength = strength.toInt();
      String button =
        value.substring(6, 8);
      intbutton = button.toInt();



      // pin 3 und 5 sind für den rechten Motor, wobei 3 die Geschwindigkeit bestimmt und 5 die Richtung
      // pin 6 und 9 sind für den linken Motor, wobei 6 die Geschwindigkeit bestimmt und 9 die Richtung
      // obere Hälfte oder untere Hälfte
      if (intangle >= 180)lower = HIGH;
      else lower = LOW;
      int leftspeed;
      int rightspeed;

      // zunächst die rechten beiden Quadranten
      if (intangle <= 90 || intangle >= 270) {
        leftspeed = (int)(intstrength * 2.55);
        if (lower) leftspeed = -leftspeed;
        float cosinus = cos(2 * intangle * pi / 180);
        // Wenn der cosinusterm kleiner null ist, die Richtung für rechts auf rückwärts
        if (cosinus < 0) rightbackward = HIGH;
        else rightbackward = LOW;
        rightspeed = leftspeed * cosinus;
        if (rightbackward) rightspeed = - rightspeed;
        if (lower)rightspeed = -rightspeed;
        } else {

        rightspeed = (int)(intstrength * 2.55);
        if (lower) rightspeed = -rightspeed;
        float cosinus = cos(2 * intangle * pi / 180);
        // Wenn der cosinusterm kleiner null ist, die Richtung für links auf rückwärts
        if (cosinus < 0) leftbackward = HIGH;
        else leftbackward = LOW;
        leftspeed = rightspeed * cosinus;
        if (leftbackward) leftspeed = - leftspeed;
        if (lower)leftspeed = -leftspeed;

        }
      // Schreiben der Stati und Geschwindigkeiten
      if (leftspeed < 0) digitalWrite(9, HIGH);
      else digitalWrite(9, LOW);

      if (rightspeed < 0) digitalWrite(5, HIGH);
      else digitalWrite(5, LOW);

      analogWrite(6, abs(leftspeed));
      analogWrite(3, abs(rightspeed));



      Serial.print("leftspeed    ");
      Serial.println(leftspeed);

      Serial.print("rightspeed  ");
      Serial.println(rightspeed);

      Serial.println();

      if (leftspeed < 0)Serial.println("left backward");
      if (rightspeed < 0)Serial.println("right backward");

      Serial.print("button:  ");
      Serial.println(button);
      Serial.flush();
    }
  }
}
