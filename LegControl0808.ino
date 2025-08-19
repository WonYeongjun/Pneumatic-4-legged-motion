#include <Servo.h>
#include <string.h>

class Pump {
public:
  int pin;  // 펌프 핀 번호
  int tmp;  // on = 1, off =0
  Pump(int pin_) {
    pin = pin_;
    tmp = 0;
  };

  void Initialize() {
    analogWrite(pin, 0);  //초기상태 pump off
    pinMode(pin, OUTPUT);
  }

  int GetTmp() {
    return tmp;
  }


  void PumpOn() {  //pump on
    if (GetTmp() == 0) {
      tmp = 1;
      analogWrite(pin, 255);
      Serial.println("Pump On");
    } else {
      Serial.println("Already On");
    }
  }


  void PumpOff() {  //pump off
    if (GetTmp() == 1) {
      tmp = 0;
      analogWrite(pin, 0);
      Serial.println("Pump Off");
    } else {
      Serial.println("Already Off");
    }
  }

  void Switch() {  // on to off OR off to on
    if (GetTmp() == 0) {
      PumpOn();
    } else {
      PumpOff();
    }
  }

  void TestOff() { // pump shutdown
    analogWrite(pin, 0);
    // Serial.println("Pump Shutdown");
    tmp = 0;
  }
};


class MyServo : public Servo {
public:
  int pin;    // Servo 핀번호
  int angle;  // Servo 각도

  MyServo(int p) {  //Constructor
    pin = p;
    angle = 0;
  }

  void Initialize() {
    attach(pin);
    write(0);
    delay(2000);  // delay는 setup에 들어가지 않도록 주의
    detach();
  }

  void Extension(int n) {
    for (int i = 0; i < 130 / n; i++) {
      if (angle == 130) {
        Serial.println("already extended");
        break;
      }
      angle += n;
      write(angle);
      delay(50);
    }
  }


  void Contraction(int n) {  //Extension 과 유사
    for (int i = 0; i < 130 / n; i++) {
      if (angle == 0) {
        Serial.println("already contracted");
        break;
      }
      angle -= n;
      write(angle);
      delay(50);
    }
  }
};

class Leg {  //Servo 3개 Pump 3개로 이루어진 leg class
public:
  MyServo sv1, sv2, sv3;
  Pump pump1, pump2, pump3;
  int unit_angle = 10;
  Leg(int s_pin1, int s_pin2, int s_pin3, int p_pin1, int p_pin2, int p_pin3)
    : sv1(s_pin1), sv2(s_pin2), sv3(s_pin3), pump1(p_pin1), pump2(p_pin2), pump3(p_pin3) {
  }

  void Leg_PumpOn() {
    pump1.PumpOn();
    pump2.PumpOn();
    pump3.PumpOn();
  }

  void Initialize() {
    sv1.Initialize();
    sv2.Initialize();
    sv3.Initialize();
    pump1.Initialize();
    pump2.Initialize();
    pump3.Initialize();
  }


  void Bending12() {  // 1, 2번 튜브 방향으로 굽어짐
    Leg_PumpOn();
    Serial.println("Bend to 12");

    sv3.attach(sv3.pin);  // Extension 먼저
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

  void Bending23() 
    Leg_PumpOn();
    Serial.println("Bend to 23");

    sv1.attach(sv1.pin);
    sv1.Extension(unit_angle);
    delay(150);
    sv1.detach();

    sv2.attach(sv2.pin);
    sv2.Contraction(unit_angle);
    delay(150);
    sv2.detach();

    sv3.attach(sv3.pin);  // 2번 펌프 수축
    sv3.Contraction(unit_angle);
    delay(150);  // 3번 펌프 팽창
    sv3.detach();
  }

  void Bending31() {  // 2, 3번 튜브 방향으로 굽어짐.
    Leg_PumpOn();
    Serial.println("Bend to 31");

    sv2.attach(sv2.pin);
    sv2.Extension(unit_angle);
    delay(150);
    sv2.detach();
    
    sv1.attach(sv1.pin);
    sv1.Contraction(unit_angle);
    delay(150);
    sv1.detach();

    sv3.attach(sv3.pin);  // 2번 펌프 수축
    sv3.Contraction(unit_angle);
    delay(150);  // 3번 펌프 팽창
    sv3.detach();
  }


  void Extension() {  // 모든 튜브 팽창
    Leg_PumpOn();
    Serial.println("팽창");

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
  }

  void Contraction() {
    Leg_PumpOn();
    Serial.println("수축");

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
  }

  void Neutral() {  // 중립 상태
    Leg_PumpOn();
    sv1.attach(sv1.pin);
    sv2.attach(sv2.pin);
    sv3.attach(sv3.pin);
    Serial.print("중립");

    sv1.write(60);
    sv2.write(60);
    sv3.write(60);
    delay(700);

    sv1.detach();
    sv2.detach();
    sv3.detach();
  }



