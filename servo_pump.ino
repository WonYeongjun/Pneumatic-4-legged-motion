#include <math.h>
#include <Servo.h>

bool pwmRunning = true;
Servo sv1;
int s1 = A2;
int s2 = 2;
// int s3 = 3;
//int pos = 0;
//int angle = 180;

int joy = 2; //조이스틱
int X = A0;
int Y = A1;
int x = 0;
int y = 0;

int stop_sign=0;
long newx = 0;
long newy = 0;
long *newxptr = &newx;
long *newyptr = &newy;
int pos;
int p11 = 3;
int p21 = 6;
int p31 = 9;
int speed_1 = 255;
int speed_2 = 0;
float minspeed = 150; // 팽창하는 최소 유량
float state[3];

void get_input(long* newx, long* newy) {
  *newx= analogRead(X);
  *newy= analogRead(Y); 
}
void get_keyboard_input(){
  if (Serial.available()) {
    char c = Serial.read();


    switch (c) {

      
      case 'A': // 뒤로
        //stop_sign=0;  
        //*newx= 0;
        //*newy=512;
        Serial.println("A");
        analogWrite(p11, 0);
        analogWrite(p21, 0);
        analogWrite(p31, 0);
        while(pos>=0){
          Serial.println(pos);

          pos = pos - 20;
          sv1.write(pos);
          delay(500);
        }
        break;

      case 'S': // 정지가 아니고 앞으로
        //stop_sign=1;
        Serial.println("S");
        analogWrite(p11, 0);
        analogWrite(p21, 0);
        analogWrite(p31, 0);
        while(pos <= 90){
          Serial.println(pos);

          pos = pos + 20;
          sv1.write(pos);
          delay(500);
        }
        
        break;
   }
  }
}

void setup() {
  sv1.attach(s1);
  //sv2.attach(s2);
  //sv3.attach(s3);

  //sv1.write(ang_in);
  //sv2.write(ang_in);
  //sv3.write(ang_in);

  pinMode(joy, INPUT_PULLUP);          
  pinMode(X, INPUT);          
  pinMode(Y, INPUT);
  pinMode(p11, OUTPUT);
  pinMode(p21, OUTPUT);
  pinMode(p31, OUTPUT); 

  Serial.begin(9600);
  // x = analogRead(X);
  // y = analogRead(Y);
  x=519;
  y=494;
  // analogWrite(p11,250);
  // analogWrite(p21,250);
  // analogWrite(p31,0);
  // delay(1500);
  // analogWrite(p11,0);
  // analogWrite(p21,0);
  // analogWrite(p31,250);
  // delay(1500);
  analogWrite(p11,0);
  analogWrite(p21,0);
  analogWrite(p31,0);
  sv1.write(0);

}

void loop() {
  get_input(&newx,&newy);
  get_keyboard_input();
  // *newxptr=1023;
  // *newyptr=0;
  double dir = angle();
  float len = length();
  Serial.print("angle : ");
  Serial.println(dir);
  Serial.print("length :");
  Serial.println(len);
  angle_to_rel_state(state, dir, len);
  PWM();
  //delay(1000);
}

double angle(){
  // printf("newx: %d, newy : %d\n", newx, newy);

  int dx = newx - x;
  int dy = newy - y;
  //double length = sqrt(dx^2+dy^2) / (512*sqrt(2));
  Serial.println(newx);
  
  float angle = atan2(dy, dx);
  // Serial.println(angle);
  if (angle<0){
    angle = angle + 2*M_PI;
    // Serial.println("in");
    // Serial.println(angle);
  }

  return angle*180/M_PI;
}

float length(){
  long dx = newx - x;
  long dy = newy - y;

  long leng = sqrt(pow(dx,2)+pow(dy,2));
  float length= leng/(512*sqrt(2));
  length*=255;
  return length;
}

void angle_to_rel_state(float* state, double angle, float length) {
  float rel_angle, low, high, low_tmp, high_tmp;

  if (30 <= angle && angle < 150) { // A : 1 , 2
    rel_angle = (angle - 30) * M_PI / 180.0;
    low_tmp = cos(rel_angle) + sin(rel_angle) / sqrt(3);
    high_tmp = 2 * sin(rel_angle) / sqrt(3);
    low = low_tmp / (low_tmp + high_tmp);
    high = high_tmp / (low_tmp + high_tmp);
    state[0] = 0;
    state[1] = low * length *(255-minspeed)/255 + minspeed;
    state[2] = high* length *(255-minspeed)/255 + minspeed;
  } 
  else if (150 <= angle && angle < 270) { // B : 2 , 0
    rel_angle = (angle - 150) * M_PI / 180.0;
    low_tmp = cos(rel_angle) + sin(rel_angle) / sqrt(3);
    high_tmp = 2 * sin(rel_angle) / sqrt(3);
    low = low_tmp / (low_tmp + high_tmp);
    high = high_tmp / (low_tmp + high_tmp);
    state[1] = 0;
    state[2] = low * length *(255-minspeed)/255 + minspeed;
    state[0] = high* length *(255-minspeed)/255 + minspeed;
  } 
  else { // C : 0 , 1
    rel_angle = (angle - 270) * M_PI / 180.0;
    low_tmp = cos(rel_angle) + sin(rel_angle) / sqrt(3);
    high_tmp = 2 * sin(rel_angle) / sqrt(3);
    low = low_tmp / (low_tmp + high_tmp);
    high = high_tmp / (low_tmp + high_tmp);
    state[2] = 0;
    state[0] = low * length *(255-minspeed)/255 + minspeed;
    state[1] = high* length *(255-minspeed)/255 + minspeed;
    }
  Serial.print("state : ");
  Serial.print((int)state[0]);
  Serial.print(", ");
  Serial.print((int)state[1]);
  Serial.print(", ");
  Serial.println((int)state[2]);
  Serial.println(" ");
}

void PWM(){
  //for(int i=0; i<3; i++){
    //if(state[i]>0 && state[i]<minspeed){
      //state[i] = minspeed;
    //}
  //}

  //sv1.write(ang_in);
  //sv2.write(ang_in);
  //sv3.write(ang_in);
  //delay(500);
  if ((int)state[0]+(int)state[1]+(int)state[2]<310){
    analogWrite(p11, 0);
    analogWrite(p21, 0);
    analogWrite(p31, 0);
  }
  else{
    analogWrite(p11, (int)state[0]);
    analogWrite(p21, (int)state[1]);
    analogWrite(p31, (int)state[2]);
  }


  //sv1.write(ang_out);
  //sv2.write(ang_out);
  //sv3.write(ang_out);
  //delay(500);

  //analogWrite(p11, (int)state[0]);
  //analogWrite(p21, (int)state[1]);
  //analogWrite(p31, (int)state[2]);
  //delay(2000);

  //analogWrite(p11, speed_2);
  //analogWrite(p21, speed_2);
  //analogWrite(p31, speed_2);
  //sv1.write(ang_mean);
  //sv2.write(ang_mean);
  //sv3.write(ang_mean);
}

/*void move(int angle){
  if(angle > pos){
    while(pos <= angle){
      pos = pos + 10;
      sv1.write(pos);
      delay(400);
    }
    pos = angle;
    sv1.write(pos);
    delay(500);
  }
  else{
    while(pos>=angle){
      pos = pos - 10;
      sv1.write(pos);
      delay(500);
    }
  }
  Serial.println(pos);
}*/

