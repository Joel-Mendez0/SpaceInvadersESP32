#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdint.h>
#include "config.h"

// Definitions
#define ROWS (EXAMPLE_LCD_V_RES / 16) // 15 ROWS
#define COLS (EXAMPLE_LCD_H_RES / 16) // 20 COLS
#define MAX_ENEMIES 99
#define MAX_BULLETS 100

/* External Variables */

extern volatile uint8_t CURRENT_ENEMIES;

typedef enum{
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3
} DIRECTION; 

/*ENUMs for WEAPON attributes*/

/* Shooting Pattern ENUM*/
typedef enum {
	DEFAULT_PATTERN = 1,
	RAPID_FIRE = 2,
	SPREAD = 3,
	BEAM_SPREAD = 4,
	BEAM = 5 
} SHOOTING_PATTERN;
//BEAM
/*
Will require extra logic to implement since it would
no longer rely on bullets but rather be a lazer. So basically 
a region would be a kill zone for enemies.
*/

/* Weapon Damage ENUM*/
typedef enum {
	DEFAULT_DAMAGE = 10,
	LIGHT = 20,
	MEDIUM = 50,
	HEAVY = 80,
	MAX = 100
} WEAPON_DAMAGE;

/* Weapon Velocity ENUM*/
typedef enum {
	DEFAULT_VELOCITY = 1,
	SLOW = 2,
	MODERATE = 3,
	FAST = 4
} WEAPON_VELOCITY;


/* STRUCTs for GAME */

/* Position Struct */
typedef struct {
	int8_t row; // row
	int8_t col; // col
} POSITION;
/*
VALID Positions will be x = [0,ROW) and y = [0,COLS)
*/

/* Weapon Struct */
typedef struct {
	WEAPON_DAMAGE weapon_damage;
	WEAPON_VELOCITY weapon_velocity;
	SHOOTING_PATTERN shooting_pattern;
} WEAPON;

/* Player Struct */
typedef struct {
	uint8_t HEALTH;
	uint8_t LEVEL;
	WEAPON CURRENT_WEAPON;
	POSITION position;
} PLAYER;
extern volatile PLAYER player;

/* Enemy Struct */
typedef struct {
	int8_t ID;
	uint8_t HEALTH;
	POSITION position;
} ENEMY;
extern volatile ENEMY enemies[MAX_ENEMIES];

/* Board Struct */
typedef struct {
	uint8_t GAME_BOARD[ROWS][COLS];
	uint8_t CURRENT_LEVEL;
	int SCORE;
	
	//PLAYER BULLETS are all initialized to [-1,-1]
	uint8_t CURRENT_PLAYER_BULLETS;
	POSITION PLAYER_BULLETS[100]; 
	
} GAME_STATE;
extern volatile GAME_STATE game_state;
/*
Board will hold IDs of enemies and bullets in their positions. 
Empty spaces will be ID 0
Player will be ID 1
Enemies will range in ID from 2-78
Bullets will range in ID from 79-178
*/


/*INITIALIZATION FUNCTIONS*/

/* Game initialization*/
void Player_Init(void); // Player
void Game_Init(void); // Board
void Enemies_Init(void); // Enemies
void Start_Game(void);

/* Spawn Enemies */
void Spawn_Enemies(void);

/*UPDATE FUNCTIONS*/
void Update_Enemies(void);

/*HELPER*/
void print_board(void);

#endif /*GAME_STATE_H*/
