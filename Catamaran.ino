#include <SoftwareSerial.h>

SoftwareSerial BTSerial(7, 8); // CONNECT BT RXD PIN TO ARDUINO 8 PIN | CONNECT BT TXN PIN TO ARDUINO 7 PIN

bool lower = false;

long joysticktimer;
long printTimer = 0;

int leftspeed = 0;
int rightspeed = 0;

int angle = 0;
int strength = 0;


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
  printTimer = millis();

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
      //reads angle and strength and smooths tem since the values highly fluctuate
      angle = (int)(angle * 0.9 +  val.substring(0, 3).toInt() * 0.1);
      strength = (int)(strength * 0.9 + val.substring(3, 6).toInt() * 0.1);
      byte button = val.substring(6, 8).toInt();


      handlejoystick();
    }
    Serial.flush();
  }
  if ((millis() - joysticktimer) > 300) {

    //stops the servos if no user input was given in a short amount of time
    angle = 0;
    strength = 0;
    handlejoystick();
  }

  if (millis() - printTimer > 1000) {

    Serial.println("Speed L: " + (String)leftspeed + "; R: " + (String)rightspeed);
    /*Serial.print("button:  ");
      Serial.println(intbutton);*/

  }
}

void handlejoystick(void) {
  //if it should stop just skip all the calculating and stop imidiently
  if (strength <= 0) {
    leftspeed = 0;
    rightspeed = 0;
    analogWrite(3, abs(0));
    analogWrite(6, abs(0));

    return;
  }

  // pin 3 and 5 are the left servo, where 3 is the velocity and 5 the direction
  // pin 6 and 9 are the left servo, where 3 is the velocity and 5 the direction
  // upper / lower half
  if (angle >= 180)lower = true;
  else lower = false;
  float cosinus;

  // zunächst die rechten beiden Quadranten
  if (angle <= 90 || angle >= 270) {
    leftspeed = strength;

    cosinus = cos(2 * angle * PI / 180);
    // if cosinus greater then zero, the right servo turns backwarts
    // i. e. smaler then 45° or greater then 315°
    rightspeed = (int)(strength * (-cosinus));


  } else {
    rightspeed = strength;

    cosinus = cos(2 * angle * PI / 180);
    // if the cosinus is greater then zero, the left servo turns backwarts

    leftspeed = (int)(strength * (-cosinus));
  }
  if (lower) {
    leftspeed = -leftspeed;
    rightspeed = -rightspeed;
  }
  // writing of velocity and direction
  if (leftspeed < 0) digitalWrite(5, HIGH);
  else digitalWrite(5, LOW);

  if (rightspeed < 0) digitalWrite(9, HIGH);
  else digitalWrite(9, LOW);


  leftspeed = sign(leftspeed) * map(abs(leftspeed) * 10, 0, 1000, 56, 100);
  rightspeed = sign (rightspeed) * map(abs(rightspeed) * 10, 0, 1000, 126, 200);

  analogWrite(3, abs(leftspeed));
  analogWrite(6, abs(rightspeed));

}

//returns signum of int num
int sign(int num) {
  if (signbit(num)) return -1;
  else return 1;
  }

//returns only true if every char in String is a digit
boolean isValidNumber(String str) {
  for (char c : str) {

    if (!isDigit(c))
      return false;
  }
  return true;
}
