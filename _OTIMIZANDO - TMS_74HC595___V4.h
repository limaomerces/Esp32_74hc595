#define Quant_74hc595 2               // Definir a quantidade de 74hc595 em série
#define leds (Quant_74hc595 * 8)      // não alterar
#define linhas 101   //101                 // em TESE, quanto menor, maior performance. Na prática, não vi diferença (olho nú)
#define DATA_bit BIT5
#define CLOCK_bit BIT12
#define LATCH_bit BIT27

#define Tms_Limite_Brilho_Inicial 60
#define Tms_Efeito_1_tempo_Inicial 10
#define Tms_Efeito_2_tempo_Inicial 10

byte          TMa[leds];                           // BASIC - Matriz responsável por ARMAZENAR o duty para o calculo.
bool          Tms_Pisca_OnOff[leds];               // PISCA - Marcador para definição de PISCA-PISCA (on, off) por led
unsigned long Tms_Pisca_Millis;                    // PISCA SINGLE - Millis
int           Tms_Pisca_Tempo;                     // PISCA SINGLE - Define o tempo do pisca
bool          Tms_Pisca_Chave_Alterar_loop_Single; // PISCA SINGLE - Define a chave do estado
byte          Tms_Efeito_SelecEfeito[leds];        // EFEITO - Seleciona efeito (0 = off, 1 = 0..100->0, 2= 0...100...0)
unsigned long Tms_Efeito_1_millis;                 // EFEITO - Armazena o valor do millis()
int           Tms_Efeito_1_tempo;                  // Efeito - Define o tempo do ciclo do efeito
byte          Tms_Efeito_1_contador;               // EFEITO - Contabiliza o add do duty
byte          Tms_Efeito_1_contador_Var;           // EFEITO - Valor a ser contabilizado
unsigned long Tms_Efeito_2_millis;                 // EFEITO - Armazena o valor do millis()
int           Tms_Efeito_2_tempo;                  // Efeito - Define o tempo do ciclo do efeito
bool          Tms_Efeito_2_chave;                  // EFEITO - Intercala entre ADD/Sub do duty
byte          Tms_Efeito_2_contador;               // EFEITO - Contabiliza os add/red do duty
byte          Tms_Efeito_2_contador_Var;           // EFEITO - Valor a ser contabilizado
byte          Tms_Limite_Brilho;                   // GERAL - Define o brlho máximo dos Leds
int           Tms_Efeito_1_tempo_Corrigido;        // FIX - Corrige o tempo do ciclo
int           Tms_Efeito_2_tempo_Corrigido;        // FIX - Corrige o tempo do ciclo
byte          Tms_DutySimples[leds];               // PWM_Simples = Define o duty
bool Tms_Habilitar_Efeito_1;					   // CHAVE - Ativa o loop do Efeito1
bool Tms_Habilitar_Efeito_2;					   // CHAVE - Ativa o loop do Efeito2
bool Tms_Habilitar_Pisca;						   // CHAVE - Ativa o loop do Pisca-Pisca

// ================================================================
// Definição de valores iniciais obrigatórios
// ================================================================
void pwm595_VI() {
  Tms_Pisca_Tempo = 1000;
  Tms_Efeito_1_tempo = Tms_Efeito_1_tempo_Inicial;
  Tms_Efeito_2_tempo = Tms_Efeito_2_tempo_Inicial;
  Tms_Efeito_2_chave = 0;
  Tms_Efeito_2_contador = 0;
  Tms_Efeito_2_contador_Var = 1;
  Tms_Efeito_1_contador = 0;
  Tms_Efeito_1_contador_Var = 1;
  Tms_Pisca_Chave_Alterar_loop_Single = 1;
  Tms_Limite_Brilho = Tms_Limite_Brilho_Inicial;
  Tms_Efeito_1_tempo_Corrigido = (100 / Tms_Limite_Brilho) * Tms_Efeito_1_tempo;
  Tms_Efeito_2_tempo_Corrigido = (100 / Tms_Limite_Brilho) * Tms_Efeito_2_tempo;
}

void Pwm595_DutySimples() {
  for (int i = 0; i < leds; i++) {
    TMa[i] = Tms_DutySimples[i];
  }
}

