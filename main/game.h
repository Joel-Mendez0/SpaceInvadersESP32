#ifndef MAIN_GAME_H_
#define MAIN_GAME_H_

#include <stdio.h>
#include "esp_lcd_panel_io.h"
#include "config.h"
#include "state.h"
#include "freertos/FreeRTOS.h"

extern esp_lcd_panel_handle_t panel_handle;

void game_task();
void initialize_game_task();


#endif