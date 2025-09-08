/*
  Helmet Motion & Orientation Alarm (Prototype)
  - Arduino Nano + 3-axis analog IMU (e.g., ADXL335)
  - Y axis (up/down) on A1
  - Z axis (left/right) on A2
  - Buzzer (active) on D4

  Behavior:
  - Baseline captured at startup (keep helmet still for ~1 second).
  - Sideways (Z) deviation -> short beeps every 1s until back to baseline.
  - Fall (Y) deviation sustained for 30s -> continuous alarm until back to baseline.
*/

const int BUZZER_PIN = 4;

const int PIN_X = A0; // not used in logic; available for future
const int PIN_Y = A1; // head up/down
const int PIN_Z = A2; // head left/right (required)

// Tuning parameters (adjust if needed)
const int SIDE_Z_THRESHOLD   = 70;   // ADC counts deviation for sideways detection
const int FALL_Y_THRESHOLD   = 140;  // ADC counts deviation to consider "fallen"
const int DEAD_BAND          = 40;   // allowed range around baseline to consider "back to normal"

const unsigned long FALL_DELAY_MS   = 30000UL; // 30 seconds
const unsigned long BEEP_PERIOD_MS  = 1000UL;  // 1 second interval
const unsigned long BEEP_ON_MS      = 150UL;   // short beep length

// Internal state
enum Mode { MODE_NORMAL, MODE_SIDEWAY_BEEP, MODE_FALL_CONTINUOUS };
Mode mode = MODE_NORMAL;

int baselineY = 0;
int baselineZ = 0;

unsigned long fallStartMs   = 0;
unsigned long sideBeepEpoch = 0;

int readAveraged(int pin) {
  long sum = 0;
  for (byte i = 0; i < 8; i++) {
    sum += analogRead(pin);
  }
  return (int)(sum / 8);
}

void calibrateBaseline(int samples) {
  long sumY = 0, sumZ = 0;
  for (int i = 0; i < samples; i++) {
    sumY += readAveraged(PIN_Y);
    sumZ += readAveraged(PIN_Z);
    delay(2); // small delay for a stable average
  }
  baselineY = (int)(sumY / samples);
  baselineZ = (int)(sumZ / samples);
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Optional: Serial for tuning
  // Serial.begin(115200);
  // delay(200);

  // Give a moment to hold the helmet still, then capture baseline
  delay(300);
  calibrateBaseline(400); // ~0.8s of averaging

  // Ready chirp
  digitalWrite(BUZZER_PIN, HIGH); delay(80);
  digitalWrite(BUZZER_PIN, LOW);

  mode = MODE_NORMAL;
  sideBeepEpoch = millis();
}

void loop() {
  // Read smoothed values
  int y = readAveraged(PIN_Y);
  int z = readAveraged(PIN_Z);

  int dy = y - baselineY;
  int dz = z - baselineZ;

  int absDy = (dy >= 0) ? dy : -dy;
  int absDz = (dz >= 0) ? dz : -dz;

  bool backToNormal = (absDy <= DEAD_BAND) && (absDz <= DEAD_BAND);
  bool fallCandidate = (absDy >= FALL_Y_THRESHOLD);

  unsigned long now = millis();

  // If already in fall alarm, hold it until back to baseline
  if (mode == MODE_FALL_CONTINUOUS) {
    if (backToNormal) {
      mode = MODE_NORMAL;
      fallStartMs = 0;
      digitalWrite(BUZZER_PIN, LOW);
    } else {
      digitalWrite(BUZZER_PIN, HIGH); // continuous tone
    }
    return; // Nothing else preempts continuous fall alarm
  }

  // Manage fall timer when not in continuous alarm
  if (fallCandidate) {
    if (fallStartMs == 0) fallStartMs = now;
    if ((now - fallStartMs) >= FALL_DELAY_MS) {
      mode = MODE_FALL_CONTINUOUS;
      digitalWrite(BUZZER_PIN, HIGH);
      return;
    }
  } else {
    fallStartMs = 0; // reset if recovered before 30s
  }

  // Sideways detection and beeping logic
  if (mode == MODE_SIDEWAY_BEEP) {
    // Keep beeping until fully back to baseline (both Y & Z within deadband)
    if (backToNormal) {
      mode = MODE_NORMAL;
      digitalWrite(BUZZER_PIN, LOW);
    } else {
      unsigned long phase = (now - sideBeepEpoch) % BEEP_PERIOD_MS;
      bool beepOn = (phase < BEEP_ON_MS);
      digitalWrite(BUZZER_PIN, beepOn ? HIGH : LOW);
    }
  } else {
    // Not currently beeping due to sideways; check for new sideways event
    if (absDz >= SIDE_Z_THRESHOLD) {
      mode = MODE_SIDEWAY_BEEP;
      sideBeepEpoch = now; // start a new beep cycle
      digitalWrite(BUZZER_PIN, HIGH); // immediate short beep start
    } else {
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}
