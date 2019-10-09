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
int old_time1;
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
}

void loop() {
  if (!digitalRead(btn_down))
    {
      curr_state = going_down;
      time1 = millis();
      old_time1 = time1;
    }
  else if (!digitalRead(btn_up)) 
  {
      curr_state = going_up;
      time1 = millis();
      old_time1 = time1;
  }


  if (curr_state == going_up)
  {
      Serial.println("Going down");
    
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      delay(50);
      
      while ( time1 - old_time1 < 50 && millis() - time1 < 50)
      {}

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
      
      while ( time1 - old_time1 > 10 && millis() - time1 < 50)
      {}

      digitalWrite(in4, LOW);

      curr_state = down;

      Serial.println("Down");
  }
}

void timer1()
{
  old_time1 = time1;
  time1 = millis();
}
