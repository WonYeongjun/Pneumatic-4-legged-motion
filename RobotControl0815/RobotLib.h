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
  void Leg_simultaneous_contraction();
  void Leg_simultaneous_extention();

  void Bending12();
  void Bending23();
  void Bending31();

  void Extension();
  void Contraction();
  void Neutral();

  void Forward();
  void Backward();
  void Standing();
};

class Robot {
public:
  Leg A, B, C;

  Robot(const int S_PIN[9], const int P_PIN[9], const int speedarray[9]);
  void Initialize();
    
  void AB_Forward();

  void FL_RR_Forward();
  void FR_RL_Forward();

  void Forward();
  void Backward();
  void TurnRight();
  void TurnLeft();
  void Standing();
};
