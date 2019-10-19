#ifndef _PID
#define _PID

typedef long TimeType; //TimeType

template<typename Type>
class PID
{
public:
    PID() = default;
    PID(Type& set_point, double kp, double ip, double dp,
	Type min_output, Type max_output) :
	set_point{set_point}, prev_time{0},
	prev_error{0}, total_error{0}, kp{kp}, ip{ip}, dp{dp},
	min_output{min_output}, max_output{max_output}
	{}

    Type update(Type input)
	{
	    TimeType curr_time = millis();
	    TimeType elapsed_time = curr_time - prev_time;

	    Type error = set_point - input;

	    total_error += (error) ;

	    Serial.print(" ");
	    Serial.print(set_point);
	    Serial.print(" - ");
	    Serial.print(input);
	    Serial.print(" = ");
	    Serial.print(error);

	    Serial.print(" kp part = ");
	    Serial.print(kp*error*elapsed_time);

	    Serial.print(" ip part = ");
	    Serial.print(ip*total_error);
	   
	    
	    Type delta_error = (error - prev_error) / elapsed_time;

	    Type output = (kp*error + ip*total_error  + dp*delta_error);

	    Serial.print(" output: ");
	    Serial.println(output);

	    if (output > max_output)
	    {
		output = max_output;
	    }
	    else if (output < min_output)
	    {
		output = min_output;
	    }
	    
	    prev_error = error;
	    prev_time = curr_time;

	    return output;
	}
    
private:
    
    Type& set_point;
    TimeType prev_time;
    Type prev_error;
    Type total_error;

    const Type min_output;
    const Type max_output;

    double kp;
    double ip;
    double dp;    
};

#endif