  void Forward() {  //예시
      Bending12();
      //Neutral();
      delay(1500);
      Bending23();
      // Neutral();
      delay(1500);
      Bending31();
      // Neutral();
      delay(1500);
  }

  void Backward() {  //예시
    Bending31();
    // Neutral();
    delay(1500);
    // Neutral();
    delay(1500);
    Bending12();
    // Neutral();
    delay(1500);
    Bending23();
    delay(1500);
  }

  void TestOff() {
    pump1.TestOff();
    pump2.TestOff();
    pump3.TestOff();
    Serial.println("Pumps Shutdown");
  }
};

class Robot {
public:
  Leg FL, FR, RL, RR;  // Front Left, Front Right, Rear Left, Rear Right
  int tmp;

  Robot(const int S_PIN[12], const int P_PIN[12])
    : FL(S_PIN[0], S_PIN[1], S_PIN[2], P_PIN[0], P_PIN[1], P_PIN[2]),
      FR(S_PIN[3], S_PIN[4], S_PIN[5], P_PIN[3], P_PIN[4], P_PIN[5]),
      RL(S_PIN[6], S_PIN[7], S_PIN[8], P_PIN[6], P_PIN[7], P_PIN[8]),
      RR(S_PIN[9], S_PIN[10], S_PIN[11], P_PIN[9], P_PIN[10], P_PIN[11]) {}

  void Initialize() {
    FL.Initialize();
    FR.Initialize();
    RL.Initialize();
    RR.Initialize();
  }

  void Forward() {  // 예시
      FL.Forward();
      FR.Forward();
      RL.Neutral();
      RR.Neutral();
      delay(500);
      FL.Neutral();
      FR.Neutral();
      RL.Forward();
      RR.Forward();
      delay(500);
  }

  void Backward() {
  }

  void TurnRight() {
  }

  void TurnLeft() {
  }
};





int s1 =24, s2 =25, s3 =26;
int s4 =28, s5 =29, s6 =30;
int s7 =32, s8 =33, s9 =34;
// int s10 = , s11 = , s12 = ;

int p1 =A0, p2 =A2, p3 =A4;
int p4 =A6, p5 =A8, p6 =A10;
int p7 =2, p8 =4, p9 =6;
// int p10 =, p11 = , p12 = ;


Leg l1(s1, s2, s3, p1, p2, p3);
Leg l2(s4, s5, s6, p4, p5, p6);
Leg l3(s7, s8, s9, p7, p8, p9);


void setup() {
  Serial.begin(9600);
  l1.Initialize();
  l2.Initialize();
  l3.Initialize();
}

void loop() {
  String input = Serial.readStringUntil('\n');  // character 변수, 알파벳 혹은 숫자 하나만 인식
  input.trim();

  if (input == "p") {
    Serial.println("123 On/Off");
    l1.pump1.Switch();
    l1.pump2.Switch();
    l1.pump3.Switch();
  }

  if (input == "1") {
    l1.pump1.Switch();
  }

  if (input == "2") {
    l1.pump2.Switch();
  }

  if (input == "3") {
    l1.pump3.Switch();
  }

  if (input == "a") {
    l1.Extension();
    l2.Extension();
    l3.Extension();
  }

  if (input == "d") {
    l1.Contraction();
    l2.Contraction();
    l3.Contraction();
  }

  if (input == "s") {
    l1.Neutral();
    l2.Neutral();
    l3.Neutral();
  }

  if (input == "12") {
    l1.Bending12();
  }
  if (input == "23") {
    l1.Bending23();
  }
  if (input == "31") {
    l1.Bending31();
  }


  if (input == "Forward") {
    while(true) {
      l1.Forward();
      delay(1500);
      l2.Forward();
      delay(1500);
      l3.Forward();
      delay(1500);

      if (Serial.available() > 0) {
        String innerCommand = Serial.readStringUntil('\n');
        innerCommand.trim();

        if (innerCommand == "Stop") {
          Serial.println("break");
          break;
        }
      }
    }
  }


  if (input == "Backward") {
    while(true) {
      l1.Backward();
      delay(1500);
      l2.Backward();
      delay(1500);
      l3.Backward();
      delay(1500);

      if (Serial.available() > 0) {
        String innerCommand = Serial.readStringUntil('\n');
        innerCommand.trim();

        if (innerCommand == "Stop") {
          Serial.println("break");
          break;
        }
      }
    }
  }


  if (input == "Off") { // 펌프 급하게 끌 때
    l1.TestOff();
    l2.TestOff();
    l3.TestOff();
  }
}
