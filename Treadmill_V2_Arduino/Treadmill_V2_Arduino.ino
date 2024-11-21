// Custom Classes
#include "lick_sensor.h"
#include "solenoid.h"
#include "trig.h"
#include "prog.h"
#include "LED.h"
#include "probabilityFunctions.h"

// Menu indices
int Trial = 0;
int Type = 1;
int Event = 2;
int Time = 3;

soundfiles_list = {
  "1kHz.wav",
  "2kHz.wav",
  "3kHz.wav",
  "4kHz.wav",
  "5kHz.wav"
}

trial_typenames = {
  "REWARD",
  "reward",
  "AIRPUFF",
  "airpuff",
  "zilch"
}

// Initializations
long experiment_start_time;
long trial_start_time;
long current_trial;
String choice;

// Pin Assignments
trig trig1(2);                  // D2   Trig1
                                // D3   MusicMaker DREQ
                                // D4   MusicMaker CCS
trig trig2(5);                  // D5   Trig2
                                // D6   MusicMaker DCS
                                // D7   MusicMaker MCS
LED CueL(8);                    // D8   CueL
LED CueR(9);                    // D9   CueR
lick_sensor lick(10);           // D10  Lick
prog prog(11);                  // D11  Prog
solenoid reward_solenoid(12);   // D12  reward
solenoid airpuff_solenoid(13);  // D13  airpuff

// LCD Screen
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
String menu_categories[4] = {"", "", "", ""};
String updatable_menu[4] = {"", "", "", ""};

// Parameters
struct configurable {

  String session;
  String animal_code;
  int n_trials;
  int cs_delay;

} configurable;

struct fixed {
  
  long big_reward;
  long small_reward;
  long big_puff;
  long small_puff;
  long iti;

} fixed;

void setup() {
  Serial.begin(115200);
  Serial.flush();

  // LCD Initializaation
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("Ready");
  lcd.setCursor(0, 3); lcd.print("Treadmill_v2.0");
  
  // Fixed
  fixed.big_reward = 200;
  fixed.small_reward = 50;
  fixed.big_puff = 200;
  fixed.small_puff = 50;
  fixed.iti = 2000;
  
  // Configurable
  configurable.session = read_config();
  configurable.animal_code = read_config();
  configurable.n_trials = read_config();
  configurable.cs_delay = read_config();

  Serial.flush();

  //      Display Configurables on LCD
  menu_categories[0] = "Session";
  menu_categories[1] = "Animal";
  menu_categories[2] = "n_trials";
  menu_categories[3] = "cs_delay";
  initialize_menu();
  updatable_menu[0] = String(configurable.session);
  updatable_menu[1] = String(configurable.animal_code);
  updatable_menu[2] = String(configuration.n_trials);
  updatable_menu[3] = (String((configuration.cs_delay/1000)) + "s");
  update_menu();

  delay(5000); // Hold on this for 5 seconds
  
  //      Initialize Menu
  menu_categories[Trial_idx] = "Trial";
  menu_categories[Type_idx] = "Type";
  menu_categories[Event_idx] = "Event";
  menu_categories[Time_idx] = "Time (s)";
  initialize_menu();

  // initialize
  experiment_start_time = millis();
  trial_start_time = millis();
  current_trial = 1;
}

void loop() {
  
  type = get_choice(5);
  
  updatable_menu[Trial_idx] = String(current_trial); update_menu();
  updatable_menu[Type_idx] = trial_typenames{type}; update_menu();
  
  prog.pulsecode(type); 
  musicMaker.play_full_file(soundfile_list{type});
  updatable_menu[Event_idx] = "Cued"; update_menu();
  updatable_menu[Time_idx] = String((millis() - experiment_start_time)/1000); update_menu();
  
  delay(configurable.cs_delay*1000);
  
  switch (choice) {
    case 1: // Big Reward
      reward_solenoid.pulse_valve(fixed.big_reward);   
      updatable_menu[Event_idx] = "REWARDED"; update_menu();
    case 2: // Small Reward
      reward_solenoid.pulse_valve(fixed.small_reward);   
      updatable_menu[Event_idx] = "rewarded"; update_menu();
    case 3: // Big Puff
      airpuff_solenoid.pulse_valve(fixed.big_puff);   
      updatable_menu[Event_idx] = "PUFFED"; update_menu();
    case 4: // Small Puff
      airpuff_solenoid.pulse_valve(fixed.small_puff);   
      updatable_menu[Event_idx] = "puffed"; update_menu();
    case 5: // Nothing
      updatable_menu[Event_idx] = "zilch"; update_menu();
  }

  updatable_menu[Time_idx] = String((millis() - experiment_start_time)/1000); update_menu();
  delay(configurable.iti);
  current_trial += 1;
  trial_start_time = millis();

  // Complete
  if (current_trial > configurable.n_trials) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Complete!");
    Serial.println("fin");
    while(1);
  }

}

// Config
long int read_config() {
  while (!Serial.available()) {}
  return Serial.parseInt();
}

// Menu functions 
void initialize_menu() {
  lcd.clear();

  lcd.setCursor(12, 0);
  lcd.print(menu_categories[0]);

  lcd.setCursor(12, 1);
  lcd.print(menu_categories[1]);

  lcd.setCursor(12, 2);
  lcd.print(menu_categories[2]);

  lcd.setCursor(12, 3);
  lcd.print(menu_categories[3]);
}

void update_menu() {
  initialize_menu();

  lcd.setCursor(0, 0);
  lcd.print(updatable_menu[0]);

  lcd.setCursor(0, 1);
  lcd.print(updatable_menu[1]);

  lcd.setCursor(0, 2);
  lcd.print(updatable_menu[2]);

  lcd.setCursor(0, 3);
  lcd.print(updatable_menu[3]);
}