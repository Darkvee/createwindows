#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "../../../Code/Image.h"
#define MAIN_MENU		0
#define SELECT_PLAYERS	1
#define SELECT_WEAPONS	2
#define IN_GAME		3

extern int game_state;
extern Image* main_menu_image;
extern Image* select_players_image;
extern Image* select_weapons_image;

extern Image* tank_images[4];


void Game_Loop();

#endif