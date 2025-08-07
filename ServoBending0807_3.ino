#include <Servo.h>

class MyServo : public Servo  {
  public :
    int pin;
    int angle;

    MyServo(int p) {
      pin = p;
      angle = 0;
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



int tmp = 0;

int s1 =A0; 
int s2 =A1; 
int s3 =A2;

int p1 = 3;
int p2 = 6;
int p3 = 9;

int c1 = 0;
int c2 = 0;
int c3 = 0;


void setup() {
  MyServo sv1(s1);
  MyServo sv2(s2);
  MyServo sv3(s3);

  Serial.begin(9600);

  pinMode(p1, OUTPUT);
  pinMode(p2, OUTPUT);
  pinMode(p3, OUTPUT); 

  analogWrite(p1, 0);
  analogWrite(p2, 0);
  analogWrite(p3, 0);
}

void loop() {
  char input = Serial.read();
  // Serial.println(current_servo);
  if(input  == '123'){
    Serial.println("123 on");
    analogWrite(p1, 255);
    analogWrite(p2, 255);
    analogWrite(p3, 255);
    c1 = 1;
    c2 = 1;
    c3 = 1;
  }
  if(input  == '321'){
    Serial.println("123 off");
    analogWrite(p1, 0);
    analogWrite(p2, 0);
    analogWrite(p3, 0);
    c1 = 0;
    c2 = 0;
    c3 = 0;
  }

  if (input=='1' && c1==0) {
    Serial.println("1 on");
    analogWrite(p1, 255);
    c1 = 1;
  }

  else if (input=='1' && c1==1) {
    Serial.println("1 off");
    analogWrite(p1, 0);
    c1 = 0;
  }

  if (input=='2' && c2==0) {
    Serial.println("2 on");
    analogWrite(p2, 255);
    c2 = 1;
  }

  else if (input=='2' && c2==1) {
    Serial.println("2 off");
    analogWrite(p2, 0);
    c2 = 0;
  }  

  if (input=='3' && c3==0) {
    Serial.println("3 on");
    analogWrite(p3, 255);
    c3 = 1;
  }

  else if (input=='3' && c3==1) {
    Serial.println("3 off");
    analogWrite(p3, 0);
    c3 = 0;
  }

  if (input=='a' && tmp == 0) {
    tmp = 1;
    sv1.attach(s1);
    sv2.attach(s2);
    sv3.attach(s3);
    
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

  if (input=='d' && tmp == 1) {
    tmp = 0;
    sv1.attach(s1);
    sv2.attach(s2);
    sv3.attach(s3);
    Serial.print("수축\n");
    for(int i=0; i<=13; i++) {
      sv1.Contraction();
      sv2.Contraction();
      sv3.Contraction();
      // Serial.println(i);
      // Serial.println(current_servo);
      delay(150);
    }
    sv1.detach();
    sv2.detach();
    sv3.detach();
  }

  if (input=='s') {
    sv1.attach(s1);
    sv2.attach(s2);
    sv3.attach(s3);

    Serial.print("고속 ");
    if(tmp == 0){
      Serial.print("팽창\n");
      sv1.write(60);
      sv2.write(60);
      sv3.write(60);
      delay(700);
      sv1.write(120);
      sv2.write(120);
      sv3.write(120);
      delay(700);
      tmp = 1;
    }
    else{
      Serial.print("수축\n");
      sv1.write(60);
      sv2.write(60);
      sv3.write(60);
      delay(700);
      sv1.write(0);
      sv2.write(0);
      sv3.write(0);
      delay(700);
      tmp = 0;
    }
    sv1.detach();
    sv2.detach();
    sv3.detach();
  }
    if (input=='m') {
    sv1.attach(s1);
    sv2.attach(s2);
    sv3.attach(s3);
    Serial.print("중립");

    sv1.write(60);
    sv2.write(60);
    sv3.write(60);
    delay(700);
    sv1.detach();
    sv2.detach();
    sv3.detach();
  }


  if (input == '12' && tmp ==0) {
    tmp = 1;
    Serial.println("Bending to 12");
    sv1.attach(s1);
    sv2.attach(s2);
    sv3.attach(s3);

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
  
   if (input == '23' && tmp ==0) {
    tmp = 1;
    Serial.println("Bending to 23");
    sv1.attach(s1);
    sv2.attach(s2);
    sv2.attach(s3);
     
    
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
  if (input == '31' && tmp ==0) {
    tmp = 1;
    Serial.println("Bending to 31");
    sv1.attach(s1);
    sv2.attach(s2);
    sv3.attach(s3);


    for(int i=0; i<=13; i++) {
      sv1.Contraction();
      sv2.Extension();
      sv3.Contraction();
      delay(150);
      delay(150);
    }
    sv1.detach();
    sv2.detach();
    sv3.detach();
  }
   
}
