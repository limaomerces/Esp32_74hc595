#include <TMS_74HC595.h>

void setup() {
  Tms74hc595_setup();
  Tms74hc595_Limite_Brilho_Geral(100);
  Tms74hc595_Duty_Pin(0, 100);
  Tms74hc595_Duty_Pin(1, 50);
  Tms74hc595_Duty_Pin(2, 25);
  Tms74hc595_Duty_Pin(3, 25);
  Tms74hc595_Duty_Pin(4, 100);
  Tms74hc595_Pisca_Pino(4, 1);
  Tms74hc595_Efeito_Pino(5, 1, 100);
  Tms74hc595_Efeito_Pino(6, 2, 100);
  Tms74hc595_Efeito_Pino(7, 3, 100);
  teste1 = millis();
  Serial.begin(115200);
}

void loop() {
	Tms74HC595_Main();
}