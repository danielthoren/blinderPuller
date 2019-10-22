#include <Arduino.h>
#include "limits.h"
#include "math.h"

#include "motor.h"

Motor::Motor(float speed, unsigned long pulses_to_bottom, int in1, int in2, int enable) :
    speed{speed}, pwm{0}, state{Motor_state::unknown},
    target_speed{9000},
    prev_pulse{0}, pulse_width{0},
    curr_pulse_pos{INT_MIN}, pulses_to_bottom{pulses_to_bottom},
    max_pwm_counter{0},
    in1{in1}, in2{in2}, enable{enable},
    pid{target_speed, -0.0005, -0.0001, -0.00001, 0, 255}
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

    analogWrite(enable, pwm);
}

void Motor::update()
{    
    if (state == going_up || state == go_up_down)
    {
	update_up();
    }
    else if (state == going_down)
    {
	update_down();
    }
}

void Motor::update_down()
{
    delay(10);
    pulse_width = micros() - prev_pulse;
    
    pwm = pid.update(pulse_width);
    analogWrite(enable, pwm);    
   
    if (pulses_to_bottom <= curr_pulse_pos)
    {
	digitalWrite(in2, LOW);
	state = down;
	Serial.println("Set state: down");
	analogWrite(enable, 0);
    }
}

void Motor::update_up()
{
    delay(10);
    pulse_width = micros() - prev_pulse;
    
    pwm = pid.update(pulse_width);
    analogWrite(enable, pwm);    
    if (pwm == 255)
    {
	max_pwm_counter++;
    }
    else
    {
	max_pwm_counter = 0;
    }
		    
    if (max_pwm_counter > 3)
    {
	digitalWrite(in1, LOW);
	curr_pulse_pos = 0;
	analogWrite(enable, 0);

	if (state == go_up_down)
	{
	    Serial.println("from up to: going_down");
	    go_down();
	}
	else
	{
	    Serial.println("Set state: up");
	    state = up;
	}
    }
}

void Motor::abort()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    if (curr_pulse_pos == INT_MIN)
    {
	Serial.println("Set state: unknown");
	state = unknown;
    }
    else
    {
	Serial.println("Set state: middle");
	state = middle;
    }
}

void Motor::go_up()
{
    if (state != up && state != going_up)
    {
	Serial.println("Set state: going_up");

	state = going_up;
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	pwm = UP_PWM;
	analogWrite(enable, pwm);
	pulse_width = target_speed;
	pid.reset();
    }    
}

void Motor::go_down()
{
    if (state != down && state != going_down)
    {
	if (state != unknown && curr_pulse_pos != INT_MIN)
	{
	    Serial.println("Set state: going_down");
	    state = going_down;
	}
	else
	{
	    Serial.println("Set state: go_up_down");
	    state = go_up_down;
	}
	
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	pwm = DOWN_PWM;
	analogWrite(enable, pwm);
	pulse_width = target_speed;
	pid.reset();
    }
}

unsigned long Motor::get_pwm()
{
    return pwm;
}

void Motor::set_speed(float speed)
{
    speed = (MAX_SPEED - MIN_SPEED) * speed + MIN_SPEED;
}

void Motor::timer()
{
    if (curr_pulse_pos != INT_MIN)
    {
	if (state == going_up)
	{
	    curr_pulse_pos--;
	}
	else
	{
	    curr_pulse_pos++;
	}
    }
    //Serial.println(curr_pulse_pos);
    unsigned long time = micros();
    if (prev_pulse == 0)
    {
	prev_pulse = time;
    }
    long pw_tmp = time - prev_pulse;
    prev_pulse = time;

    // Serial.print("pulse_width = ");
    // Serial.println(pw_tmp);

    if (pw_tmp > 0)
    {	
	pulse_width = pw_tmp;
    }
}
