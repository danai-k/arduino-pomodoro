# Arduino Waveshare LCD1602 Pomodoro Focus Timer !

A Pomodoro Focus Timer built for Arduino, using specifically a **Waveshare LCD1602 I2C module**, a push button, and a buzzer.

Unlike standard LCD1602 screens that use the PCF8574 I2C expansion chip, Waveshare modules utilize the **AiP31068 controller**. Therefore, Standard Arduino libraries (such as `LiquidCrystal_I2C`) are incompatible with this chip. This repository provides a custom header driver (`WaveshareLCD.h`).

---

## Features

* **Custom Waveshare Driver:**  low-level I2C communication tailored for the AiP31068 controller (`0x3E` / `0x27` address).
* **Non-Blocking Timer Logic:** Built using `millis()` math rather than `delay()`, ensuring instant button responsiveness.
* **Finite State Machine (FSM):** Structured progression across four distinct states:
  1. `START`: Ready prompt screen.
  2. `MIN25`: 25-minute active work/focus countdown.
  3. `BUZZ`: 1-second audio buzzer sequence upon completion.
  4. `MIN5`: 5-minute break countdown (auto-resets back to `START`).
* **Zero External Resistors Needed:** Utilizes the Arduino's internal `INPUT_PULLUP` resistor for the push button.

---

## Hardware Required
* **Microcontroller:** Arduino Uno, Nano, or compatible board.
* **Display:** Waveshare LCD1602 I2C Module (AiP31068 controller).
* **Input:** 1x Push Button.
* **Output:** 1x Buzzer.
* **Miscellaneous:** Breadboard and some Jumper Wires.

---

## Wiring Instructions

Follow these step-by-step connections to wire up your components:

### 1. Ground & Power setup
* Connect a wire from the **GND** pin on your Arduino to the **`-` (ground) rail** on your breadboard. (allows all components to share a common ground.)

### 2. Waveshare LCD1602 Module (I2C)
* Connect **VCC** to the Arduino **5V** pin.
* Connect **GND** to the breadboard **GND rail**.
* Connect **SDA** to Arduino pin **A4** (or dedicated SDA pin).
* Connect **SCL** to Arduino pin **A5** (or dedicated SCL pin).

### 3. Push Button
* Place the button straddling the center divider of your breadboard.
* Connect **Leg 1** to Arduino Digital Pin **2**.
* Connect **Leg 2** to the breadboard **GND rail**.
*(Note: No resistor is needed—the code enables the Arduino's internal pull-up resistor).*

### 4. Buzzer
* Connect the **Positive (+ / longer leg)** to Arduino Digital Pin **8**.
* Connect the **Negative (- / shorter leg)** to the breadboard **GND rail**.

---

## File Structure

```text
arduino-waveshare-pomodoro/
├── WaveshareLCD.h      # Custom driver header for Waveshare AiP31068 display
├── PomodoroTimer.ino   # Main sketch containing the state machine & timer logic
└── README.md           # Project documentation
```
---

## How to Install and Run

1. **Clone or Download** this repository:
   ```bash
   git clone [https://github.com/YOUR-USERNAME/arduino-waveshare-pomodoro.git](https://github.com/YOUR-USERNAME/arduino-waveshare-pomodoro.git)
   ```
2. Open `PomodoroTimer.ino` in the **Arduino IDE**.
3. Ensure `WaveshareLCD.h` is located in the same directory as `PomodoroTimer.ino` (it should appear as a tab in the editor).
4. Verify the I2C address in `WaveshareLCD.h` matches your module:
   ```cpp
   #define LCD_ADDR 0x3E  // Default Waveshare address (change to 0x27 if needed)
   ```
5. Select your Board (e.g., **Arduino Uno**) under **Tools > Board**.
6. Select your COM port under **Tools > Port**.
7. Click **Upload**.
8. 
---

## State Machine Architecture

```text
+-------------------+
  |      START        | <----------------------+
  | "Pomodoro Ready!" |                        |
  +---------+---------+                        |
            |                                  |
    (Button Pressed)                           |
            v                                  |
  +-------------------+                        |
  |      MIN25        |                        |
  | 25-Min Countdown  |                        |
  +---------+---------+                        |
            |                                  |
    (Time Expired)                             |
            v                                  |
  +-------------------+               +--------+--------+
  |       BUZZ        |               |      MIN5       |
  |  1-Sec Audio Tone | ------------> | 5-Min Countdown |
  +-------------------+               +-----------------+
                                        (Time Expired)
```

---
