// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Nome do Projeto: Esp32_74hc595_PWM
//  Objetivo: Gerar sinal PWM, através do CI74hc595 com auxílio do controlador ESP32
//  Autor: Tiago Mercês Silva
//  Email: tiago.merces@hotmail.com
//
//  Histórico de Atualizações:
//          ??/03/2020 - 1.0   - Controle do 595
//          ??/08/2020 - 1.1   - Adição do PWM
//          ??/09/2020 - 1.2   - Adição do pisca
//          ??/02/2021 - 1.2.1 - Otimização de código
//          ??/02/2021 - 1.3   - Adição de efeito crescente
//          ??/02/2021 - 1.4   - Adição de efeito "vai-e-volta"
//          ??/02/2022 - 1.4.1 - Otimização de código
//          ??/03/2022 - 1.4.2 - Otimização de código
//          ??/04/2022 - 1.4.3 - Otimização de código
//          20/06/2022 - 2.0   - Reestruturação do código (grandes melhorias de performance)
//          28/06/2022 - 2.1   - Adição da interrupção.
//          29/06/2022 - 2.1.1 - Otimização de Código.
//          29/06/2022 - 2.1.2 - Otimização de Código.
//          29/06/2022 - 2.2   - Adição do efeito decrescente
//          30/06/2022 - 2.2.1 - Redução visual do código.
//          30/06/2022 - 2.2.2 - Otimização de código.

// =========================================
// Inclusões
// =========================================
#include "TMS_74HC595_USERSETUP.h"


// =========================================
// Definições
// =========================================
#define leds (Quant_74hc595 * 8)      // não alterar
#define linhas 101                    // em TESE, quanto menor, maior performance. Na prática, não vi diferença (olho nú)

// ================================================================
// Configurações para a interrupção
// ================================================================
#if (interrupt595==1)
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
#endif

// ================================================================
// Declaração das variáveis
// ================================================================
byte          TMa[leds];                           // BASIC - Matriz responsável por ARMAZENAR o duty para o calculo.
bool          Tms_Pisca_OnOff[leds];               // PISCA - Marcador para definição de PISCA-PISCA (on, off) por led
unsigned long Tms_Pisca_Millis;                    // PISCA SINGLE - Millis
int           Tms_Pisca_Tempo;                     // PISCA SINGLE - Define o tempo do pisca
bool          Tms_Pisca_Chave_Alterar; // PISCA SINGLE - Define a chave do estado
bool          Tms_Habilitar_Pisca;                 // CHAVE - Ativa o loop do Pisca-Pisca

byte          Tms_Efeito_SelecEfeito[leds];        // EFEITO - Seleciona efeito (0 = off, 1 = 0..100->0, 2= 0...100...0)
unsigned long Tms_Efeito_millis[3];                 // EFEITO - Armazena o valor do millis()
int           Tms_Efeito_tempo[3];                  // Efeito - Define o tempo do ciclo do efeito
byte          Tms_Efeito_contador[3];               // EFEITO - Contabiliza o add do duty
byte          Tms_Efeito_contador_Var[3];           // EFEITO - Valor a ser contabilizado
bool          Tms_Efeito_2_chave;                  // EFEITO - Intercala entre ADD/Sub do duty
int           Tms_Efeito_tempo_Corrigido[3];        // FIX - Corrige o tempo do ciclo
bool          Tms_Habilitar_Efeito[3];              // CHAVE - Ativa o loop do Efeito1

byte          Tms_Limite_Brilho;                   // GERAL - Define o brlho máximo dos Leds
byte          Tms_DutySimples[leds];               // PWM_Simples = Define o duty


// ================================================================
// Definição de valores iniciais obrigatórios
// ================================================================
void pwm595_VI() {
  Tms_Pisca_Tempo = 1000;
  Tms_Limite_Brilho = 100;
  Tms_Efeito_tempo[0] = 10;
  Tms_Efeito_tempo[1] = 10;
  Tms_Efeito_tempo[2] = 10;
  Tms_Efeito_contador[0] = 0;
  Tms_Efeito_contador[1] = 0;
  Tms_Efeito_contador[2] = Tms_Limite_Brilho;  

  Tms_Pisca_Chave_Alterar = 1;
  for (byte i = 0; i < 3; i++) {
    Tms_Efeito_tempo_Corrigido[i] = (100 / Tms_Limite_Brilho) * Tms_Efeito_tempo[i];
    Tms_Efeito_contador_Var[i] = 1;
  }
}

