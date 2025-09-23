
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
  if (pin!=7){
    attach(pin);
    write(0);
    delay(2000);
    detach();
  }
}

void MyServo::Extension(int n) {
  if (pin!=7) {
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
  else {
    if (servopos==1){
      servopos=0; //0이 extension, 1이 contraction
      write(180);
      delay(400); //작동시간 조절 필요
      write(90);
    }
    else {
      Serial.println(F("already extended"));
    }
  }
}

void MyServo::Contraction(int n) {
  if (pin!=7) {
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
  else {
    if (servopos==0){
      servopos=1; //0이 extension, 1이 contraction
      write(0);
      delay(365); //작동시간 조절 필요
      write(95);
    }
    else {
      Serial.println(F("already contracted"));
    }
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
void Leg::Bending1() {
    Leg_PumpOff();
    Serial.println(F("Bend to 1"));
    
    sv1.attach(sv1.pin);
    sv1.Contraction(unit_angle);
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

void Leg::Bending2() {
    Leg_PumpOff();
    Serial.println(F("Bend to 2"));
    
    sv2.attach(sv2.pin);
    sv2.Contraction(unit_angle);
    delay(150);
    sv2.detach();
    
    sv3.attach(sv3.pin);
    sv3.Extension(unit_angle);
    delay(150);
    sv3.detach();
    
    sv1.attach(sv1.pin);
    sv1.Extension(unit_angle);
    delay(150);
    sv1.detach();

    


    Leg_PumpOn();
}

void Leg::Bending3() {
    Leg_PumpOff();
    Serial.println(F("Bend to 3"));
    
    sv3.attach(sv3.pin);
    sv3.Contraction(unit_angle);
    delay(150);
    sv3.detach();
    
    sv1.attach(sv1.pin);
    sv1.Extension(unit_angle);
    delay(150);
    sv1.detach();

    sv2.attach(sv2.pin);
    sv2.Extension(unit_angle);
    delay(150);
    sv2.detach();




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

void Leg::Extension2() {
  // Leg_PumpOn();
  // Serial.println(F("팽창"));

  Leg_PumpOff();

  sv1.attach(sv1.pin);
  sv1.Extension(unit_angle);
  delay(150);
  sv1.detach();

  sv3.attach(sv3.pin);
  sv3.Extension(unit_angle);
  delay(150);
  sv3.detach();
    
    sv2.attach(sv2.pin);
    sv2.Extension(unit_angle);
    delay(150);
    sv2.detach();

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
  Contraction();
  Bending12();
  delay(1500);
    
    Bending23();
    delay(1000);
    
Leg_PumpOff();



  sv2.attach(sv2.pin);
  sv2.Extension(unit_angle);
  delay(150);
  sv2.detach();
  Leg_PumpOn();
  delay(1200);
    
    

    
  Bending3();
  delay(500);
    
    pump1.PumpOff();
  pump2.PumpOff();
  pump3.PumpOff();
  delay(1000);
    
    
    
    Bending1();
    delay(200);

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

//void Leg::Forward() {
//    Contraction();
//    delay(300);
//    
//    pump1.Intensity(pump1.speed-50);
//    
//    Bending12();
//    delay(400);
//    
//    pump1.Intensity(pump1.speed);
//    
//    sv1.attach(sv1.pin);
//    sv1.Extension(unit_angle);
//    delay(150);
//    sv1.detach();
//    
//    sv2.attach(sv2.pin);
//    sv2.Extension(unit_angle);
//    delay(150);
//    sv2.detach();
//    
//    delay(200);
//    
//    Bending23();
//    delay(300);
//    
//    pump3.Intensity(pump3.speed-50);
//    
//    Bending31();
//    delay(2200);
//    
//    pump3.Intensity(pump3.speed);
//    
//    pump1.PumpOff();
//    pump2.PumpOff();
//    pump3.PumpOff();
//    delay(2000);
//
//    sv1.attach(sv1.pin);
//    sv1.Contraction(unit_angle);
//    delay(150);
//    sv1.detach();
//    
//    sv2.attach(sv2.pin);
//    sv2.Contraction(unit_angle);
//    delay(150);
//    sv2.detach();
//
//    sv3.attach(sv3.pin);
//    sv3.Contraction(unit_angle);
//    delay(150);
//    sv3.detach();
//
//    Leg_PumpOn();
// 
//    delay(1000);
//    
//    
//    
//    
//  
//}


void Leg::Backward() {
    Contraction();
    Bending31();
    delay(2500);
      
      Bending23();
      delay(1000);
      
  Leg_PumpOff();

    sv3.attach(sv3.pin);
    sv3.Extension(unit_angle);
    delay(150);
    sv3.detach();
    Leg_PumpOn();
    delay(1200);
    
    Bending2();
    delay(500);
      
      pump1.PumpOff();
    pump2.PumpOff();
    pump3.PumpOff();
    delay(1000);
      
      
      
      Bending1();
      delay(200);


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

// -------- Robot --------
Robot::Robot(const int S_PIN[9], const int P_PIN[9], const int speedarray[9])
    : A(S_PIN[0], S_PIN[1], S_PIN[2], P_PIN[0], P_PIN[1], P_PIN[2], speedarray[0],speedarray[1],speedarray[2]),
      B(S_PIN[3], S_PIN[4], S_PIN[5], P_PIN[3], P_PIN[4], P_PIN[5],speedarray[3],speedarray[4],speedarray[5]),
      C(S_PIN[6], S_PIN[7], S_PIN[8], P_PIN[6], P_PIN[7], P_PIN[8],speedarray[6],speedarray[7],speedarray[8]),
      D(S_PIN[9], S_PIN[10], S_PIN[11], P_PIN[9], P_PIN[10], P_PIN[11],speedarray[9],speedarray[10],speedarray[11]){}

      

void Robot::Initialize() {
  A.Initialize();
  B.Initialize();
  C.Initialize();
}

void Robot::AB_Forward() {
//    Standing();
//    delay(100);
//    B.Bending23();
//    delay(1000);
//    B.Extension();
//    delay(1000);
//    B.Leg_PumpOff();
//    
//    D.Bending23();
//    delay(1000);
//    D.Extension();
//    delay(1000);
//    
//    C.Contraction();
//    delay(2000);
//    
//  
//    A.Forward();
//    
//    Standing();
//    delay(100);
//    
//    A.Extension();
//    delay(800);
//    A.Leg_PumpOff();
//    
//    
//    B.Backward();
//    Standing();
//    delay(100);
    
    
    A.Leg_PumpOff();
    B.Leg_PumpOff();
    C.Leg_PumpOff();
    D.Leg_PumpOff();
    
    
    // A 수축
    A.sv1.attach(A.sv1.pin);
    A.sv1.Contraction(A.unit_angle);
    delay(150);
    A.sv1.detach();

    A.sv2.attach(A.sv2.pin);
    A.sv2.Contraction(A.unit_angle);
    delay(150);
    A.sv2.detach();

    A.sv3.attach(A.sv3.pin);
    A.sv3.Contraction(A.unit_angle);
    delay(150);
    A.sv3.detach();

    // B 수축
    B.sv1.attach(B.sv1.pin);
    B.sv1.Contraction(B.unit_angle);
    delay(150);
    B.sv1.detach();

    B.sv2.attach(B.sv2.pin);
    B.sv2.Contraction(B.unit_angle);
    delay(150);
    B.sv2.detach();

    B.sv3.attach(B.sv3.pin);
    B.sv3.Contraction(B.unit_angle);
    delay(150);
    B.sv3.detach();
    
    //C 수축
    
    C.sv1.attach(C.sv1.pin);
    C.sv1.Contraction(C.unit_angle);
    delay(150);
    C.sv1.detach();

    C.sv2.attach(C.sv2.pin);
    C.sv2.Contraction(C.unit_angle);
    delay(150);
    C.sv2.detach();

    C.sv3.attach(C.sv3.pin);
    C.sv3.Contraction(C.unit_angle);
    delay(150);
    C.sv3.detach();
    
    //D 수축
    
    D.sv1.attach(D.sv1.pin);
    D.sv1.Contraction(D.unit_angle);
    delay(150);
    D.sv1.detach();

    D.sv2.attach(D.sv2.pin);
    D.sv2.Contraction(D.unit_angle);
    delay(150);
    D.sv2.detach();

    D.sv3.attach(D.sv3.pin);
    D.sv3.Contraction(D.unit_angle);
    delay(150);
    D.sv3.detach();
    
    //
    
    A.Leg_PumpOn();
    B.Leg_PumpOn();
    C.Leg_PumpOn();
    D.Leg_PumpOn();
    
    delay(10000);
//    A.Leg_PumpOff();
    
    
    C.Bending23();
    delay(1000);
    C.Leg_PumpOff();
    //
    A.Bending1();
    delay(150);
    
    A.sv1.attach(A.sv1.pin);
    A.sv1.Extension(A.unit_angle);
    delay(150);
    A.sv1.detach();
    delay(500);
    
    
    
    A.Leg_PumpOff();
    
    
    B.Extension();
    delay(600);
    
    //잘 될 때 : 800
    
    B.Leg_PumpOff();
    
    //new
    
//    A.Extension();
//    delay(600);
    
    A.Contraction();
    delay(1200);
    
    A.Leg_PumpOff();
    
    
    
    A.Bending2();
    delay(1500);
    
    A.Extension();
    delay(1800);
    
    // 원래 딜레이 1200
    
    
//    A.Leg_PumpOff();
////    
////    B.Contraction();
////    delay(800);
//    
    
    
    A.Contraction();
    delay(500);
    
//    B.Leg_PumpOff();
//    D.Leg_PumpOff();
    
    A.Bending31();
    A.pump1.PumpOff();
    delay(2000);
    
    
//    A.Contraction();
//    delay(3000);
//    
//    A.Bending2();
//    delay(1000);
//    
//    A.Extension();
//    delay(1500);
//    
//    
//    
//    A.Contraction();
//    delay(500);
//    A.Bending31();
//    delay(2000);
    
    
    A.Contraction();
    B.Contraction();
    C.Contraction();
    D.Contraction();
    delay(3000);
    
    
    

}


void Robot::Forward()  {
}
void Robot::Backward() {
    // A 수축
    A.sv1.attach(A.sv1.pin);
    A.sv1.Contraction(A.unit_angle);
    delay(150);
    A.sv1.detach();

    A.sv2.attach(A.sv2.pin);
    A.sv2.Contraction(A.unit_angle);
    delay(150);
    A.sv2.detach();

    A.sv3.attach(A.sv3.pin);
    A.sv3.Contraction(A.unit_angle);
    delay(150);
    A.sv3.detach();

    // B 수축
    B.sv1.attach(B.sv1.pin);
    B.sv1.Contraction(B.unit_angle);
    delay(150);
    B.sv1.detach();

    B.sv2.attach(B.sv2.pin);
    B.sv2.Contraction(B.unit_angle);
    delay(150);
    B.sv2.detach();

    B.sv3.attach(B.sv3.pin);
    B.sv3.Contraction(B.unit_angle);
    delay(150);
    B.sv3.detach();
    
    //C 수축
    
    C.sv1.attach(C.sv1.pin);
    C.sv1.Contraction(C.unit_angle);
    delay(150);
    C.sv1.detach();

    C.sv2.attach(C.sv2.pin);
    C.sv2.Contraction(C.unit_angle);
    delay(150);
    C.sv2.detach();

    C.sv3.attach(C.sv3.pin);
    C.sv3.Contraction(C.unit_angle);
    delay(150);
    C.sv3.detach();
    
    //D 수축
    
    D.sv1.attach(D.sv1.pin);
    D.sv1.Contraction(D.unit_angle);
    delay(150);
    D.sv1.detach();

    D.sv2.attach(D.sv2.pin);
    D.sv2.Contraction(D.unit_angle);
    delay(150);
    D.sv2.detach();

    D.sv3.attach(D.sv3.pin);
    D.sv3.Contraction(D.unit_angle);
    delay(150);
    D.sv3.detach();
    
    //
    
    A.Leg_PumpOn();
    B.Leg_PumpOn();
    C.Leg_PumpOn();
    D.Leg_PumpOn();
    
    delay(10000);
//    A.Leg_PumpOff();
    
    
    
    A.Leg_PumpOff();
    
    A.sv1.attach(A.sv1.pin);
    A.sv1.Extension(A.unit_angle);
    delay(150);
    A.sv1.detach();
    delay(500);
    
    A.pump1.PumpOn();
    delay(2500);
    
    
    A.Leg_PumpOff();
    
    
    
    
//    A.Bending23();
//    delay(1800);
//    A.Leg_PumpOff();
    
    // new
    C.Bending1();
    delay(200);
    
    C.sv1.attach(C.sv1.pin);
    C.sv1.Extension(C.unit_angle);
    delay(150);
    C.sv1.detach();
    delay(200);
    
    
    
    C.Leg_PumpOff();
    
    
    B.Extension2();
    delay(600);
    

    
    B.Leg_PumpOff();
    
    
    C.Contraction();
    delay(1200);
    
    A.Leg_PumpOff();
    
    
    
    C.Bending3();
    delay(1000);
    
    C.Extension2();
    delay(1500);
    
    C.sv1.attach(C.sv1.pin);
    C.sv1.Contraction(C.unit_angle);
    delay(150);
    C.sv1.detach();
    delay(200);
    
    // 원래 딜레이 1200

    
    C.Contraction();
    delay(1200);
    C.Bending12();
    C.pump1.PumpOff();
    delay(1500);
    
    C.Contraction();
    A.Contraction();
    B.Contraction();
    D.Contraction();
    delay(3000);
    
    
}
void Robot::TurnRight(){
    
    A.Leg_PumpOff();
    B.Leg_PumpOff();
    C.Leg_PumpOff();
    D.Leg_PumpOff();
    
    
    // A 수축
    A.sv1.attach(A.sv1.pin);
    A.sv1.Contraction(A.unit_angle);
    delay(150);
    A.sv1.detach();

    A.sv2.attach(A.sv2.pin);
    A.sv2.Contraction(A.unit_angle);
    delay(150);
    A.sv2.detach();

    A.sv3.attach(A.sv3.pin);
    A.sv3.Contraction(A.unit_angle);
    delay(150);
    A.sv3.detach();

    // B 수축
    B.sv1.attach(B.sv1.pin);
    B.sv1.Contraction(B.unit_angle);
    delay(150);
    B.sv1.detach();

    B.sv2.attach(B.sv2.pin);
    B.sv2.Contraction(B.unit_angle);
    delay(150);
    B.sv2.detach();

    B.sv3.attach(B.sv3.pin);
    B.sv3.Contraction(B.unit_angle);
    delay(150);
    B.sv3.detach();
    
    //C 수축
    
    C.sv1.attach(C.sv1.pin);
    C.sv1.Contraction(C.unit_angle);
    delay(150);
    C.sv1.detach();

    C.sv2.attach(C.sv2.pin);
    C.sv2.Contraction(C.unit_angle);
    delay(150);
    C.sv2.detach();

    C.sv3.attach(C.sv3.pin);
    C.sv3.Contraction(C.unit_angle);
    delay(150);
    C.sv3.detach();
    
    //D 수축
    
    D.sv1.attach(D.sv1.pin);
    D.sv1.Contraction(D.unit_angle);
    delay(150);
    D.sv1.detach();

    D.sv2.attach(D.sv2.pin);
    D.sv2.Contraction(D.unit_angle);
    delay(150);
    D.sv2.detach();

    D.sv3.attach(D.sv3.pin);
    D.sv3.Contraction(D.unit_angle);
    delay(150);
    D.sv3.detach();
    
    //
    
    A.Leg_PumpOn();
    B.Leg_PumpOn();
    C.Leg_PumpOn();
    D.Leg_PumpOn();
    
    delay(10000);
//    A.Leg_PumpOff();
    
    
    A.Bending1();
    delay(1500);
    
    A.Leg_PumpOff();
    B.Leg_PumpOff();
    C.Leg_PumpOff();
    D.Leg_PumpOff();
    
    
    // A 수축
    A.sv1.attach(A.sv1.pin);
    A.sv1.Contraction(A.unit_angle);
    delay(150);
    A.sv1.detach();

    A.sv2.attach(A.sv2.pin);
    A.sv2.Contraction(A.unit_angle);
    delay(150);
    A.sv2.detach();

    A.sv3.attach(A.sv3.pin);
    A.sv3.Contraction(A.unit_angle);
    delay(150);
    A.sv3.detach();

    // B 수축
    B.sv1.attach(B.sv1.pin);
    B.sv1.Contraction(B.unit_angle);
    delay(150);
    B.sv1.detach();

    B.sv2.attach(B.sv2.pin);
    B.sv2.Contraction(B.unit_angle);
    delay(150);
    B.sv2.detach();

    B.sv3.attach(B.sv3.pin);
    B.sv3.Contraction(B.unit_angle);
    delay(150);
    B.sv3.detach();
    
    //C 수축
    
    C.sv1.attach(C.sv1.pin);
    C.sv1.Contraction(C.unit_angle);
    delay(150);
    C.sv1.detach();

    C.sv2.attach(C.sv2.pin);
    C.sv2.Contraction(C.unit_angle);
    delay(150);
    C.sv2.detach();

    C.sv3.attach(C.sv3.pin);
    C.sv3.Contraction(C.unit_angle);
    delay(150);
    C.sv3.detach();
    
    //D 수축
    
    D.sv1.attach(D.sv1.pin);
    D.sv1.Contraction(D.unit_angle);
    delay(150);
    D.sv1.detach();

    D.sv2.attach(D.sv2.pin);
    D.sv2.Contraction(D.unit_angle);
    delay(150);
    D.sv2.detach();

    D.sv3.attach(D.sv3.pin);
    D.sv3.Contraction(D.unit_angle);
    delay(150);
    D.sv3.detach();
    
    //
    
    A.Leg_PumpOn();
    B.Leg_PumpOn();
    C.Leg_PumpOn();
    D.Leg_PumpOn();
    
    
    
    
}
void Robot::TurnLeft() {
    A.Leg_PumpOff();
        B.Leg_PumpOff();
        C.Leg_PumpOff();
        D.Leg_PumpOff();
        
        
        // A 수축
        A.sv1.attach(A.sv1.pin);
        A.sv1.Contraction(A.unit_angle);
        delay(150);
        A.sv1.detach();

        A.sv2.attach(A.sv2.pin);
        A.sv2.Contraction(A.unit_angle);
        delay(150);
        A.sv2.detach();

        A.sv3.attach(A.sv3.pin);
        A.sv3.Contraction(A.unit_angle);
        delay(150);
        A.sv3.detach();

        // B 수축
        B.sv1.attach(B.sv1.pin);
        B.sv1.Contraction(B.unit_angle);
        delay(150);
        B.sv1.detach();

        B.sv2.attach(B.sv2.pin);
        B.sv2.Contraction(B.unit_angle);
        delay(150);
        B.sv2.detach();

        B.sv3.attach(B.sv3.pin);
        B.sv3.Contraction(B.unit_angle);
        delay(150);
        B.sv3.detach();
        
        //C 수축
        
        C.sv1.attach(C.sv1.pin);
        C.sv1.Contraction(C.unit_angle);
        delay(150);
        C.sv1.detach();

        C.sv2.attach(C.sv2.pin);
        C.sv2.Contraction(C.unit_angle);
        delay(150);
        C.sv2.detach();

        C.sv3.attach(C.sv3.pin);
        C.sv3.Contraction(C.unit_angle);
        delay(150);
        C.sv3.detach();
        
        //D 수축
        
        D.sv1.attach(D.sv1.pin);
        D.sv1.Contraction(D.unit_angle);
        delay(150);
        D.sv1.detach();

        D.sv2.attach(D.sv2.pin);
        D.sv2.Contraction(D.unit_angle);
        delay(150);
        D.sv2.detach();

        D.sv3.attach(D.sv3.pin);
        D.sv3.Contraction(D.unit_angle);
        delay(150);
        D.sv3.detach();
        
        //

        A.Leg_PumpOn();
        B.Leg_PumpOn();
        C.Leg_PumpOn();
        D.Leg_PumpOn();
        
        delay(10000);
        
        //
        C.Bending23();
        delay(1000);
        C.Leg_PumpOff();
        
        
        B.Extension();
        delay(600);
        
        //잘 될 때 : 800
        
        B.Leg_PumpOff();
        
        
        
        A.Bending1();
        delay(1500);
        
        A.Leg_PumpOff();
        
        
        B.Contraction();
        C.Contraction();
        
        delay(1000);
        
        A.Contraction();
    
}

void Robot::Standing() {
//    A.Contraction();
//    A.Leg_PumpOff();
//    B.Contraction();
//    B.Leg_PumpOff();
//    C.Contraction();
//    C.Leg_PumpOff();
//    D.Contraction();
//    A.Leg_PumpOn();
//    B.Leg_PumpOn();
//    C.Leg_PumpOn();
    
    
    A.Leg_PumpOff();
    B.Leg_PumpOff();
    C.Leg_PumpOff();
    D.Leg_PumpOff();
    
    
    // A 수축
    A.sv1.attach(A.sv1.pin);
    A.sv1.Contraction(A.unit_angle);
    delay(150);
    A.sv1.detach();

    A.sv2.attach(A.sv2.pin);
    A.sv2.Contraction(A.unit_angle);
    delay(150);
    A.sv2.detach();

    A.sv3.attach(A.sv3.pin);
    A.sv3.Contraction(A.unit_angle);
    delay(150);
    A.sv3.detach();

    // B 수축
    B.sv1.attach(B.sv1.pin);
    B.sv1.Contraction(B.unit_angle);
    delay(150);
    B.sv1.detach();

    B.sv2.attach(B.sv2.pin);
    B.sv2.Contraction(B.unit_angle);
    delay(150);
    B.sv2.detach();

    B.sv3.attach(B.sv3.pin);
    B.sv3.Contraction(B.unit_angle);
    delay(150);
    B.sv3.detach();
    
    //C 수축
    
    C.sv1.attach(C.sv1.pin);
    C.sv1.Contraction(C.unit_angle);
    delay(150);
    C.sv1.detach();

    C.sv2.attach(C.sv2.pin);
    C.sv2.Contraction(C.unit_angle);
    delay(150);
    C.sv2.detach();

    C.sv3.attach(C.sv3.pin);
    C.sv3.Contraction(C.unit_angle);
    delay(150);
    C.sv3.detach();
    
    //D 수축
    
    D.sv1.attach(D.sv1.pin);
    D.sv1.Contraction(D.unit_angle);
    delay(150);
    D.sv1.detach();

    D.sv2.attach(D.sv2.pin);
    D.sv2.Contraction(D.unit_angle);
    delay(150);
    D.sv2.detach();

    D.sv3.attach(D.sv3.pin);
    D.sv3.Contraction(D.unit_angle);
    delay(150);
    D.sv3.detach();
    
    //
    
    A.Leg_PumpOn();
    B.Leg_PumpOn();
    C.Leg_PumpOn();
    D.Leg_PumpOn();
    
    delay(10000);
//    A.Leg_PumpOff();
    
    
    C.Bending23();
    delay(1000);
    C.Leg_PumpOff();
    //
    A.Bending1();
    delay(150);
    
    A.sv1.attach(A.sv1.pin);
    A.sv1.Extension(A.unit_angle);
    delay(150);
    A.sv1.detach();
    delay(500);
    
    
    
    A.Leg_PumpOff();
    
    
    B.Extension();
    delay(600);
    
    //잘 될 때 : 800
    
    B.Leg_PumpOff();
    
    //new
    
//    A.Extension();
//    delay(600);
    
    A.Contraction();
    delay(1200);
    
    A.Leg_PumpOff();
    
    
    
    A.Bending2();
    delay(1500);
    
    A.Extension();
    delay(1800);
    
    // 원래 딜레이 1200
    
    
//    A.Leg_PumpOff();
////
////    B.Contraction();
////    delay(800);
//
    
    
    A.Contraction();
    delay(500);
    
//    B.Leg_PumpOff();
//    D.Leg_PumpOff();
    
    A.Bending31();
    A.pump1.PumpOff();
    delay(2000);
    
    
    
    A.Contraction();
    B.Contraction();
    C.Contraction();
    D.Contraction();
    delay(3000);
    
    A.Leg_PumpOff();
    
    A.sv1.attach(A.sv1.pin);
    A.sv1.Extension(A.unit_angle);
    delay(150);
    A.sv1.detach();
    delay(500);
    
    A.pump1.PumpOn();
    delay(2500);
    
    
    A.Leg_PumpOff();
    
    // new
    C.Bending1();
    delay(200);
    
    C.sv1.attach(C.sv1.pin);
    C.sv1.Extension(C.unit_angle);
    delay(150);
    C.sv1.detach();
    delay(200);
    
    
    
    C.Leg_PumpOff();
    
    
    B.Extension2();
    delay(600);
    

    
    B.Leg_PumpOff();
    
    
    C.Contraction();
    delay(1200);
    
    A.Leg_PumpOff();
    
    
    
    C.Bending3();
    delay(1000);
    
    C.Extension2();
    delay(1200);
    
    C.sv1.attach(C.sv1.pin);
    C.sv1.Contraction(C.unit_angle);
    delay(150);
    C.sv1.detach();
    delay(200);
    
    // 원래 딜레이 1200

    
    C.Contraction();
    delay(1200);
    C.Bending12();
    C.pump1.PumpOff();
    delay(1500);
    
    C.Contraction();
    A.Contraction();
    B.Contraction();
    D.Contraction();
    delay(3000);
    
//    // 뽀잉
//    C.Bending23();
//    delay(1000);
//    C.Leg_PumpOff();
//    
//    
//    B.Extension();
//    delay(600);
//    
//    //잘 될 때 : 800
//    
//    B.Leg_PumpOff();
//    
//    
//    
//    A.Bending1();
//    delay(1500);
//    
//    A.Leg_PumpOff();
//    
//    
//    B.Contraction();
//    C.Contraction();
//    
//    delay(1000);
//    
//    A.Contraction();
//    
    
    
    
    
}
