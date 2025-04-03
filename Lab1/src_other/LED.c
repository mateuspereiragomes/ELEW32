/*----------------------------------------------------------------------------
 * Name:    BTN.c
 * Purpose: Fun��es de baixo n�vel para os bot�es de press�o
 * Note(s): Implementa a interface b�sica para leitura dos bot�es PJ0 e PJ1
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

#include <TM4C129.h>  // Inclui as defini��es espec�ficas do microcontrolador TM4C129

/*----------------------------------------------------------------------------
  BTN_Initialize - Configura os pinos dos bot�es (PJ0 e PJ1)
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
  
  // Habilita a fun��o digital para PJ0 e PJ1 (DEN = Digital Enable)
  GPIOJ_AHB->DEN  |=  ((1ul << 0) | (1ul << 1)); /* PJ0, PJ1 is digital func. */
}

/*----------------------------------------------------------------------------
  BTN_Get - L� o estado dos bot�es
  @return: M�scara de bits onde cada bit representa o estado de um bot�o (1 = pressionado)
  Nota: A l�gica � invertida (~) porque os bot�es est�o configurados com pull-up
        (0V quando pressionado, 3.3V quando solto)
 *----------------------------------------------------------------------------*/
uint32_t BTN_Get (void) {
  // L� o registrador DATA e aplica m�scara para PJ0 e PJ1
  // Inverte os bits (~) porque a l�gica � ativa-baixa (pull-up)
  return (~(GPIOJ_AHB->DATA) & ((1ul << 0) | (1ul << 1)));
}