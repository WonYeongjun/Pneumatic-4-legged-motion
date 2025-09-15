
// RobotLib.cpp
#include "RobotLib.h"

// -------- Joy --------
Joy::Joy(int X_, int Y_, int joy_, int ctrx_, int ctry_) : X(X_), Y(Y_), joy(joy_), newx(0), newy(0), ctrx(ctrx_), ctry(ctry_) {}
void Joy::Initialize() {
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  pinMode(joy, INPUT);
  pos = 0; //0: extension, 1: contraction
}
void Joy::get_input() {
  newx = analogRead(X);
  newy = analogRead(Y);
  if (digitalRead(joy) == HIGH and button_attached==0) {
    pos = 1 - pos;
    button_attached=1;
  }
  if (button_attached==1 and digitalRead(joy) == LOW){
    button_attached=0;
  }
}

void Joy::cal_angle() {
  int dx = newx - ctrx;
  int dy = newy - ctry;

  angle = atan2(dy, dx);
  if (pos==1){
    angle-=M_PI;
  }
  if (angle<0){
    angle = angle + 2*M_PI;
  }
  angle=angle*180/M_PI;
}

void Joy::cal_length() {
  long dx = newx - ctrx;
  long dy = newy - ctry;
  
  long leng = sqrt(pow(dx,2)+pow(dy,2));
  Serial.println(leng);
  length=leng*0.7;
  Serial.println(length);
}

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
Leg::Leg(int s_pin1, int s_pin2, int s_pin3, int p_pin1, int p_pin2, int p_pin3, int speed1, int speed2, int speed3, int X_, int Y_, int joy_, int ctrx_, int ctry_)
: sv1(s_pin1), sv2(s_pin2), sv3(s_pin3), pump1(p_pin1, speed1), pump2(p_pin2, speed2), pump3(p_pin3, speed3), joy(X_, Y_, joy_, ctrx_, ctry_) {}

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
void Leg::Leg_simultaneous_extention() {
  sv1.attach(sv1.pin);
  sv2.attach(sv2.pin);
  sv3.attach(sv3.pin);
  for (int i = 0; i < 130 / 10; i++) {
    if (sv1.angle == 130) {
      Serial.println(F("already extended"));
      break;
    }
    sv1.angle += 10;
    sv1.write(sv1.angle);
    sv2.write(sv1.angle);
    sv3.write(sv1.angle);
    delay(150);
  }
  sv1.detach();
  sv2.detach();
  sv3.detach();
}

void Leg::Leg_simultaneous_contraction() {
  sv1.attach(sv1.pin);
  sv2.attach(sv2.pin);
  sv3.attach(sv3.pin);
  for (int i = 0; i < 130 / 10; i++) {
    if (sv1.angle == 0) {
      Serial.println(F("already contracted"));
      break;
    }
    sv1.angle -= 10;
    sv1.write(sv1.angle);
    sv2.write(sv1.angle);
    sv3.write(sv1.angle);
    delay(150);
  }
}

void Leg::Initialize() {
  sv1.Initialize();
  sv2.Initialize();
  sv3.Initialize();
  pump1.Initialize();
  pump2.Initialize();
  pump3.Initialize();
  joy.Initialize();
}

void Leg::Bending12() {
  Leg_PumpOff();
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

  Leg_PumpOn();
}

void Leg::Bending23() {
  Leg_PumpOff();
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
  Leg_PumpOn();
}

void Leg::Bending31() {
  Leg_PumpOff();
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

  Leg_PumpOn();

}

void Leg::Extension() {
  // Leg_PumpOn();
  // Serial.println(F("팽창"));

  Leg_PumpOff();

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
  Serial.println(F("팽창"));
}

void Leg::Contraction() {
  // Leg_PumpOn();
  // Serial.println(F("수축"));

  Leg_PumpOff();

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
  Serial.println(F("수축"));
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
  // Contraction();
  Bending12();
  delay(2000);

  sv1.attach(sv1.pin);
  sv1.Extension(unit_angle);
  delay(150);
  sv1.detach();

  sv2.attach(sv2.pin);
  sv2.Extension(unit_angle);
  delay(150);
  sv2.detach();

  // sv3.attach(sv3.pin);
  // sv3.Extension(unit_angle);
  // delay(150);
  // sv3.detach();
  // delay(100);
  
  Leg_PumpOff();
  delay(1000);

  // sv2.attach(sv3.pin);
  // sv2.Extension(unit_angle);
  // delay(150);
  // sv2.detach();

  sv3.attach(sv3.pin);
  sv3.Contraction(unit_angle);
  delay(150);
  sv3.detach();


  // pump1.speed=150;
  // pump1.PumpOn();
  pump2.PumpOn();
  pump3.PumpOn();

  delay(4000);

  // pump1.PumpOff();
  pump2.PumpOff();
  pump3.PumpOff();
  delay(2000);

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

  delay(1000);
}

