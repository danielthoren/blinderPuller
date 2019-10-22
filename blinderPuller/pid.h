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
	    //TimeType elapsed_time = curr_time - prev_time;
	    TimeType elapsed_time = 1;

	    Type error = set_point - input;

	    total_error += (error / elapsed_time);
	    
	    Type delta_error = (error - prev_error) / elapsed_time;

	    Type output = (kp*error + ip*total_error  + dp*delta_error);

	    Serial.print(" output: ");
	    Serial.print(output);
	   
	    Serial.print(" kp part = ");
	    Serial.print(kp*error);

	    Serial.print(" ip part = ");
	    Serial.print(ip*total_error);

	    Serial.print(" error: ");
	    Serial.print(error);

	    Serial.print(" total_error: ");
	    Serial.println(total_error);

	    Serial.print(" delta: ");
	    Serial.print(delta_error);

	    Serial.print("       ");
	    Serial.print(set_point);
	    Serial.print(" - ");
	    Serial.print(input);
	    Serial.print(" = ");
	    Serial.print(error);

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

    void reset()
	{
	    total_error = 0;
	    prev_time = millis();
	    prev_error = 0;
	}
    
//private:
    
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
