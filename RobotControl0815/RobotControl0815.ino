
#include "MakeOrder.h"
#include "RobotLib.h"
#include <string.h>


int s1[3] = {A0, A1, A2};
int p1[3] = {2, 4, 6};
int speed_1[12] = {255, 255, 255};

int s2[3] = {A3, A4, A5};
int p2[3] = {8, 10, 12};
int speed_2[12] = {255, 255, 255};

int s3[3] = {A6, A7, A8};
int p3[3] = {};
int speed_3[12] = {255, 255, 255};

int s4[3] = {A9, A10, A11};
int p4[3] = {};
int speed_4[12] = {255, 255, 255};

int S_pin[12] = {s1[0], s1[1], s1[2], s2[0], s2[1], s2[2], s3[0], s3[1], s3[2], s4[0], s4[1], s4[2]};
int P_pin[12] = {p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], p3[0], p3[1], p3[2], p4[0], p4[1], p4[2]};
int speedarray[12] = {speed_1[0], speed_1[1], speed_1[2], speed_2[0], speed_2[1], speed_2[2], speed_3[0], speed_3[1], speed_3[2]};



// Leg l1(s1[0], s1[1], s1[2], p1[0], p1[1], p1[2]); // Robot 정의 시 필요없음.
// // Leg l2(s2[0], s2[1], s2[2], p2[0], p2[1], p2[2]);
// // Leg l3(s3[0], s3[1], s3[2], p3[0], p3[1], p3[2]);
// // Leg l4(s4[0], s4[1], s4[2], p4[0], p4[1], p4[2]);

Robot robot(S_pin, P_pin, speedarray);




void setup() {
  Serial.begin(9600);
  robot.Initialize();
}

void loop() {
  MakeOrder(robot);
}
