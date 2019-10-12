#include "motor.h"

const int in1 = 12;
const int in2 = 11;
const int in3 = 10;
const int in4 = 9;

const int m1_en = 5;
const int m2_en = 6;

const int o_sensor1 = 2; //alternating between 0 and 1

//Buttons, active low
const int btn_down = 4; 
const int btn_up = 7;

Motor m1(0.5, 100000, in3, in4, m1_en);

void setup() {
  Serial.begin(9600);
  
  pinMode(btn_down, INPUT_PULLUP);
  pinMode(btn_up, INPUT_PULLUP);

  pinMode(o_sensor1, INPUT);

  attachInterrupt(digitalPinToInterrupt(o_sensor1), timer, CHANGE);
}

void loop() {
  if (!digitalRead(btn_down))
    {
	m1.go_down();
    }
  else if (!digitalRead(btn_up)) 
  {
      m1.go_up();
  }
  m1.update();
  Serial.println(m1.get_pwm());
}

void timer()
{
    m1.timer();
}


