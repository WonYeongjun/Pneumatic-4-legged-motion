// MakeOrder.cpp
#include "MakeOrder.h"

String Input() {
  String input = Serial.readStringUntil('\n');
  input.trim();
  return input;
}

// ===== Kill-switch helpers =====
//static void KillAllPumps(Robot& robot) {
//  robot.FL.pump1.PumpOff(); robot.FL.pump2.PumpOff(); robot.FL.pump3.PumpOff();
//  robot.FR.pump1.PumpOff(); robot.FR.pump2.PumpOff(); robot.FR.pump3.PumpOff();
//  robot.RL.pump1.PumpOff(); robot.RL.pump2.PumpOff(); robot.RL.pump3.PumpOff();
//  robot.RR.pump1.PumpOff(); robot.RR.pump2.PumpOff(); robot.RR.pump3.PumpOff();
//  Serial.println(F("[KILL] All pumps OFF"));
//}

//===== Kill-switch helpers =====
static void KillAllPumps(Robot& robot) {
 robot.A.pump1.PumpOff(); robot.A.pump2.PumpOff(); robot.A.pump3.PumpOff();
 robot.B.pump1.PumpOff(); robot.B.pump2.PumpOff(); robot.B.pump3.PumpOff();
 robot.C.pump1.PumpOff(); robot.C.pump2.PumpOff(); robot.C.pump3.PumpOff();
 robot.D.pump1.PumpOff(); robot.D.pump2.PumpOff(); robot.D.pump3.PumpOff();
 Serial.println(F("[KILL] All pumps OFF"));
}


static inline bool HandleKill(const String& s, Robot& robot) {
  if (s.length() == 1 && (s[0] == 'p' || s[0] == 'P')) {
    KillAllPumps(robot);
    return true; // 입력을 소비했으니 상위 메뉴로 계속
  }
  return false;
}

// helper: run one pump action (1~4)  ← Robot& 추가 (킬스위치용)
static void DoPumpAction(Pump& P, int pa, Robot& robot) {
  switch (pa) {
    case 1: P.PumpOn();  break;
    case 2: P.PumpOff(); break;
    case 3: P.Switch();  break;
    default:
      Serial.println(F("Invalid"));
      break;
  }
}