void Pwm595_Manager() {
  ////////////////////////////////////////////////////////////////
  //               ETAPA 1: ONTROLE DE EFEITOS                  //
  ////////////////////////////////////////////////////////////////
  // =================================================
  // EFEITO 1: Crescente -> Ao chegar em 100%, retorna imediatamente para 0%
  // =================================================
  if (Tms_Habilitar_Efeito[0] == 1) {
    if (millis() - Tms_Efeito_millis[0] > Tms_Efeito_tempo_Corrigido[0]) {
      Tms_Efeito_contador[0] = (Tms_Efeito_contador[0] > Tms_Limite_Brilho) ? 0 : Tms_Efeito_contador[0] + Tms_Efeito_contador_Var[0];
      Tms_Efeito_millis[0] = millis();
    }
  }
  // ================================
  // EFEITO 2: Cresce e Decresce -> Vai de loop(0% á 100% á 0%)
  // ================================
  if (Tms_Habilitar_Efeito[1] == 1) {
    if (millis() - Tms_Efeito_millis[1] > Tms_Efeito_tempo_Corrigido[1]) {
      if (Tms_Efeito_contador[1] == Tms_Limite_Brilho) Tms_Efeito_2_chave = 0;
      else if (Tms_Efeito_contador[1] == 0) Tms_Efeito_2_chave = 1;
      Tms_Efeito_contador[1] += (Tms_Efeito_2_chave) ? Tms_Efeito_contador_Var[1] : -Tms_Efeito_contador_Var[1];
      Tms_Efeito_millis[1] = millis();
    }
  }
  // =================================================
  // EFEITO 3: Decrescente -> Ao chegar em 0%, retorna imediatamente para 100%
  // =================================================
  if (Tms_Habilitar_Efeito[2] == 1) {
    if (millis() - Tms_Efeito_millis[2] > Tms_Efeito_tempo_Corrigido[2]) {
      Tms_Efeito_contador[2] = (Tms_Efeito_contador[2]) ? Tms_Efeito_contador[2] - Tms_Efeito_contador_Var[2] : Tms_Limite_Brilho;
      Tms_Efeito_millis[2] = millis();
    }
  }

  ////////////////////////////////////////////////////////////////
  //                   ETAPA 2: PROCESSAMENTO                   //
  ////////////////////////////////////////////////////////////////
  // ===================================================================
  // Processar efeito: Define Duty Base e o atualiza com efeitos
  // ===================================================================
  for (int i = 0; i < leds; i++) {
    switch (Tms_Efeito_SelecEfeito[i]) {
      case 0: TMa[i] = Tms_DutySimples[i];           break;// Define o Duty Base
      case 1: TMa[i] = (linhas * Tms_Efeito_contador[0]) / 100; break;// Atualiza o Duty, caso efeito 1 ativado
      case 2: TMa[i] = (linhas * Tms_Efeito_contador[1]) / 100; break;// Atualiza o Duty, caso efeito 2 ativado
      case 3: TMa[i] = (linhas * Tms_Efeito_contador[2]) / 100; break;// Atualiza o Duty, caso efeito 3 ativado
    }
  }

  ////////////////////////////////////////////////////////////////
  //             ETAPA 3: CONTROLE DOS PISCA-PISCAS             //
  ////////////////////////////////////////////////////////////////
  // ================================================================
  // Responsável por gerenciar o pisca no código
  // ================================================================
  if (Tms_Habilitar_Pisca == 1) {
    if (millis() - Tms_Pisca_Millis > Tms_Pisca_Tempo) {
      Tms_Pisca_Chave_Alterar = !Tms_Pisca_Chave_Alterar;
      Tms_Pisca_Millis = millis();
    }
  }
}

// ================================================================
//     Motor Principal - Analisa os dados e envia para o GPIO
// ================================================================
void pwm595_Motor() {
  for (int i = 0; i < linhas - 1; i++) {                                  // Matriz[x]
    REG_WRITE(GPIO_OUT_W1TC_REG, BIT(_LATCH));                         // (Latch) LOW, para captação de dados
    for (int i2 = 0; i2 < leds; i2++) {                                   // Matriz[y]
      if ((TMa[i2] > 0) && (TMa[i2] > i)) {                               // Definição para Duty do PWM
        if ((Tms_Pisca_OnOff[i2] == 1) && (Tms_Pisca_Chave_Alterar == 0)) {  // Cond check para pisca single ou mult.
          REG_WRITE(GPIO_OUT_W1TC_REG, BIT(_DATA));                 // (DATA) Se o pisca estiver ativo e "low", o "high" do pino fica low.
        }
        else {
          REG_WRITE(GPIO_OUT_W1TS_REG, BIT(_DATA));                 // (DATA) Caso contrário, HIGH
        }
      }
      else {
        REG_WRITE(GPIO_OUT_W1TC_REG, BIT(_DATA)) ;                  // (DATA) Cond. para duty -
      }
      REG_WRITE(GPIO_OUT_W1TC_REG, BIT(_CLOCK));                    // (CLOCK) LOW
      REG_WRITE(GPIO_OUT_W1TS_REG, BIT(_CLOCK));                    // (CLOCK) HIGH
    }
    REG_WRITE(GPIO_OUT_W1TS_REG, BIT(_LATCH));                      // (LATCH) HIGH. Conclui edição e envia a informação.
  }
}

#include "TMS_74HC595_FUNC.h"


#if (interrupt595==1)
// ==================================
// Caso a interrupção esteja ATIVADA
// ==================================
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  Pwm595_Manager();           // Gerencia os efeitos
  pwm595_Motor();                 // Contabiliza e envia os dados para o hc595
  portEXIT_CRITICAL_ISR(&timerMux);
}

void hc595_interrupcao() {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, interruptTimer, true);
  timerAlarmEnable(timer);
}
#else
// ==================================
// Caso a interrupção esteja DESATIVADA
// ==================================
void Tms74HC595_Main() {
  Pwm595_Manager();           // Gerencia os efeitos
  pwm595_Motor();                 // Contabiliza e envia os dados para o hc595
}
#endif

void Tms74hc595_setup() {
  //void setup() {
  pinMode(DATA, OUTPUT);                // data
  pinMode(CLOCK, OUTPUT);                // Clock
  pinMode(LATCH, OUTPUT);                // Latch
  pwm595_VI();                        // Utilizado para definir os valores iniciais nos vetores necessários
  pwm595_RESET();                     // Da um reset, para o caso de algo não ter carregado corretamente.
#if (interrupt595==1)
  hc595_interrupcao();
#endif
  //Tms_Limite_Brilho_Func(100);
  //pwm595_Duty_Geral(40);
  //pwm595_Pisca_Geral(1);
  pwm595_Efeito(2);
}

//void loop() 
