#include <Arduino.h>
#include <nrf.h>   // Nordic Register-Definitionen
#include "rgb_led.h"

// Beispiel: PWM0, Kanal 0
// Pin-Auswahl: hier nur symbolisch, bitte tatsächliches Mapping prüfen.
// Für Nano 33 BLE entspricht D9 -> P0.27 (Beispiel, je nach Core unterschiedlich)
#define PWM_PIN_PORT 0      // Port 0
#define PWM_PIN_NUMBER 26   // Pin 27 (P0.27)

#define PWM_TOP       100  // COUNTERTOP: Auflösung (z.B. 1000 Schritte)
#define PWM_DUTY      50   // 50 % Duty-Cycle (500 / 1000)

// Duty-Werte müssen im RAM liegen und 16-bit sein
uint16_t pwm_seq_values[1];

void pwm_init_registers() {
  // 1. PWM0 stoppen und reset-artige Konfiguration
  NRF_PWM0->ENABLE = 0;

  // 2. Ausgangspin konfigurieren über PSEL.OUT[0]
  // Format: [31:5] = 0, [4] = Connect (0=Connected), [3:0] = Pinnummer
  // Für nRF52 allgemein: Bitlayout ist meist:
  // Bits 0-4: Pin Index, Bit 31: Verbindung (0=connected, 1=disconnected)
  NRF_PWM0->PSEL.OUT[0] =
      (PWM_PIN_NUMBER << PWM_PSEL_OUT_PIN_Pos)    |  // Pin-Nummer
      (PWM_PIN_PORT   << PWM_PSEL_OUT_PORT_Pos)   |  // Port (0 oder 1)
      (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);

  // 3. PWM-Modus einstellen
  NRF_PWM0->MODE = PWM_MODE_UPDOWN_Up;   // Einfacher Up-Zählmodus

  // 4. Prescaler einstellen
  // Clock-Basis: 16 MHz / 2^PRESCALER
  // z.B. PRESCALER=0 -> 16 MHz, 1 -> 8 MHz, ...
  NRF_PWM0->PRESCALER = PWM_PRESCALER_PRESCALER_DIV_16; // Beispiel: Teiler 16

  // 5. COUNTERTOP einstellen (Max-Wert)
  NRF_PWM0->COUNTERTOP = PWM_TOP;       // 1000 Schritte

  // 6. Decoder einstellen
  // LOAD: wie die Sequenzwerte auf Kanäle gemappt werden
  // MODE: z.B. Refresh-Mode
  NRF_PWM0->DECODER =
      (PWM_DECODER_LOAD_Common << PWM_DECODER_LOAD_Pos) |   // gemeinsamer Wert für alle Kanäle
      (PWM_DECODER_MODE_RefreshCount << PWM_DECODER_MODE_Pos);

  // 7. Loop-Konfiguration (wie oft Sequenz wiederholt wird)
  NRF_PWM0->LOOP = 0;  // 0 = unendlich wiederholen

  // 8. Sequenz-Daten vorbereiten (Duty-Cycle in "PWM-Einheiten")
  // Bits [0..14]: Pulsweite; Bit 15: Polarity (0 = normal, 1 = invertiert)
  pwm_seq_values[0] = PWM_DUTY;  // 50 % von PWM_TOP

  // 9. Sequenz 0 konfigurieren (Pointer & Länge)
  NRF_PWM0->SEQ[0].PTR = (uint32_t)pwm_seq_values;
  NRF_PWM0->SEQ[0].CNT = 1;   // 1 Wert in der Sequenz
  NRF_PWM0->SEQ[0].REFRESH = 0;
  NRF_PWM0->SEQ[0].ENDDELAY = 0;

  // 10. PWM0 aktivieren
  NRF_PWM0->ENABLE = 1;

  // 11. Sequenz starten (TASKS_SEQSTART[0])
  NRF_PWM0->TASKS_SEQSTART[0] = 1;
}

void setup() {
  // Pin als Ausgang für Sicherheit (GPIO)
  pinMode(9, OUTPUT);  // Wenn D9 wirklich P0.27 ist; sonst anpassen

  pwm_init_registers();
}

void loop() {
  // Beispiel: Duty-Cycle ändern (z.B. langsam hoch und runter)
  static int val = 0;
  static int dir = 1;

  delay(10);
  val += dir * 10;
  if (val >= PWM_TOP) { val = PWM_TOP; dir = -1; }
  if (val <= 0)       { val = 0;       dir = 1; }

  pwm_seq_values[0] = val;

  // Sequenz neu starten, damit neuer Duty übernommen wird
  NRF_PWM0->TASKS_SEQSTART[0] = 1;
}