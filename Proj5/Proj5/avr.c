/**
 * avr.c
 * Copyright (C) 2001-2021, Tony Givargis
 */

#include "avr.h"

void
avr_init(void)
{
  WDTCR = 15;
}
