import processing.serial.*;
import controlP5.*;
import ddf.minim.*;

Serial arduino_Obj;
ControlP5 cp5;
Minim minim;
AudioPlayer[] audioFiles = new AudioPlayer[5];

// defaults
int n_trials = 100; 
int us_delay = 5;  
int iti = 30;       
int prob = 100;

// Spacing
int leftMargin = 20;
int rightMargin = 20;
int sectionSpacing = 20; // Spacing between sections
int appWidth = 420; // Wider application
int appHeight = 320; // Application height

// CS-US pairing variables
boolean bigR_togglestate = true;
int bigR_p_cue = 90;
int bigR_p_deliver = 90;

boolean smallR_togglestate = true;
int smallR_p_cue = 90;
int smallR_p_deliver = 90;

boolean bigA_togglestate = true;
int bigA_p_cue = 90;
int bigA_p_deliver = 90;

boolean smallA_togglestate = true;
int smallA_p_cue = 90;
int smallA_p_deliver = 90;

boolean nothing_togglestate = true;
int nothing_p_cue = 90;
int nothing_p_deliver = 90;

void settings() {
  size(appWidth, appHeight);
}

void setup() {
  cp5 = new ControlP5(this);

  // Title
  cp5.addLabel("Treadmill v2.0")
     .setPosition(leftMargin, 20)
     .setFont(createFont("Gill Sans MT", 24))
     .setColorValue(color(0));
  
  cp5.addButton("Reinitialize Arduino")
     .setPosition(appWidth - (200 + rightMargin), 10)
     .setSize(200, 40)
     .getCaptionLabel()
     .setFont(createFont("Gill Sans MT", 16));

  // Configuration fields
  int configY = 60; // Y position for the configuration row
  int heightOffset = 20;
  int fieldOffset = 180;
  int fieldHeight = 20;
  int fieldWidth = 200;

  // n_trials
  cp5.addLabel("Number of Trials:")
     .setPosition(leftMargin, configY)
     .setFont(createFont("Gill Sans MT", 14))
     .setColorValue(color(0));
  cp5.addSlider("n_trials")
     .setPosition(leftMargin + fieldOffset, configY)
     .setSize(fieldWidth, fieldHeight)
     .setRange(1, 500)
     .setValue(n_trials);

  
  // us_delay
  cp5.addLabel("US Delay (s):")
     .setPosition(leftMargin, configY + heightOffset)
     .setFont(createFont("Gill Sans MT", 14))
     .setColorValue(color(0));
  cp5.addSlider("us_delay")
     .setPosition(leftMargin + fieldOffset, configY + heightOffset)
     .setSize(fieldWidth, fieldHeight)
     .setRange(1, 10)
     .setValue(us_delay);
  
  // iti
  cp5.addLabel("Inter-Trial Interval (s):")
     .setPosition(leftMargin, configY + heightOffset * 2)
     .setFont(createFont("Gill Sans MT", 14))
     .setColorValue(color(0));
  cp5.addSlider("iti")
     .setPosition(leftMargin + fieldOffset, configY + heightOffset * 2)
     .setSize(fieldWidth, fieldHeight)
     .setRange(1, 60)
     .setValue(iti);

  cp5.addLabel("p_cue")
   .setPosition(leftMargin + 215, configY + 70)
   .setFont(createFont("Gill Sans MT", 14))
   .setColorValue(color(0));
  cp5.addLabel("p_deliver")
   .setPosition(leftMargin + 300, configY + 70)
   .setFont(createFont("Gill Sans MT", 14))
   .setColorValue(color(0));

  // CS-US pairings
  String[] cueDescriptions = {
    "1kHz : Big Reward",
    "2kHz : Small Reward",
    "3kHz : Big Airpuff",
    "4kHz : Small Airpuff",
    "5kHz : Nothing"
  };

  int cueStartY = configY + 90; // Start below the configuration section
  int rowHeight = 20; // Height per row for CS-US pairings

    // Big Reward (1kHz)
  cp5.addLabel(cueDescriptions[0])
     .setPosition(leftMargin, cueStartY)
     .setFont(createFont("Gill Sans MT", 14))
     .setColorValue(color(0));
  cp5.addToggle("bigR_togglestate")
     .setPosition(leftMargin + 180, cueStartY)
     .setSize(20, 20)
     .getCaptionLabel()
     .set("Enable")
     .toUpperCase(false);
  cp5.addSlider("bigR_p_cue")
     .setPosition(leftMargin + 200, cueStartY)
     .setSize(90, 20)
     .setRange(1, 100)
     .setValue(bigR_p_cue)
     .setDecimalPrecision(1);
  cp5.addSlider("bigR_p_deliver")
     .setPosition(leftMargin + 290, cueStartY)
     .setSize(90, 20)
     .setRange(1, 100)
     .setValue(bigR_p_deliver)
     .setDecimalPrecision(1);

  // Small Reward (2kHz)
  cp5.addLabel(cueDescriptions[1])
     .setPosition(leftMargin, cueStartY + rowHeight)
     .setFont(createFont("Gill Sans MT", 14))
     .setColorValue(color(0));
  cp5.addToggle("smallR_togglestate")
     .setPosition(leftMargin + 180, cueStartY + rowHeight)
     .setSize(20, 20)
     .getCaptionLabel()
     .set("Enable")
     .toUpperCase(false);
  cp5.addSlider("smallR_p_cue")
     .setPosition(leftMargin + 200, cueStartY + rowHeight)
     .setSize(90, 20)
     .setRange(1, 100)
     .setValue(smallR_p_cue)
     .setDecimalPrecision(1);
  cp5.addSlider("smallR_p_deliver")
     .setPosition(leftMargin + 290, cueStartY + rowHeight)
     .setSize(90, 20)
     .setRange(1, 100)
     .setValue(smallR_p_deliver)
     .setDecimalPrecision(1);

  // Big Airpuff (3kHz)
  cp5.addLabel(cueDescriptions[2])
     .setPosition(leftMargin, cueStartY + 2 * rowHeight)
     .setFont(createFont("Gill Sans MT", 14))
     .setColorValue(color(0));
  cp5.addToggle("bigA_togglestate")
     .setPosition(leftMargin + 180, cueStartY + 2 * rowHeight)
     .setSize(20, 20)
     .getCaptionLabel()
     .set("Enable")
     .toUpperCase(false);
  cp5.addSlider("bigA_p_cue")
     .setPosition(leftMargin + 200, cueStartY + 2 * rowHeight)
     .setSize(90, 20)
     .setRange(1, 100)
     .setValue(bigA_p_cue)
     .setDecimalPrecision(1);
  cp5.addSlider("bigA_p_deliver")
     .setPosition(leftMargin + 290, cueStartY + 2 * rowHeight)
     .setSize(90, 20)
     .setRange(1, 100)
     .setValue(bigA_p_deliver)
     .setDecimalPrecision(1);

  // Small Airpuff (4kHz)
  cp5.addLabel(cueDescriptions[3])
     .setPosition(leftMargin, cueStartY + 3 * rowHeight)
     .setFont(createFont("Gill Sans MT", 14))
     .setColorValue(color(0));
  cp5.addToggle("smallA_togglestate")
     .setPosition(leftMargin + 180, cueStartY + 3 * rowHeight)
     .setSize(20, 20)
     .getCaptionLabel()
     .set("Enable")
     .toUpperCase(false);
  cp5.addSlider("smallA_p_cue")
     .setPosition(leftMargin + 200, cueStartY + 3 * rowHeight)
     .setSize(90, 20)
     .setRange(1, 100)
     .setValue(smallA_p_cue)
     .setDecimalPrecision(1);
  cp5.addSlider("smallA_p_deliver")
     .setPosition(leftMargin + 290, cueStartY + 3 * rowHeight)
     .setSize(90, 20)
     .setRange(1, 100)
     .setValue(smallA_p_deliver)
     .setDecimalPrecision(1);

  // Nothing (5kHz)
  cp5.addLabel(cueDescriptions[4])
     .setPosition(leftMargin, cueStartY + 4 * rowHeight)
     .setFont(createFont("Gill Sans MT", 14))
     .setColorValue(color(0));
  cp5.addToggle("nothing_togglestate")
     .setPosition(leftMargin + 180, cueStartY + 4 * rowHeight)
     .setSize(20, 20)
     .getCaptionLabel()
     .set("Enable")
     .toUpperCase(false);
  cp5.addSlider("nothing_p_cue")
     .setPosition(leftMargin + 200, cueStartY + 4 * rowHeight)
     .setSize(90, 20)
     .setRange(1, 100)
     .setValue(nothing_p_cue)
     .setDecimalPrecision(1);
  cp5.addSlider("nothing_p_deliver")
     .setPosition(leftMargin + 290, cueStartY + 4 * rowHeight)
     .setSize(90, 20)
     .setRange(1, 100)
     .setValue(nothing_p_deliver)
     .setDecimalPrecision(1);

  // Buttons
  cp5.addButton("Run")
     .setPosition(leftMargin, appHeight - 60)
     .setSize(appWidth - (leftMargin + rightMargin), 40)
     .getCaptionLabel()
     .setFont(createFont("Gill Sans MT", 16));

  // Initialize Arduino and load sounds
  initializeArduino();

  minim = new Minim(this);
  audioFiles[0] = minim.loadFile("1kHz.wav");
  audioFiles[1] = minim.loadFile("2kHz.wav");
  audioFiles[2] = minim.loadFile("3kHz.wav");
  audioFiles[3] = minim.loadFile("4kHz.wav");
  audioFiles[4] = minim.loadFile("5kHz.wav");
}

