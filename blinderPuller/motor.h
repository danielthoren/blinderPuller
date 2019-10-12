#ifndef MOTOR
#define MOTOR

#include "pid.h"

class Motor
{
public:
    enum Motor_state{
	up,
	going_up,
	down,
	going_down,
	unknown
    };
    
    Motor(float speed, unsigned long pulses_to_bottom, int in1, int in2, int enable);

    //Must be called repeatedly, tracks state of motor.
    void update();

    void go_up();
    void go_down();

    //Updates time variables, called by interrupt
    void timer();

    unsigned long get_pwm();
    
private:
    void init();
    
    float speed;                 //speed in %
    unsigned long pwm;           //pwm number
    Motor_state state;
    unsigned long prev_pulse;    //Time of latest pulse from o_sensor
    unsigned long pulse_width;   //Time betwen latest pulses
    int curr_pulse_pos;
    const int in1;
    const int in2;
    const int enable;

    //Number of pulses until blinder at bottom, positive downwards
    const unsigned long pulses_to_bottom;

    PID<unsigned long> pid;

    static const int MAX_SPEED = 20;      //Max speed in ms
    static const int DEFAULT_PWM = 100;   //Default value for pwm
};


#endif
