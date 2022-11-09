// Assign Names to Pins on Arduino
int p3 = 10;
int p2 = 11;
int p1 = 12;
int p0 = 13;
int freq_shifter = A0;
int buzzer = 9;
int led3 = 2;
int led2 = 3;
int led1 = 4;
int led0 = 5;

// Defines Some Standard Musical Notes for Reference
int C_MAJOR_SCALE[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88};

// Stores Sequencer Frequency Pattern 
int freq_pattern[] = {0, 0, 0, 0};

// Stores Value from Potentionmeter Attached to an Analog Pin
int freq_offset = 0;

// Stores Current Frequency (Tempo) of Sequencer
int tempo = 500;

// Assign Names to Pin States. Since Buttons are Low-Active, States are Reversed.
bool ON = LOW;
bool OFF = HIGH;

void setup() {
  Serial.begin(9600);

  // Initialize Arduino Pins
  pinMode(p3, INPUT_PULLUP);
  pinMode(p2, INPUT_PULLUP);
  pinMode(p1, INPUT_PULLUP);
  pinMode(p0, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  pinMode(led3, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led0, OUTPUT);

  // Run Default Sequence
  create_sequence(1, 1, 1, 1);
}

/* Assigns Given Frequency Values to Sequencer Pattern */
void create_sequence(int a, int b, int c, int d) {  
  freq_pattern[0] = a;
  freq_pattern[1] = b;
  freq_pattern[2] = c;
  freq_pattern[3] = d;
}

/* Outputs and Sequences Frequency Pattern at Specified Tempo */
void run_sequencer() {
  tone(buzzer, freq_pattern[0]);
  digitalWrite(led3, HIGH);
  delay(tempo);
  noTone(buzzer);
  tone(buzzer, freq_pattern[1]);
  digitalWrite(led3, LOW);
  digitalWrite(led2, HIGH);
  delay(tempo);
  noTone(buzzer);
  tone(buzzer, freq_pattern[2]);
  digitalWrite(led2, LOW);
  digitalWrite(led1, HIGH);
  delay(tempo);
  noTone(buzzer);
  tone(buzzer, freq_pattern[3]);
  digitalWrite(led1, LOW);
  digitalWrite(led0, HIGH);
  delay(tempo);
  noTone(buzzer);
  digitalWrite(led0, LOW);  
}

/* Digital Logic XOR Gate. Allows Only One Button Press at a Time */
/* Use: to Program Frequencies into Sequencer Pattern */
int XOR(int a, int b, int c, int d) {
  if (a == ON && b == OFF && c == OFF && d == OFF) {
    freq_pattern[0] = freq_offset + C_MAJOR_SCALE[0];
  } 
  else if (a == OFF && b == ON && c == OFF && d == OFF) {
    freq_pattern[1] = freq_offset + C_MAJOR_SCALE[0];
  }
  else if (a == OFF && b == OFF && c == ON && d == OFF) {
    freq_pattern[2] = freq_offset + C_MAJOR_SCALE[0];
  }
  else if (a == OFF && b == OFF && c == OFF && d == ON) {
    freq_pattern[3] = freq_offset + C_MAJOR_SCALE[0];
  }
}

/* Digital Logic AND Gate. Responds to Combinations of Buttons Pressed */
/* Use: Adjust Sequencer Frequency (Tempo) */
int AND(int a, int b, int c, int d) {
  if (a == ON && b == ON && c == OFF && d == OFF) {
    tempo = freq_offset / 4;
  }   
}

void loop() {
  // Save Current Button States
  int p3_state = digitalRead(p3);
  int p2_state = digitalRead(p2);
  int p1_state = digitalRead(p1);
  int p0_state = digitalRead(p0);

  // Set freq_offset Value by Reading an Analog Pin Attached to Terminal Pin of Potentiometer
  freq_offset = analogRead(freq_shifter);

  // Run Logic Gates to Handle User Input via Buttons
  XOR(p3_state, p2_state, p1_state, p0_state);
  AND(p3_state, p2_state, p1_state, p0_state);

  run_sequencer();

  delay(25);
}