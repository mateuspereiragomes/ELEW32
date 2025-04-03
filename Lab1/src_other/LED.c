/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Funções de baixo nível para controle de LEDs
 * Note(s): Implementa o controle básico para LEDs conectados em PN1, PN0, PF4 e PF0
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <TM4C129.h>  // Registradores do microcontrolador TM4C129
#include "LED.h"      // Definições da interface LED

// Mapeamento dos LEDs para seus respectivos pinos:
// LED0: PN1, LED1: PN0, LED2: PF4, LED3: PF0
const uint32_t led_mask[] = { 1UL << 1, 1UL << 0, 1UL << 4, 1UL << 0  };

/*----------------------------------------------------------------------------
  LED_Initialize - Configura os pinos dos LEDs como saídas digitais
  Habilita os clocks e configura os pinos GPIO para os LEDs:
  - PN1 (LED0), PN0 (LED1) - Port N
  - PF4 (LED2), PF0 (LED3) - Port F (usando AHB para melhor performance)
 *----------------------------------------------------------------------------*/
void LED_Initialize (void) {
  /* Habilita clock para os GPIO Port N (bit 12) e Port F (bit 5) */
  SYSCTL->RCGCGPIO |= (1UL << 12) | (1UL << 5);  /* enable clock for GPIOs    */
  
  /* Configura Port N - LEDs 0 e 1 */
  GPION->DIR     |= led_mask[0] | led_mask[1];   /* PN1, PN0 como saídas      */
  GPION->DEN     |= led_mask[0] | led_mask[1];   /* Habilita função digital   */
  
  /* Configura Port F (AHB) - LEDs 2 e 3 */
  GPIOF_AHB->DIR |= led_mask[2] | led_mask[3];   /* PF4, PF0 como saídas      */
  GPIOF_AHB->DEN |= led_mask[2] | led_mask[3];   /* Habilita função digital   */
}

/*----------------------------------------------------------------------------
  LED_On - Acende um LED específico
  @param num: Número do LED (0 a LED_NUM-1)
  Operação atômica via escrita no registrador DATA (seta o bit correspondente)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {
  if (num < LED_NUM) {  // Verifica se o LED é válido
    switch (num) {
      case 0: GPION->DATA     |=  led_mask[num]; break;  // LED0 (PN1)
      case 1: GPION->DATA     |=  led_mask[num]; break;  // LED1 (PN0)
      case 2: GPIOF_AHB->DATA |=  led_mask[num]; break;  // LED2 (PF4)
      case 3: GPIOF_AHB->DATA |=  led_mask[num]; break;  // LED3 (PF0)
    }
  }
}

/*----------------------------------------------------------------------------
  LED_Off - Apaga um LED específico
  @param num: Número do LED (0 a LED_NUM-1)
  Operação atômica via escrita no registrador DATA (limpa o bit correspondente)
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) {
  if (num < LED_NUM) {  // Verifica se o LED é válido
    switch (num) {
      case 0: GPION->DATA     &= ~led_mask[num]; break;  // LED0 (PN1)
      case 1: GPION->DATA     &= ~led_mask[num]; break;  // LED1 (PN0)
      case 2: GPIOF_AHB->DATA &= ~led_mask[num]; break;  // LED2 (PF4)
      case 3: GPIOF_AHB->DATA &= ~led_mask[num]; break;  // LED3 (PF0)
    }
  }
}

/*----------------------------------------------------------------------------
  LED_Out - Controla todos os LEDs simultaneamente com um padrão de bits
  @param value: Máscara de bits onde cada bit representa o estado de um LED
  (bit 0 = LED0, bit 1 = LED1, etc. - 1 = aceso, 0 = apagado)
 *----------------------------------------------------------------------------*/
void LED_Out(uint32_t value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {  // Itera por cada LED
    if (value & (1<<i)) {          // Verifica se o bit correspondente está setado
      LED_On (i);                  // Acende o LED se o bit for 1
    } else {
      LED_Off(i);                  // Apaga o LED se o bit for 0
    }
  }
}