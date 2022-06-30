// /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ //
//                              FUNÇÕES PARA O USUÁRIO                                  //
// /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ //


// =====================================
// Recurso - habilitar efeitos e pisca
// =====================================
void pwm595_Check_Habilitar() {
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
// Limite Geral - Define o brilho máximo dos leds (1 á 100)
// =========================================================
void Tms_Limite_Brilho_Func(byte i) {
  Tms_Limite_Brilho = i;
  for (byte i = 0; i < 3; i++) Tms_Efeito_tempo_Corrigido[i] = (100 / Tms_Limite_Brilho) * Tms_Efeito_tempo[i];
}

// ================================================================
// PWM BASE - Duty em todos os leds
// ================================================================
void pwm595_Duty_Geral(byte duty) {
  duty = (duty > Tms_Limite_Brilho) ? Tms_Limite_Brilho : duty;
  for (int i = 0; i < leds; i++) Tms_DutySimples[i] = (linhas * duty) / 100;
}
// ================================================================
// PWM BASE - Duty em pino definido
// ================================================================
void pwm595_Duty_Pin(byte pino, byte duty) {
  TMa[pino] = (duty > Tms_Limite_Brilho) ? Tms_Limite_Brilho : duty;
}

// ================================================================
// Pisca-Pisca - Ativação dos piscas (1 = on, 2= off)
// ================================================================
void pwm595_Pisca_Geral(bool OnOff) {
  for (int i = 0; i < leds; i++) Tms_Pisca_OnOff[i] = OnOff;
  pwm595_Check_Habilitar();
}
// ================================================================
// Pisca-Pisca - define tempo do pisca
// ================================================================
void pwm595_Pisca_Tempo (int valor) {
  Tms_Pisca_Tempo = valor;
}

// ================================================================
// Pisca-Pisca - (des)Ativação individual (1 = on, 0= off)
// ================================================================
void pwm595_Pisca_Pino(byte pino, bool OnOff) {
  Tms_Pisca_OnOff[pino] = OnOff;
  pwm595_Check_Habilitar();
}

// ================================================================
//               Define EFEITO para todos os pinos
// 0 = Desabilitado, 1 = 0..100->0...100, 2= 0...100...0...10...
// ================================================================
void pwm595_Efeito(byte valor) {
  for (int i = 0; i < leds; i++) Tms_Efeito_SelecEfeito[i] = valor;
  pwm595_Check_Habilitar();
}
// ================================================================
//           Define TEMPO do EFEITO para todos os pinos
// ================================================================
void pwm595_EfeitoTempo(byte efeito, int valor) {
  if (efeito > 0) {
	efeito -= 1;
    Tms_Efeito_tempo[efeito] = (valor / 200);
    Tms_Efeito_tempo_Corrigido[efeito] = (100 / Tms_Limite_Brilho) * Tms_Efeito_tempo[efeito];
  }
}

// ================================================================
//                 Define EFEITO para o pino "N".
// 0 = Desabilitado, 1 = 0..100->0...100, 2= 0...100...0...10...
// ================================================================
void pwm595_EfeitoPE(byte pino, byte ativo) { //
  Tms_Efeito_SelecEfeito[pino] = ativo;
  pwm595_Check_Habilitar();
}

// ================================================================
//                 Zera todos os efeitos
// ================================================================
void pwm595_ZerarEfeitos() {
  for (byte i = 0; i < leds; i++) Tms_Efeito_SelecEfeito[i] = 0;
}
// ================================================================
// RESET 1:
// ================================================================
void pwm595_RESET() {
  pwm595_Duty_Geral(0);
  pwm595_Pisca_Geral(0);
  pwm595_ZerarEfeitos();
}
// ================================================================
// Master Reset (tudo para o estado inicial e em off)
// ================================================================
void pwm595_MR() {
  pwm595_VI();
  pwm595_RESET();
}