void draw() {
  background(240);

  // Check for serial input
  if (arduino_Obj.available() > 0) {
    String input = arduino_Obj.readStringUntil('\n');
    input = input.trim(); // Remove any extra whitespace or newlines
    int soundIndex = PApplet.parseInt(input);

    // Reset and play the sound for 1 second, then stop
    audioFiles[soundIndex - 1].cue(0); // Reset to the start of the track
    audioFiles[soundIndex - 1].play(); // Play the track
    delay(1000);  // Wait for 1 second (can be adjusted based on your needs)
  }
}

void controlEvent(ControlEvent event) {
  if (event.isFrom(cp5.getController("Run"))) {
    sendToArduino(n_trials);
    sendToArduino(us_delay);
    sendToArduino(iti);

    // Send CS-US pairing variables
    sendToArduino(bigR_togglestate ? 1 : 0);
    sendToArduino(bigR_p_cue);
    sendToArduino(bigR_p_deliver);

    sendToArduino(smallR_togglestate ? 1 : 0);
    sendToArduino(smallR_p_cue);
    sendToArduino(smallR_p_deliver);

    sendToArduino(bigA_togglestate ? 1 : 0);
    sendToArduino(bigA_p_cue);
    sendToArduino(bigA_p_deliver);

    sendToArduino(smallA_togglestate ? 1 : 0);
    sendToArduino(smallA_p_cue);
    sendToArduino(smallA_p_deliver);

    sendToArduino(nothing_togglestate ? 1 : 0);
    sendToArduino(nothing_p_cue);
    sendToArduino(nothing_p_deliver);

    println("Configuration sent: n_trials=" + n_trials 
      + ", cs_delay=" + us_delay 
      + ", iti=" + iti
      + ", bigR_togglestate=" + bigR_togglestate
      + ", bigR_p_cue=" + bigR_p_cue
      + ", bigR_p_deliver=" + bigR_p_deliver
      + ", smallR_togglestate=" + smallR_togglestate
      + ", smallR_p_cue=" + smallR_p_cue
      + ", smallR_p_deliver=" + smallR_p_deliver
      + ", bigA_togglestate=" + bigA_togglestate
      + ", bigA_p_cue=" + bigA_p_cue
      + ", bigA_p_deliver=" + bigA_p_deliver
      + ", smallA_togglestate=" + smallA_togglestate
      + ", smallA_p_cue=" + smallA_p_cue
      + ", smallA_p_deliver=" + smallA_p_deliver
      + ", nothing_togglestate=" + nothing_togglestate
      + ", nothing_p_cue=" + nothing_p_cue
      + ", nothing_p_deliver=" + nothing_p_deliver);
  } else if (event.isFrom(cp5.getController("Reinitialize Arduino"))) {
    initializeArduino();
  }  
}

void initializeArduino() {
  if (arduino_Obj != null) {
    arduino_Obj.stop(); // Stop the current connection if active
  }
  try {
    arduino_Obj = new Serial(this, "COM7", 115200);
    delay(5000); // Wait for the Arduino to reset
    println("Arduino reinitialized successfully.");
  } catch (Exception e) {
    println("Error initializing Arduino: " + e.getMessage());
  }
}

void sendToArduino(int value) {
  arduino_Obj.write(value + "\n"); // Append newline to mimic CR
}

void stop() {
  // Clean up Minim
  for (AudioPlayer player : audioFiles) {
    player.close();
  }
  minim.stop();
  if (arduino_Obj != null) {
    arduino_Obj.stop();
  }
  super.stop();
}
