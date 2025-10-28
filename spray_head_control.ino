#include <Servo.h>
Servo SprayHead;
// Set the Servo angles: CW=lower number, CCW=higher number. adjust in 2-5 deg increments
const int Set_ON = 160; //angle when spray is on.
const int Set_OFF = 120; //angle when spray is off.
//-------------------------------------------------------------------

bool Enable_Pot_Influence = 0; //set to 1 to enable the external dial adjustment for the ON angle. Normally set to 0 to disable
const int Pot_Max_Influence = 20;

int Previous_Influence_Angle = 0;
int Current_Influence_Angle = 0;

const int MAX_ON = 170; //max allowable angle for on position
const int MIN_OFF = 90; //min allowable angle for off position

int A_OFF = Set_OFF; //actual angle value sent to servo
int A_ON = Set_ON; //actual angle value sent to servo
bool PreviousSprayState = 0;
bool CurrentSprayState = 0;

//Servo angle calibration
float angle_adjust = 0.75; //180deg signal = 225deg actual 

// Analog input pin
int M106_Input_Pin = A5;      // Controls Servo and LED
int Pot_Input_Pin = A1;   //External position dial Analog Input[4-20ma signal generator]

// Output pins
int servoPin = 10;      // Servo control pin

// Sensor values
float M106_Input_Reading = 0;
float M106_Input_Voltage = 0;
int M106_int = 0;
int Pot_Input = 0;
int Pot_Angle_Influence = 0;


int delayValue = 50;

void setup() {
  Serial.begin(9600);
  Serial.println("--------------------------------------");
  Serial.println("Airbrush Control Servo program started");
  Serial.println("--------------------------------------");

  Serial.print("Angle_ON: ");Serial.print(A_ON); Serial.print("    Angle_OFF: ");Serial.println(A_OFF);
  //Serial.print("angle_adjust: ");Serial.println(angle_adjust);
  if(Enable_Pot_Influence == 1) { Serial.print("Attention: External angle ON adjustment dial is LIVE. "); Serial.print("Current angle adjustment value: ");Serial.println(Pot_Angle_Influence);Serial.println("Note: Servo angles direction CW=lower angle, CCW=higher angle");}
  Serial.println("--------------------------------------\n");
  SprayHead.attach(servoPin);
  SprayHead.write(A_OFF * angle_adjust); // Start in OFF position
  Serial.print(" spray is OFF    ");Serial.println(A_OFF);
}

void loop() {
    // Read analog voltage from A5
  M106_Input_Reading = analogRead(M106_Input_Pin);
  M106_Input_Voltage = round((M106_Input_Reading / 1023.0) * 5.0); //scale 0-5V
  
  if (Enable_Pot_Influence == 1) {
    // Read extenal angle influence from pot. Max CW = 0. Max CCW = 674. Mid = 312.
    Pot_Input = analogRead(Pot_Input_Pin);
    Pot_Angle_Influence = (Pot_Input - 312) * Pot_Max_Influence / 312;
    if (Pot_Angle_Influence > Pot_Max_Influence) { Pot_Angle_Influence = Pot_Max_Influence;}
    if (Pot_Angle_Influence < (-1 * Pot_Max_Influence)) { Pot_Angle_Influence = (-1 * Pot_Max_Influence);}
    Current_Influence_Angle = Pot_Angle_Influence;

    if (Current_Influence_Angle != Previous_Influence_Angle) {
      if (CurrentSprayState == 1) {
        Serial.print("Pot_Input_Value:"); Serial.print(Pot_Input); Serial.print("  Pot_Angle_Influence:"); Serial.print(Pot_Angle_Influence); 
        A_ON = round(Set_ON + Pot_Angle_Influence); //change setpoint of A_ON
        Serial.print("   A_ON:");Serial.print(A_ON);
        if (A_ON > MAX_ON) {
          A_ON = MAX_ON;
          Serial.print(" limited to MAX_ON ");Serial.println(MAX_ON);
        }
        else {Serial.println();}
        SprayHead.write(A_ON * angle_adjust);   // Set Servo to new Spray ON angle 
        Previous_Influence_Angle = Current_Influence_Angle; 
      }
    }
  }


  // Control spray head and LED
    //Serial.print("M106_Input_Voltage: ");Serial.println(M106_Input_Voltage);
    M106_int = round(M106_Input_Voltage);
    //Serial.print("M106_int: ");Serial.println(M106_int);
  if (M106_int > 3) {
    CurrentSprayState = 1; //if Spray is ON
    if (CurrentSprayState != PreviousSprayState) { //|| (abs(Current_Influence_Angle - Previous_Influence_Angle) > 2 )); {
      SprayHead.write(A_ON * angle_adjust);   // Set Servo to Spray ON
      Serial.print(" spray is ON     ");Serial.println(A_ON);
    }
  } else {
    CurrentSprayState = 0;
    if (CurrentSprayState != PreviousSprayState) {
      SprayHead.write(A_OFF * angle_adjust);     // Set Servo to Spray OFF
      Serial.print(" spray is OFF    ");Serial.println(A_OFF);
    }
  }
  PreviousSprayState = CurrentSprayState;

  delay(delayValue); // Wait before next reading
}
