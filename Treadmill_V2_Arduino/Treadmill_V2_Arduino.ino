// Audio Libraries
/* #include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
*/

// Custom Classes
#include "lick_sensor.h"
#include "solenoid.h"
#include "trig.h"
#include "prog.h"
#include "LED.h"
#include "probabilityFunctions.h"

/*
// MusicMaker Shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)
#define CARDCS 4     // Card chip select pin
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
*/

// Menu indices
int Trial_idx = 0;
int Type_idx = 1;
int Event_idx = 2;
int Time_idx = 3;

/*
const char *soundfiles_list[] = {
  "/1kHz.wav",
  "/2kHz.wav",
  "/3kHz.wav",
  "/4kHz.wav",
  "/5kHz.wav"
};
*/

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

  int n_trials;
  int cs_delay;

} configurable;

// Fixed
int big_reward = 400;
int small_reward = 50;
int big_puff = 400;
int small_puff = 50;
long iti = 2000;
  

void setup() {
  Serial.begin(115200);
  Serial.flush();

/*
  // MusicPlayer
  if (! musicPlayer.begin()) {while (1);}
  if (!SD.begin(CARDCS)) {while (1);}
  musicPlayer.setVolume(20,20);
*/

  // LCD Initialization
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("Ready");
  lcd.setCursor(0, 3); lcd.print("Treadmill_v2.0");
  
  // Configurable
  configurable.n_trials = read_config();
  configurable.cs_delay = read_config();
  
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
  updatable_menu[Type_idx] = trial_typenames[type]; update_menu();
  
  prog.pulse_code(type); 
  updatable_menu[Event_idx] = "Cued"; update_menu();
  updatable_menu[Time_idx] = String((millis() - experiment_start_time)/1000); update_menu();
  
  delay(configurable.cs_delay*1000);
  
  switch (type) {
    case 1: // Big Reward
      reward_solenoid.pulse_valve(big_reward);   
      updatable_menu[Event_idx] = "REWARDED"; update_menu();
    case 2: // Small Reward
      reward_solenoid.pulse_valve(small_reward);   
      updatable_menu[Event_idx] = "rewarded"; update_menu();
    case 3: // Big Puff
      airpuff_solenoid.pulse_valve(big_puff);   
      updatable_menu[Event_idx] = "PUFFED"; update_menu();
    case 4: // Small Puff
      airpuff_solenoid.pulse_valve(small_puff);   
      updatable_menu[Event_idx] = "puffed"; update_menu();
    case 5: // Nothing
      updatable_menu[Event_idx] = "zilch"; update_menu();
  }

  updatable_menu[Time_idx] = String((millis() - experiment_start_time)/1000); update_menu();
  delay(iti);
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