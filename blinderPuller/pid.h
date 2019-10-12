#ifndef _PID
#define _PID

template<typename T>
class PID
{
public:
    PID() = default;
    PID(T& input, T& output, T set_point, T kp, T ip, T dp) :
	input{input}, output{output}, set_point{set_point}, prev_time{0},
	prev_error{0}, integral{0}, kp{kp}, ip{ip}, dp{dp}
	{}

    void update()
	{
	    T curr_time = millis();
	    T elapsed_time = curr_time - prev_time;

	    T error = set_point - elapsed_time;

	    integral += error * input;
	    T deriv = (error - prev_error)/elapsed_time;

	    output = kp*error + ip*integral + dp*deriv;

	    prev_error = error;
	    prev_time = curr_time;
	}
    
private:

    T& input;
    T& output;
    
    T set_point;
    T prev_time;
    T prev_error;
    T integral;

    T kp;
    T ip;
    T dp;    
};

#endif
