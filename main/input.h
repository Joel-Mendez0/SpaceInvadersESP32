#ifndef MAIN_INPUT_H_
#define MAIN_INPUT_H_

#include <stdint.h>
#include "game.h"
#include "state.h"
#include "driver/gpio.h"
#include "config.h"

extern uint8_t gpio_pins[];
extern const int gpio_count;

void InitGPIO(void);

void handleInput();


#endif /* MAIN_INPUT_H_ */