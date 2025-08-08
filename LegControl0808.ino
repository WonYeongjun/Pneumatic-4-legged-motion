  #include <Servo.h>
  
  
  class Pump {
    public :
      int pin; // 펌프 핀 번호
      int tmp; // on = 1, off =0
      Pump(int pin_) {
        pin = pin_;
        tmp = 0;
        analogWrite(pin, 0); //초기상태 pump off
        pinMode(pin, OUTPUT); 
      };

    
      void PumpOn() { //pump on
        if (tmp == 0) {
          tmp = 1;
          analogWrite(pin, 255);
          Serial.println("Pump On");
        }
        else {
          Serial.println("Already On");
        }
      }


      void PumpOff() { //pump off
        if (tmp == 1) {
          tmp = 0;
          analogWrite(pin, 0);
          Serial.println("Pump Off");
        }
        else {
          Serial.println("Already Off");
        }
      }

      void Switch() { // on to off OR off to on
        if (tmp == 0) {
          PumpOn();
        }
        else {
          PumpOff();
        }
      }

  };


  class MyServo : public Servo  {
    public :
      int pin; // Servo 핀번호
      int angle; // Servo 각도

      MyServo(int p) { //Constructor
        pin = p;
        angle = 0;
      } 
      
      void Initialize() {
        attach(pin);
        write(0);
        delay(2000); // setup에 들어가지 않도록 주의
        detach(); 
      }

      void Extension() {
        if (angle != 130) { 
          angle +=10; 
          write(angle); // 밸브 작동 순차적으로 할 시 여기에 딜레이 넣으면 될 듯?
        }

        else {
          Serial.println("Full Extended");
        }
      }


      void Contraction() { //Extension 과 유사
        if (angle != 0) {
          angle -=10;
          write(angle);
        }
        else {
          Serial.println("Full Contracted");
        }
      }  
  };

  class Leg { //Servo 3개 Pump 3개로 이루어진 leg class
    public :
      MyServo sv1, sv2, sv3;
      Pump pump1, pump2, pump3;
      int tmp = pump1.tmp * pump2.tmp * pump3.tmp;
      Leg(int s_pin1, int s_pin2, int s_pin3, int p_pin1, int p_pin2, int p_pin3) :
        sv1(s_pin1), sv2(s_pin2), sv3(s_pin3), pump1(p_pin1), pump2(p_pin2), pump3(p_pin3) {
      }

      void Initialize() {
        sv1.Initialize();
        sv2.Initialize();
        sv3.Initialize();
      }

      bool PumpCheck() {
        if (tmp == 1) {
          return true;
        }
        if (tmp == 0) {
          Serial.println("All pumps should be on");
          return false;
        }
      }

      void Bending12() { // 1, 2번 튜브 방향으로 굽어짐
        if (PumpCheck()) { // 펌프 3개가 모두 켜져있을 때만 작동
          Serial.println("Bend to 12");
          sv1.attach(sv1.pin);
          sv2.attach(sv2.pin);
          sv3.attach(sv3.pin);
         for(int i=0; i<=13; i++) {
          sv1.Contraction(); // 1번 펌프 수축
          sv2.Contraction(); // 2번 펌프 수축
          sv3.Extension();   // 3번 펌프 팽창
          delay(150); // 서보가 위에서 입력한 각도까지 움직이는 시간, 현재는 3개의 서보가 동시에 움직이는 코드
          }
          sv1.detach();
          sv2.detach();
          sv3.detach();
        }
      }

      void Bending23() { // 2, 3번 튜브 방향으로 굽어짐.
        if (PumpCheck()) {
          Serial.println("Bend to 23");
          sv1.attach(sv1.pin);
          sv2.attach(sv2.pin);
          sv3.attach(sv3.pin);
         for(int i=0; i<=13; i++) {
          sv1.Extension();
          sv2.Contraction();
          sv3.Contraction();
          delay(150);
          }
          sv1.detach();
          sv2.detach();
          sv3.detach();
        }
      }

      void Bending31() { // 2, 3번 튜브 방향으로 굽어짐.
        if (PumpCheck()) {
          Serial.println("Bend to 31");
          sv1.attach(sv1.pin);
          sv2.attach(sv2.pin);
          sv3.attach(sv3.pin);
         for(int i=0; i<=13; i++) {
          sv1.Contraction();
          sv2.Extension();
          sv3.Contraction();
          delay(150);
          }
          sv1.detach();
          sv2.detach();
          sv3.detach();
        }
      }

      void Extension() { // 모든 튜브 팽창
        if (PumpCheck()) {
          sv1.attach(sv1.pin);
          sv2.attach(sv2.pin);
          sv3.attach(sv3.pin);
        
          Serial.print("팽창\n");
          for(int i=0; i<=13; i++) {
            sv1.Extension();
            sv2.Extension();
            sv3.Extension();
          // Serial.println(i);
          // Serial.println(current_servo);
            delay(150);
          }
          sv1.detach();
          sv2.detach();
          sv3.detach();
        }
      }

      void Contraction() {
        if (PumpCheck()) {}
          sv1.attach(sv1.pin);
          sv2.attach(sv2.pin);
          sv3.attach(sv3.pin);
          Serial.print("수축\n");
          for(int i=0; i<=13; i++) {
            sv1.Contraction();
            sv2.Contraction();
            sv3.Contraction();
            delay(150);
          }
          sv1.detach();
          sv2.detach();
          sv3.detach();
      }

      void Neutral() { // 중립 상태
        if (PumpCheck()) {
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
      }
      


      void Forward() { //예시
        while (PumpCheck()) {
          Bending12();
          Neutral();
          delay(500);
          Bending23();
          Neutral();
          delay(500);
          Bending31();
          Neutral();
          delay(500);
        }
      } 
      void Backward() { //예시
        while (PumpCheck()) {
          Bending31();
          Neutral();
          delay(500);
          Bending23();
          Neutral();
          delay(500);
          Bending12();
          Neutral();
          delay(500);
        }

      }

      
  };

  class Robot {
    public :
    Leg FL, FR, RL, RR; // Front Left, Front Right, Rear Left, Rear Right
    int tmp = FL.tmp * FR.tmp * RL.tmp * RR.tmp;
    Robot(const int S_PIN[12], const int P_PIN[12]) :
      FL(S_PIN[0], S_PIN[1], S_PIN[2], P_PIN[0], P_PIN[1], P_PIN[2]),
      FR(S_PIN[3], S_PIN[4], S_PIN[5], P_PIN[3], P_PIN[4], P_PIN[5]),
      RL(S_PIN[6], S_PIN[7], S_PIN[8], P_PIN[6], P_PIN[7], P_PIN[8]),
      RR(S_PIN[9], S_PIN[10], S_PIN[11], P_PIN[9], P_PIN[10], P_PIN[11]) {}

    bool PumpCheck() {
        if (tmp == 1) {
          return true;
        }
        if (tmp == 0) {
          Serial.println("All pumps should be on");
          return false;
        }
      }


    void Forward() { // 예시
      while (true) {
        FL.Forward();
        FR.Forward();
        RL.Neutral();
        RR.Neutral();
        delay (500);
        FL.Neutral();
        FR.Neutral();
        RL.Forward();
        RR.Forward();
        delay (500);

      }
    }

    void Backward() {

    }

    void TurnRight() {

    }

    void TurnLeft() {

    }
  };


  


  int s1 =A0; 
  int s2 =A1; 
  int s3 =A2;

  int p1 = 3;
  int p2 = 6;
  int p3 = 9;


  Leg l1(s1, s2, s3, p1, p2, p3);

  // MyServo sv1(s1);
  // MyServo sv2(s2);
  // MyServo sv3(s3);


  void setup() {
    Serial.begin(9600);
    l1.Initialize();
  }

  void loop() {
    char input = Serial.read(); // character 변수, 알파벳 혹은 숫자 하나만 인식
    // Serial.println(current_servo);
    if(input  == 'P'){
      Serial.println("123 on");
      l1.pump1.PumpOn();
      l1.pump2.PumpOn();
      l1.pump3.PumpOn();
    }
    if(input  == 'p'){
      Serial.println("123 off");
      l1.pump1.PumpOff();
      l1.pump2.PumpOff();
      l1.pump3.PumpOff();
    }

    if (input=='1') {
      l1.pump1.Switch();
    }

    if (input=='2') {
      l1.pump2.Switch();
    }

    if (input=='3') {
      l1.pump3.Switch();
    }

    if (input=='a') {
      l1.Extension();
    }

    if (input=='d') {
      l1.Contraction();
    }

      if (input=='m') {
      l1.Neutral();
    }


    if (input == 'q') {
      l1.Bending12();
    }
    
    if (input == 'w') {
      l1.Bending23();
    }
    if (input == 'e') {
      l1.Bending31();
    }
    
  }
