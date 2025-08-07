  #include <Servo.h>
  
  
  class Pump {
    public :
      int pin; //핀 번호
      int tmp; // on = 1, off =0
      Pump(int pin_) {
        pin = pin_;
        tmp = 0;
        analogWrite(pin, 0);
        pinMode(pin, OUTPUT);
      };

    
      void PumpOn() {
        if (tmp == 0) {
          tmp = 1;
          analogWrite(pin, 255);
          Serial.println("Pump On");
        }
        else {
          Serial.println("Already On");
        }
      }
      void PumpOff() {
        if (tmp == 1) {
          tmp = 0;
          analogWrite(pin, 0);
          Serial.println("Pump Off");
        }
        else {
          Serial.println("Already Off");
        }
      }

      void Switch() {
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
      int pin;
      int angle;

      MyServo(int p) {
        pin = p;
        angle = 0;
      } 
      
      void Initialize() {
        attach(pin);
        write(0);
        delay(2000);
        detach();
      }

      void Extension() {
        if (angle != 130) {
          angle +=10;
          write(angle);
        }

        else {
          Serial.println("Full Extended");
        }
      }


      void Contraction() {
        if (angle != 0) {
          angle -=10;
          write(angle);
        }
        else {
          Serial.println("Full Contracted");
        }
      }  
  };

  class Leg {
    public :
      MyServo sv1, sv2, sv3;
      Pump pump1, pump2, pump3;
      Leg(int s_pin1, int s_pin2, int s_pin3, int p_pin1, int p_pin2, int p_pin3) :
        sv1(s_pin1), sv2(s_pin2), sv3(s_pin3), pump1(p_pin1), pump2(p_pin2), pump3(p_pin3) {
      }

      void Initialize() {
        sv1.Initialize();
        sv2.Initialize();
        sv3.Initialize();
      }

      void Bending12() {
        if (pump1.tmp * pump2.tmp * pump3.tmp == 1 ) {
          Serial.println("Bend to 12");
          sv1.attach(sv1.pin);
          sv2.attach(sv2.pin);
          sv3.attach(sv3.pin);
         for(int i=0; i<=13; i++) {
          sv1.Contraction();
          sv2.Contraction();
          sv3.Extension();
          delay(150);
          }
          sv1.detach();
          sv2.detach();
          sv3.detach();
        }
        else {
          Serial.println("All pumps should be on");
        }
      }

      void Bending23() {
        if (pump1.tmp * pump2.tmp * pump3.tmp == 1 ) {
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
        else {
          Serial.println("All pumps should be on");
        }
      }

      void Bending31() {
        if (pump1.tmp * pump2.tmp * pump3.tmp == 1 ) {
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
        else {
          Serial.println("All pumps should be on");
        }
      }

      void Extension() {
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

      void Contraction() {
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

      void Neutral() {
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
      


      void Forward() {
        
      }
      void Backward() {

      }

      
  };

  class Robot {
    public :
    Leg FL, FR, RL, RR;
    Robot(const int S_PIN[12], const int P_PIN[12]) :
      FL(S_PIN[0], S_PIN[1], S_PIN[2], P_PIN[0], P_PIN[1], P_PIN[2]),
      FR(S_PIN[3], S_PIN[4], S_PIN[5], P_PIN[3], P_PIN[4], P_PIN[5]),
      RL(S_PIN[6], S_PIN[7], S_PIN[8], P_PIN[6], P_PIN[7], P_PIN[8]),
      RR(S_PIN[9], S_PIN[10], S_PIN[11], P_PIN[9], P_PIN[10], P_PIN[11]) {}
    void Forward() {

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
    char input = Serial.read();
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