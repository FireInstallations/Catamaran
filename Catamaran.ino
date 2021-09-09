





#include <SoftwareSerial.h>

SoftwareSerial BTSerial(7, 8); // CONNECT BT RXD PIN TO ARDUINO 8 PIN | CONNECT BT TXN PIN TO ARDUINO 7 PIN

int intangle = 0;
int intstrength = 0;
int intbutton = 0;
float pi = 3.1416;
bool rightbackward = 0;
bool leftbackward = 0;
bool lower = 0;
long joysticktimer;


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
}

void loop()
{
  if (BTSerial.available() > 0)
  {
    joysticktimer = millis();
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


      handlejoystick();
      BTSerial.flush();
    }
    if ((millis() - joysticktimer) > 300) {
      Serial.println("HERE");
      intstrength = 0;
      intangle = 0;
      handlejoystick();
    }
    delay(100);
  }
}

void handlejoystick(void) {

  // pin 3 und 5 sind für den rechten Motor, wobei 3 die Geschwindigkeit bestimmt und 5 die Richtung
  // pin 6 und 9 sind für den linken Motor, wobei 6 die Geschwindigkeit bestimmt und 9 die Richtung
  // obere Hälfte oder untere Hälfte
  if (intangle >= 180)lower = HIGH;
  else lower = LOW;
  int leftspeed;
  int rightspeed;
  float cosinus;

  // zunächst die rechten beiden Quadranten
  if (intangle <= 90 || intangle >= 270) {
    leftspeed = (int)(intstrength * 2.55);

    cosinus = cos(2 * intangle * pi / 180);
    // Wenn der cosinusterm größer null ist, die Richtung für rechts auf rückwärts
    // also kleiner als 45° oder größer als 315°
    rightspeed = (int)(intstrength * 2.55) * (-cosinus);


  } else {

    rightspeed = (int)(intstrength * 2.55);

    cosinus = cos(2 * intangle * pi / 180);
    // Wenn der cosinusterm größer null ist, die Richtung für links auf rückwärts

    leftspeed = (int)(intstrength * 2.55) * (-cosinus);
  }
  if (lower) {
    leftspeed = -leftspeed;
    rightspeed = -rightspeed;
  }
  // Schreiben der Stati und Geschwindigkeiten
  if (leftspeed < 0) digitalWrite(9, HIGH);
  else digitalWrite(9, LOW);

  if (rightspeed < 0) digitalWrite(5, HIGH);
  else digitalWrite(5, LOW);

  analogWrite(6, abs(leftspeed));
  analogWrite(3, abs(rightspeed));



  

  Serial.print("button:  ");
  Serial.println(intbutton);
  
}
