// Copyright 2014 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Driver for the front panel switches.
// SW1 == SW_REC -> SW_FREEZE == PC2 
// SW2 == SW_MODE -> SW_BUF == PB6
// SW3 == SW_LOAD -> SW_MEM == PB5

// Supercell Adaptation
// SW2 == SW_MODE -> SW_TIME = PE11
// SW3 == SW_LOAD -> SW_MEM == PE12
// SW1 == SW_CAPTURE -> SW_TRIG == PE9
// SW4 == SW_HOLD -> SW_FREEZE = PE10

// Additional switches 
// SW5/6 == SW_MUTE_IN/OUT
// PE7/8



#include "supercell/drivers/switches.h"

#include <algorithm>

namespace clouds {

using namespace std;

void Switches::Init() {
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
  GPIO_InitTypeDef gpio_init;
  gpio_init.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  gpio_init.GPIO_Pin |= GPIO_Pin_11 | GPIO_Pin_12;
  gpio_init.GPIO_Mode = GPIO_Mode_IN;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_Speed = GPIO_Speed_25MHz;
  gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOE, &gpio_init);

  fill(&switch_state_[0], &switch_state_[kNumSwitches], 0xff);
}

void Switches::Debounce() {
  const uint16_t pins[] = { 
    GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_9,
    GPIO_Pin_10, GPIO_Pin_7, GPIO_Pin_8 
  };
  // All switches on PORTE
  for (uint8_t i = 0; i < kNumSwitches; ++i) {
    switch_state_[i] = (switch_state_[i] << 1) | \
        (!GPIO_ReadInputDataBit(GPIOE, pins[i]));
  }
}

}  // namespace clouds
