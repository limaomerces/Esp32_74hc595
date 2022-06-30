// ==========================================================
//              CONFIGURAÇÕES DO USUÁRIO
// ==========================================================
#define DATA 5              // Definir o pino do _DATA
#define CLOCK 12            // Definir o pino do Clock
#define LATCH 27            // Definir o pino do Latch
#define Tms595_Quant 2      // Definir a quantidade de 74hc595 em série
#define interrupt595 1		// Interrupção (1 = sim. 0 = não)

// Definição do tempo de interrupção para o código completo//
#define interruptTimer 5000 // valor em ms. (cuidado)



// ==========================================================
//         NÃO ALTERAR NADA A PARTIR DESTE PONTO
// ==========================================================

// ~~~~~~~~~~~~~~~~~~~~~~
// ===== Tms595DATA BIT ======
// ~~~~~~~~~~~~~~~~~~~~~~
#if (DATA == 0)
  #define _DATA BIT0
#elif (DATA == 1)
  #define _DATA BIT1
#elif (DATA == 2)
  #define _DATA BIT2
#elif (DATA == 4)
  #define _DATA BIT4
#elif (DATA == 5)
  #define _DATA BIT5
#elif (DATA == 12)
  #define _DATA BIT12
#elif (DATA == 13)
  #define _DATA BIT13
#elif (DATA == 14)
  #define _DATA BIT14
#elif (DATA == 15)
  #define _DATA BIT15
#elif (DATA == 16)
  #define _DATA BIT16
#elif (DATA == 17)
  #define _DATA BIT17
#elif (DATA == 18)
  #define _DATA BIT18
#elif (DATA == 19)
  #define _DATA BIT19
#elif (DATA == 20)
  #define _DATA BIT20
#elif (DATA == 21)
  #define _DATA BIT21
#elif (DATA == 22)
  #define _DATA BIT22
#elif (DATA == 23)
  #define _DATA BIT23
#elif (DATA == 24)
  #define _DATA BIT24
#elif (DATA == 25)
  #define _DATA BIT25
#elif (DATA == 26)
  #define _DATA BIT26
#elif (DATA == 27)
  #define _DATA BIT27
#endif

// ~~~~~~~~~~~~~~~~~~~~~~
// ==== Tms595CLOCK BIT ======
// ~~~~~~~~~~~~~~~~~~~~~~
#if (CLOCK == 0)
  #define _CLOCK BIT0
#elif (CLOCK == 1)
  #define _CLOCK BIT1
#elif (CLOCK == 2)
  #define _CLOCK BIT2
#elif (CLOCK == 4)
  #define _CLOCK BIT4
#elif (CLOCK == 5)
  #define _CLOCK BIT5
#elif (CLOCK == 12)
  #define _CLOCK BIT12
#elif (CLOCK == 13)
  #define _CLOCK BIT13
#elif (CLOCK == 14)
  #define _CLOCK BIT14
#elif (CLOCK == 15)
  #define _CLOCK BIT15
#elif (CLOCK == 16)
  #define _CLOCK BIT16
#elif (CLOCK == 17)
  #define _CLOCK BIT17
#elif (CLOCK == 18)
  #define _CLOCK BIT18
#elif (CLOCK == 19)
  #define _CLOCK BIT19
#elif (CLOCK == 20)
  #define _CLOCK BIT20
#elif (CLOCK == 21)
  #define _CLOCK BIT21
#elif (CLOCK == 22)
  #define _CLOCK BIT22
#elif (CLOCK == 23)
  #define _CLOCK BIT23
#elif (CLOCK == 24)
  #define _CLOCK BIT24
#elif (CLOCK == 25)
  #define _CLOCK BIT25
#elif (CLOCK == 26)
  #define _CLOCK BIT26
#elif (CLOCK == 27)
  #define _CLOCK BIT27
#endif

// ~~~~~~~~~~~~~~~~~~~~~~
// ==== Tms595LATCH BIT ======
// ~~~~~~~~~~~~~~~~~~~~~~
#if (LATCH == 0)
  #define _LATCH BIT0
#elif (LATCH == 1)
  #define _LATCH BIT1
#elif (LATCH == 2)
  #define _LATCH BIT2
#elif (LATCH == 4)
  #define _LATCH BIT4
#elif (LATCH == 5)
  #define _LATCH BIT5
#elif (LATCH == 12)
  #define _LATCH BIT12
#elif (LATCH == 13)
  #define _LATCH BIT13
#elif (LATCH == 14)
  #define _LATCH BIT14
#elif (LATCH == 15)
  #define _LATCH BIT15
#elif (LATCH == 16)
  #define _LATCH BIT16
#elif (LATCH == 17)
  #define _LATCH BIT17
#elif (LATCH == 18)
  #define _LATCH BIT18
#elif (LATCH == 19)
  #define _LATCH BIT19
#elif (LATCH == 20)
  #define _LATCH BIT20
#elif (LATCH == 21)
  #define _LATCH BIT21
#elif (LATCH == 22)
  #define _LATCH BIT22
#elif (LATCH == 23)
  #define _LATCH BIT23
#elif (LATCH == 24)
  #define _LATCH BIT24
#elif (LATCH == 25)
  #define _LATCH BIT25
#elif (LATCH == 26)
  #define _LATCH BIT26
#elif (LATCH == 27)
  #define _LATCH BIT27
#endif
