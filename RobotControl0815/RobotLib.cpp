
// RobotLib.cpp
#include "RobotLib.h"

// -------- Pump --------
Pump::Pump(int pin_, int speed_) : pin(pin_), speed(speed_), tmp(0) {}

void Pump::Initialize() {
  analogWrite(pin, 0);
  pinMode(pin, OUTPUT);
}

int Pump::GetTmp() { return tmp; }

void Pump::PumpOn() {
  if (GetTmp() == 0) {
    tmp = 1;
    analogWrite(pin, speed);
    Serial.println(F("Pump On"));
  } else {
    Serial.println(F("Already On"));
  }
}

void Pump::PumpOff() {
  if (GetTmp() == 1) {
    tmp = 0;
       analogWrite(pin, 0);
    Serial.println(F("Pump Off"));
  } else {
    Serial.println(F("Already Off"));
  }
}

void Pump::Switch() {
  if (GetTmp() == 0) PumpOn();
  else PumpOff();
}

void Pump::Intensity(int n) {
    tmp = 1;
    analogWrite(pin, n);
}

// -------- MyServo --------
MyServo::MyServo(int p) : pin(p), angle(0) {}

void MyServo::Initialize() {
  attach(pin);
  write(0);
  delay(2000);
  detach();
}

void MyServo::Extension(int n) {
  for (int i = 0; i < 130 / n; i++) {
    if (angle == 130) {
      Serial.println(F("already extended"));
      break;
    }
    angle += n;
    write(angle);
    delay(50);
  }
}

void MyServo::Contraction(int n) {
  for (int i = 0; i < 130 / n; i++) {
    if (angle == 0) {
      Serial.println(F("already contracted"));
      break;
    }
    angle -= n;
    write(angle);
    delay(50);
  }
}

// -------- Leg --------
Leg::Leg(int s_pin1, int s_pin2, int s_pin3, int p_pin1, int p_pin2, int p_pin3, int speed1, int speed2, int speed3)
: sv1(s_pin1), sv2(s_pin2), sv3(s_pin3), pump1(p_pin1, speed1), pump2(p_pin2, speed2), pump3(p_pin3, speed3) {}

void Leg::Leg_PumpOn() {
  pump1.PumpOn();
  pump2.PumpOn();
  pump3.PumpOn();
}
void Leg::Leg_PumpOff() {
  pump1.PumpOff();
  pump2.PumpOff();
  pump3.PumpOff();
}

void Leg::Initialize() {
  sv1.Initialize();
  sv2.Initialize();
  sv3.Initialize();
  pump1.Initialize();
  pump2.Initialize();
  pump3.Initialize();
}

void Leg::Bending12() {
  Leg_PumpOn();
  Serial.println(F("Bend to 12"));

  sv3.attach(sv3.pin);
  sv3.Extension(unit_angle);
  delay(150);
  sv3.detach();

  sv1.attach(sv1.pin);
  sv1.Contraction(unit_angle);
  delay(150);
  sv1.detach();

  sv2.attach(sv2.pin);
  sv2.Contraction(unit_angle);
  delay(150);
  sv2.detach();
}

void Leg::Bending23() {
  Leg_PumpOn();
  Serial.println(F("Bend to 23"));

  sv1.attach(sv1.pin);
  sv1.Extension(unit_angle);
  delay(150);
  sv1.detach();

  sv2.attach(sv2.pin);
  sv2.Contraction(unit_angle);
  delay(150);
  sv2.detach();

  sv3.attach(sv3.pin);
  sv3.Contraction(unit_angle);
  delay(150);
  sv3.detach();
}

void Leg::Bending31() {
  Leg_PumpOn();
  Serial.println(F("Bend to 31"));

  sv2.attach(sv2.pin);
  sv2.Extension(unit_angle);
  delay(150);
  sv2.detach();

  sv3.attach(sv3.pin);
  sv3.Contraction(unit_angle);
  delay(150);
  sv3.detach();

  sv1.attach(sv1.pin);
  sv1.Contraction(unit_angle);
  delay(150);
  sv1.detach();
}

void Leg::Extension() {
  Serial.println(F("팽창"));

  sv1.attach(sv1.pin);
  sv1.Extension(unit_angle);
  delay(150);
  sv1.detach();

  sv2.attach(sv2.pin);
  sv2.Extension(unit_angle);
  delay(150);
  sv2.detach();

  sv3.attach(sv3.pin);
  sv3.Extension(unit_angle);
  delay(150);
  sv3.detach();
  Leg_PumpOn();
}

void Leg::Contraction() {
  Serial.println(F("수축"));

  sv1.attach(sv1.pin);
  sv1.Contraction(unit_angle);
  delay(150);
  sv1.detach();

  sv2.attach(sv2.pin);
  sv2.Contraction(unit_angle);
  delay(150);
  sv2.detach();

  sv3.attach(sv3.pin);
  sv3.Contraction(unit_angle);
  delay(150);
  sv3.detach();
  Leg_PumpOn();
}

void Leg::Neutral() {
  Leg_PumpOn();
  sv1.attach(sv1.pin);
  sv2.attach(sv2.pin);
  sv3.attach(sv3.pin);
  Serial.print(F("중립"));

  sv1.write(60);
  sv2.write(60);
  sv3.write(60);
  delay(700);
  sv1.detach();
  sv2.detach();
  sv3.detach();
}

void Leg::Forward() {
  Bending12();
  delay(150);
  Bending23();
  delay(150);
  Bending31();
  delay(150);
}

void Leg::Backward() {
  Bending31();
  delay(1500);
  Bending12();
  delay(1500);
}

// -------- Robot --------
Robot::Robot(const int S_PIN[12], const int P_PIN[12], const int speedarray[12])
    : FL(S_PIN[0], S_PIN[1], S_PIN[2], P_PIN[0], P_PIN[1], P_PIN[2], speedarray[0],speedarray[1],speedarray[2]),
      FR(S_PIN[3], S_PIN[4], S_PIN[5], P_PIN[3], P_PIN[4], P_PIN[5],speedarray[3],speedarray[4],speedarray[5]),
      RL(S_PIN[6], S_PIN[7], S_PIN[8], P_PIN[6], P_PIN[7], P_PIN[8],speedarray[6],speedarray[7],speedarray[8]),
      RR(S_PIN[9], S_PIN[10], S_PIN[11], P_PIN[9], P_PIN[10], P_PIN[11],speedarray[9],speedarray[10],speedarray[11]) {}

void Robot::Initialize() {
  FL.Initialize();
  FR.Initialize();
  RL.Initialize();
  RR.Initialize();
}

void Robot::FL_RR_Forward() {
  FL.Bending12(); delay(1500);
  RR.Bending12(); delay(1500);
  FL.Bending23(); delay(1500);
  RR.Bending23(); delay(1500);
  FL.Bending31(); delay(1500);
  RR.Bending31(); delay(1500);
}

void Robot::FR_RL_Forward() {
  FR.Bending12(); delay(1500);
  RL.Bending12(); delay(1500);
  FR.Bending23(); delay(1500);
  RL.Bending23(); delay(1500);
  FR.Bending31(); delay(1500);
  RL.Bending31(); delay(1500);
}

void Robot::Forward()  {
    Serial.println("Robot is going straight");
    FL_RR_Forward();
    FR_RL_Forward();
}
void Robot::Backward() { /* TODO */     }
void Robot::TurnRight(){ /* TODO */ }
void Robot::TurnLeft() { /* TODO */ }

