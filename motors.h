#include <stdio.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//Define port for motor and servo
#define PWM_CHANNEL1 8
#define PWM_CHANNEL2 9
#define PWM_CHANNEL3 10
#define PWM_CHANNEL4 11
#define m1a 12
#define m1b 13
#define m2a 14
#define m2b 15
#define Servo_1 7
#define Servo_2 6
#define Servo_3 5
#define Servo_4 4

#define MIN_USEC 200 //PWM pulse-width turn servo angle to 0
#define MAX_USEC 415 //PWM pulse-width turn servo angle to 180

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setPWMMotor(int m1A, int m1B, int m2A, int m2B, int xL, int xR) //Declare drivertrain's motor pwm calculation function
{
  int n1a = m1A;
  int n1b = m1B;
  int n2a = m2A;
  int n2b = m2B;

  if (xL > 0)
    {

      if (n1a > 0 && n2a > 0)
      {
        n1a = m1A - xL;
        n2a = m2A + xL;
      }

      if (n1b > 0 && n2b > 0)
      { 
        n1b = m1B - xL;
        n2b = m2B + xL; 
      }

      if (n1a == 0 && n1b == 0 && n2a == 0 && n2b == 0)
      {
        n1a = xL;
        n2b = xL;
      }
    }
  
  if (xR > 0)
    {
      if (n1a > 0 && n2a > 0)
      {
        n1a = m1A + xR;
        n2a = m2A - xR;
      }

      if (n1b > 0 && n2b > 0)
      {
        n1b = m1B + xR;
        n2b = m1B - xR;
      }
      if (n1a == 0 && n1b == 0 && n2a == 0 && n2b == 0)
      {
        n1b = xR;
        n2a = xR;
      }
    }
  Serial.println("Calculated value for motor A and B:");
  char dbg_str[50];
  sprintf(dbg_str,"n1a: %d n1b: %d n2a: %d n2b: %d", n1a, n1b, n2a, n2b);
  Serial.println(dbg_str);

  pwm.setPin(m1a, n1a, 0);
  pwm.setPin(m1b, n1b, 0);
  pwm.setPin(m2a, n2a, 0);
  pwm.setPin(m2b, n2b, 0);

}

void setPWMLinear(int speed, bool x) //Declare linear's motor controlling function
{
  int n1 = 0; 
  int n2 = 0;
  switch(x)
  {
    case(1):
    {
      n1 = speed;
      n2 = 0;
      break;
    }
    case(0):
    {
      n1 = 0;
      n2 = speed;
      break;
    }
  }
  char dbg_str[50];
  sprintf(dbg_str,"N1: %d N2: %d", n1, n2);
  Serial.println(dbg_str);
  pwm.setPin(PWM_CHANNEL1, n1, 0);
  pwm.setPin(PWM_CHANNEL2, n2, 0);
}

void setPWMIntake(int speed, bool x) //Declare intake's motor controlling function
{
  int s1 = 0; 
  int s2 = 0;
  switch(x)
  {
    case(1):
    {
      s1 = speed;
      s2 = 0;
      break;
    }
    case(0):
    {
      s1 = 0;
      s2 = speed;
      break;
    }
  }
  char dbg_str[50];
  sprintf(dbg_str,"S1: %d S2: %d", s1, s2);
  Serial.println(dbg_str);
  pwm.setPin(PWM_CHANNEL3, s1, 0);
  pwm.setPin(PWM_CHANNEL4, s2, 0);
}

void setServoAngle(int channel, int angle) //Declare servo's angle controlling function
{
  int pulseWidth = map(angle, 0, 90, MIN_USEC, MAX_USEC);
  pwm.setPWM(channel, 0, pulseWidth);
}

void setServo360(int channel, int ms) //Declare servo 360 degree controlling function
{
  pwm.setPWM(channel, 0, ms);
}

void initMotors()
{
  Wire.begin(); // SDA, SCL,400000);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Wire.setClock(400000);
  setPWMMotor(0, 0, 0, 0, 0, 0);
  setPWMLinear(0, 0);
  setPWMIntake(0, 0);
  setServoAngle(Servo_3, 0);
  setServo360(Servo_1, STOP_usec);
  setServo360(Servo_2, STOP_usec);
}
