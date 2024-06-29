// Using SPI2 in the example, as it also supports octal modes on some targets
#define LCD_HOST       SPI2_HOST
// To speed up transfers, every SPI transfer sends a bunch of lines. This define specifies how many.
// More means more memory use, but less overhead for setting up / finishing transfers. Make sure 240
// is dividable by this.
#define PARALLEL_LINES 16

#define EXAMPLE_LCD_PIXEL_CLOCK_HZ (20 * 1000 * 1000)
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL  0
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_DATA0          23  /*!< for 1-line SPI, this also refereed as MOSI */
#define EXAMPLE_PIN_NUM_PCLK           18
#define EXAMPLE_PIN_NUM_CS             15
#define EXAMPLE_PIN_NUM_DC             2
#define EXAMPLE_PIN_NUM_RST            4
#define EXAMPLE_PIN_NUM_BK_LIGHT       GPIO_NUM_13

// The pixel number in horizontal and vertical
#define EXAMPLE_LCD_H_RES              320
#define EXAMPLE_LCD_V_RES              240
// Bit number used to represent command and parameter
#define EXAMPLE_LCD_CMD_BITS           8
#define EXAMPLE_LCD_PARAM_BITS         8

#define GPIO_LEFT 0
#define GPIO_RIGHT 33
#define ACTION 5
#define SELECT 21

#define FPS 24

#define WHITE 0x0000
#define BLACK 0xFFFF
#define GREEN 0x7FF0
#define RED 0x81FF
#define BLUE 0xF100
#define BACKGROUND 0x0000
