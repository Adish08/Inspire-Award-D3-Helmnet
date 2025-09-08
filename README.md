# 🪖 Inspire-Award-D3-Helmet

### Distraction Detection Device (D3)

> 🚨 A **proof-of-concept prototype** created by *Class 12th C students* for the **Inspire Award Submission**.\
> Detects **helmet motion & orientation** to alert against **distraction** or **fall incidents**.

---

## ✨ Features

- ✅ **Portable & Compact:** Battery-powered, small (\~1 inch) device that can be attached to any helmet.
- ✅ **Distraction Alert:** Warns users if they look away from the road.
- ✅ **Sideways Detection (Z-axis):** Short beeps every 1s if head tilts sideways.
- ✅ **Fall Detection (Y-axis):** Continuous alarm if fallen position is held for 30s.
- ✅ **Baseline Calibration:** Captures normal head position automatically at startup (keep helmet still for \~1s).

---

## 🛠️ Hardware Requirements

- **Arduino Nano (or compatible)**
- **3-axis Analog IMU** *(e.g., ADXL335)*
- **Active Buzzer**
- **Helmet-mounted setup**

**Connections:**

```text
Y-axis → A1 (head up/down)
Z-axis → A2 (head left/right)
Buzzer → D4
```

---

## ⚙️ How It Works

1. ⏱️ **Startup:** Device calibrates baseline position (\~1s).
2. ↔️ **Side Tilt:** Beyond threshold → short beeps until corrected.
3. ⬇️ **Fall Position:** Sustained for 30s → continuous alarm.
4. ✅ **Recovery:** Alarm stops when head returns to baseline.

---

## 📌 Notes

> ⚠️ Prototype is made for **educational demonstration only**.\
> ❌ Usage without team permission is **strictly prohibited**.

---

## 🚀 Quick Start

1. Clone this repository:
   ```bash
   git clone https://github.com/<your-username>/Inspire-Award-D3-Helmet.git
   ```
2. Open the `.ino` file in Arduino IDE.
3. Select **Arduino Nano** board and correct COM port.
4. Upload to your device and test with helmet setup.

---

## 🏆 Project Info

- 🎓 **Developed by:** Students of Class 12th C
- 📖 **Category:** Inspire Award Project Submission
- 🔑 **Code Name:** D3 (Distraction Detection Device)

---

## 📝 To-Do (Future Enhancements)

- [ ] Crash Detection

- [ ] Emergency SOS Message

- [ ] Mobile Connectivity (Bluetooth/WiFi
