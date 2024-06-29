#include "draw.h"
#include "state.h"

void draw_pixel(int x, int y, uint16_t color) {
    // This function should draw a single pixel on the LCD at (x, y) with the given color
    esp_lcd_panel_draw_bitmap(panel_handle, x, y, x + 1, y + 1, &color);
}

void draw_line(int x0, int y0, int x1, int y1, uint16_t color, int thickness)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2; /* error value e_xy */

    while (1) {
        // Adjust thickness by drawing multiple pixels
        for (int i = 0; i < thickness; i++) {
            esp_lcd_panel_draw_bitmap(panel_handle, x0, y0 + i, x0 + 1, y0 + 1 + i, &color); // Draw pixel
        }
        
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }
}

void draw_circle(int xc, int yc, int radius, uint16_t color, int thickness)
{
    int x = radius, y = 0;
    int err = 0;

    while (x >= y) {
        // Adjust thickness by drawing multiple circles with increasing radius
        for (int i = 0; i < thickness; i++) {
            esp_lcd_panel_draw_bitmap(panel_handle, xc + x, yc + y + i, xc + x + 1, yc + y + 1 + i, &color);
            esp_lcd_panel_draw_bitmap(panel_handle, xc + y, yc + x + i, xc + y + 1, yc + x + 1 + i, &color);
            esp_lcd_panel_draw_bitmap(panel_handle, xc - y, yc + x + i, xc - y + 1, yc + x + 1 + i, &color);
            esp_lcd_panel_draw_bitmap(panel_handle, xc - x, yc + y + i, xc - x + 1, yc + y + 1 + i, &color);
            esp_lcd_panel_draw_bitmap(panel_handle, xc - x, yc - y - i, xc - x + 1, yc - y + 1 - i, &color);
            esp_lcd_panel_draw_bitmap(panel_handle, xc - y, yc - x - i, xc - y + 1, yc - x + 1 - i, &color);
            esp_lcd_panel_draw_bitmap(panel_handle, xc + y, yc - x - i, xc + y + 1, yc - x + 1 - i, &color);
            esp_lcd_panel_draw_bitmap(panel_handle, xc + x, yc - y - i, xc + x + 1, yc - y + 1 - i, &color);
        }

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }

        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

void fill_screen_color(uint16_t color)
{
    // Allocate memory for a single line of black pixels
    uint16_t *line = heap_caps_malloc(EXAMPLE_LCD_H_RES * sizeof(uint16_t), MALLOC_CAP_DMA);
    assert(line != NULL);

    // Set all pixels in the line to black
    for (int i = 0; i < EXAMPLE_LCD_H_RES; i++) {
        line[i] = color; // Black color in RGB565 format (0x0000)
    }

    // Send each line to the LCD
    for (int y = 0; y < EXAMPLE_LCD_V_RES; y++) {
        // Draw black line
        esp_lcd_panel_draw_bitmap(panel_handle, 0, y, EXAMPLE_LCD_H_RES, y + 1, line);
    }

    // Free the allocated memory
    heap_caps_free(line);
}

void draw_char(uint8_t ch, int x, int y, uint16_t color, int scale) {
    const uint8_t *bitmap = font8x8_basic[ch - 32];
    for (int col = 0; col < 8; col++) {
        for (int row = 0; row < 8; row++) {
            if (bitmap[row] & (1 << col)) {
                for (int dx = 0; dx < scale; dx++) {
                    for (int dy = 0; dy < scale; dy++) {
                        draw_pixel(x + row * scale + dx, y + col * scale + dy, color);
                    }
                }
            }
        }
    }
}

void draw_string(const char *str, int x, int y, uint16_t color, int scale) {
    while (*str) {
        draw_char(*str, x, y, color, scale);
        x += 8 * scale; // Move to the next character position
        str++;
    }
}

void draw_rectangle(int x, int y, int width, int height, uint16_t color, bool fill) {
    // Buffer to hold the line colors
    uint16_t *line = heap_caps_malloc(width * sizeof(uint16_t), MALLOC_CAP_DMA);
    if (line == NULL) {
        // Handle memory allocation failure
        return;
    }

    // Set all pixels in the line buffer to the rectangle color
    for (int i = 0; i < width; i++) {
        line[i] = color;
    }

    // Draw the top horizontal line
    esp_lcd_panel_draw_bitmap(panel_handle, x, y, x + width, y + 1, line);

    // Draw the bottom horizontal line
    esp_lcd_panel_draw_bitmap(panel_handle, x, y + height - 1, x + width, y + height, line);

    // Free the allocated memory for the horizontal lines
    heap_caps_free(line);

    // Buffer to hold the vertical line colors
    line = heap_caps_malloc(height * sizeof(uint16_t), MALLOC_CAP_DMA);
    if (line == NULL) {
        // Handle memory allocation failure
        return;
    }

    // Set all pixels in the line buffer to the rectangle color
    for (int i = 0; i < height; i++) {
        line[i] = color;
    }

    // Draw the left vertical line
    esp_lcd_panel_draw_bitmap(panel_handle, x, y, x + 1, y + height, line);

    // Draw the right vertical line
    esp_lcd_panel_draw_bitmap(panel_handle, x + width - 1, y, x + width, y + height, line);

    // Free the allocated memory for the vertical lines
    heap_caps_free(line);
    
    if(fill){
		fill_rectangle_color(x, y, width, height, color); 
	}
}

void fill_rectangle_color(int x, int y, int width, int height, uint16_t color) {
    // Buffer to hold the line colors
    uint16_t *line = heap_caps_malloc(width * sizeof(uint16_t), MALLOC_CAP_DMA);
    if (line == NULL) {
        // Handle memory allocation failure
        return;
    }

    // Set all pixels in the line buffer to the rectangle color
    for (int i = 0; i < width; i++) {
        line[i] = color;
    }

    // Draw each line of the rectangle
    for (int i = 0; i < height; i++) {
        esp_lcd_panel_draw_bitmap(panel_handle, x, y + i, x + width, y + i + 1, line);
    }

    // Free the allocated memory for the lines
    heap_caps_free(line);
}


void draw_spaceship(uint16_t color) {
    uint16_t start_x = get_top_left_coordinates_x(player.position.col);
    uint16_t start_y = get_top_left_coordinates_y(player.position.row);
    
    // 0th row *empty
    
    // 1st row *empty
    
    // 2nd row *empty
    
    // 3rd row
    draw_pixel(start_x+7, start_y+3, color);
    draw_pixel(start_x+8, start_y+3, color);
    
    // 4th row
     draw_pixel(start_x+7, start_y+4, color);
     draw_pixel(start_x+8, start_y+4, color);   
    
    // 5th row
     draw_pixel(start_x+6, start_y+5, color);
     draw_pixel(start_x+7, start_y+5, color);
     draw_pixel(start_x+8, start_y+5, color);
     draw_pixel(start_x+9, start_y+5, color);
    
    // 6th row
     draw_pixel(start_x+6, start_y+6, color);
     draw_pixel(start_x+9, start_y+6, color);
    
    // 7th row
     draw_pixel(start_x+0, start_y+7, color);
     draw_pixel(start_x+1, start_y+7, color);
     draw_pixel(start_x+2, start_y+7, color);
     draw_pixel(start_x+3, start_y+7, color);
     draw_pixel(start_x+4, start_y+7, color);
     draw_pixel(start_x+5, start_y+7, color);
     draw_pixel(start_x+6, start_y+7, color);
     draw_pixel(start_x+9, start_y+7, color);
     draw_pixel(start_x+10, start_y+7, color);
     draw_pixel(start_x+11, start_y+7, color);
     draw_pixel(start_x+12, start_y+7, color);
     draw_pixel(start_x+13, start_y+7, color);
     draw_pixel(start_x+14, start_y+7, color);
     draw_pixel(start_x+15, start_y+7, color);
    
    // 8th row
     draw_pixel(start_x+1, start_y+8, color);
     draw_pixel(start_x+2, start_y+8, color);
     draw_pixel(start_x+3, start_y+8, color);
     draw_pixel(start_x+4, start_y+8, color);
     draw_pixel(start_x+5, start_y+8, color);
     draw_pixel(start_x+6, start_y+8, color);
     draw_pixel(start_x+9, start_y+8, color);
     draw_pixel(start_x+10, start_y+8, color);
     draw_pixel(start_x+11, start_y+8, color);
     draw_pixel(start_x+12, start_y+8, color);
     draw_pixel(start_x+13, start_y+8, color);
     draw_pixel(start_x+14, start_y+8, color);
    
    // 9th row
     draw_pixel(start_x+2, start_y+9, color);
     draw_pixel(start_x+3, start_y+9, color);
     draw_pixel(start_x+4, start_y+9, color);
     draw_pixel(start_x+5, start_y+9, color);
     draw_pixel(start_x+6, start_y+9, color);
     draw_pixel(start_x+9, start_y+9, color);
     draw_pixel(start_x+10, start_y+9, color);
     draw_pixel(start_x+11, start_y+9, color);
     draw_pixel(start_x+12, start_y+9, color);
     draw_pixel(start_x+13, start_y+9, color); 
    
    // 10th row
    draw_pixel(start_x+6, start_y+10, color);
    draw_pixel(start_x+9, start_y+10, color);
    // 11th row
    draw_pixel(start_x+6, start_y+11, color);
    draw_pixel(start_x+9, start_y+11, color);
    // 12th row
    draw_pixel(start_x+6, start_y+12, color);
    draw_pixel(start_x+9, start_y+12, color);
    // 13th row *empty
    
    // 14th row *empty
    
    // 15th row *empty

    return;
}

void erase_spaceship() {
    uint16_t start_x = get_top_left_coordinates_x(player.position.col);
    uint16_t start_y = get_top_left_coordinates_y(player.position.row);
    
    // 0th row *empty
    
    // 1st row *empty
    
    // 2nd row *empty
    
    // 3rd row
    draw_pixel(start_x+7, start_y+3, BACKGROUND);
    draw_pixel(start_x+8, start_y+3, BACKGROUND);
    
    // 4th row
     draw_pixel(start_x+7, start_y+4, BACKGROUND);
     draw_pixel(start_x+8, start_y+4, BACKGROUND);   
    
    // 5th row
     draw_pixel(start_x+6, start_y+5, BACKGROUND);
     draw_pixel(start_x+7, start_y+5, BACKGROUND);
     draw_pixel(start_x+8, start_y+5, BACKGROUND);
     draw_pixel(start_x+9, start_y+5, BACKGROUND);
    
    // 6th row
     draw_pixel(start_x+6, start_y+6, BACKGROUND);
     draw_pixel(start_x+9, start_y+6, BACKGROUND);
    
    // 7th row
     draw_pixel(start_x+0, start_y+7, BACKGROUND);
     draw_pixel(start_x+1, start_y+7, BACKGROUND);
     draw_pixel(start_x+2, start_y+7, BACKGROUND);
     draw_pixel(start_x+3, start_y+7, BACKGROUND);
     draw_pixel(start_x+4, start_y+7, BACKGROUND);
     draw_pixel(start_x+5, start_y+7, BACKGROUND);
     draw_pixel(start_x+6, start_y+7, BACKGROUND);
     draw_pixel(start_x+9, start_y+7, BACKGROUND);
     draw_pixel(start_x+10, start_y+7, BACKGROUND);
     draw_pixel(start_x+11, start_y+7, BACKGROUND);
     draw_pixel(start_x+12, start_y+7, BACKGROUND);
     draw_pixel(start_x+13, start_y+7, BACKGROUND);
     draw_pixel(start_x+14, start_y+7, BACKGROUND);
     draw_pixel(start_x+15, start_y+7, BACKGROUND);
    
    // 8th row
     draw_pixel(start_x+1, start_y+8, BACKGROUND);
     draw_pixel(start_x+2, start_y+8, BACKGROUND);
     draw_pixel(start_x+3, start_y+8, BACKGROUND);
     draw_pixel(start_x+4, start_y+8, BACKGROUND);
     draw_pixel(start_x+5, start_y+8, BACKGROUND);
     draw_pixel(start_x+6, start_y+8, BACKGROUND);
     draw_pixel(start_x+9, start_y+8, BACKGROUND);
     draw_pixel(start_x+10, start_y+8, BACKGROUND);
     draw_pixel(start_x+11, start_y+8, BACKGROUND);
     draw_pixel(start_x+12, start_y+8, BACKGROUND);
     draw_pixel(start_x+13, start_y+8, BACKGROUND);
     draw_pixel(start_x+14, start_y+8, BACKGROUND);
    
    // 9th row
     draw_pixel(start_x+2, start_y+9, BACKGROUND);
     draw_pixel(start_x+3, start_y+9, BACKGROUND);
     draw_pixel(start_x+4, start_y+9, BACKGROUND);
     draw_pixel(start_x+5, start_y+9, BACKGROUND);
     draw_pixel(start_x+6, start_y+9, BACKGROUND);
     draw_pixel(start_x+9, start_y+9, BACKGROUND);
     draw_pixel(start_x+10, start_y+9, BACKGROUND);
     draw_pixel(start_x+11, start_y+9, BACKGROUND);
     draw_pixel(start_x+12, start_y+9, BACKGROUND);
     draw_pixel(start_x+13, start_y+9, BACKGROUND); 
    
    // 10th row
    draw_pixel(start_x+6, start_y+10, BACKGROUND);
    draw_pixel(start_x+9, start_y+10, BACKGROUND);
    // 11th row
    draw_pixel(start_x+6, start_y+11, BACKGROUND);
    draw_pixel(start_x+9, start_y+11, BACKGROUND);
    // 12th row
    draw_pixel(start_x+6, start_y+12, BACKGROUND);
    draw_pixel(start_x+9, start_y+12, BACKGROUND);
    // 13th row *empty
    
    // 14th row *empty
    
    // 15th row *empty

    return;
}

void draw_spaceinvader(uint8_t row, uint8_t col, uint16_t color) {
    uint16_t start_x = get_top_left_coordinates_x(col);
    uint16_t start_y = get_top_left_coordinates_y(row);
    
    // 0th row *empty
    
    // 1st row *empty
    
    // 2nd row *empty
    
    // 3rd row
    draw_pixel(start_x+5, start_y+3, color);
    draw_pixel(start_x+10, start_y+3, color);
    
    // 4th row
    draw_pixel(start_x+5, start_y+4, color);
    draw_pixel(start_x+10, start_y+4, color);   
    
    // 5th row
     draw_pixel(start_x+6, start_y+5, color);
     draw_pixel(start_x+9, start_y+5, color);
    
    // 6th row
     draw_pixel(start_x+5, start_y+6, color);
     draw_pixel(start_x+6, start_y+6, color);
     draw_pixel(start_x+7, start_y+6, color);
     draw_pixel(start_x+8, start_y+6, color);
     draw_pixel(start_x+9, start_y+6, color);
     draw_pixel(start_x+10, start_y+6, color);
    
    // 7th row
     draw_pixel(start_x+5, start_y+6, color);
     draw_pixel(start_x+7, start_y+6, color);
     draw_pixel(start_x+8, start_y+6, color);
     draw_pixel(start_x+10, start_y+6, color);
    
    // 8th row
     draw_pixel(start_x+3, start_y+8, color);
     draw_pixel(start_x+4, start_y+8, color);
     draw_pixel(start_x+5, start_y+8, color);
     draw_pixel(start_x+6, start_y+8, color);
     draw_pixel(start_x+7, start_y+8, color);
     draw_pixel(start_x+8, start_y+8, color);
     draw_pixel(start_x+9, start_y+8, color);
     draw_pixel(start_x+10, start_y+8, color);
     draw_pixel(start_x+11, start_y+8, color);
     draw_pixel(start_x+12, start_y+8, color);
    
    // 9th row
     draw_pixel(start_x+3, start_y+9, color);
     draw_pixel(start_x+4, start_y+9, color);
     draw_pixel(start_x+5, start_y+9, color);
     draw_pixel(start_x+6, start_y+9, color);
     draw_pixel(start_x+7, start_y+9, color);
     draw_pixel(start_x+8, start_y+9, color);
     draw_pixel(start_x+9, start_y+9, color);
     draw_pixel(start_x+10, start_y+9, color);
     draw_pixel(start_x+11, start_y+9, color);
     draw_pixel(start_x+12, start_y+9, color); 
    
    // 10th row
    draw_pixel(start_x+2, start_y+10, color);
    draw_pixel(start_x+5, start_y+10, color);
    draw_pixel(start_x+6, start_y+10, color);
    draw_pixel(start_x+7, start_y+10, color);
    draw_pixel(start_x+8, start_y+10, color);
    draw_pixel(start_x+9, start_y+10, color);
    draw_pixel(start_x+10, start_y+10, color);
    draw_pixel(start_x+13, start_y+10, color);
    // 11th row
    draw_pixel(start_x+1, start_y+11, color);
    draw_pixel(start_x+5, start_y+11, color);
    draw_pixel(start_x+10, start_y+11, color);
    draw_pixel(start_x+14, start_y+11, color);
    // 12th row *empty
    
    // 13th row *empty
    
    // 14th row *empty
    
    // 15th row *empty

    return;
}

void erase_spaceinvader(uint8_t row, uint8_t col) {
    uint16_t start_x = get_top_left_coordinates_x(col);
    uint16_t start_y = get_top_left_coordinates_y(row);
    
    // 0th row *empty
    
    // 1st row *empty
    
    // 2nd row *empty
    
    // 3rd row
    draw_pixel(start_x+5, start_y+3, BACKGROUND);
    draw_pixel(start_x+10, start_y+3, BACKGROUND);
    
    // 4th row
    draw_pixel(start_x+5, start_y+4, BACKGROUND);
    draw_pixel(start_x+10, start_y+4, BACKGROUND);   
    
    // 5th row
     draw_pixel(start_x+6, start_y+5, BACKGROUND);
     draw_pixel(start_x+9, start_y+5, BACKGROUND);
    
    // 6th row
     draw_pixel(start_x+5, start_y+6, BACKGROUND);
     draw_pixel(start_x+6, start_y+6, BACKGROUND);
     draw_pixel(start_x+7, start_y+6, BACKGROUND);
     draw_pixel(start_x+8, start_y+6, BACKGROUND);
     draw_pixel(start_x+9, start_y+6, BACKGROUND);
     draw_pixel(start_x+10, start_y+6, BACKGROUND);
    
    // 7th row
     draw_pixel(start_x+5, start_y+6, BACKGROUND);
     draw_pixel(start_x+7, start_y+6, BACKGROUND);
     draw_pixel(start_x+8, start_y+6, BACKGROUND);
     draw_pixel(start_x+10, start_y+6, BACKGROUND);
    
    // 8th row
     draw_pixel(start_x+3, start_y+8, BACKGROUND);
     draw_pixel(start_x+4, start_y+8, BACKGROUND);
     draw_pixel(start_x+5, start_y+8, BACKGROUND);
     draw_pixel(start_x+6, start_y+8, BACKGROUND);
     draw_pixel(start_x+7, start_y+8, BACKGROUND);
     draw_pixel(start_x+8, start_y+8, BACKGROUND);
     draw_pixel(start_x+9, start_y+8, BACKGROUND);
     draw_pixel(start_x+10, start_y+8, BACKGROUND);
     draw_pixel(start_x+11, start_y+8, BACKGROUND);
     draw_pixel(start_x+12, start_y+8, BACKGROUND);
    
    // 9th row
     draw_pixel(start_x+3, start_y+9, BACKGROUND);
     draw_pixel(start_x+4, start_y+9, BACKGROUND);
     draw_pixel(start_x+5, start_y+9, BACKGROUND);
     draw_pixel(start_x+6, start_y+9, BACKGROUND);
     draw_pixel(start_x+7, start_y+9, BACKGROUND);
     draw_pixel(start_x+8, start_y+9, BACKGROUND);
     draw_pixel(start_x+9, start_y+9, BACKGROUND);
     draw_pixel(start_x+10, start_y+9, BACKGROUND);
     draw_pixel(start_x+11, start_y+9, BACKGROUND);
     draw_pixel(start_x+12, start_y+9, BACKGROUND); 
    
    // 10th row
    draw_pixel(start_x+2, start_y+10, BACKGROUND);
    draw_pixel(start_x+5, start_y+10, BACKGROUND);
    draw_pixel(start_x+6, start_y+10, BACKGROUND);
    draw_pixel(start_x+7, start_y+10, BACKGROUND);
    draw_pixel(start_x+8, start_y+10, BACKGROUND);
    draw_pixel(start_x+9, start_y+10, BACKGROUND);
    draw_pixel(start_x+10, start_y+10, BACKGROUND);
    draw_pixel(start_x+13, start_y+10, BACKGROUND);
    // 11th row
    draw_pixel(start_x+1, start_y+11, BACKGROUND);
    draw_pixel(start_x+5, start_y+11, BACKGROUND);
    draw_pixel(start_x+10, start_y+11, BACKGROUND);
    draw_pixel(start_x+14, start_y+11, BACKGROUND);
    // 12th row *empty
    
    // 13th row *empty
    
    // 14th row *empty
    
    // 15th row *empty

    return;
}

void draw_bullet(void){
	return;
}

void draw_bullet_impact(void){
	return;
}



uint16_t get_top_left_coordinates_x(uint8_t col){
    return col * 16;
}

uint16_t get_top_left_coordinates_y(uint8_t row) {
    return row * 16;
}






























