#include "input.h"
#include "draw.h"
#include <stdint.h>

uint8_t gpio_pins[] = {GPIO_LEFT, GPIO_RIGHT, ACTION, SELECT};
const int gpio_count = sizeof(gpio_pins) / sizeof(gpio_pins[0]);

static QueueHandle_t gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg) {
    int gpio_num = (int)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

void InitGPIO() {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_NEGEDGE; // Trigger on falling edge (button press)
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 0;
    io_conf.pin_bit_mask = 0;
    for (int i = 0; i < gpio_count; i++) {
        io_conf.pin_bit_mask |= (1ULL << gpio_pins[i]);
    }
    gpio_config(&io_conf);
    
    gpio_evt_queue = xQueueCreate(10, sizeof(int));
    
    gpio_install_isr_service(0);
    for (int i = 0; i < gpio_count; i++) {
        gpio_isr_handler_add(gpio_pins[i], gpio_isr_handler, (void*)(intptr_t)gpio_pins[i]);
    }
    
    printf("Buttons Configured!\n");
}

void handleInput() {
    int gpio_num;
    static uint32_t last_press_time = 0;
    uint32_t current_time = xTaskGetTickCount();

    if (xQueueReceive(gpio_evt_queue, &gpio_num, pdMS_TO_TICKS(10))) {
        if (current_time - last_press_time > pdMS_TO_TICKS(300)) {
            printf("Button GPIO %d pressed\n", gpio_num);
            last_press_time = current_time;
            
            game_state.GAME_BOARD[player.position.row][player.position.col] = 0;
            
            
            switch (gpio_num) {
                case GPIO_LEFT: 
                	if(player.position.col == 0){
						game_state.GAME_BOARD[player.position.row][player.position.col] = 1;
						break;
					}
					erase_spaceship();
                    player.position.col -= 1;                   
                    game_state.GAME_BOARD[player.position.row][player.position.col] = 1;
                    draw_spaceship(BLACK);
                    break;
                case GPIO_RIGHT:
                	if(player.position.col == COLS-1){
						game_state.GAME_BOARD[player.position.row][player.position.col] = 1;
						break;
					}
					erase_spaceship();
                    player.position.col += 1;
                    game_state.GAME_BOARD[player.position.row][player.position.col] = 1;
                    draw_spaceship(BLACK);
                    break;
                case ACTION:
                    break;
                case SELECT:
                default:
                    break;
            }
            print_board();
        }
    }
}
