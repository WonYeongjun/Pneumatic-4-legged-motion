\
// RobotLib.h
#pragma once
#include <Arduino.h>
#include <Servo.h>
#include <math.h>

class Joy {
public:
  int joy;
  int X;
  int Y;
  int ctrx;
  int ctry;
  int pos;

  int stop_sign=0;
  long newx;
  long newy;
  float angle;
  long length;
  float state[3];
  explicit Joy(int X_, int Y_, int joy_,int ctrx=519, int ctry=494);
  void Initialize();
  void get_input();
  void cal_angle();
  void cal_length();
  //void angle_to_rel_state();
};

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
  Joy joy;
  int unit_angle = 10;

  Leg(int s_pin1, int s_pin2, int s_pin3, int p_pin1, int p_pin2, int p_pin3, int speed1, int speed2, int speed3, int X_=1, int Y_=2, int joy_=3, int ctrx_=519, int ctry_=494);
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
  Leg A, B, C, D;
  int unit_angle=10;

  Robot(const int S_PIN[12], const int P_PIN[12], const int speedarray[12]);
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
