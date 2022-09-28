

# TMS Esp32 74hc595 PWM
 
   Objetivo: Gerar sinal PWM, através do CI74hc595 com auxílio do controlador ESP32
 
   Autor: Tiago Mercês Silva
 
  ## Histórico de Atualizações
  
          ??/03/2020 - 1.0   - Controle do 595
          ??/08/2020 - 1.1   - Adição do PWM
          ??/09/2020 - 1.2   - Adição do pisca
          ??/02/2021 - 1.2.1 - Otimização de código
          ??/02/2021 - 1.3   - Adição de efeito crescente
          ??/02/2021 - 1.4   - Adição de efeito "vai-e-volta"
          ??/02/2022 - 1.4.1 - Otimização de código
          ??/03/2022 - 1.4.2 - Otimização de código
          ??/04/2022 - 1.4.3 - Otimização de código
          20/06/2022 - 2.0   - Reestruturação do código (grandes melhorias de performance)
          28/06/2022 - 2.1   - Adição de opção de execução com interrupção.
          29/06/2022 - 2.1.1 - Otimização de Código.
          29/06/2022 - 2.1.2 - Otimização de Código.
          29/06/2022 - 2.2   - Adição do efeito decrescente
          30/06/2022 - 2.2.1 - Redução visual do código.
          30/06/2022 - 2.2.2 - Otimização de código.
          30/06/2022 - 2.3   - Adição de opção de execução com FreeRTOS (core 0)
          01/07/2022 - 2.3.1 - Correções de bug
          01/07/2022 - 2.4   - Adição para vários dutyMax nos efeitos.
          02/07/2022 - 2.4.1 - Melhorias no visual do código
          02/07/2022 - 2.4.2 - Correções de bug e melhorias na identificação das funções
          02/07/2022 - 2.4.3 - Adição de Correção opcional (duty 0% mesmo efeito)
          03/07/2022 - 2.5   - Adição do limite de brilho individual
          09/07/2022 - 2.5.1 - Melhoria das definições do usuário
       	11/09/2022 - 2.6   - Adicionado recurso de PWM com maior precisão
 		19/09/2022 - 2.6.1 - Melhora nas configurações para o usuário
          

## Sobre

Na tentativa de economizar pinos do ESP32, resolvi utilizar o integrado 74HC595, entretanto, era necessário que este trabalhasse com PWM para fornecer controle de brilho.

Sendo assim, resolvi produzir meu próprio código para utilizar em um projeto pessoal e, por aprender com a comunidade, resolvi compartilhar.




## Instruções

Veja o arquivo "instrucoes.txt"




