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
//          28/06/2022 - 2.1   - Adição de opção de execução com interrupção.
//          29/06/2022 - 2.1.1 - Otimização de Código.
//          29/06/2022 - 2.1.2 - Otimização de Código.
//          29/06/2022 - 2.2   - Adição do efeito decrescente
//          30/06/2022 - 2.2.1 - Redução visual do código.
//          30/06/2022 - 2.2.2 - Otimização de código.
//          30/06/2022 - 2.3   - Adição de opção de execução com FreeRTOS (core 0)
//          01/07/2022 - 2.3.1 - Correções de bug
//          01/07/2022 - 2.4   - Adição para vários dutyMax nos efeitos.
//          02/07/2022 - 2.4.1 - Melhorias no visual do código
//          02/07/2022 - 2.4.2 - Correções de bug e melhorias na identificação das funções
//          02/07/2022 - 2.4.3 - Adição de Correção opcional (duty 0% mesmo efeito)
//          03/07/2022 - 2.5   - Adição do limite de brilho individual
//          09/07/2022 - 2.5.1 - Melhoria das definições do usuário
//      	11/09/2022 - 2.6   - Adicionado recurso de PWM com maior precisão
// 			19/09/2022 - 2.6.1 - Melhora nas configurações para o usuário

// =========================================
// Inclusões
// =========================================
#include <TMS_74HC595_USERSETUP.h>

// =========================================
// Definições
// =========================================
#define leds (Quant_74hc595 * 8)      // não alterar
#define linhas Tms595Pwm_Range        // em TESE, quanto menor, maior performance. Na prática, não vi diferença (olho nú)

// ================================================================
// Configurações para execução alternativa
// ================================================================
#ifdef TmsINTERRUPT               // Interrupção
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
#endif
#ifdef TmsFreeRTOS_C0               // FreeRTOS CORE 0
void TaskTMS74hc595( void *pvParameters );
#endif


// ================================================================
// Declaração das variáveis
// ================================================================
byte           TMa[leds];                           // BASIC - Matriz responsável por ARMAZENAR o duty para o calculo.
bool          Tms_Pisca_OnOff[leds];               // PISCA - Marcador para definição de PISCA-PISCA (on, off) por led
unsigned long Tms_Pisca_Millis;                    // PISCA SINGLE - Millis
int           Tms_Pisca_Tempo;                     // PISCA SINGLE - Define o tempo do pisca
bool          Tms_Pisca_Chave_Alterar;             // PISCA SINGLE - Define a chave do estado
bool          Tms_Habilitar_Pisca;                 // CHAVE - Ativa o loop do Pisca-Pisca

byte          Tms_Efeito_SelecEfeito[leds];        // EFEITO - Seleciona efeito (0 = off, 1 = 0..100->0, 2= 0...100...0)
unsigned long Tms_Efeito_millis[3];                // EFEITO - Armazena o valor do millis()
int           Tms_Efeito_tempo[3];                 // Efeito - Define o tempo do ciclo do efeito
float         Tms_Efeito_contador[3][leds];        // EFEITO - Contabiliza o add do duty
byte          Tms_Efeito_contador_Var[3];          // EFEITO - Valor a ser contabilizado
bool          Tms_Efeito_2_chave;                  // EFEITO - Intercala entre ADD/Sub do duty
bool          Tms_Habilitar_Efeito[3];             // CHAVE  - Ativa o loop do Efeito1
float         Tms_Efeito_TopDuty[leds];            // EFEITO - Define DutyMax para efeito NO pino.

byte          Tms_Limite_Brilho[leds];             // GERAL - Define o brlho máximo dos Leds
byte          Tms_DutySimples[leds];               // PWM_Simples = Define o duty

#ifdef Def_TMS595_PwmPrecisao
int       linhas_count = 0;            // GERAL - Define o estado/linha do motor a ser computado
#endif

// ================================================================
// Definição de valores iniciais obrigatórios
// ================================================================
void Tms74hc595_VI() {
  Tms_Pisca_Tempo = 1000;

  for (byte i = 0; i < leds; i++) {
    Tms_Efeito_TopDuty[i] = 100;
    Tms_Efeito_contador[0][i] = 0.00;
    Tms_Efeito_contador[1][i] = 0.00;
    Tms_Efeito_contador[2][i] = 100.00;
    Tms_Limite_Brilho[i] = 100;
  }

  Tms_Pisca_Chave_Alterar = 1;
  for (byte i = 0; i < 3; i++) {
    Tms_Efeito_tempo[i] = 10;;
    Tms_Efeito_contador_Var[i] = 1;
  }
}

