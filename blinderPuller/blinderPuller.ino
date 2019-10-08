const int in1 = 12;
const int in2 = 11;
const int in3 = 10;
const int in4 = 9;

//pwm for motor speed
const int m1_en = 5;
const int m2_en = 6;

const int o_sensor = 2; //alternating between 0 and 1

const int btn_down = 4; 
const int btn_up = 7; 

void setup() {
  Serial.begin(9600);
    
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(o_sensor, INPUT);
  
  pinMode(btn_down, INPUT_PULLUP);
  pinMode(btn_up, INPUT_PULLUP);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  analogWrite(m1_en, 100);
}

void loop() {
  if (digitalRead(btn_down))
    {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
  else if (digitalRead(btn_up)) 
  {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
  }
  else
  {
     digitalWrite(in1, LOW);
     digitalWrite(in2, LOW);  
  }

}
