// Custom Classes
#include "lick_sensor.h"
#include "solenoid.h"
#include "trig.h"
#include "prog.h"
#include "LED.h"
#include "probabilityFunctions.h"

// Menu indices
int Trial_idx = 0;
int Type_idx = 1;
int Event_idx = 2;
int Time_idx = 3;

String trial_typenames[] = {
  "REWARD",
  "reward",
  "AIRPUFF",
  "airpuff",
  "zilch"
};

// Initializations
long experiment_start_time;
long trial_start_time;
long current_trial;
int type;
bool p_cue_decision;
bool p_deliver_decision;

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

  long int n_trials;
  long int cs_delay;
  long int cs_delay_ms;
  long int iti;
  long int iti_ms;
  long int p_cue;
  long int p_deliver;

} configurable;

// Fixed
int big_reward = 400;
int small_reward = 50;
int big_puff = 400;
int small_puff = 50;
  

void setup() {
  Serial.begin(115200);
  Serial.flush();

  // LCD Initialization
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Treadmill_v2.0");
  lcd.setCursor(0, 1); lcd.print("Ready");
  
  // Configurable
  configurable.n_trials = read_config();
  configurable.cs_delay = read_config();
  configurable.iti = read_config();

  configurable.cs_delay_ms = configurable.cs_delay * 1000;
  configurable.iti_ms = configurable.iti * 1000;

  Serial.flush();

  lcd.setCursor(0, 2); lcd.print("Configured"); delay(2000);
  lcd.setCursor(0, 3); lcd.print("lets get this bread"); delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Params:");
  lcd.setCursor(0, 1); lcd.print("n_trials: " + String(configurable.n_trials));
  lcd.setCursor(0, 2); lcd.print("cs_delay: " + String(configurable.cs_delay));
  lcd.setCursor(0, 3); lcd.print("iti: " + String(configurable.iti));

  delay(5000);
  
  //      Initialize Menu
  menu_categories[Trial_idx] = "Trial";
  menu_categories[Type_idx] = "Type";
  menu_categories[Event_idx] = "Event";
  menu_categories[Time_idx] = "Elasped (s)";
  initialize_menu();

  // Photometry Ready ON Signal
  trig1.trig_on();

  // initialize
  experiment_start_time = millis();
  trial_start_time = millis();
  current_trial = 1;
}

void loop() {
  
  type = get_choice(5);
  
  updatable_menu[Trial_idx] = String(current_trial); update_menu();
  updatable_menu[Type_idx] = trial_typenames[type-1]; update_menu();
  Serial.flush();
  
  // play the cue 90% of the time
  p_cue_decision = enforce_prob(configurable.p_cue);
  switch(p_cue_decision) {
    case true:
      Serial.println(String(type));
      prog.pulse_code(type);
      updatable_menu[Event_idx] = "Cued"; update_menu();
      updatable_menu[Time_idx] = String((millis() - experiment_start_time)/1000); update_menu();
      break;
    case false:
      updatable_menu[Event_idx] = "Uncued"; update_menu();
      updatable_menu[Time_idx] = String((millis() - experiment_start_time)/1000); update_menu();
      break;
  }
  
  delay(configurable.cs_delay_ms);

  // Deliver the event 90% of the time
  p_deliver_decision = enforce_prob(configurable.p_deliver);
  switch(p_deliver_decision) {
    case true:
      switch (type) {
        case 1: // Big Reward
          reward_solenoid.pulse_valve(big_reward);
          break;   
        case 2: // Small Reward
          reward_solenoid.pulse_valve(small_reward);
          break;   
        case 3: // Big Puff
          airpuff_solenoid.pulse_valve(big_puff);
          break;   
        case 4: // Small Puff
          airpuff_solenoid.pulse_valve(small_puff);
          break;   
        case 5: // Nothing
          break;
      }
      updatable_menu[Event_idx] = "Delivered"; update_menu();
      updatable_menu[Time_idx] = String((millis() - experiment_start_time)/1000); update_menu();
      break;
    case false:
      updatable_menu[Event_idx] = "Omitted"; update_menu();
      updatable_menu[Time_idx] = String((millis() - experiment_start_time)/1000); update_menu();
      break;
  }

  delay(configurable.iti_ms);
  current_trial += 1;
  trial_start_time = millis();

  // Complete
  if (current_trial > configurable.n_trials) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Complete!");
    trig1.trig_off(); // Photometry Ready OFF Signal
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