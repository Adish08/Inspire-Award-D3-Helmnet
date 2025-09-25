const int buzzerPin = 6;
const int xPin = A0;  // Front-back tilt
const int yPin = A1;  // Left-right tilt  
const int zPin = A2;  // Up-down tilt

// Calibration values (neutral/front-facing position)
int neutralX, neutralY, neutralZ;

// Sensitivity threshold - adjust based on testing
const int movementThreshold = 25;

// Timing variables
unsigned long lastGoodPosition = 0;
unsigned long buzzerStartTime = 0;
bool isBuzzing = false;
bool buzzerOn = false;

// Time constants - reduced durations
const unsigned long warningDelay = 2000;  // 3 seconds before warning
const unsigned long buzzDuration = 1000;  // 1 second buzz (reduced from 3)
const unsigned long buzzBreak = 500;      // 0.5 second break (reduced from 1)

// Sensor smoothing
int prevX, prevY, prevZ;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  
  Serial.println("Helmet Safety System");
  Serial.println("Keep head straight for calibration...");
  
  // Wait and calibrate neutral position
  delay(3000);
  
  // Take average readings for stable calibration
  long sumX = 0, sumY = 0, sumZ = 0;
  for(int i = 0; i < 100; i++) {  // Increased samples for better calibration
    sumX += analogRead(xPin);
    sumY += analogRead(yPin);
    sumZ += analogRead(zPin);
    delay(10);
  }
  
  neutralX = sumX / 100;
  neutralY = sumY / 100;
  neutralZ = sumZ / 100;
  
  // Initialize previous values
  prevX = neutralX;
  prevY = neutralY;
  prevZ = neutralZ;
  
  lastGoodPosition = millis();
  
  Serial.println("Calibration complete!");
  Serial.print("Neutral - X: "); Serial.print(neutralX);
  Serial.print(", Y: "); Serial.print(neutralY);
  Serial.print(", Z: "); Serial.println(neutralZ);
  
  // Confirmation beep
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  // Read current accelerometer values with smoothing
  int rawX = analogRead(xPin);
  int rawY = analogRead(yPin);
  int rawZ = analogRead(zPin);
  
  // Simple smoothing filter to reduce noise
  int currentX = (rawX + prevX) / 2;
  int currentY = (rawY + prevY) / 2;
  int currentZ = (rawZ + prevZ) / 2;
  
  prevX = currentX;
  prevY = currentY;
  prevZ = currentZ;
  
  // Calculate deviation from neutral position
  int deltaX = abs(currentX - neutralX);
  int deltaY = abs(currentY - neutralY);
  int deltaZ = abs(currentZ - neutralZ);
  
  // Check if head position is acceptable (looking forward)
  // More lenient on X-axis (forward/backward), stricter on Y and Z
  bool headStraight = (deltaY < movementThreshold && deltaZ < movementThreshold);
  
  // Allow some forward lean but limit excessive tilting
  if(deltaX > movementThreshold * 1.5) {
    headStraight = false;
  }
  
  unsigned long currentTime = millis();
  
  if(headStraight) {
    // Head is in good position - immediately stop buzzer and reset timers
    lastGoodPosition = currentTime;
    if(isBuzzing) {
      isBuzzing = false;
      buzzerOn = false;
      digitalWrite(buzzerPin, LOW);  // Immediate shutoff
      Serial.println("Head position corrected - Warning stopped immediately");
    }
  } else {
    // Head is turned away from forward position
    if(!isBuzzing && (currentTime - lastGoodPosition >= warningDelay)) {
      // Start warning sequence after 3 seconds
      isBuzzing = true;
      buzzerStartTime = currentTime;
      buzzerOn = true;
      digitalWrite(buzzerPin, HIGH);  // Start buzzing immediately
      Serial.println("WARNING: Head turned away - Buzzer activated");
    }
    
    // Handle buzzer pattern when warning is active
    if(isBuzzing) {
      unsigned long buzzerElapsed = currentTime - buzzerStartTime;
      unsigned long cyclePosition = buzzerElapsed % (buzzDuration + buzzBreak);
      
      if(cyclePosition < buzzDuration) {
        // Should be buzzing
        if(!buzzerOn) {
          digitalWrite(buzzerPin, HIGH);
          buzzerOn = true;
        }
      } else {
        // Should be in quiet period
        if(buzzerOn) {
          digitalWrite(buzzerPin, LOW);
          buzzerOn = false;
        }
      }
    }
  }
  
  // Debug output every 500ms
  static unsigned long lastDebug = 0;
  if(currentTime - lastDebug > 500) {
    Serial.print("Raw - X: "); Serial.print(rawX);
    Serial.print(", Y: "); Serial.print(rawY);
    Serial.print(", Z: "); Serial.print(rawZ);
    Serial.print(" | Deltas - X: "); Serial.print(deltaX);
    Serial.print(", Y: "); Serial.print(deltaY);
    Serial.print(", Z: "); Serial.print(deltaZ);
    Serial.print(" | Straight: "); Serial.print(headStraight);
    Serial.print(" | Buzzing: "); Serial.println(isBuzzing);
    lastDebug = currentTime;
  }
  
  delay(10); // Stable sampling rate
}
