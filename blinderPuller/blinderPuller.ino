const int in1 = 12;
const int in2 = 11;
const int in3 = 10;
const int in4 = 9;

//pwm for motor speed
const int m1_en = 5;
const int m2_en = 6;

int m1_speed = 255;
int m2_speed = 255;

const int o_sensor1 = 2; //alternating between 0 and 1
const int OLD_COUNT = 1;

//saves old time values, latest value at last position
int old_times1[OLD_COUNT];
int deriv1 = 0;
int time1;

//Buttons, active low
const int btn_down = 4; 
const int btn_up = 7; 

enum State{
  up,
  going_up,
  down,
  going_down,
  unknown
  };

State curr_state;

void setup() {
  Serial.begin(9600);
    
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  pinMode(m1_en, OUTPUT);
  pinMode(m2_en, OUTPUT);

  analogWrite(m1_en, m1_speed);
  analogWrite(m2_en, m2_speed);
  
  pinMode(btn_down, INPUT_PULLUP);
  pinMode(btn_up, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(o_sensor1), timer1, CHANGE);

  curr_state = unknown;
  reset_old_times();
}

void loop() {
  if (!digitalRead(btn_down))
    {
      curr_state = going_down;
      time1 = millis();
      reset_old_times();
    }
  else if (!digitalRead(btn_up)) 
  {
      curr_state = going_up;
      time1 = millis();
      reset_old_times();
  }

  if (curr_state == going_up)
  {
      Serial.println("Going up");
    
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      delay(50);
      
      //while ( time1 - old_time1 < 20)

      while (deriv1 < 15)
      {
      }

      digitalWrite(in3, LOW);

      curr_state = up;

      Serial.println("Up");
  }
  else if (curr_state == going_down)
  {
      Serial.println("Going down");
      
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      delay(50);
      
      while ( deriv1 < 15 )
      {
        }

      digitalWrite(in4, LOW);

      curr_state = down;

      Serial.println("Down");
  }
}

void reset_old_times()
{
  for (int i = 0; i < OLD_COUNT; i++)
  {
      old_times1[i] = 10;  
  }
  time1 = 10;
}

void timer1()
{
  for (int i = 0; i < OLD_COUNT - 1; i++)
  {
    old_times1[i] = old_times1[i+1];
  }
  old_times1[OLD_COUNT - 1] = time1;
  time1 = millis();
  deriv1 = time1 - old_times1[0]; 
  Serial.println(deriv1);
}