void Leg::Backward() {
  Bending31();
  delay(400);
  Bending23();
  delay(150);
  Bending12();
  delay(400);
  Bending31();
  delay(400);
  Contraction();
  delay(150);
}

void Leg::Standing() {
  Contraction();
  Leg_PumpOff();
 

  Leg_PumpOn();
  delay(2000);
  Leg_PumpOff();
  

  Bending23();
  Leg_PumpOff();
  
  Leg_PumpOn();
  delay(1000);
  Leg_PumpOff();
}
void Leg::Leg_joystick_control() {
  if (joy.pos==0 and joy.button_attached==1){
    Extension();
  }
  else if (joy.pos==1 and joy.button_attached==1){
    Contraction();
  }
  joy.get_input();
  Serial.print("Button ");
  Serial.println(joy.button_attached);
  Serial.print(joy.newx);
  Serial.print(", ");
  Serial.println(joy.newy);
  joy.cal_angle();
  joy.cal_length();
  Serial.print("Angle : ");
  Serial.print(joy.angle);
  Serial.print(", Length : ");
  Serial.println(joy.length);
  angle_to_rel_state();

  Serial.print("pump2.speed: ");
  Serial.println(pump2.speed);
  pump1.Intensity((int)state[0]);
  pump2.Intensity((int)state[1]);
  pump3.Intensity((int)state[2]);
  // Serial.print("pump2.speed: ");
  // Serial.println(pump2.speed);

}

void Leg::angle_to_rel_state() {
  float rel_angle, low, high, low_tmp, high_tmp;

  if (30 <= joy.angle && joy.angle < 150) { // A : 1 , 2
    rel_angle = (joy.angle - 30) * M_PI / 180.0;
    low_tmp = cos(rel_angle) + sin(rel_angle) / sqrt(3);
    high_tmp = 2 * sin(rel_angle) / sqrt(3);
    low = low_tmp / (low_tmp + high_tmp);
    high = high_tmp / (low_tmp + high_tmp);
    Serial.println(high);
    state[0] = 0;
    state[1] = low * joy.length *(pump2.speed-minspeed)/pump2.speed + minspeed;
    Serial.println(low * joy.length *(pump2.speed-minspeed)+minspeed);
    state[2] = high* joy.length *(pump3.speed-minspeed)/pump3.speed + minspeed;
  } 
  else if (150 <= joy.angle && joy.angle < 270) { // B : 2 , 0
    rel_angle = (joy.angle - 150) * M_PI / 180.0;
    low_tmp = cos(rel_angle) + sin(rel_angle) / sqrt(3);
    high_tmp = 2 * sin(rel_angle) / sqrt(3);
    low = low_tmp / (low_tmp + high_tmp);
    high = high_tmp / (low_tmp + high_tmp);
    state[1] = 0;
    state[2] = low * joy.length *(pump3.speed-minspeed)/pump3.speed + minspeed;
    state[0] = high* joy.length *(pump1.speed-minspeed)/pump1.speed + minspeed;
  } 
  else { // C : 0 , 1
    rel_angle = (joy.angle - 270) * M_PI / 180.0;
    low_tmp = cos(rel_angle) + sin(rel_angle) / sqrt(3);
    high_tmp = 2 * sin(rel_angle) / sqrt(3);
    low = low_tmp / (low_tmp + high_tmp);
    high = high_tmp / (low_tmp + high_tmp);
    state[2] = 0;
    state[0] = low * joy.length *(pump1.speed-minspeed)/pump1.speed + minspeed;
    state[1] = high* joy.length *(pump2.speed-minspeed)/pump2.speed + minspeed;
    }
  Serial.println(pump2.speed);
  Serial.print("state : ");
  Serial.print((int)state[0]);
  Serial.print(", ");
  Serial.print((int)state[1]);
  Serial.print(", ");
  Serial.println((int)state[2]);
  Serial.println(" ");
}
// -------- Robot --------
Robot::Robot(const int S_PIN[12], const int P_PIN[12], const int speedarray[12], const int joystickarray[12])
    : A(S_PIN[0], S_PIN[1], S_PIN[2], P_PIN[0], P_PIN[1], P_PIN[2], speedarray[0],speedarray[1],speedarray[2], joystickarray[0], joystickarray[1], joystickarray[2]),
      B(S_PIN[3], S_PIN[4], S_PIN[5], P_PIN[3], P_PIN[4], P_PIN[5],speedarray[3],speedarray[4],speedarray[5], joystickarray[3], joystickarray[4], joystickarray[5]),
      C(S_PIN[6], S_PIN[7], S_PIN[8], P_PIN[6], P_PIN[7], P_PIN[8],speedarray[6],speedarray[7],speedarray[8], joystickarray[6], joystickarray[7], joystickarray[8]),
      D(S_PIN[9], S_PIN[10], S_PIN[11], P_PIN[9], P_PIN[10], P_PIN[11],speedarray[9],speedarray[10],speedarray[11], joystickarray[9], joystickarray[10], joystickarray[11]) {}

      

