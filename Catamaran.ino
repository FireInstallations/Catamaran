#include <SoftwareSerial.h>

SoftwareSerial BTSerial(7, 8); // CONNECT BT RXD PIN TO ARDUINO 8 PIN | CONNECT BT TXN PIN TO ARDUINO 7 PIN

bool lower = false;
long joysticktimer;

int leftspeed = 0;
int rightspeed = 0;


void setup()
{
  pinMode(3, OUTPUT);// links speed
  pinMode(5, OUTPUT);// links direction LOW FORWARD
  pinMode(6, OUTPUT);// rechts speed
  pinMode(9, OUTPUT);// rechts direction LOW FORWARD

  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(9, LOW);
  Serial.begin(9600);

  BTSerial.begin(9600);  // HC-05 default speed in AT command more
  joysticktimer = millis();

  delay(200);

}

void loop()
{ 
  if (BTSerial.available() > 0)
  {
    joysticktimer = millis();
    String val =
      BTSerial.readStringUntil('#');
    if ((val.length() == 7) && (isValidNumber(val))) {
      int angle = val.substring(0, 3).toInt();
      int strength = val.substring(3, 6).toInt();
      byte button = val.substring(6, 8).toInt();


      handlejoystick(angle, strength);
    }
    Serial.flush();
  }
  if ((millis() - joysticktimer) > 300) {
      Serial.println("HERE");
      handlejoystick(0, 0);
    }
    delay(100);
}

void handlejoystick(int angle, int strength) {

  // pin 3 und 5 sind für den linken Motor, wobei 3 die Geschwindigkeit bestimmt und 5 die Richtung
  // pin 6 und 9 sind für den rechten Motor, wobei 6 die Geschwindigkeit bestimmt und 9 die Richtung
  // obere Hälfte oder untere Hälfte
  if (angle >= 180)lower = HIGH;
  else lower = LOW;
  float cosinus;

  // zunächst die rechten beiden Quadranten
  if (angle <= 90 || angle >= 270) {
    leftspeed = (int)(leftspeed * 0.9 + strength * 2.55 * 0.1);

    cosinus = cos(2 * angle * PI / 180);
    // Wenn der cosinusterm größer null ist, die Richtung für rechts auf rückwärts
    // also kleiner als 45° oder größer als 315°
    rightspeed = (int)(rightspeed *0.9 + (strength * 2.55) * (-cosinus) * 0.1);


  } else {

    rightspeed = (int)(rightspeed *0.9 + (strength * 2.55) *0.1);

    cosinus = cos(2 * angle * PI / 180);
    // Wenn der cosinusterm größer null ist, die Richtung für links auf rückwärts

    leftspeed = (int)(leftspeed * 0.9 + (strength * 2.0) * (-cosinus) * 0.1);
  }
  if (lower) {
    leftspeed = -leftspeed;
    rightspeed = -rightspeed;
  }
  // Schreiben der Stati und Geschwindigkeiten
  if (leftspeed < 0) digitalWrite(5, HIGH);
  else digitalWrite(5, LOW);

  if (rightspeed < 0) digitalWrite(9, HIGH);
  else digitalWrite(9, LOW);

  analogWrite(3, abs(leftspeed));
  analogWrite(6, abs(rightspeed));


  Serial.println("Speed L: " + (String)leftspeed + "; R: " + (String)rightspeed);
  /*Serial.print("button:  ");
  Serial.println(intbutton);*/

}

//returns only true if every char in String is a digit
boolean isValidNumber(String str) {
  for (char c : str) {

    if (!isDigit(c))
      return false;
  }
  return true;
}
