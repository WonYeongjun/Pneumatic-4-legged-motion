#pragma once
#include <Arduino.h>
#include "Robotlib.h"  // Robot, Leg, Pump, MyServo 클래스 포함 라이브러리

void MakeOrder(Robot& robot);
String Input();
static void DoPumpAction(Pump& P, int pa);
static void KillAllPumps(Robot& robot);
static inline bool HandleKill(const String& s, Robot& robot);
