/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s):
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

// Modified Mar-2025 for compatibility with compiler v6

/**
 * @file     main.c
 * @author   Douglas Renaux
 * @brief    Basic demo of kit functionality - alterna LEDs em sequência
 * @version  v2025-03 for Keil
 ******************************************************************************/

/*------------------------------------------------------------------------------
 *
 *      File includes
 *
 *------------------------------------------------------------------------------*/
#include <stdio.h>                  // Biblioteca padrão de I/O
#include "TM4C129.h"                // Cabeçalho específico do microcontrolador TM4C129
#include "LED.h"                    // Biblioteca para controle dos LEDs
#include "BTN.h"                    // Biblioteca para leitura dos botões

/*------------------------------------------------------------------------------
 *
 *      Typedefs and constants
 *
 *------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 *      Global vars
 *
 *------------------------------------------------------------------------------*/
volatile uint32_t msTicks;          // Contador global de ticks de 1ms (volatile para acesso seguro em ISR)

/*------------------------------------------------------------------------------
 *
 *      Functions
 *
 *------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  SysTick_Handler - Interrupt Service Routine para o timer SysTick
  Incrementa o contador de msTicks a cada interrupção (1ms)
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        // Incrementa o contador de milissegundos
}

/*----------------------------------------------------------------------------
  Delay - Cria um atraso baseado no SysTick
  @param dlyTicks - número de milissegundos para o atraso
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;               // Armazena o valor atual do contador
  // Aguarda até que a diferença de tempo atinja o valor desejado
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); } // __NOP() = No Operation (otimiza o loop)
}

/**
 * Main function - Ponto de entrada do programa
 * @returns int    - nunca retorna (loop infinito)
 * @brief - Demonstração simples que alterna LEDs em sequência ou aciona todos quando botão pressionado
 */
int main (void) {
  int32_t idx  = -1, dir = 1;       // idx: índice do LED atual, dir: direção da sequência (1=forward, -1=backward)
  uint32_t btns = 0;                // Estado dos botões

  // Inicializações:
  SystemCoreClockUpdate();           // Atualiza a variável SystemCoreClock com a frequência do clock
  
  LED_Initialize();                 // Configura os LEDs
  BTN_Initialize();                 // Configura os botões

  // Configura SysTick para interrupção a cada 1ms
  SysTick_Config(SystemCoreClock / 1000ul);

  // Loop principal
  while(1) {
    btns = BTN_Get();               // Lê o estado dos botões

    if (btns == 0) {                // Se nenhum botão está pressionado
      /* Lógica de sequência de LEDs:
        0?1?...?LED_NUM-1?LED_NUM-1?...?1?0?0?... */
      idx += dir;
      if (idx == LED_NUM) { dir = -1; idx =  LED_NUM-1; } // Inverte direção no final
      else if (idx < 0)   { dir =  1; idx =  0;         } // Inverte direção no início

      LED_On (idx);                 // Acende o LED atual
      Delay(200);                   // Espera 200ms
      LED_Off(idx);                 // Apaga o LED atual
    }
    else {                          // Se algum botão está pressionado
      LED_Out ((1ul << LED_NUM) -1); // Acende todos os LEDs (máscara de bits)
      Delay(200);                   // Espera 200ms
      LED_Out (0x00);               // Apaga todos os LEDs
    }
  }
}