#include "state.h"
#include "draw.h"
#include "game.h"

volatile GAME_STATE game_state;
volatile PLAYER player;
volatile uint8_t CURRENT_ENEMIES;
volatile ENEMY enemies[MAX_ENEMIES];

volatile uint8_t enemy_move_count = 4;
uint8_t direction = LEFT;
bool down = false;

void Player_Init(void){
	//Initialize player_weapon
	WEAPON player_weapon;
	player_weapon.shooting_pattern = DEFAULT_PATTERN;
	player_weapon.weapon_damage = DEFAULT_DAMAGE;
	player_weapon.weapon_velocity = DEFAULT_VELOCITY;
	
	//Initialize player
	player.CURRENT_WEAPON = player_weapon;
	player.HEALTH = 100;
	player.LEVEL = 1;
	player.position.row = 14;
	player.position.col = 10;
}

void Game_Init(){
	//Board initialization
	for(int r = 0; r < ROWS; r++){
		for(int c = 0; c < COLS; c++){
			game_state.GAME_BOARD[r][c] = 0;
		}
	}
	
	game_state.GAME_BOARD[player.position.row][player.position.col] = 1;
	draw_spaceship(BLACK);
	
	//HAL_UART_Transmit(&huart2, (uint8_t *)"HEllo from Game\r\n", 19 , 100);
	//Setting Initial Level
	game_state.CURRENT_LEVEL = 1;
	
	//Setting Initial Score
	game_state.SCORE = 0;
	
	
	//Player Bullets
	game_state.CURRENT_PLAYER_BULLETS = 0;
	for (int i = 0; i < MAX_BULLETS; i++){
		game_state.PLAYER_BULLETS[i].row = -1;
		game_state.PLAYER_BULLETS[i].col = -1;
	}
}

void Enemies_Init(void){
	//Enemy Initialization
	
	
	//Individually create each enemy and place on board
	// Enemy ID of -1 means dead enemy or non existent
	for (int i = 0; i < MAX_ENEMIES; i++){
		enemies[i].ID = -1;
		enemies[i].HEALTH = 0;
		enemies[i].position.row = -1; // row
		enemies[i].position.col = -1; // col
	}
	
}

void Spawn_Enemies(void){
	//Update enemy count based on level
	CURRENT_ENEMIES = game_state.CURRENT_LEVEL * 11;
	
	//Initialize Enemies for round
	for (int i = 0; i < CURRENT_ENEMIES; i++){
		enemies[i].ID = i + 2;
		enemies[i].HEALTH = 10*game_state.CURRENT_LEVEL;
		enemies[i].position.col = (i % 11) + 4; // col
		enemies[i].position.row = (0 + (i/11)) + 2; // row
		
		game_state.GAME_BOARD[enemies[i].position.row][enemies[i].position.col] = enemies[i].ID;
		
		draw_spaceinvader(enemies[i].position.row, enemies[i].position.col, BLUE);
	}
}

void Update_Enemies(void) {
	if(down == true){
		for(int i = 0; i < CURRENT_ENEMIES; i++){
			if (game_state.GAME_BOARD[enemies[i].position.row][enemies[i].position.col] >= 2) {
            	// Erase current position
            	erase_spaceinvader(enemies[i].position.row, enemies[i].position.col);
            	game_state.GAME_BOARD[enemies[i].position.row][enemies[i].position.col] = 0;
                
                enemies[i].position.row += 1;

            	// Update game board and draw new position
            	game_state.GAME_BOARD[enemies[i].position.row][enemies[i].position.col] = enemies[i].ID;
            	draw_spaceinvader(enemies[i].position.row, enemies[i].position.col, BLUE);
        }
        
        	if(enemies[i].position.row == ROWS-1){
				Start_Game();
				return;
			}			
		}
		down = false;
	}
	if(direction == RIGHT){
    	for (int i = CURRENT_ENEMIES-1; i >= 0; i--) {
        	if (game_state.GAME_BOARD[enemies[i].position.row][enemies[i].position.col] >= 2) {
            	// Erase current position
            	erase_spaceinvader(enemies[i].position.row, enemies[i].position.col);
            	game_state.GAME_BOARD[enemies[i].position.row][enemies[i].position.col] = 0;
                enemies[i].position.col += 1;

				// Update game board and draw new position
            	game_state.GAME_BOARD[enemies[i].position.row][enemies[i].position.col] = enemies[i].ID;
            	draw_spaceinvader(enemies[i].position.row, enemies[i].position.col, BLUE);
        }
    }
		
	}
	if(direction == LEFT){
    	for (int i = 0; i < CURRENT_ENEMIES; i++) {
        	if (game_state.GAME_BOARD[enemies[i].position.row][enemies[i].position.col] >= 2) {
            	// Erase current position
            	erase_spaceinvader(enemies[i].position.row, enemies[i].position.col);
            	game_state.GAME_BOARD[enemies[i].position.row][enemies[i].position.col] = 0;
                
                enemies[i].position.col -= 1;

            	// Update game board and draw new position
            	game_state.GAME_BOARD[enemies[i].position.row][enemies[i].position.col] = enemies[i].ID;
            	draw_spaceinvader(enemies[i].position.row, enemies[i].position.col, BLUE);
        }
    }}
    print_board();

    enemy_move_count++;

    if (direction == LEFT) {
        if (enemy_move_count == 7) {
            direction = RIGHT;
            enemy_move_count = 0;
        }
    } else if (direction == RIGHT) {
        if (enemy_move_count == 7) {
            direction = LEFT;
            enemy_move_count = 0;
            down = true;
        }
    }
}
void Start_Game(void){
    fill_screen_color(BACKGROUND);
    Player_Init();
    Game_Init();
    Enemies_Init();
    Spawn_Enemies();
    enemy_move_count = 4;
    direction = LEFT;
    down = false;
}
void print_board() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            printf(" %hu ", game_state.GAME_BOARD[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}