#include <Servo.h>

Servo SprayHead;

// Analog input pin
int M106_Input_Pin = A5;      // Controls Servo and LED

// Output pins
int servoPin = 10;      // Servo control pin

// Servo angles
int A_ON = 0;
int A_OFF = 25;
// Sensor values
float M106_Input_Reading = 0;
float M106_Input_Voltage = 0;

int delayValue = 10;

void setup() {
  //Serial.begin(9600);
  SprayHead.attach(servoPin);
  SprayHead.write(A_OFF); // Start in OFF position
}

void loop() {
  // Read analog voltage from A5
  M106_Input_Reading = analogRead(M106_Input_Pin);
  M106_Input_Voltage = (M106_Input_Reading / 1023.0) * 5.0; //scale 0-5V

  // Print sensor voltage for debugging
  //Serial.print("Servo Sensor Voltage (A5): ");
  //Serial.println(voltage_sense, 3);

  // Control spray head and LED
  if (M106_Input_Voltage > 3) {
    SprayHead.write(A_ON);   // Spray ON
  } else {
    SprayHead.write(A_OFF);     // Spray OFF
  }

  delay(delayValue); // Wait before next reading
}
