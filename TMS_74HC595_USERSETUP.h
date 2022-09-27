// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  =====================================================================================
//                     CONFIGURAÇÕES PARA O USUÁRIO
//  =====================================================================================
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ======= Informações do CI =======
#define DATA 5              // Definir o pino do _DATA
#define CLOCK 12            // Definir o pino do Clock
#define LATCH 27            // Definir o pino do Latch
#define Quant_74hc595 1     // Definir a quantidade de 74hc595 em série

// ======= RANGE DO PWM ========
// O range deve ser DE 10 á 250
// 10 = Grande Performance, Baixa qualidade  ///   250 = Grande Qualidade, baixa performance
// Obs- Para o PwmPrecisao, quanto menor o range e menor a interrupção, maior a frequência.
// Para referência (Range ~ Ciclos por segundo): 
// 		10 ~  21.000
// 		50 ~  6.300
//		100 ~ 3.300
// 		150 ~ 2300 
//		200 ~ 1700
//		250 ~ 1400
#define Tms595Pwm_Range 250

// ======= Veículo de execução da função =========
//#define TmsINTERRUPT 		// Interrupção 
#define TmsFreeRTOS_C0 		// Core0 FreeRTOS 
//#define TmsVoidLoop 	    // Core1, void loop()

// ======= PWM de precisão ========
// Utiliza um tempo de intervalo definido (interrupção) para cada ponto do duty.
// recomendado com o uso da interrupção.

// #define Def_TMS595_PwmPrecisao
#define Def_TMS595_PwmPadrao

// ====== Definição do tempo de interrupção para o código completo =======
#define interruptTimer 250 // valor em ms. (cuidado)

// ====== Definição do tempo de delay com FreeRTOS ========
#define TmsFreeRTOS_Delay 1


// ======= CORREÇÃO do brilho ao atingir 0% nos efeitos (sincronia) =======
// Define todos os dutys igual a 0%, caso algum led de mesmo efeito atinja 0%.

//#define Def_TMS595_CORRECAO  		  		// Caso utilize o PwmPrecisao, mantenha comentada esta linha

// ======= Correção para Interrupção e FreeRTOS ========
// Por causa das "pausas" entre leituras, pode ocorrer grande diferença entre duty 100% e 99%

#define Def_TMS595_CORRECAO_2  			// Caso utilize o PwmPrecisao, mantenha comentada esta linha


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  =====================================================================================
//                     FIM DAS CONFIGURAÇÕES PARA O USUÁRIO
//  =====================================================================================
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#include <TMS_74HC595_PINREF.h>