#include <Arduino.h>

#define ENCA1 2  
#define ENCB1 4  

#define IN1A 6    
#define IN1B 7   

#define ENCA2 1   
#define ENCB2 5   

#define IN2A 8    
#define IN2B 9    

#define ENCA3 3  
#define ENCB3 10  

#define IN3A 11   
#define IN3B 12   

volatile int position1 = 0;  
volatile int position2 = 0;  
volatile int position3 = 0;  
const int targetPosition1 = 10;  
const int targetPosition2 = 10;  
const int targetPosition3 = 10;  
bool motor1Stopped = false;
bool motor2Stopped = false;
bool motor3Stopped = false;

void IRAM_ATTR readEncoder1() {
    if (digitalRead(ENCB1) == HIGH) {  
        position1++;  
    } else {  
        position1--;  
    }
}

void IRAM_ATTR readEncoder2() {
    if (digitalRead(ENCB2) == HIGH) {  
        position2--;  
    } else {  
        position2++;  
    }
}

// **Encoder Interrupt for Motor 3**
void IRAM_ATTR readEncoder3() {
    if (digitalRead(ENCB3) == HIGH) {  
        position3--;  
    } else {  
        position3++;  
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(ENCA1, INPUT_PULLUP);
    pinMode(ENCB1, INPUT_PULLUP);
    pinMode(ENCA2, INPUT_PULLUP);
    pinMode(ENCB2, INPUT_PULLUP);
    pinMode(ENCA3, INPUT_PULLUP);
    pinMode(ENCB3, INPUT_PULLUP);
    
    attachInterrupt(digitalPinToInterrupt(ENCA1), readEncoder1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCA2), readEncoder2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCA3), readEncoder3, CHANGE);

    pinMode(IN1A, OUTPUT);
    pinMode(IN1B, OUTPUT);
    pinMode(IN2A, OUTPUT);
    pinMode(IN2B, OUTPUT);
    pinMode(IN3A, OUTPUT);
    pinMode(IN3B, OUTPUT);

    Serial.println("Three Motors Running at Full Speed");

    digitalWrite(IN1A, LOW); 
    digitalWrite(IN1B, HIGH);

    digitalWrite(IN2A, HIGH);  
    digitalWrite(IN2B, LOW);

    digitalWrite(IN3A, HIGH);  
    digitalWrite(IN3B, LOW);
}

void loop() {
    int pos1, pos2, pos3;
    
    noInterrupts();
    pos1 = position1;
    pos2 = position2;
    pos3 = position3;
    interrupts();

    if (pos1 >= targetPosition1 && !motor1Stopped) {
        stopMotor1();
        Serial.println("Motor 1 Stopped at Target");
        motor1Stopped = true;
    }

    if (pos2 >= targetPosition2 && !motor2Stopped) {
        stopMotor2();
        Serial.println("Motor 2 Stopped at Target");
        motor2Stopped = true;
    }

    if (pos3 >= targetPosition3 && !motor3Stopped) {
        stopMotor3();
        Serial.println("Motor 3 Stopped at Target");
        motor3Stopped = true;
    }

    Serial.print("Motor 1 Target: "); Serial.print(targetPosition1);
    Serial.print(" | Position: "); Serial.print(pos1);
    Serial.print(" || Motor 2 Target: "); Serial.print(targetPosition2);
    Serial.print(" | Position: "); Serial.print(pos2);
    Serial.print(" || Motor 3 Target: "); Serial.print(targetPosition3);
    Serial.print(" | Position: "); Serial.println(pos3);

    delay(500);
}

void stopMotor1() {
    digitalWrite(IN1A, LOW);
    digitalWrite(IN1B, LOW);
}

void stopMotor2() {
    digitalWrite(IN2A, LOW);
    digitalWrite(IN2B, LOW);
}

void stopMotor3() {
    digitalWrite(IN3A, LOW);
    digitalWrite(IN3B, LOW);
}
