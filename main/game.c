#include "game.h"
#include "state.h"
#include "input.h"

esp_lcd_panel_handle_t panel_handle;

void game_task() { 
	
	Start_Game();
     
    static uint32_t last_frame_time = 0;
    const uint32_t refresh_period = pdMS_TO_TICKS(1000 / FPS);

    while (1) {
        uint32_t current_frame_time = xTaskGetTickCount();

        if (current_frame_time - last_frame_time >= refresh_period) {
            last_frame_time = current_frame_time;
           
            handleInput();
            Update_Enemies();
            
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void initialize_game_task() {
    xTaskCreate(&game_task, "game_task", 4096, NULL, 5, NULL);
}