void MakeOrder(Robot& robot) {
  bool showRoot = true;
  while (true) {
    if (showRoot) {
      Serial.println(F("=== ROOT ==="));
      Serial.println(F("1. Robot Movement"));
      Serial.println(F("2. Leg"));
      Serial.println(F("3. Joy"));
      Serial.println(F("0. Exit"));
      showRoot = false;
    }

    String s = Input();
    if (HandleKill(s, robot)) { showRoot = true; continue; }  // ★ 전역 p 처리
    if (s.length() == 0) { delay(1); continue; }
    if (s == "0") return;
    int root = s.toInt();

    switch (root) {
      // ---------------- Robot Movement ----------------
      case 1: {
        bool showRM = true;
        while (true) {
          if (showRM) {
            Serial.println(F("Robot Movement : 1.AB_Forward  2.Backward  3.TurnLeft  4.TurnRight 5.Standing 0.Back"));
            showRM = false;
          }
          String rs = Input();
          if (HandleKill(rs, robot)) { showRM = true; continue; } // ★
          if (rs.length() == 0) { delay(1); continue; }
          if (rs == "0") break;
          int sel = rs.toInt();
          switch (sel) {
            case 1: robot.AB_Forward();   break;
            case 2: robot.Backward();  break;
            case 3: robot.TurnLeft();  break;
            case 4: robot.TurnRight(); break;
            case 5: robot.Standing(); break;
            default: Serial.println(F("Invalid")); break;
          }
          showRM = true;
        }
      } break;

      // ---------------- Leg ----------------
      case 2: {
        bool showLegSel = true;
        while (true) {
          if (showLegSel) {
            Serial.println(F("Select Leg : 1.A 2.B 3.C 4.D  /  0.Back"));
            showLegSel = false;
          }
          String ls = Input();
          if (HandleKill(ls, robot)) { showLegSel = true; continue; } // ★
          if (ls.length() == 0) { delay(1); continue; }
          if (ls == "0") break;

          int legSel = ls.toInt();
          Leg* L = nullptr;
          if      (legSel == 1) L = &robot.A;
          else if (legSel == 2) L = &robot.B;
          else if (legSel == 3) L = &robot.C;
          else if (legSel == 4) L = &robot.D;
          else { Serial.println(F("Invalid")); showLegSel = true; continue; }

          bool showLegMenu = true;
          while (true) {
            if (showLegMenu) {
              Serial.println(F("Leg Menu : 1.Leg Movement  2.Pump  3.Servo  /  0.Back"));
              showLegMenu = false;
            }
            String ms = Input();
            if (HandleKill(ms, robot)) { showLegMenu = true; continue; } // ★
            if (ms.length() == 0) { delay(1); continue; }
            if (ms == "0") break;

            int m = ms.toInt();

            // ---------- 2-?-1 Leg Movement ----------
            if (m == 1) {
              bool showLM = true;
              while (true) {
                if (showLM) {
                  Serial.println(F("Leg Movement : 1.Forward  2.Backward  3.Bending  4.Extension  5.Contraction 6.kill_pump /  0.Back"));
                  showLM = false;
                }
                String mms = Input();
                if (HandleKill(mms, robot)) { showLM = true; continue; } // ★
                if (mms.length() == 0) { delay(1); continue; }
                if (mms == "0") break;

                int mm = mms.toInt();
                if      (mm == 1) { L->Forward(); }
                else if (mm == 2) { L->Backward(); }
                else if (mm == 3) {
                  bool showB = true;
                  while (true) {
                    if (showB) {
                      Serial.println(F("Bending : 1.Bending12  2.Bending23  3.Bending31  /  0.Back"));
                      showB = false;
                    }
                    String bs = Input();
                    if (HandleKill(bs, robot)) { showB = true; continue; } // ★
                    if (bs.length() == 0) { delay(1); continue; }
                    if (bs == "0") break;
                    int b = bs.toInt();
                    if      (b == 1) L->Bending12();
                    else if (b == 2) L->Bending23();
                    else if (b == 3) L->Bending31();
                    else             Serial.println(F("Invalid"));
                    showB = true;
                  }
                }
                else if (mm == 4) { L->Extension(); }
                else if (mm == 5) { L->Contraction(); }
                else if (mm == 6) {L->Leg_PumpOff();}
                else              { Serial.println(F("Invalid")); }

                showLM = true;
              }
            }
            // ---------- 2-?-2 Pump ----------
            else if (m == 2) {
              bool showPumpSel = true;
              while (true) {
                if (showPumpSel) {
                  Serial.println(F("Pump : 1.Pump1  2.Pump2  3.Pump3  4.All  /  0.Back"));
                  showPumpSel = false;
                }
                String pss = Input();
                if (HandleKill(pss, robot)) { showPumpSel = true; continue; } // ★
                if (pss.length() == 0) { delay(1); continue; }
                if (pss == "0") break;

                int ps = pss.toInt();
                if (ps < 1 || ps > 4) { Serial.println(F("Invalid")); showPumpSel = true; continue; }

                bool showPumpAct = true;
                while (true) {
                  if (showPumpAct) {
                    Serial.println(F("Pump Action : 1.TurnOn  2.TurnOff  3.Switch /  0.Back"));
                    showPumpAct = false;
                  }
                  String pas = Input();
                  if (HandleKill(pas, robot)) { showPumpAct = true; continue; } // ★
                  if (pas.length() == 0) { delay(1); continue; }
                  if (pas == "0") break;

                  int pa = pas.toInt();
                  if (pa < 1 || pa > 4) { Serial.println(F("Invalid")); continue; }

                  if (ps == 4) {
                    // All legs' selected leg's 3 pumps → but here ps==4 means "this leg's 3 pumps"
                    DoPumpAction(L->pump1, pa, robot);
                    DoPumpAction(L->pump2, pa, robot);
                    DoPumpAction(L->pump3, pa, robot);
                  } else {
                    Pump* P = (ps == 1 ? &L->pump1 : ps == 2 ? &L->pump2 : &L->pump3);
                    DoPumpAction(*P, pa, robot);
                  }
                  showPumpAct = true;
                }
                showPumpSel = true;
              }
            }
            // ---------- 2-?-3 Servo ----------
            else if (m == 3) {
              bool showServoSel = true;
              while (true) {
                if (showServoSel) {
                  Serial.println(F("Servo : 1.Servo1  2.Servo2  3.Servo3 4.Leg_simultaneous_extention 5.Leg_simultaneous_contraction /  0.Back"));
                  showServoSel = false;
                }
                String sss = Input();
                if (HandleKill(sss, robot)) { showServoSel = true; continue; } // ★
                if (sss.length() == 0) { delay(1); continue; }
                if (sss == "0") break;

                int ss = sss.toInt();
                MyServo* S = nullptr;
                if      (ss == 1) S = &L->sv1;
                else if (ss == 2) S = &L->sv2;
                else if (ss == 3) S = &L->sv3;
                // else if (ss == 4) {L->Leg_simultaneous_extention();
                // continue;}
                // else if (ss == 5) {L->Leg_simultaneous_contraction();
                // continue;}
                else { Serial.println(F("Invalid")); showServoSel = true; continue; }

                bool showServoAct = true;
                while (true) {
                  if (showServoAct) {
                    Serial.println(F("Servo Action : 1.Extension  2.Contraction  /  0.Back"));
                    showServoAct = false;
                  }
                  String sas = Input();
                  if (HandleKill(sas, robot)) { showServoAct = true; continue; } // ★
                  if (sas.length() == 0) { delay(1); continue; }
                  if (sas == "0") break;

                  int sa = sas.toInt();
                  if (sa == 1 || sa == 2) {
                    S->attach(S->pin);
                    if (sa == 1) S->Extension(L->unit_angle);
                    else         S->Contraction(L->unit_angle);
                    delay(150);
                    S->detach();
                  } else {
                    Serial.println(F("Invalid"));
                  }
                  showServoAct = true;
                }
                showServoSel = true;
              }
            }
            else {
              Serial.println(F("Invalid"));
            }
            showLegMenu = true;
          } // end Leg menu
          showLegSel = true;
        }   // end Leg select
      } break;

      // Joystick
      case 3: {
        bool showJoy = true;
        while (true) {
          if (showJoy) {
            Serial.println(F("Joystick mode"));
            Serial.println(F("Select Leg : 1.A 2.B 3.C 4.D  /  0.Back"));
            showJoy = false;
          }
          String jy = Input();
          if (HandleKill(jy, robot)) { showJoy = true; continue; } // ★
          if (jy.length() == 0) { delay(1); continue; }
          if (jy == "0") break;

          int joySel = jy.toInt();
          Leg* J = nullptr;
          if      (joySel == 1) J = &robot.A;
          else if (joySel == 2) J = &robot.B;
          else if (joySel == 3) J = &robot.C;
          else if (joySel == 4) J = &robot.D;
          else { Serial.println(F("Invalid")); showJoy = true; continue; }

          bool showJoyMenu = true;
          while (true) {
            if (showJoyMenu) {
              Serial.println(F("Control by Joystick. Press 0 to return."));
              showJoyMenu = false;
            }
            String jm = Input();
            if (HandleKill(jm, robot)) { showJoyMenu = true; continue; } // ★
            if (jm.length() == 0) {J->Leg_joystick_control() ; continue; }
            if (jm == "0") break;
            else {Serial.println(F("Invalid")); showJoyMenu = true; continue;}
          }
          showJoyMenu = true;
        }
        showJoy = true; // end Joy
      } break;

      default:
        Serial.println(F("Invalid"));
        break;
    }
    showRoot = true;
  }
}

