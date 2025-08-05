#include <Servo.h>
Servo sv1, sv2, sv3;
int tmp = 0;

int s1 =A0; 
int s2 =A1; 
int s3 =A2;
int current_servo=0;

int p1 = 3;
int p2 = 6;
int p3 = 9;

int c1 = 0;
int c2 = 0;
int c3 = 0;

void setup() {
  sv1.attach(s1);
  sv2.attach(s2);
  sv3.attach(s3);
  sv1.write(0);
  sv2.write(0);
  sv3.write(0);
  delay(2000);
  sv1.detach();
  sv2.detach();
  sv3.detach();

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
      current_servo=sv1.read();
      sv1.write(current_servo + 10);
      sv2.write(current_servo + 10);
      sv3.write(current_servo + 10);
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
      current_servo=sv1.read();
      sv1.write(current_servo - 10);
      sv2.write(current_servo - 10);
      sv3.write(current_servo - 10);
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
}