void Pwm595_Efeitos() {
  // =================================================
  // EFEITO 1: Ao chegar em 100%, retorna imediatamente para 0%
  // =================================================
  if (Tms_Habilitar_Efeito_1 == 1){
  if (millis() - Tms_Efeito_1_millis > Tms_Efeito_1_tempo_Corrigido) {
    Tms_Efeito_1_contador += Tms_Efeito_1_contador_Var;
    if (Tms_Efeito_1_contador > Tms_Limite_Brilho) Tms_Efeito_1_contador = 0;
    Tms_Efeito_1_millis = millis();
  }
  }
  // ================================
  // EFEITO 2: Ao chegar em 100%, decai até 0%
  // ================================
  if (Tms_Habilitar_Efeito_2 == 1){
  if (millis() - Tms_Efeito_2_millis > Tms_Efeito_2_tempo_Corrigido) {
    if (Tms_Efeito_2_contador == Tms_Limite_Brilho) Tms_Efeito_2_chave = 0;
    else if (Tms_Efeito_2_contador == 0) Tms_Efeito_2_chave = 1;
    if (Tms_Efeito_2_chave == 0) Tms_Efeito_2_contador -= Tms_Efeito_2_contador_Var;
    else if (Tms_Efeito_2_chave == 1) Tms_Efeito_2_contador += Tms_Efeito_2_contador_Var;
    Tms_Efeito_2_millis = millis();
  }
  }
  // =======================================
  // Processar efeito: Atualização do duty, caso efeito ativo
  // =======================================
  for (int i = 0; i < leds; i++) {
    if (Tms_Efeito_SelecEfeito[i] == 1) TMa[i] = (linhas * Tms_Efeito_1_contador) / 100;
    else if (Tms_Efeito_SelecEfeito[i] == 2) TMa[i] = (linhas * Tms_Efeito_2_contador) / 100;
  }
}

// ================================================================
// Responsável por gerenciar o pisca no código
// ================================================================
void pwm595_Pisca() {
  if (Tms_Habilitar_Pisca == 1){
  if (millis() - Tms_Pisca_Millis > Tms_Pisca_Tempo) {
    Tms_Pisca_Chave_Alterar_loop_Single = !Tms_Pisca_Chave_Alterar_loop_Single;
    Tms_Pisca_Millis = millis();
  }
}
}
// ================================================================
// Motor Principal
// ================================================================
void pwm595_Motor() {
  for (int i = 0; i < linhas - 1; i++) {                             // Matriz[x]
    REG_WRITE(GPIO_OUT_W1TC_REG, LATCH_bit);                         // (Latch) LOW, para captação de dados
    for (int i2 = 0; i2 < leds; i2++) {                              // Matriz[y]
      if ((TMa[i2] > 0) && (TMa[i2] > i)) {                          // Definição para Duty do PWM
        if ((Tms_Pisca_OnOff[i2] == 1) && (Tms_Pisca_Chave_Alterar_loop_Single == 0)) {  // Cond check para pisca single ou mult.
          REG_WRITE(GPIO_OUT_W1TC_REG, DATA_bit);                 // (DATA) Se o pisca estiver ativo e "low", o "high" do pino fica low.
        }
        else {
          REG_WRITE(GPIO_OUT_W1TS_REG, DATA_bit);                 // (DATA) Caso contrário, HIGH
        }
      }
      else {
        REG_WRITE(GPIO_OUT_W1TC_REG, DATA_bit) ;                  // (DATA) Cond. para duty -
      }
      REG_WRITE(GPIO_OUT_W1TC_REG, CLOCK_bit);                    // (CLOCK) LOW
      REG_WRITE(GPIO_OUT_W1TS_REG, CLOCK_bit);                    // (CLOCK) HIGH
    }
    REG_WRITE(GPIO_OUT_W1TS_REG, LATCH_bit);                      // (LATCH) HIGH. Conclui edição e envia a informação.
  }
}

#include "_OTIMIZANDO - TMS_74HC595___V4_FUNC.h"

void loop_595() {
//void loop() {
  //Pwm595_DutySimples();   // Definição do duty simples
  Pwm595_Efeitos();       // Efeitos (sobrepõem duty simples)
  pwm595_Pisca();         // Pisca - Acumulativo
  pwm595_Motor();         
}

void setup_595() {
//void setup() {
  pinMode(5, OUTPUT);                // data
  pinMode(12, OUTPUT);                // Clock
  pinMode(27, OUTPUT);                // Latch
  pwm595_VI();                        // Utilizado para definir os valores iniciais nos vetores necessários
  pwm595_RESET();                     // Da um reset, para o caso de algo não ter carregado corretamente.
 // Tms_Limite_Brilho_Func(40);
 // pwm595_Duty_Geral(60);
 // pwm595_Pisca_Geral(1);
 // pwm595_Efeito(2);
}