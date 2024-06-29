#ifndef LCD_DRAW_H
#define LCD_DRAW_H

#include <stdint.h>
#include "state.h"
#include "game.h"
#include "font.h"
#include "esp_lcd_panel_ops.h"

void draw_pixel(int x, int y, uint16_t color);

void draw_line(int x0, int y0, int x1, int y1, uint16_t color, int thickness);

void draw_circle(int xc, int yc, int radius, uint16_t color, int thickness);

void fill_screen_color(uint16_t color);

void draw_char(uint8_t ch, int x, int y, uint16_t color, int scale);

void draw_string(const char *str, int x, int y, uint16_t color, int scale);

void draw_rectangle(int x, int y, int width,int height, uint16_t color, bool fill);

void fill_rectangle_color(int x, int y, int width, int height, uint16_t color);

void draw_spaceship(uint16_t color);

void erase_spaceship(void);

void draw_spaceinvader(uint8_t row, uint8_t col, uint16_t color);

void erase_spaceinvader(uint8_t row, uint8_t col);

void draw_bullet(void);

void draw_bullet_impact(void);

uint16_t get_top_left_coordinates_x(uint8_t col);

uint16_t get_top_left_coordinates_y(uint8_t row);







#endif