void Tms74hc595_Manager() {
  ////////////////////////////////////////////////////////////////
  //               ETAPA 1: ONTROLE DE EFEITOS                  //
  ////////////////////////////////////////////////////////////////
  // =================================================
  // EFEITO 1: Crescente -> Ao chegar em 100%, retorna imediatamente para 0%
  // =================================================
  if (Tms_Habilitar_Efeito[0] == 1) {
    if (millis() - Tms_Efeito_millis[0] > Tms_Efeito_tempo[0]) {
      for (byte i = 0; i < leds; i++) {
        if ((Tms_Efeito_SelecEfeito[i] == 1) && (Tms_Efeito_TopDuty[i])) Tms_Efeito_contador[0][i] = (Tms_Efeito_contador[0][i] >= Tms_Efeito_TopDuty[i]) ? 0 : Tms_Efeito_contador[0][i] + (Tms_Efeito_contador_Var[0] * Tms_Efeito_TopDuty[i] / 100);
      }
      Tms_Efeito_millis[0] = millis();
    }
  }
  // ================================
  // EFEITO 2: Cresce e Decresce -> Vai de loop(0% á 100% á 0%)
  // ================================
  if (Tms_Habilitar_Efeito[1] == 1) {
    if (millis() - Tms_Efeito_millis[1] > Tms_Efeito_tempo[1]) {
      for (byte i = 0; i < leds; i++) {
        if ((Tms_Efeito_SelecEfeito[i] == 2) && (Tms_Efeito_TopDuty[i])) {
          if (Tms_Efeito_contador[1][i] >= Tms_Efeito_TopDuty[i]) Tms_Efeito_2_chave = 0;
          else if (Tms_Efeito_contador[1][i] <= 0) Tms_Efeito_2_chave = 1;
          Tms_Efeito_contador[1][i] += ((Tms_Efeito_2_chave) ? Tms_Efeito_contador_Var[1] : -Tms_Efeito_contador_Var[1]) * Tms_Efeito_TopDuty[i] / 100;
        }
      }
      Tms_Efeito_millis[1] = millis();
    }
  }
  // =================================================
  // EFEITO 3: Decrescente -> Ao chegar em 0%, retorna imediatamente para 100%
  // =================================================
  if (Tms_Habilitar_Efeito[2] == 1) {
    if (millis() - Tms_Efeito_millis[2] > Tms_Efeito_tempo[2]) {
      for (byte i = 0; i < leds; i++) {
        if ((Tms_Efeito_SelecEfeito[i] == 3) && (Tms_Efeito_TopDuty[i]))
          Tms_Efeito_contador[2][i] = (Tms_Efeito_contador[2][i] > 0) ? Tms_Efeito_contador[2][i] - (Tms_Efeito_contador_Var[2] * Tms_Efeito_TopDuty[i] / 100) : Tms_Efeito_TopDuty[i];
      }
      Tms_Efeito_millis[2] = millis();
    }
  }
  ////////////////////////////////////////////////////////////////
  //                   ETAPA 2: PROCESSAMENTO                   //
  ////////////////////////////////////////////////////////////////
  // ===================================================================
  // Processar efeito: Define Duty Base e o atualiza com efeitos
  // ===================================================================
#ifdef Def_TMS595_CORRECAO      // Caso a correção do duty 0% seja necessária
  byte zerar_count[3] = {0, 0, 0};
#endif

  for (int i = 0; i < leds; i++) {
    switch (Tms_Efeito_SelecEfeito[i]) {
      case 0: TMa[i] =  Tms_DutySimples[i] * Tms_Limite_Brilho[i] / 100;   break;// Define o Duty Base
      case 1: TMa[i] = ((linhas * Tms_Efeito_contador[0][i]) / 100) * Tms_Limite_Brilho[i] / 100;  break;// Atualiza o Duty, caso efeito 1 ativado
      case 2: TMa[i] = ((linhas * Tms_Efeito_contador[1][i]) / 100) * Tms_Limite_Brilho[i] / 100;  break;// Atualiza o Duty, caso efeito 2 ativado
      case 3: TMa[i] = ((linhas * Tms_Efeito_contador[2][i]) / 100) * Tms_Limite_Brilho[i] / 100;  break;// Atualiza o Duty, caso efeito 3 ativado
    }
#ifdef Def_TMS595_CORRECAO_2
    if (TMa[i] == linhas) TMa[i] -= 1;
#endif
    // ======================================================================
    // Correção: Força todos os leds, de mesmo efeito, ao
    // ======================================================================
#ifdef Def_TMS595_CORRECAO
    if ((TMa[i] == 0) && (Tms_Efeito_TopDuty[i])) {
      if (Tms_Efeito_SelecEfeito[i] == 1)zerar_count[0]++;
      if (Tms_Efeito_SelecEfeito[i] == 2)zerar_count[1]++;
      if (Tms_Efeito_SelecEfeito[i] == 3)zerar_count[2]++;
    }
#endif
  }
#ifdef Def_TMS595_CORRECAO
  if (zerar_count[0]) {
    for (byte i = 2; i < leds; i++) {
      if (Tms_Efeito_SelecEfeito[i] == 1) TMa[i] = 0;
    }
  }
  zerar_count[0] = 0;
  if (zerar_count[1]) {
    for (byte i = 2; i < leds; i++) {
      if (Tms_Efeito_SelecEfeito[i] == 2) TMa[i] = 0;
    }
  }
  zerar_count[1] = 0;
  if (zerar_count[2]) {
    for (byte i = 2; i < leds; i++) {
      if (Tms_Efeito_SelecEfeito[i] == 3) TMa[i] = 0;
    }
  }
  zerar_count[2] = 0;
#endif                       // FIM DA CORREÇÃO

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
void Tms74hc595_Motor() {
#ifdef Def_TMS595_PwmPadrao
  for (int i = 0; i < linhas; i++) {                                // Matriz[x]
#endif
  REG_WRITE(GPIO_OUT_W1TC_REG, _LATCH);                         // (Latch) LOW, para captação de dados
  for (int i2 = 0; i2 < leds; i2++) {                                   // Matriz[y]

#ifdef Def_TMS595_PwmPrecisao
    if ((TMa[i2] > 0) && (TMa[i2] > linhas_count)) {                               // Definição para Duty do PWM
#endif
#ifdef Def_TMS595_PwmPadrao
    if ((TMa[i2] > 0) && (TMa[i2] > i)) {                               // Definição para Duty do PWM
#endif

      if ((Tms_Pisca_OnOff[i2] == 1) && (Tms_Pisca_Chave_Alterar == 0)) {  // Cond check para pisca single ou mult.
        REG_WRITE(GPIO_OUT_W1TC_REG, _DATA);                 // (DATA) Se o pisca estiver ativo e "low", o "high" do pino fica low.
      }
      else REG_WRITE(GPIO_OUT_W1TS_REG, _DATA);              // (DATA) Caso contrário, HIGH

    }
    else REG_WRITE(GPIO_OUT_W1TC_REG, _DATA) ;                  // (DATA) Cond. para duty -
      
    REG_WRITE(GPIO_OUT_W1TC_REG, _CLOCK);                    // (CLOCK) LOW
    REG_WRITE(GPIO_OUT_W1TS_REG, _CLOCK);                    // (CLOCK) HIGH
  }
  REG_WRITE(GPIO_OUT_W1TS_REG, _LATCH);                      // (LATCH) HIGH. Conclui edição e envia a informação.

#ifdef Def_TMS595_PwmPrecisao
  if (linhas_count < linhas) linhas_count++;
  else linhas_count = 0;
#endif
#ifdef Def_TMS595_PwmPadrao
}
#endif
}

#include <TMS_74HC595_FUNC.h>


#ifdef TmsINTERRUPT
// ==================================
// Caso a interrupção esteja ATIVADA
// ==================================
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  Tms74hc595_Manager();           // Gerencia os efeitos
  Tms74hc595_Motor();                 // Contabiliza e envia os dados para o hc595
  portEXIT_CRITICAL_ISR(&timerMux);
}

void hc595_interrupcao() {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, interruptTimer, true);
  timerAlarmEnable(timer);
}
#endif
// ==================================
// Caso a Execução seja pelo Core0
// ==================================
#ifdef TmsFreeRTOS_C0
void TaskTMS74hc595(void *pvParameters) {
  (void) pvParameters;
  for (;;)
  {
    Tms74hc595_Manager();           // Gerencia os efeitos
    Tms74hc595_Motor();
    vTaskDelay(TmsFreeRTOS_Delay);
  }
}
#endif

// ==================================
// Execução através do void loop()
// ==================================
void Tms74HC595_Main() {
#ifdef TmsVoidLoop
  Tms74hc595_Manager();           // Gerencia os efeitos
  Tms74hc595_Motor();                 // Contabiliza e envia os dados para o hc595
#endif
}


void Tms74hc595_setup() {
  //void setup() {
  pinMode(DATA, OUTPUT);                // data
  pinMode(CLOCK, OUTPUT);                // Clock
  pinMode(LATCH, OUTPUT);                // Latch
  Tms74hc595_VI();                        // Utilizado para definir os valores iniciais nos vetores necessários
  Tms74hc595_RESET();                     // Da um reset, para o caso de algo não ter carregado corretamente.
#ifdef TmsINTERRUPT
  hc595_interrupcao();
  Serial.println("74hc595 - Interrupção");
#endif
#ifdef TmsFreeRTOS_C0
  xTaskCreatePinnedToCore(TaskTMS74hc595, "TaskTMS74hc595", 1024, NULL, 2, NULL, 0);
  Serial.println("74hc595 - FreeRTOS");
#endif
#ifdef TmsVoidLoop
  Serial.println("74hc595 - VOID LOOP");
#endif
}

//void loop()