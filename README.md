# Inspire-Award-D3-Helmet  
**Distraction Detection Device (D3)**  

This project is a **proof-of-concept prototype** made by Class 12th C students for the Inspire Award submission.  
It detects **helmet motion & orientation** to alert against distraction or falls.  

## Features  
- **Sideways Detection (Z-axis):**  
  - Triggers short beeps every 1s if the head tilts sideways.  
- **Fall Detection (Y-axis):**  
  - If a fall position is held for **30s**, a continuous alarm sounds until the helmet returns upright.  
- **Baseline Calibration:**  
  - Captures normal head position at startup (keep helmet still for ~1s).  

## Hardware Requirements  
- **Arduino Nano (or similar)**  
- **3-axis Analog IMU** (e.g., ADXL335)  
- **Active Buzzer**  
- Helmet-mounted setup  

**Connections:**  
- Y-axis → `A1` (head up/down)  
- Z-axis → `A2` (head left/right)  
- Buzzer → `D4`  

## How It Works  
1. On startup, the device calibrates the baseline position.  
2. **Side tilt** beyond threshold → short beeps.  
3. **Fall tilt** held for 30s → continuous alarm.  
4. Alarm stops once head returns to normal.  

## Notes  
- Prototype made for **educational demonstration only**.  
- Usage without team permission is strictly prohibited.
