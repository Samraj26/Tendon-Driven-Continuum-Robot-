#include <util/atomic.h> 

#define ENCA 2 
#define ENCB 3 
#define PWM 5
#define IN2 6
#define IN1 7

volatile int posi = 0; 
long prevT = 0;
float eprev = 0;
float eintegral = 0;


void setup() {
  Serial.begin(9600);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, CHANGE); 
  
  pinMode(PWM, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  Serial.println("Target position");
}

void loop() {
  int target = 2000; 

  float kp = 1.0;
  float kd = 0.05; 
  float ki = 0.0; 

  long currT = micros();
  float deltaT = ((float)(currT - prevT)) / (1.0e6);
  prevT = currT;

  int pos = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = posi;
  }
  
  int e = target - pos;

  if (abs(e) < 5) {  
      eintegral = 0;  
      eprev = 0;      
      setMotor(2, 0, PWM, IN1, IN2);  
      Serial.println("Motor stopped at target");
      return;
  }

  if (abs(e) < 50) {
    eintegral += e * deltaT;
  } else {
    eintegral = 0;
  }


  float dedt = (e - eprev) / deltaT;
  float u = kp * e + kd * dedt + ki * eintegral;
  
  float pwr = fabs(u);
  if (pwr > 255) pwr = 255;

  int dir = (u > 0) ? 1 : -1;

  setMotor(dir, pwr, PWM, IN1, IN2);

  eprev = e;

  Serial.print("Target: ");
  Serial.print(target);
  Serial.print(" | Position: ");
  Serial.println(pos);
}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2) {
  if (dir == 1) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
  } else if (dir == -1) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
  } else if (dir == 2) {  
      digitalWrite(in1, HIGH);
      digitalWrite(in2, HIGH);
      analogWrite(pwm, 0);
  } else {  
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      analogWrite(pwm, 0);
  }
}

void readEncoder() {
  int b = digitalRead(ENCB);
  if (b > 0) {
    posi--;
  } else {
    posi++;
  }
}
