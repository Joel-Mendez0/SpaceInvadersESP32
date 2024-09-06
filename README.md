
# Space Invaders ESP32 Game

This project implements a clone of the classic Space Invaders game on an ESP32 microcontroller. The game is played on an LCD screen, with buttons used for player input. It utilizes FreeRTOS to manage tasks, including updating the game state, rendering the graphics, and handling user input. The game is rendered using SPI communication with an LCD display, providing smooth gameplay at 24 frames per second.

## Features
- **LCD Display**: The game runs on a 320x240 pixel LCD display.
- **Button Input**: Control the spaceship using physical buttons connected to GPIO pins.
- **FreeRTOS Tasks**: Game logic, rendering, and input are managed as separate tasks in FreeRTOS.
- **Smooth Graphics Rendering**: Optimized graphics using SPI communication.
- **Game Mechanics**: Features enemy movement, shooting bullets, and enemy collision detection.
- **Weapon System**: Multiple weapon types including rapid fire and beam spread.

## Hardware Requirements
- ESP32 microcontroller
- 320x240 SPI LCD screen
- Physical buttons for input (wired to GPIO pins)
- Power supply for ESP32 and LCD

## GPIO Pin Configuration

| Function           | GPIO Pin |
|--------------------|----------|
| LCD Data (MOSI)    | 23       |
| LCD Pixel Clock    | 18       |
| LCD Chip Select    | 15       |
| LCD Data/Command   | 2        |
| LCD Reset          | 4        |
| LCD Backlight      | 13       |
| Button - Left      | 0        |
| Button - Right     | 33       |
| Button - Action    | 5        |
| Button - Select    | 21       |

## Game Specifications
- **Resolution**: 320x240 pixels.
- **Frame Rate**: 24 FPS.
- **Gameplay**: Control a spaceship, shoot at invading aliens, and avoid their attacks.
- **Weapons**: Various shooting patterns and velocities are available, including rapid fire and spread shots.
- **Enemies**: Dynamic enemy movement and collision detection.
- **Game Board**: 20 columns and 15 rows, divided into pixel blocks for drawing enemies and player movement.

## Software Structure

### Files and Functionality

#### 1. `draw.h`
Handles all drawing functions for the LCD screen, including:
- Drawing shapes (lines, circles, rectangles).
- Rendering the spaceship, bullets, and enemies.
- Handling screen updates and pixel manipulation.

#### 2. `game.h`
Manages the core game loop and game logic:
- Tasks for updating the game state, including enemy movement and player actions.
- Initializing the game board and game state.
- Spawning and updating enemies.

#### 3. `input.h`
Handles user input from the buttons:
- Configures GPIO pins for input.
- Reads button presses and passes them to the game logic.

#### 4. `state.h`
Defines all game-related structures, including:
- Player and enemy attributes.
- Weapon types and properties.
- The game board, player health, level, and score.

### FreeRTOS Task Structure
- **Game Task**: Manages the game loop, updating enemy positions, and detecting collisions.
- **Render Task**: Handles the LCD rendering, drawing the game objects to the screen.
- **Input Task**: Reads button inputs and processes player actions.

## How to Build and Flash
1. Clone the repository to your local machine.
2. Set up the ESP-IDF environment by following [Espressif's official guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html).
3. Configure the project for your ESP32 by running:
   ```bash
   idf.py menuconfig
   ```
4. Build the project:
   ```bash
   idf.py build
   ```
5. Flash the firmware onto your ESP32:
   ```bash
   idf.py flash
   ```
6. Monitor the output:
   ```bash
   idf.py monitor
   ```

## Game Controls

- **Move Left**: Left button (GPIO 0)
- **Move Right**: Right button (GPIO 33)
- **Shoot**: Action button (GPIO 5)
- **Select/Start**: Select button (GPIO 21)

## License
This project is licensed under the MIT License.
