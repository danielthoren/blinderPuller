#include <Arduino.h>
#include "math.h"

#include "motor.h"

Motor::Motor(float speed, unsigned long pulses_to_bottom, int in1, int in2, int enable) :
    speed{speed}, pwm{100}, state{Motor_state::unknown}, prev_pulse{millis()},
    pulse_width{20}, curr_pulse_pos{0}, in1{in1}, in2{in2}, enable{enable},
    pulses_to_bottom{pulses_to_bottom},
    pid{pulse_width, pwm, speed*MAX_SPEED, 0.5, 0, 0}
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

    analogWrite(enable, DEFAULT_PWM);
}

void Motor::update()
{
  if (state == going_up)
  {    
      if (pwm == 255)
      {
	  digitalWrite(in1, LOW);
	  state = up;
	  Serial.println("is up");
	  curr_pulse_pos = 0;
	  analogWrite(enable, DEFAULT_PWM);
      }
  }
  else if (state == going_down)
  {
      if (pulses_to_bottom <= curr_pulse_pos)
      {
      	  digitalWrite(in2, LOW);
      	  state = down;
      	  Serial.println("is down");
	  analogWrite(enable, DEFAULT_PWM);
      }
  }
}

void Motor::go_up()
{
    if (state != up)
    {
	pwm = DEFAULT_PWM;
	Serial.println("Set state up");
	state = going_up;
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	delay(100);
    }    
}

void Motor::go_down()
{
    state = up; //temporary, should be removed later
    if (state != down && state != unknown)
    {
	pwm = DEFAULT_PWM;
	Serial.println("Set state down");
	state = going_down;
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	delay(100);
    }
}

unsigned long Motor::get_pwm()
{
    return pwm;
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

    pid.update();
    if (pwm > 255)
    {
	pwm = 255;
    }
    analogWrite(enable, pwm);
    Serial.println(pwm);
}
