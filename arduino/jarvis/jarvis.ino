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

int LIGHT_MIN = 0;
int LIGHT_MAX = 255;

boolean is_mask_open = false;
boolean is_instakill_enabled = false;

void ear_lights(boolean on=true){
  int light_val = LIGHT_MIN;
  if (on){
    light_val = LIGHT_MAX;
  }
  analogWrite(R_EAR_LIGHT, light_val);
  analogWrite(L_EAR_LIGHT, light_val);
}

void hostile_eyes(boolean hostile=true){
  int r_eye_blue_val = LIGHT_MIN;
  int l_eye_blue_val = LIGHT_MIN;
  int r_eye_red_val = LIGHT_MIN;
  int l_eye_red_val = LIGHT_MIN;
  if (hostile){
    r_eye_red_val = LIGHT_MAX;
    l_eye_red_val = LIGHT_MAX;
  } else{
    r_eye_blue_val = LIGHT_MAX;
    l_eye_blue_val = LIGHT_MAX;
  }
  analogWrite(R_EYE_BLUE,r_eye_blue_val);
  analogWrite(L_EYE_BLUE,l_eye_blue_val);
  analogWrite(R_EYE_RED,r_eye_red_val);
  analogWrite(L_EYE_RED,l_eye_red_val);
}

void mask_lights(boolean on=true){
  if (on){
    for (int i=LIGHT_MIN; i<LIGHT_MAX; i++){
      analogWrite(R_EYE_BLUE,i);
      analogWrite(L_EYE_BLUE,i);
      delay(10);
    }
  } else{
    for (int i=LIGHT_MAX; i>=LIGHT_MIN; i--){
      analogWrite(R_EYE_BLUE,i);
      analogWrite(L_EYE_BLUE,i);
      delay(10);
    }
  }
}

void open_mask(Servo *right_mask, Servo *left_mask, boolean close_visor=true){
  if (close_visor){
    for(int i=0; i<50; i++){
      right_mask->write(50 - i);
      left_mask->write(130 + i);
      delay(10);
    }
  } else{
    for(int i=0; i<50; i++){
      right_mask->write(i);
      left_mask->write(180 - i);
      delay(10);
    }
  }

}
boolean rotate_ears(Servo *right_ear, Servo *left_ear, boolean ears_up=true){
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
      delay(10);
    }
  }
  return true;
}


void setup() {
  
  Serial.begin(9600);
  pinMode(R_EYE_BLUE, OUTPUT);
  pinMode(R_EYE_RED, OUTPUT);
  pinMode(R_EAR_LIGHT, OUTPUT);
  pinMode(L_EYE_BLUE, OUTPUT);
  pinMode(L_EYE_RED, OUTPUT);
  pinMode(L_EAR_LIGHT, OUTPUT);

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
    int sync =1;
    while (Serial.available() >0 || sync ==1){
      sync = 2;
      Serial.read();
      api_is_mask_open = Serial.parseInt();
      Serial.read();
      api_is_instakill_enabled = Serial.parseInt();
      if (api_is_instakill_enabled and !is_instakill_enabled){
        // close mask it not already close
        if (is_mask_open){
          open_mask(&R_MASK_SERVO, &L_MASK_SERVO);
        }
        // eyes red
        hostile_eyes();
        // ears down
        rotate_ears(&R_EAR_SERVO, &L_EAR_SERVO, false);
        // ear lights on
        ear_lights();
        is_instakill_enabled = true;
        is_mask_open = false;
      } else if (is_instakill_enabled and !api_is_instakill_enabled){
        // ear lights off
        ear_lights(false);
        // ears up
        rotate_ears(&R_EAR_SERVO, &L_EAR_SERVO);
        // eyes back to blue
        hostile_eyes(false);
        is_instakill_enabled = false;
      } else if (api_is_mask_open and !is_mask_open){
        mask_lights(false);
        open_mask(&R_MASK_SERVO, &L_MASK_SERVO, false);
        is_mask_open = true;
      } else if (is_mask_open and !api_is_mask_open){
        is_mask_open = false;
        open_mask(&R_MASK_SERVO, &L_MASK_SERVO);
        mask_lights();
        // close mask
      }

    }
  }
}
