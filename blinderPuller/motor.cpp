#include <Arduino.h>
#include "limits.h"
#include "math.h"

#include "motor.h"

Motor::Motor(float speed, unsigned long pulses_to_bottom, int in1, int in2, int enable) :
    speed{speed}, pwm{0}, state{Motor_state::unknown},
    target_speed{10},
    prev_pulse{0},
    curr_pulse_pos{INT_MIN}, pulses_to_bottom{pulses_to_bottom},
    max_pwm_counter{0},
    in1{in1}, in2{in2}, enable{enable},
    pid_up{target_speed, -5, 0, -0.5, 50, 255},
    pid_down{target_speed, -2, 0, 0, -1, 255}
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

    reset_old_pulses();
}

void Motor::reset_old_pulses()
{
    for (int i{0}; i < OLD_PULSE_SIZE; i++)
    {
	old_pulses[i] = target_speed;
	Serial.print(old_pulses[i]);
    }
    Serial.println("]");
}

unsigned long Motor::get_average_pulse()
{
    unsigned long average{0};
    
    for (int i{0}; i < OLD_PULSE_SIZE; i++)
    {
	average += old_pulses[i];
    }
    return average/OLD_PULSE_SIZE;
}

void Motor::update()
{    
    if (state == going_up || state == go_up_down)
    {
	update_up();
    }
    else if (state == going_down)
    {
	Serial.println("go down");
	update_down();
    }
}

void Motor::update_down()
{
    delay(10);
    unsigned long pulse_width = millis() - prev_pulse;
    
    pwm = pid_down.update( (pulse_width + get_average_pulse()) / 2 );
   
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
    unsigned long pulse_width = millis() - prev_pulse;

    pwm = pid_up.update( (pulse_width + get_average_pulse()) / 2 );
    
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
	reset_old_pulses();
	pid_up.reset();
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

	    digitalWrite(in1, LOW);
	    digitalWrite(in2, HIGH);
	    pwm = DOWN_PWM;
	    analogWrite(enable, pwm);
	    reset_old_pulses();
	    pid_down.reset();
	}
	else
	{
	    Serial.println("Set state: go_up_down");
	    //state = go_up_down;
	    //go_up();

	    state = up;
	    curr_pulse_pos = 0;
	    go_down();                                                      //obs, remove when done with pid!!!
	}	
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
    
    unsigned long time = millis();
    if (prev_pulse == 0)
    {
	prev_pulse = time;
    }
    unsigned long pw_tmp = time - prev_pulse;
    Serial.println(pw_tmp);
    prev_pulse = time;

    if (pw_tmp > 0)
    {
	for (int i = OLD_PULSE_SIZE - 1; i >= 0; i--)
	{
	    old_pulses[i] = old_pulses[i-1];
	}
	old_pulses[0] = pw_tmp;
    }
}
