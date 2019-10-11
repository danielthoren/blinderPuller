#ifndef MOTOR
#define MOTOR

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
    Motor(unsigned long pulses_to_bottom, int in1, int in2, int enable);

    //Must be called repeatedly, tracks state of motor.
    void update();

    void go_up();
    void go_down();

    //Updates time variables, called by interrupt
    void timer();
    
private:
    void init();
    
    int speed;
    Motor_state state;
    unsigned long prev_pulse;    //Time of latest pulse from o_sensor
    unsigned long pulse_width;   //Time betwen latest pulses
    int curr_pulse_pos;
    const int in1;
    const int in2;
    const int enable;

    //Number of pulses until blinder at bottom, positive downwards
    const unsigned long pulses_to_bottom;
};


#endif
