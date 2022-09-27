// /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ //
//                              FUNÇÕES PARA O USUÁRIO                                  //
// /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ //


// =====================================
// Recurso - habilitar efeitos e pisca
// =====================================
void Tms74hc595_Check_Habilitar() {
  byte efeito[3] = {0, 0, 0};
  byte pisca_count = 0;

  for (byte i = 0; i < leds; i++) {
    pisca_count += Tms_Pisca_OnOff[i];
    efeito[0] += (Tms_Efeito_SelecEfeito[i] == 1) ? Tms_Efeito_SelecEfeito[i] : 0;
    efeito[1] += (Tms_Efeito_SelecEfeito[i] == 2) ? Tms_Efeito_SelecEfeito[i] : 0;
    efeito[2] += (Tms_Efeito_SelecEfeito[i] == 3) ? Tms_Efeito_SelecEfeito[i] : 0;
  }
  Tms_Habilitar_Pisca = (pisca_count > 0) ?  1 : 0;
  for (byte i = 0; i < 3; i++) Tms_Habilitar_Efeito[i] = (efeito[i]) ?  1 : 0;
}

// =========================================================
// Limite de Brilho (Geral) - Define o brilho máximo dos leds (1 á 100)
// =========================================================
void Tms74hc595_Limite_Brilho_Geral(byte TopLimiteGeral) {
  for (byte i = 0; i< leds; i++) Tms_Limite_Brilho[i] = TopLimiteGeral;
}

// =========================================================
// Limite de Brilho (PINO) - Define o brilho máximo dos leds (1 á 100)
// =========================================================
void Tms74hc595_Limite_Brilho_Pino(byte pino, byte TopLimitePino) {
  Tms_Limite_Brilho[pino] = TopLimitePino;										
}

// ================================================================
// PWM BASE - Duty em todos os leds
// ================================================================
void Tms74hc595_Duty_Geral(byte duty) {
  for (int i = 0; i < leds; i++) Tms_DutySimples[i] = linhas * duty / 100;
}
// ================================================================
// PWM BASE - Duty em pino definido
// ================================================================
void Tms74hc595_Duty_Pin(byte pino, byte duty) {
  Tms_DutySimples[pino] = linhas * duty / 100;
}

// ================================================================
// Pisca-Pisca - Ativação dos piscas (1 = on, 2= off)
// ================================================================
void Tms74hc595_Pisca_Geral(bool OnOff) {
  for (int i = 0; i < leds; i++) Tms_Pisca_OnOff[i] = OnOff;
  Tms74hc595_Check_Habilitar();
}
// ================================================================
// Pisca-Pisca - define tempo do pisca
// ================================================================
void Tms74hc595_Pisca_Tempo(int tempo) {
  Tms_Pisca_Tempo = tempo;
}

// ================================================================
// Pisca-Pisca - (des)Ativação individual (1 = on, 0= off)
// ================================================================
void Tms74hc595_Pisca_Pino(byte pino, bool OnOff) {
  Tms_Pisca_OnOff[pino] = OnOff;
  Tms74hc595_Check_Habilitar();
}

// ================================================================
//               Define EFEITO para todos os pinos
// 0 = Desabilitado, 1 = 0..100->0...100, 2= 0...100...0...10...
// ================================================================
void Tms74hc595_Efeito_Geral(byte efeito, float topduty) {
  for (int i = 0; i < leds; i++) {
	  Tms_Efeito_SelecEfeito[i] = efeito;
	  Tms_Efeito_TopDuty[i] = topduty;
	  }
  Tms74hc595_Check_Habilitar();
}
// ================================================================
//           Define TEMPO do EFEITO para todos os pinos
// ================================================================
void Tms74hc595_Efeito_Tempo(byte efeito, int tempo) {
  if (efeito > 0) {
	efeito -= 1;
    Tms_Efeito_tempo[efeito] = (tempo / 200);   //100 - linhas
  }
}


// ================================================================
//                 Define EFEITO para o pino "N".
// 0 = Desabilitado, 1 = 0..100->0...100, 2= 0...100...0...10...
// ================================================================
void Tms74hc595_Efeito_Pino(byte pino, byte efeito, float topduty) { //
  Tms_Efeito_SelecEfeito[pino] = efeito;
  Tms_Efeito_TopDuty[pino] = topduty;
  if (efeito == 3) {
	  if (topduty == 0)Tms_Efeito_contador[2][pino] = 0;
	  else Tms_Efeito_contador[2][pino] = 100;
	}
  else if (efeito == 0)Tms_Efeito_TopDuty[pino] = 0;
	Tms74hc595_Check_Habilitar();
}
// ================================================================
// RESET 1:
// ================================================================
void Tms74hc595_RESET() {
  Tms74hc595_Duty_Geral(0);
  Tms74hc595_Pisca_Geral(0);
  for (int i = 0; i < leds; i++) Tms_Efeito_SelecEfeito[i] = 0;
  Tms74hc595_Check_Habilitar();
}
// ================================================================
// Master Reset (tudo para o estado inicial e em off)
// ================================================================
void Tms74hc595_MR() {
  Tms74hc595_VI();
  Tms74hc595_RESET();
}