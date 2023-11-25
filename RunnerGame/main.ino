#include <LiquidCrystal.h>
#include "main.h"

// Initialize
LiquidCrystal lcd(
    RS,
    E,
    DATA_INPUT1,
    DATA_INPUT2,
    DATA_INPUT3,
    DATA_INPUT4);

int obstacle_initial_position = 0;
int player_cord[2] = {15, 1};
int dead = 0;
int points = 0;
int speedModifier = 0;

int melody[8] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int noteDurations[8] = {
    4, 8, 8, 4, 4, 4, 4, 4};

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
}
void loop()
{
  if (dead == 0)
  {
    play_game();
  }
  if (dead == 1)
  {
    display_game_over();
  }
}

void display_game_over()
{
  lcd.setCursor(0, 0);
  lcd.print("Game over");
  lcd.setCursor(0, 1);
  lcd.print("pts:");
  lcd.setCursor(4, 1);
  lcd.print(points);

  if (analogRead(A0) <= 200 || analogRead(A0) >= 900)
  {
    dead = 0;
    points = 0;
    speedModifier = 0;
    lcd.clear();
  }
}

void play_game()
{
  lcd.setCursor(player_cord[0], player_cord[1]);
  lcd.print(PLAYER_SYMBOL);
  if (analogRead(A0) <= 200)
  {
    clear_lcd_char_at(15, 0);
    change_player_y(1);
  }
  if (analogRead(A0) >= 900)
  {
    clear_lcd_char_at(15, 1);
    change_player_y(0);
  }
  obstacle_initial_position += 1;
  int obstacle_cord[2] = {obstacle_initial_position % 16, 0};
  lcd.setCursor(obstacle_cord[0], obstacle_cord[1]);
  lcd.print(OBSTACLE_SYMBOL);
  if (obstacle_cord[0] == player_cord[0] &&
      obstacle_cord[1] == player_cord[1])
  {
    lcd.setCursor(obstacle_cord[0], obstacle_cord[1]);
    lcd.print(DEAD_SYMBOL);
    delay(400);
    lcd.clear();
    dead = 1;
    play_end_sound();
  }
  clear_lcd_char_at(obstacle_cord[0] - 1, obstacle_cord[1]);

  obstacle_cord[1] = 1;
  lcd.setCursor(obstacle_cord[0] + 2, obstacle_cord[1]);
  lcd.print(OBSTACLE_SYMBOL);
  if (obstacle_cord[0] + 2 == player_cord[0] &&
      obstacle_cord[1] == player_cord[1])
  {
    lcd.setCursor(obstacle_cord[0], obstacle_cord[1]);
    lcd.print(DEAD_SYMBOL);
    delay(400);
    lcd.clear();
    dead = 1;
    play_end_sound();
  }
  clear_lcd_char_at(obstacle_cord[0] + 1, obstacle_cord[1]);

  delay(150 - speedModifier);
  points += 2;
  speedModifier += 1;
}

void change_player_y(int y_idx)
{
  player_cord[1] = y_idx;
  lcd.setCursor(15, player_cord[1]);
  lcd.print(PLAYER_SYMBOL);
}

void clear_lcd_char_at(int x, int y)
{
  lcd.setCursor(x, y);
  lcd.print(" ");
}

void play_end_sound()
{
  for (int thisNote = 0; thisNote < 8; thisNote++)
  {

    // to calculate the note duration, take one second divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER_PIN);
  }
}