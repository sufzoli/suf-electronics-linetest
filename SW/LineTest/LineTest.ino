
#define MainsSwitchStraight 3
#define MainsSwitchCross 2
#define SenseNeutral 0
#define SenseLine 1
#define OnOffSwitch 7
#define LED_Yellow 10
#define LED_Blue 8
#define LED_Green 9

byte NeutralCount;
byte LineCount;
byte NoEarthCount;
byte OnOffCount;
byte DeadTime;
bool OnOffTriggered;
bool StraightSwitchState;
bool CrossSwitchState;

byte LedState;
/*
 * Bit
 * 0-1  Yellow (0 - Off, 1 - On, 2 - Blink)
 * 2-3  Green  (0 - Off, 1 - On, 2 - Blink)
 * 4-5  Blue   (0 - Off, 1 - On, 2 - Blink)
 * 6    Green/Blue Blink
 */
 byte LedCounter;

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
  OnOffCount = 0;
  OnOffTriggered = false;
  LedState = 0;
  LedCounter = 0;
  NoEarthCount = 0;
  digitalWrite(MainsSwitchStraight, LOW);
  digitalWrite(MainsSwitchCross, LOW);
  StraightSwitchState = false;
  CrossSwitchState = false;
  DeadTime = 0;
}

void LedWorker()
{
  LedCounter++;
  switch(LedState & 0x03)
  {
      case 0:
        digitalWrite(LED_Yellow, LOW);
        break;
      case 1:
        digitalWrite(LED_Yellow, HIGH);
        break;
      case 2:
        digitalWrite(LED_Yellow, (LedCounter > 5) ? HIGH : LOW);
        break;
  }
  if(LedState & 0x40)
  {
    if(LedCounter > 5)
    {
      digitalWrite(LED_Green, LOW);
      digitalWrite(LED_Blue, HIGH);
    }
    else
    {
      digitalWrite(LED_Green, HIGH);
      digitalWrite(LED_Blue, LOW);
    }
  }
  else
  {
    switch(LedState & 0x0C)
    {
        case 0:
          digitalWrite(LED_Green, LOW);
          break;
        case 4:
          digitalWrite(LED_Green, HIGH);
          break;
        case 8:
          digitalWrite(LED_Green, (LedCounter > 5) ? HIGH : LOW);
          break;
    }
    switch(LedState & 0x30)
    {
        case 0:
          digitalWrite(LED_Blue, LOW);
          break;
        case 0x10:
          digitalWrite(LED_Blue, HIGH);
          break;
        case 0x20:
          digitalWrite(LED_Blue, (LedCounter > 5) ? HIGH : LOW);
          break;
    }
    
  }
  if(LedCounter > 9)
    LedCounter = 0;
}

void SwitchOff()
{
    StraightSwitchState = false;
    CrossSwitchState = false;
    digitalWrite(MainsSwitchStraight, LOW);
    digitalWrite(MainsSwitchCross, LOW);
    LedState = 0;
    DeadTime=10;
}

void loop()
{
  if(DeadTime != 0)
    DeadTime--;
  if(digitalRead(SenseNeutral) == HIGH)
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
  if(digitalRead(SenseLine) == HIGH)
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

  if(digitalRead(OnOffSwitch) == LOW)
  {
    if(OnOffCount < 10)
    {
      OnOffCount++;
      OnOffTriggered = (OnOffCount == 10);
    }
  }
  else
  {
    OnOffCount = 0;
  }
  // Count the non-Earthed state
  if(LineCount == 0 && NeutralCount == 0)
  {
    if(NoEarthCount < 10)
    {
      NoEarthCount++;
    }
  }
  else
  {
    NoEarthCount = 0;
  }
  
  if(StraightSwitchState || CrossSwitchState)
  {
    // system was online
    if(NoEarthCount < 10)
    {
      if(StraightSwitchState)
      {
        if(LineCount < 10)
        {
          SwitchOff();
        }
      }
      if(CrossSwitchState)
      {
        if(NeutralCount < 10)
        {
          SwitchOff();
        }
      }
    }
    if(LineCount >= 10 && NeutralCount >= 10)
    {
      SwitchOff();
    }
    if(OnOffTriggered)
    {
      SwitchOff();
      OnOffTriggered = false;
    }
  }
  else
  {
    // The system was offline
    if(DeadTime == 0)
    {
      if(LineCount >= 10 && NeutralCount == 0)
      {
        if(OnOffTriggered)
        {
          digitalWrite(MainsSwitchStraight, HIGH);
          // Switch on Green;
          LedState = 0x04;
          StraightSwitchState = true;
          OnOffTriggered = false;
        }
        else
        {
          // Blink Green;
          LedState = 0x08;
        }
      }
      if(NeutralCount >= 10 && LineCount == 0)
      {
        if(OnOffTriggered)
        {
          digitalWrite(MainsSwitchCross, HIGH);
          // Switch on Blue
          LedState = 0x10;
          CrossSwitchState = true;
          OnOffTriggered = false;
        }
        else
        {
          // Blink Blue
          LedState = 0x20;
        }
      }
      if(NoEarthCount >= 10)
      {
        if(OnOffTriggered)
        {
          digitalWrite(MainsSwitchStraight, HIGH);
          LedState = 0x16;
          StraightSwitchState = true;
          OnOffTriggered = false;
        }
        else
        {
          LedState = 0x42;
        }
      }
    }
  }
  LedWorker();
  delay(100);
}
