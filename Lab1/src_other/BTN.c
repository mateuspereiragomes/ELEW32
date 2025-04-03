/*----------------------------------------------------------------------------
 * Name:    BTN.c
 * Purpose: Funções de baixo nível para os botões de pressão
 * Note(s): Implementa a interface básica para leitura dos botões PJ0 e PJ1
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

#include <TM4C129.h>  // Inclui as definições específicas do microcontrolador TM4C129

/*----------------------------------------------------------------------------
  BTN_Initialize - Configura os pinos dos botões (PJ0 e PJ1)
  Configura os pinos PJ0 e PJ1 como entradas digitais com pull-up
 *----------------------------------------------------------------------------*/
void BTN_Initialize (void) {
  // Habilita o clock para o GPIO Port J (bit 8 no registrador RCGCGPIO)
  SYSCTL->RCGCGPIO |= (1ul << 8);                /* enable clock for GPIOs    */
  
  // Configura os pinos PJ0 e PJ1 para drive de 2mA (DR2R = 2mA Drive Select)
  GPIOJ_AHB->DR2R |=  ((1ul << 0) | (1ul << 1)); /* PJ0, PJ1 2-mA Drive       */
  
  // Habilita resistores de pull-up internos para PJ0 e PJ1
  GPIOJ_AHB->PUR  |=  ((1ul << 0) | (1ul << 1)); /* PJ0, PJ1 pull-up          */
  
  // Configura PJ0 e PJ1 como entradas (DIR = Direction, 0 = input)
  GPIOJ_AHB->DIR  &= ~((1ul << 0) | (1ul << 1)); /* PJ0, PJ1 is intput        */
  
  // Habilita a função digital para PJ0 e PJ1 (DEN = Digital Enable)
  GPIOJ_AHB->DEN  |=  ((1ul << 0) | (1ul << 1)); /* PJ0, PJ1 is digital func. */
}

/*----------------------------------------------------------------------------
  BTN_Get - Lê o estado dos botões
  @return: Máscara de bits onde cada bit representa o estado de um botão (1 = pressionado)
  Nota: A lógica é invertida (~) porque os botões estão configurados com pull-up
        (0V quando pressionado, 3.3V quando solto)
 *----------------------------------------------------------------------------*/
uint32_t BTN_Get (void) {
  // Lê o registrador DATA e aplica máscara para PJ0 e PJ1
  // Inverte os bits (~) porque a lógica é ativa-baixa (pull-up)
  return (~(GPIOJ_AHB->DATA) & ((1ul << 0) | (1ul << 1)));
}