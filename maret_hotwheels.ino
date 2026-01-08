#include <avr/io.h>
#include <util/delay.h>

#define BUTTON_PIN PB0
#define FRONT_LIGHTS PB1
#define REAR_LIGHTS PB2

uint8_t mode = 0;
unsigned long previousMillis = 0;
const long interval = 50;  

void setup() {
  DDRB |= (1 << FRONT_LIGHTS) | (1 << REAR_LIGHTS);  // Set lampu sebagai output
  DDRB &= ~(1 << BUTTON_PIN);                        // Set tombol sebagai input
  PORTB |= (1 << BUTTON_PIN);                        // Pull-up resistor
}

void loop() {
  static uint8_t lastButtonState = 1;
  uint8_t buttonState = PINB & (1 << BUTTON_PIN);

  // Deteksi tombol ditekan
  if (buttonState == 0 && lastButtonState == 1) {
    _delay_ms(50);  // Debounce
    if ((PINB & (1 << BUTTON_PIN)) == 0) {
      mode = (mode + 1) % 3;
    }
  }
  lastButtonState = buttonState;

  // Non-blocking delay untuk kedip-kedip
  unsigned long currentMillis = millis();

  switch (mode) {
    case 0:  // Semua mati
      PORTB &= ~((1 << FRONT_LIGHTS) | (1 << REAR_LIGHTS));
      break;
    case 1:  // Semua nyala
      PORTB |= (1 << FRONT_LIGHTS) | (1 << REAR_LIGHTS);
      break;
    case 2:  // Kedip-kedip 1 per 1
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (PORTB & (1 << FRONT_LIGHTS)) {
          PORTB &= ~(1 << FRONT_LIGHTS);
          PORTB |= (1 << REAR_LIGHTS);
        } else {
          PORTB |= (1 << FRONT_LIGHTS);
          PORTB &= ~(1 << REAR_LIGHTS);
        }
      }
      break;
  }
}
