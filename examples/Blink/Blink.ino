/////////////////////////////////////////////////////////
// Runnable
// Arduino simple cooperative multitask library
// (c)2017, Alexander Emelianov (a.m.emelianov@gmail.com)
//
// Simple blink example
//

// GPIO PIN to which LED is connected
#define LED1_PIN    D0
// Blinking period
#define LED1_ON_OFF 1000

#include <Runnable.h>

// Class that changes LED state on/off
class Blinking : public Runnable {
public:
  Blinking(int16_t p) {
    pin = p;
    pinMode(p, OUTPUT); // Set GPIO mode for output
    digitalWrite(p, HIGH);
  }
private:
  int16_t pin;
  uint32_t run() {
    if (digitalRead(pin) == HIGH) {
      digitalWrite(pin, LOW);
    } else {
      digitalWrite(pin, HIGH);
    }
    return LED1_ON_OFF; // Return time to next blink() function execution back to scheduller
  }
};

Blinking blink(LED1_PIN);

void setup() {
  Serial.begin(74880);
  blink.runNow(); // Append task to run immediatly
}

void loop() {
  runnableLoop(); // Sheduller function
  delay(100);
}