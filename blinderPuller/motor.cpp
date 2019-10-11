#include <Arduino.h>
#include "motor.h"

Motor::Motor(unsigned long pulses_to_bottom, int in1, int in2, int enable) :
    speed{100}, state{Motor_state::unknown}, prev_pulse{millis()},
    pulse_width{20}, curr_pulse_pos{0}, in1{in1}, in2{in2}, enable{enable},
    pulses_to_bottom{pulses_to_bottom}
{
    init();
}

void Motor::init()
{
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(enable, OUTPUT);

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);

    analogWrite(enable, speed );
}

void Motor::update()
{
  if (state == going_up)
  {    
      // if (cond)
      // {
      // digitalWrite(in1, LOW);
      // state = up;
      // Serial.println("is up");
      // curr_pulse_pos = 0;
      // }
  }
  else if (state == going_down)
  {
      if (pulses_to_bottom <= curr_pulse_pos)
      {
      	  digitalWrite(in2, LOW);
      	  state = down;
      	  Serial.println("is down");
      }
  }
}

void Motor::go_up()
{
    if (state != up)
    {
	Serial.println("Set state up");
	state = going_up;
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
    }    
}

void Motor::go_down()
{
    if (state != down && state != unknown)
    {
	Serial.println("Set state down");
	state = going_down;
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
    }
}

void Motor::timer()
{
    unsigned long time = millis();
    pulse_width = time - prev_pulse;
    prev_pulse = time;

    if (state == going_up)
    {
	curr_pulse_pos--;
    }
    else
    {
	curr_pulse_pos++;
    }
}
