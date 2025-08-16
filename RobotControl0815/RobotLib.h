\
// RobotLib.h
#pragma once
#include <Arduino.h>
#include <Servo.h>

class Pump {
public:
  int pin;   // pump pin
  int tmp;   // on=1, off=0
  int speed;

  explicit Pump(int pin_, int speed_);
  void Initialize();
  int  GetTmp();
  void PumpOn();
  void PumpOff();
  void Switch();
  void Intensity(int n);
};

class MyServo : public Servo {
public:
  int pin;    // servo pin
  int angle;  // current angle
  explicit MyServo(int p);
  void Initialize();
  void Extension(int n);
  void Contraction(int n);
};

class Leg {
public:
  MyServo sv1, sv2, sv3;
  Pump pump1, pump2, pump3;
  int unit_angle = 10;

  Leg(int s_pin1, int s_pin2, int s_pin3, int p_pin1, int p_pin2, int p_pin3, int speed1, int spped2, int speed3);
  void Leg_PumpOn();
  void Leg_PumpOff();
  void Initialize();

  void Bending12();
  void Bending23();
  void Bending31();

  void Extension();
  void Contraction();
  void Neutral();

  void Forward();
  void Backward();
};

class Robot {
public:
  Leg FL, FR, RL, RR;
  int tmp;

  Robot(const int S_PIN[12], const int P_PIN[12], const int speedarray[12]);
  void Initialize();

  void FL_RR_Forward();
  void FR_RL_Forward();

  void Forward();
  void Backward();
  void TurnRight();
  void TurnLeft();
};
