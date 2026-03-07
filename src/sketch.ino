#include <Arduino.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int read_button = 8;

int game_array[2][16] = {0};
int player_state = 1;
bool game_on = true;

int rows = 2, cols = 16;

int max_jump_time = 600;
long time_last_jump = 0;
int jump_delay = 300;
long time_last_fall = 0;

int min_time_between_obstacle = 500;
long time_between_obstacle = 0;
int time_between_scenery_move = 200;
long time_scenery_moved = 0;
int previous_obstacle = 0;

int min_time_between_lane_toggle = 100;
long last_lane_toggle_time = 0;

long game_time_start = millis();

void print_array()
{

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      Serial.print(game_array[i][j]);
    }
    Serial.println();
  }
}

bool can_jump()
{
  int ground_time = millis() - time_last_fall;
  if (player_state == 1 && ground_time >= jump_delay)
    return true;
  else
    return false;
}

bool jump()
{

  if (can_jump())
  {
    player_state = 0;

    time_last_jump = millis();

    return true;
  }
  else
    return false;
}

bool can_fall()
{
  int air_time = millis() - time_last_jump;
  if (player_state == 0 && air_time >= max_jump_time)
    return true;
  else
    return false;
}

bool fall()
{

  if (can_fall())
  {
    player_state = 1;

    time_last_fall = millis();

    return true;
  }
  else
    return false;
}

void toggle_lane()
{
  int since_last_lane_toggle = millis() - last_lane_toggle_time;

  if (since_last_lane_toggle < min_time_between_lane_toggle) return;

  if (player_state == 0)
    player_state = 1;
  else
    player_state = 0;
  
    last_lane_toggle_time = millis();
}

void put_player_in_game()
{
  lcd.setCursor(0, player_state);
  lcd.print(1);
}

void put_player_in_game_graphics()
{
  lcd.setCursor(0, player_state);
  lcd.print(">");
}

void move_scenery()
{

  int time_last_scenery = millis() - time_scenery_moved;

  if (time_last_scenery < time_between_scenery_move)
    return;

  for (int i = 0; i < rows; i++)
  {
    for (int j = 1; j < cols; j++)
    {

      game_array[i][j - 1] = game_array[i][j];
      game_array[i][j] = 0;
    }
  }
  time_scenery_moved = millis();
  add_obstacle();
}

int create_obstacle()
{

  int time_last_obstacle = millis() - time_between_obstacle;

  // if (time_last_obstacle < min_time_between_obstacle)
  // {
  //   // Serial.print("Failed\n");
  //   return 0;
  // }

  int fail_in_rolls = 6;
  int rolls = 2 + fail_in_rolls;
  int obstecle = random(rolls);
  // Serial.print(obstecle);
  time_between_obstacle = millis();

  if (obstecle < rolls - 2)
  {
    return 0;
  }

  if (obstecle == rolls - 2)
  {
    return 1;
  }

  if (obstecle == rolls - 1)
  {
    return 2;
  }
}

void add_obstacle()
{
  int obstacle = create_obstacle();

  if (obstacle == 0)
  {
    previous_obstacle = 0;
    return;
  }

  if (obstacle == 1 || obstacle == 2 && previous_obstacle == 1)
  {
    game_array[1][cols - 1] = 2;
    previous_obstacle = 1;
    return;
  }

  if (obstacle == 2 || obstacle == 1 && previous_obstacle == 2)
  {
    game_array[0][cols - 1] = 2;
    previous_obstacle = 2;
    return;
  }
}

bool is_collision()
{
  if (game_array[player_state][0] == 2)
    return true;

  else
    return false;
}

void lose_screen()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lose Play again?");
  lcd.setCursor(0, 1);
  lcd.print((millis() - game_time_start) / 1000);
  lcd.print(" s");
}

void collision_logic()
{
  if (is_collision())
  {
    lose_screen();
    game_on = false;
  }
  else
  {
    lcd.clear();
    print_lcd_graphics();
    put_player_in_game_graphics();
  }
}

int jump_time()
{
  int time_jump_elapsed = millis() - time_last_jump;
}

void print_lcd()
{

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      lcd.setCursor(j, i);
      lcd.print(game_array[i][j]);
    }
    // Serial.println();
  }
}

void print_lcd_graphics()
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      lcd.setCursor(j, i);
      int game_object = game_array[i][j];

      switch (game_object)
      {
      case 0:
        break;
      case 1:
        lcd.print(">");
        break;
      case 2:
        lcd.print("-");
        break;
      }
    }
    // Serial.println();
  }
}

void clear_game_array()
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      game_array[i][j] = 0;
    }
  }
}

void setup()
{

  lcd.begin(16, 2);
  // lcd.print("This a __");

  pinMode(read_button, INPUT);
  Serial.begin(9600);

  // print_array();
  game_array[1][0] = 1;
}

void loop()
{
  int buttonState = digitalRead(read_button);
  if (game_on)
  {

    // lcd.setCursor(0, 1);
    // lcd.print(millis() / 1000);

    if (buttonState == HIGH)
    {
      // jump();
      toggle_lane();
    }
    // fall();

    // add_obstacle();
    move_scenery();
    collision_logic();

    // Serial.println("\n\n\n\n\n\n\n\n\n\n");
    // print_array();
    // print_lcd();

    delay(50);
  }
  else
  {
    if (buttonState == HIGH)
    {
      game_on = true;
      clear_game_array();

      delay(200);
      game_time_start = millis();
      Serial.println(game_time_start / 1000);
    }
  }
}
