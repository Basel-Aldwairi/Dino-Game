#include <Arduino.h>
#include <LiquidCrystal.h>


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int read_button = 8;

int game_array[2][16] = {0}; 

int rows = 2, cols = 16;

int max_jump_time = 500;
int time_last_jump = 0;
int jump_delay = 500;
int time_last_fall = 0;


void print_array(){

  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      Serial.print(game_array[i][j]);
    }
    Serial.println();
  }


}

bool can_jump(){
  int ground_time = millis() - time_last_fall;
  if (game_array[1][0] == 1 && ground_time >= jump_delay) return true;
  else return false;
}

bool jump(){

  if(can_jump()){
    game_array[0][0] = 1;
    game_array[1][0] = 0;

    time_last_jump = millis();
    
    return true;
  }
  else return false;
  
}

bool can_fall(){
  int air_time = millis() - time_last_jump;
  if (game_array[0][0] == 1 && air_time >= max_jump_time) return true;
  else return false;
}

bool fall(){

  if(can_fall()){
    game_array[0][0] = 0;
    game_array[1][0] = 1;
  
    time_last_fall = millis();

    return true;
  }
  else return false;

}

bool collision(){
}

int jump_time(){

  int time_jump_elapsed = millis() - time_last_jump;

}

void print_lcd(){

  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      lcd.setCursor(j, i);
      lcd.print(game_array[i][j]);
    }
    // Serial.println();
  }
}



void setup() {

  lcd.begin(16, 2);
  lcd.print("This a __");

  pinMode(read_button, INPUT);
  Serial.begin(9600);

  // print_array();
  game_array[1][0] = 1;

}

void loop() {

  // lcd.setCursor(0, 1);
  // lcd.print(millis() / 1000);

  int buttonState = digitalRead(read_button);
  if(buttonState == HIGH){
    jump();
  }
  fall();


  Serial.println("\n\n\n\n\n\n\n\n\n\n");
  // print_array();
  print_lcd();

  delay(50);

}
