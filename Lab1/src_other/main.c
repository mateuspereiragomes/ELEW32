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
 * @brief    Basic demo of kit functionality - alterna LEDs em sequ�ncia
 * @version  v2025-03 for Keil
 ******************************************************************************/

/*------------------------------------------------------------------------------
 *
 *      File includes
 *
 *------------------------------------------------------------------------------*/
#include <stdio.h>                  // Biblioteca padr�o de I/O
#include "TM4C129.h"                // Cabe�alho espec�fico do microcontrolador TM4C129
#include "LED.h"                    // Biblioteca para controle dos LEDs
#include "BTN.h"                    // Biblioteca para leitura dos bot�es

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
  Incrementa o contador de msTicks a cada interrup��o (1ms)
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        // Incrementa o contador de milissegundos
}

/*----------------------------------------------------------------------------
  Delay - Cria um atraso baseado no SysTick
  @param dlyTicks - n�mero de milissegundos para o atraso
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;               // Armazena o valor atual do contador
  // Aguarda at� que a diferen�a de tempo atinja o valor desejado
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); } // __NOP() = No Operation (otimiza o loop)
}

/**
 * Main function - Ponto de entrada do programa
 * @returns int    - nunca retorna (loop infinito)
 * @brief - Demonstra��o simples que alterna LEDs em sequ�ncia ou aciona todos quando bot�o pressionado
 */
int main (void) {
  int32_t idx  = -1, dir = 1;       // idx: �ndice do LED atual, dir: dire��o da sequ�ncia (1=forward, -1=backward)
  uint32_t btns = 0;                // Estado dos bot�es

  // Inicializa��es:
  SystemCoreClockUpdate();           // Atualiza a vari�vel SystemCoreClock com a frequ�ncia do clock
  
  LED_Initialize();                 // Configura os LEDs
  BTN_Initialize();                 // Configura os bot�es

  // Configura SysTick para interrup��o a cada 1ms
  SysTick_Config(SystemCoreClock / 1000ul);

  // Loop principal
  while(1) {
    btns = BTN_Get();               // L� o estado dos bot�es

    if (btns == 0) {                // Se nenhum bot�o est� pressionado
      /* L�gica de sequ�ncia de LEDs:
        0?1?...?LED_NUM-1?LED_NUM-1?...?1?0?0?... */
      idx += dir;
      if (idx == LED_NUM) { dir = -1; idx =  LED_NUM-1; } // Inverte dire��o no final
      else if (idx < 0)   { dir =  1; idx =  0;         } // Inverte dire��o no in�cio

      LED_On (idx);                 // Acende o LED atual
      Delay(200);                   // Espera 200ms
      LED_Off(idx);                 // Apaga o LED atual
    }
    else {                          // Se algum bot�o est� pressionado
      LED_Out ((1ul << LED_NUM) -1); // Acende todos os LEDs (m�scara de bits)
      Delay(200);                   // Espera 200ms
      LED_Out (0x00);               // Apaga todos os LEDs
    }
  }
}