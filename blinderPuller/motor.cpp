#include <Arduino.h>
#include "math.h"

#include "motor.h"

Motor::Motor(float speed, unsigned long pulses_to_bottom, int in1, int in2, int enable) :
    speed{speed}, pwm{0}, state{Motor_state::unknown},
    target_speed{9000},
    prev_pulse{0},
    pulse_width{0}, curr_pulse_pos{0}, in1{in1}, in2{in2}, enable{enable},
    pulses_to_bottom{pulses_to_bottom},
    pid{target_speed, -0.0005, -0.0001, 0.00001, 0, 255}
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
    if (state == going_up)
    {
	delay(10);
	pulse_width = micros() - prev_pulse;
	pwm = pid.update(pulse_width);
	
    	if (pwm == 255)
	{
	    max_pwm_counter++;
	    
	    if (max_pwm_counter > 3)
	    {
		Serial.println("max pwm going up");
		digitalWrite(in1, LOW);
		state = up;
		Serial.println("is up");
		curr_pulse_pos = 0;
		analogWrite(enable, 0);
	    }
	}
	else
	{
	    max_pwm_counter = 0;
	}
    }
	else if (state == going_down)
	{
	    if (pulses_to_bottom <= curr_pulse_pos)
	    {
		digitalWrite(in2, LOW);
		state = down;
		Serial.println("is down");
		analogWrite(enable, 0);
	    }
	}
}

void Motor::go_up()
{
    if (state != up && state != going_up)
    {
	Serial.println("Set state up");
	state = going_up;
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	pwm = DEFAULT_PWM;
	analogWrite(enable, pwm);
	pid.reset();
    }    
}

void Motor::go_down()
{
    state = up; //temporary, should be removed later
    if (state != down && state != unknown && state != going_down)
    {
	Serial.println("Set state down");
	state = going_down;
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	pwm = DEFAULT_PWM;
	analogWrite(enable, pwm);
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
    if (state == going_up)
    {
    	curr_pulse_pos--;
    }
    else
    {
    	curr_pulse_pos++;
    }

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
	analogWrite(enable, pwm);
    }
}
