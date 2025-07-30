int p1 = 3;
int p2 = 6;
int p3 = 9;

int c1 = 0;
int c2 = 0;
int c3 = 0;

void setup() {
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
}