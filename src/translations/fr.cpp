/*
 * Authors (alphabetical order)
 * - Bertrand Songis <bsongis@gmail.com>
 * - Bryan J. Rentoul (Gruvin) <gruvin@gmail.com>
 * - Cameron Weeks <th9xer@gmail.com>
 * - Erez Raviv
 * - Jean-Pierre Parisy
 * - Karl Szmutny <shadow@privy.de>
 * - Michael Blandford
 * - Michal Hlavinka
 * - Pat Mackenzie
 * - Philip Moss
 * - Rob Thomson
 * - Romolo Manfredini <romolo.manfredini@gmail.com>
 * - Thomas Husterer
 *
 * open9x is based on code named
 * gruvin9x by Bryan J. Rentoul: http://code.google.com/p/gruvin9x/,
 * er9x by Erez Raviv: http://code.google.com/p/er9x/,
 * and the original (and ongoing) project by
 * Thomas Husterer, th9x: http://code.google.com/p/th9x/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "../open9x.h"

enum FrenchPrompts {
  PROMPT_NUMBERS_BASE = 0,
  PROMPT_ZERO = PROMPT_NUMBERS_BASE+0,
  /* ... */
  PROMPT_VINGT = PROMPT_NUMBERS_BASE+20,
  PROMPT_TRENTE = PROMPT_NUMBERS_BASE+21,
  /* ... */
  PROMPT_QUATREVINGTDIX = PROMPT_NUMBERS_BASE+27,
  PROMPT_CENT = PROMPT_NUMBERS_BASE+28,
  PROMPT_MILLE = PROMPT_NUMBERS_BASE+29,

  PROMPT_HEURE = 40,
  PROMPT_MINUTE = 41,
  PROMPT_SECONDE = 42,

  PROMPT_ET = 47,
  PROMPT_MOINS = 48,

  PROMPT_UNITS_BASE = 50,
  PROMPT_VOLTS = PROMPT_UNITS_BASE+UNIT_VOLTS,
  PROMPT_AMPS = PROMPT_UNITS_BASE+UNIT_AMPS,
  PROMPT_METERS_PER_SECOND = PROMPT_UNITS_BASE+UNIT_METERS_PER_SECOND,
  PROMPT_SPARE1 = PROMPT_UNITS_BASE+UNIT_RAW,
  PROMPT_KMH = PROMPT_UNITS_BASE+UNIT_KMH,
  PROMPT_METERS = PROMPT_UNITS_BASE+UNIT_METERS,
  PROMPT_DEGREES = PROMPT_UNITS_BASE+UNIT_DEGREES,
  PROMPT_PERCENT = PROMPT_UNITS_BASE+UNIT_PERCENT,
  PROMPT_MILLIAMPS = PROMPT_UNITS_BASE+UNIT_MILLIAMPS,
};

#if defined(SOMO)

void playNumber(int16_t number, uint8_t unit, uint8_t att)
{
/*  if digit >= 1000000000:
      temp_digit, digit = divmod(digit, 1000000000)
      prompts.extend(self.getNumberPrompt(temp_digit))
      prompts.append(Prompt(GUIDE_00_BILLION, dir=2))
  if digit >= 1000000:
      temp_digit, digit = divmod(digit, 1000000)
      prompts.extend(self.getNumberPrompt(temp_digit))
      prompts.append(Prompt(GUIDE_00_MILLION, dir=2))
*/
  if (number >= 1000) {
    if (number >= 2000)
      playNumber(number / 1000);
    pushPrompt(PROMPT_MILLE);
    number %= 1000;
    if (number == 0)
      return;
  }
  if (number >= 100) {
    if (number >= 200)
      pushPrompt(PROMPT_ZERO + number/100);
    pushPrompt(PROMPT_CENT);
    number %= 100;
    if (number == 0)
      return;
  }
  if (number >= 20) {
    pushPrompt(PROMPT_VINGT + (number-20)/10);
    number %= 10;
    if (number == 0)
      return;
  }
  pushPrompt(PROMPT_ZERO+number);

  if (unit) {
    pushPrompt(PROMPT_UNITS_BASE+unit-1);
  }
}

void playDuration(int16_t seconds)
{
  if (seconds < 0) {
    pushPrompt(PROMPT_MOINS);
    seconds = -seconds;
  }

  uint8_t tmp = seconds / 3600;
  seconds %= 3600;
  if (tmp > 0) {
    playNumber(tmp);
    pushPrompt(PROMPT_HEURE);
  }

  tmp = seconds / 60;
  seconds %= 60;
  if (tmp > 0) {
    playNumber(tmp);
    pushPrompt(PROMPT_MINUTE);
    if (seconds > 0)
      pushPrompt(PROMPT_ET);
  }

  if (seconds > 0) {
    playNumber(seconds);
    pushPrompt(PROMPT_SECONDE);
  }
}

#endif