void Robot::Initialize() {
  A.Initialize();
  B.Initialize();
  C.Initialize();
  D.Initialize();
}

void Robot::Standing() {
  A.Contraction();
  A.Leg_PumpOff();
  B.Contraction();
  B.Leg_PumpOff();
  C.Contraction();
  C.Leg_PumpOff();
  D.Contraction();
  D.Leg_PumpOff();
  
  A.Leg_PumpOn();
  B.Leg_PumpOn();
  C.Leg_PumpOn();
  D.Leg_PumpOn();
  delay(2000);
  A.Leg_PumpOff();
  B.Leg_PumpOff();
  C.Leg_PumpOff();
  D.Leg_PumpOff();

  A.Bending23();
  A.Leg_PumpOff();
  B.Bending23();
  B.Leg_PumpOff();
  C.Bending23();
  C.Leg_PumpOff();
  D.Bending23();
  D.Leg_PumpOff();

  A.Leg_PumpOn();
  B.Leg_PumpOn();
  C.Leg_PumpOn();
  D.Leg_PumpOn();
  delay(5000);
  A.Leg_PumpOff();
  B.Leg_PumpOff();
  C.Leg_PumpOff();
  D.Leg_PumpOff();

}

void Robot::AB_Forward() {
  int A_temp[3] = {A.pump1.speed, A.pump2.speed, A.pump3.speed};
  int B_temp[3] = {B.pump1.speed, B.pump2.speed, B.pump3.speed};

  B.pump1.speed = 130;
  B.pump2.speed = 130;
  B.pump3.speed = 130;
  B.Standing();

  A.Extension();
  delay(800);
  
  A.Bending12();
  delay(2000);
  B.Leg_PumpOn();
  delay(500);
  B.Leg_PumpOff();


  A.Bending23();
  delay(2000);
  B.Leg_PumpOn();
  delay(500);
  B.Leg_PumpOff();


  A.Bending31();
  delay(2000);
  B.Leg_PumpOn();
  delay(500);
  B.Leg_PumpOff();


  A.Bending12();
  delay(2000);
  B.Leg_PumpOn();
  delay(500);
  B.Leg_PumpOff();

  A.Contraction();

  B.pump1.speed = B_temp[0];
  B.pump2.speed = B_temp[1];
  B.pump3.speed = B_temp[2];

  A.pump1.speed = 130;
  A.pump2.speed = 130;
  A.pump3.speed = 130;
  A.Standing();

  B.Extension();
  delay(500);
  
  B.Bending12();
  delay(2000);
  A.Leg_PumpOn();
  delay(500);
  A.Leg_PumpOff();

  B.Bending23();
  delay(2000);
  A.Leg_PumpOn();
  delay(500);
  A.Leg_PumpOff();


  B.Bending31();
  delay(2000);
  A.Leg_PumpOn();
  delay(500);
  A.Leg_PumpOff();


  B.Bending12();
  delay(2000);
  A.Leg_PumpOn();
  delay(500);
  A.Leg_PumpOff();

  B.Contraction();

  A.pump1.speed = A_temp[0];
  A.pump2.speed = A_temp[1];
  A.pump3.speed = A_temp[2];

}

//void Robot::FR_RL_Forward() {
//
//}

void Robot::Forward()  {
  Standing();
  A.Forward();
}
void Robot::Backward() {
  Standing();
  A.Extension();
  A.Leg_PumpOff();
  C.Contraction();
  A.Leg_PumpOn();
  delay(1000);
  A.Leg_PumpOff();
  C.Leg_PumpOff();

  A.Contraction();
  A.Bending31();
  delay(1000);
  A.Leg_PumpOff();
  A.sv1.Extension(unit_angle);
  A.sv2.Contraction(unit_angle);
  A.sv3.Extension(unit_angle);
  A.Leg_PumpOn();
  A.pump1.PumpOff();
  delay(1000);

}
void Robot::TurnRight(){ /* TODO */ }
void Robot::TurnLeft() { /* TODO */ }


