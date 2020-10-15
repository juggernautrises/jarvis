  int R_EYE_RED = 3;
  int L_EYE_RED = 3;
  int R_EYE_BLUE = 5;
  int L_EYE_BLUE = 5;

  int R_MASK =  6;
  int L_MASK = 9;
  int R_EAR_SERVO = 10;
  int L_EAR_SERVO = 11;
  int R_EAR_LIGHT = 12;
  int L_EAR_LIGHT = 12;
  boolean is_mask_open = false;
  boolean is_instakill_enabled = false;

void setup() {
  
  Serial.begin(9600);
  pinMode(R_EYE_BLUE, OUTPUT);
  pinMode(R_EYE_RED, OUTPUT);
  
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
        // open mask
      } else if (is_mask_open and !api_is_mask_open){
        // close mask
      }
      
      
      if (api_is_mask_open == 1 and !is_mask_open){
        is_mask_open = true;
        analogWrite(R_EYE_RED, 0);        
      } else if (api_is_mask_open == 0 and  is_mask_open){
        is_mask_open = false;
        analogWrite(R_EYE_RED, 255);        
      }
    }
  }
}
