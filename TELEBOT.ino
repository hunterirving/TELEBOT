
#include <Servo.h>
#include <stdlib.h>

Servo mouth;
Servo rightEye;
Servo leftEye;

//Read in mic inputs
int analogPin = A1;

//mouth movements
int pos = 0;
int maxInput = 15;
int mouthOpenAngle = 41;
int mouthCloseAngle = 31;
int val = 0;
int mouthDelay = 5;
int mouthPrev;
int newAngle;
int steps;
int mouthTimeNum = 50;
unsigned long lastMouthAction;

//eye movements
int leftEyeOpenAngle = 39;
int rightEyeOpenAngle = 43;

int leftEyeCloseAngle = 62;
int rightEyeCloseAngle = 22;

int ledPinR = 5;
int ledPinL = 6;

//blinking stuff
unsigned long lastTime;
unsigned long curTime;
int secBetweenBlinks = 3;
bool isBlinking = false;
bool isClosing = false;
double blinkJumpDistanceR = 0.037;
double blinkJumpDistanceL = 0.05;
double lastAngleL;
double lastAngleR;


void setup() 
{
  Serial.begin(115200);
  
  mouth.attach(9);  // attaches the servo on pin 9 to the servo object
  leftEye.attach(10);
  rightEye.attach(11);
  
  pinMode(ledPinL, OUTPUT);
  pinMode(ledPinR, OUTPUT);
  lastTime = millis();

  mouth.write(mouthCloseAngle);
  
  mouthPrev = mouthCloseAngle;
  lastAngleL = leftEyeOpenAngle;
  lastAngleR = rightEyeOpenAngle;

  leftEye.write(leftEyeOpenAngle);
  rightEye.write(rightEyeOpenAngle);

  startupTwo();

  
  
}

void loop() 
{
  //if currently in the process of blinking
  if(isBlinking)
  {
    Serial.println("BLINKINGGG");
    if(isClosing)
    {
      //make the angle smaller until eyeOpenAngle
      lastAngleL = lastAngleL + blinkJumpDistanceL;
      lastAngleR = lastAngleR - blinkJumpDistanceR;
      //check if new angles cross eyeCloseAngles
      if(!(lastAngleL > leftEyeCloseAngle || lastAngleR < rightEyeCloseAngle))
      {
        leftEye.write(lastAngleL);
        rightEye.write(lastAngleR);
      }
      else
      {
        //eyes are now shut, start opening
        leftEye.write(leftEyeCloseAngle);
        rightEye.write(rightEyeCloseAngle);
        lastAngleL = leftEyeCloseAngle;
        lastAngleR = rightEyeCloseAngle;
        isClosing = false;
      }
        
    }
    else
    {
      //make the angle greater until eyeOpenAngle
      lastAngleL = lastAngleL - blinkJumpDistanceL;
      lastAngleR = lastAngleR + blinkJumpDistanceR;
      //check if new angles cross eyeCloseAngles
      if(!(lastAngleL < leftEyeOpenAngle || lastAngleR > rightEyeOpenAngle))
      {
        leftEye.write(lastAngleL);
        rightEye.write(lastAngleR);
      }
      else
      {
        //eyes are now open, done blinking
        leftEye.write(leftEyeOpenAngle);
        rightEye.write(rightEyeOpenAngle);
        lastAngleL = leftEyeOpenAngle;
        lastAngleR = rightEyeOpenAngle;
        isBlinking = false;
      }
    }
  }
  else
  {
    curTime = millis();
    Serial.println(curTime - lastTime);
    //if difference in time is greater than secBetweenBlinks
    if((curTime - lastTime) > (secBetweenBlinks * 1000))
    {
      //record new time
      lastTime = millis();
      //50/50 chance to start blinking
      if(rand() % 2)
      {
        isBlinking = true;
        isClosing = true;
      }
      
    }
  }

  curTime = millis();
  if(curTime - lastMouthAction > mouthTimeNum)
  {
   lastMouthAction = curTime;
   openMouth(analogRead(analogPin));
  }
   
}




void openMouth(int power)
{
  if(power > maxInput)
  {
    power = maxInput;
  }

  newAngle = mouthCloseAngle + power;
  
  //if it needs to open more
  if(mouthCloseAngle + power > mouthPrev)
  {
    newAngle = mouthCloseAngle + power + 1;
    if(newAngle > mouthOpenAngle)
    {
      newAngle = mouthOpenAngle;
    }
    mouth.write(newAngle);
    mouthPrev = newAngle;
      
    if(!isBlinking)
    {
      analogWrite(ledPinR, power*5);
      analogWrite(ledPinL, power*5);
    }
    else
    {
    analogWrite(ledPinR, 0);
      analogWrite(ledPinL, 0);   
    }
  }
  else //else it needs to close more
  {
    newAngle = mouthCloseAngle + power - 3;
    if(newAngle < mouthCloseAngle)
    {
      newAngle = mouthCloseAngle;
    }
    mouth.write(newAngle);
    mouthPrev = newAngle;
    if(!isBlinking)
    {
      analogWrite(ledPinR, power*2+15);
      analogWrite(ledPinL, power*2+15);
    }
    else
    {
       analogWrite(ledPinR, 0);
      analogWrite(ledPinL, 0);    
      }
  }
}

void startupOne()
{
  leftEye.write(leftEyeCloseAngle);
  rightEye.write(rightEyeCloseAngle);

  mouth.write(mouthOpenAngle);
  delay(10000);
  
}

void startupTwo()
{
  leftEye.write(leftEyeOpenAngle);
  analogWrite(ledPinR, 250);
  analogWrite(ledPinL, 250);
  delay(1000);
  rightEye.write(rightEyeOpenAngle);
  
}
