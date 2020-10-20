#include <Servo.h>

Servo R_MASK_SERVO;
Servo L_MASK_SERVO;
Servo R_EAR_SERVO;
Servo L_EAR_SERVO;


int R_EYE_RED = 3;
int L_EYE_RED = 3;
int R_EYE_BLUE = 5;
int L_EYE_BLUE = 5;

int ARC_REACTOR_RED = 3;
int ARC_REACTOR_BLUE = 5;

int R_MASK_SERVO_PIN =  6;
int L_MASK_SERVO_PIN = 9;
int R_EAR_SERVO_PIN = 10;
int L_EAR_SERVO_PIN = 11;

int R_EAR_LIGHT = 12;
int L_EAR_LIGHT = 12;

int R_START_POS = 0;
int L_START_POS = 180;
int R_END_POS = 0;
int L_END_POS = 180;

boolean is_mask_open = false;
boolean is_instakill_enabled = false;

void ear_lights(boolean on=true){
  if on{
    //ear lights on
  } else{
    //ear lights off
  }
}

void mask_lights(boolean on=true, boolean armed=false){
  if (on){
    if (armed){

    }
  }
}

void open_mask(Servo *right_mask, Servo *left_mask, boolean open=true){
  if (open){
    for(int i=0; i<90; i++){
      right_mask->write(90-i);
      left_mask->write(90+i);
      delay(15);
    }
  } else{
    for(int i=0; i<90; i++){
      right_mask->write(i);
      left_mask->write(180-i);
      delay(15);
    }
  }

}
void rotate_ears(Servo *right_ear, Servo *left_ear, boolean ears_up=true){
  if (ears_up){
    for (int i=0; i <90; i++){
      right_ear->write(90 + i);
      left_ear->write(90 - i);
      delay(15);
    }
  } else{
    for (int i=0; i<90; i++){
      right_ear->write(180 - i);
      left_ear->write(0 + i);
      delay(15);
    }
  }
}


void setup() {
  
  Serial.begin(9600);
  // pinMode(R_EYE_BLUE, OUTPUT);
  // pinMode(R_EYE_RED, OUTPUT);
  R_MASK_SERVO.attach(R_MASK_SERVO_PIN);
  L_MASK_SERVO.attach(L_MASK_SERVO_PIN);
  
  R_EAR_SERVO.attach(R_EAR_SERVO_PIN);
  L_EAR_SERVO.attach(L_EAR_SERVO_PIN);


}

void loop() {
  if (Serial.available()){
    delay(100);
    int api_is_instakill_enabled;
    int api_is_mask_open; 
    while (Serial.available() >0){
      
      Serial.read();
      api_is_mask_open = Serial.parseInt();
      Serial.read();
      api_is_instakill_enabled = Serial.parseInt();
      if (api_is_instakill_enabled and !is_instakill_enabled){
        // close mask
        // ears down
        // lights red
      } else if (is_instakill_enabled and !api_is_instakill_enabled){
        // turn off insta kill
        // return ears
        // eyes blue, ears off
      } else if (api_is_mask_open and !is_mask_open){
        is_mask_open = true;


      } else if (is_mask_open and !api_is_mask_open){
        // close mask
      }

    }
  }
}
