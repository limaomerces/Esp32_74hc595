// /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ //
//                              FUNÇÕES PARA O USUÁRIO                                  //
// /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ //


// =====================================
// Recurso - habilitar efeitos e pisca
// =====================================
void pwm595_Check_Habilitar() {
  byte pisca_count = 0;
  byte efeito1 = 0;
  byte efeito2 = 0;
  for (byte i = 0; i < leds; i++) {
    pisca_count += Tms_Pisca_OnOff[i];
    if (Tms_Efeito_SelecEfeito[i] == 1) {
      efeito1 += Tms_Efeito_SelecEfeito[i];
    }
    else if (Tms_Efeito_SelecEfeito[i] == 2) {
      efeito2 += Tms_Efeito_SelecEfeito[i];
    }
  }
  if (pisca_count > 0) Tms_Habilitar_Pisca = 1;
  else Tms_Habilitar_Pisca = 0;
  if (efeito1 > 0) Tms_Habilitar_Efeito_1 = 1;
  else Tms_Habilitar_Efeito_1 = 0;
  if (efeito2 > 0) Tms_Habilitar_Efeito_2 = 1;
  else Tms_Habilitar_Efeito_2 = 0;
}

// =========================================================
// Limite Geral - Define o brilho máximo dos leds (1 á 100)
// =========================================================
void Tms_Limite_Brilho_Func(byte i) {
  Tms_Limite_Brilho = i;
  Tms_Efeito_1_tempo_Corrigido = (100 / Tms_Limite_Brilho) * Tms_Efeito_1_tempo;
  Tms_Efeito_2_tempo_Corrigido = (100 / Tms_Limite_Brilho) * Tms_Efeito_2_tempo;
}

// ================================================================
// PWM BASE - Duty em todos os leds
// ================================================================
void pwm595_Duty_Geral(byte duty) {
  if (duty > Tms_Limite_Brilho) {
    duty = Tms_Limite_Brilho;
  }
  for (int i = 0; i < leds; i++) {
    Tms_DutySimples[i] = (linhas * duty) / 100;
  }
}
// ================================================================
// PWM BASE - Duty em pino definido
// ================================================================
void pwm595_Duty_Pin(byte pino, byte duty) {
  if (duty > Tms_Limite_Brilho) {
    duty = Tms_Limite_Brilho;
  }
  TMa[pino] = duty;
}

// ================================================================
// Pisca-Pisca - Ativação dos piscas (1 = on, 2= off)
// ================================================================
void pwm595_Pisca_Geral(bool OnOff) {
  for (int i = 0; i < leds; i++) {
    Tms_Pisca_OnOff[i] = OnOff;
  }
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
  for (int i = 0; i < leds; i++) {
    Tms_Efeito_SelecEfeito[i] = valor;
  }
  pwm595_Check_Habilitar();
}
// ================================================================
//           Define TEMPO do EFEITO para todos os pinos
// ================================================================
void pwm595_EfeitoTempo(byte efeito, int valor) {
  if (efeito == 1) {
    Tms_Efeito_1_tempo = valor / 200;
    Tms_Efeito_1_tempo_Corrigido = (100 / Tms_Limite_Brilho) * Tms_Efeito_1_tempo;
  }
  else if (efeito == 2) {
    Tms_Efeito_2_tempo = valor / 200;
    Tms_Efeito_2_tempo_Corrigido = (100 / Tms_Limite_Brilho) * Tms_Efeito_2_tempo;
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
  for (byte i = 0; i < leds; i++) {
    Tms_Efeito_SelecEfeito[i] = 0;
  }
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