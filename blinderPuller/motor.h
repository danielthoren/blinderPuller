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
	go_up_down, //When state us unknown but want to go down
	middle,     //When not up or down but position known
	unknown     //State is unknown
    };
    
    Motor(float speed, unsigned long pulses_to_bottom, int in1, int in2, int enable);

    //Must be called repeatedly, tracks state of motor.
    void update();

    void go_up();
    void go_down();
    void abort();

    //Updates time variables, called by interrupt
    void timer();

    unsigned long get_pwm();
    void set_speed(float speed);
    
private:
    static const int UP_PWM = 0;//250;       //Default value for pwm when going up
    static const int DOWN_PWM = 0;//70;      //Default value for pwm when going down
    static const int MAX_SPEED = 10000;  //Max speed in ms
    static const int MIN_SPEED = 20000;
    static const int OLD_PULSE_SIZE = 4;
    
    void init();
    void update_up();
    void update_down();
    unsigned long get_average_pulse();
    void reset_old_pulses();
    
    float speed;            //speed in %
    long pwm;               //pwm number
    Motor_state state;
    long target_speed;      //set point for pid
<<<<<<< HEAD

    unsigned long prev_pulse;
    unsigned long old_pulses[OLD_PULSE_SIZE];    //Time betwen latest pulses
    int curr_pulse_pos;    //Current pulse position
    
=======
    
    long prev_pulse;        //Time of latest pulse from o_sensor
    long pulse_width;       //Time betwen latest pulses
    int curr_pulse_pos;     //Current pulse position
>>>>>>> d15e1ceb4af7c23e5d07c249c9f3f9acfafe4b6e
    //Number of pulses until blinder at bottom, positive downwards
    const unsigned long pulses_to_bottom;
    
    int max_pwm_counter;    //counts how many updates pwm = 255
    
    const int in1;
    const int in2;
    const int enable;

    PID<long> pid;
<<<<<<< HEAD
=======

    static const int UP_PWM = 250;       //Default value for pwm when going up
    static const int DOWN_PWM = 70;      //Default value for pwm when going down
    static const int MAX_SPEED = 10000;  //Max speed in ms
    static const int MIN_SPEED = 20000;
>>>>>>> d15e1ceb4af7c23e5d07c249c9f3f9acfafe4b6e
};


#endif
