#include <Servo.h>
Servo servo;

int motor =4;
int current_servo=0;
void setup() {
  servo.attach(motor);
  servo.write(0);

  Serial.begin(9600);
}

void loop() {
  char input = Serial.read();

  if (input=='a') {
    Serial.print("팽창\n");
    for(int i=0; i<=10; i++) {
      current_servo=servo.read();
      servo.write(13*i);
      Serial.println(i);
      Serial.println(current_servo);
      delay(150);
    }
  }

  if (input=='d') {
    Serial.print("수축\n");
    for(int i=10; i>=0; i--) {
      current_servo=servo.read();
      servo.write(13*i);
      Serial.println(10-i);
      Serial.println(current_servo);
      delay(150);
    }
  }
}