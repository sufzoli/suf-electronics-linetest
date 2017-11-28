
#define MainsSwitchStraight 3
#define MainsSwitchCross 2
#define SenseNeutral 0
#define SenseLine 1
#define LED_Yellow 10
#define LED_Blue 8
#define LED_Green 9

byte NeutralCount;
byte LineCount;
bool StraightSwitchState;
bool CrossSwitchState;

void setup()
{
  pinMode(MainsSwitchStraight, OUTPUT);
  pinMode(MainsSwitchCross, OUTPUT);
  pinMode(SenseNeutral, INPUT);
  pinMode(SenseLine, INPUT);
  pinMode(LED_Yellow, OUTPUT);
  pinMode(LED_Blue, OUTPUT);
  pinMode(LED_Green, OUTPUT);
  NeutralCount = 0;
  LineCount = 0;
  digitalWrite(MainsSwitchStraight, LOW);
  digitalWrite(MainsSwitchCross, LOW);
  StraightSwitchState = false;
  CrossSwitchState = false;
}

void loop()
{
  if(digitalRead(SenseNeutral == HIGH))
  {
    if(NeutralCount < 10)
    {
      NeutralCount++;
    }
  }
  else
  {
    NeutralCount = 0;
  }
  if(digitalRead(SenseLine == HIGH))
  {
    if(LineCount < 10)
    {
      LineCount++;
    }
  }
  else
  {
    LineCount = 0;
  }
  if(StraightSwitchState || CrossSwitchState)
  {
    // system was online
    if(StraightSwitchState)
    {
      if(LineCount < 10)
      {
        StraightSwitchState = false;
        CrossSwitchState = false;
        digitalWrite(MainsSwitchStraight, LOW);
        digitalWrite(MainsSwitchCross, LOW);
        digitalWrite(LED_Green, LOW);
        digitalWrite(LED_Blue, LOW);
        delay(1000);
      }
    }
    if(CrossSwitchState)
    {
      if(NeutralCount < 10)
      {
        StraightSwitchState = false;
        CrossSwitchState = false;
        digitalWrite(MainsSwitchStraight, LOW);
        digitalWrite(MainsSwitchCross, LOW);
        digitalWrite(LED_Green, LOW);
        digitalWrite(LED_Blue, LOW);
        delay(1000);
      }
    }
    if(LineCount >= 10 && NeutralCount >= 10)
    {
      StraightSwitchState = false;
      CrossSwitchState = false;
      digitalWrite(MainsSwitchStraight, LOW);
      digitalWrite(MainsSwitchCross, LOW);
      digitalWrite(LED_Green, LOW);
      digitalWrite(LED_Blue, LOW);
      delay(1000);
    }
  }
  else
  {
    // The system was offline
    if(LineCount >= 10 && NeutralCount == 0)
    {
      digitalWrite(MainsSwitchStraight, HIGH);
      digitalWrite(LED_Green, HIGH);
      StraightSwitchState = true;
    }
    if(NeutralCount >= 10 && LineCount == 0)
    {
      digitalWrite(MainsSwitchCross, HIGH);
      digitalWrite(LED_Blue, HIGH);
      CrossSwitchState = true;
    }
  }
  delay(100);
}
