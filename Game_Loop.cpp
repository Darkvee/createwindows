#include "Game_Loop.h"
#include <Windows.h>
#include "../../../Code/Font.h"
#include <sstream>
#include "Tank.h"
using namespace std;

int game_state = MAIN_MENU;
Image* main_menu_image;
Image* select_players_image;
Image* select_weapons_image;
Image* tank_images[4]; 

POINT mouse_position;

 void Game_Loop()
 {
	GetCursorPos(&mouse_position);
	mouse_position.y = 600-mouse_position.y;

	//debugg
	if(GetAsyncKeyState(VK_UP))
	{
		game_state++;
	}

	if(game_state == MAIN_MENU)
	{
		main_menu_image->DrawImageScaled(400,300,800,600,-1);
		 
            //240 600
		//190 300

		//PLAY BUTTON
		if(mouse_position.x > 240 && mouse_position.x < 600
			&& mouse_position.y > 190 && mouse_position.y < 300)
		{
			if(GetAsyncKeyState(VK_LBUTTON))
			{
				game_state = SELECT_PLAYERS;  
			}
		}

	} 
	else
	if(game_state == SELECT_PLAYERS)
	{ 
		select_players_image->DrawImageScaled(400,300,800,600,-1);

		for(int i = 0; i < player_vector.size(); i++)
		{
			player_vector[i].image->DrawImage(player_vector[i].x,player_vector[i].y,-1);
		}

		if(GetAsyncKeyState(VK_LBUTTON))
		{ 
			//New Player Button
			if(mouse_position.x > 530 && mouse_position.x < 733
				&& mouse_position.y > 96 && mouse_position.y < 133)
			{
				if(player_vector.size() < 4)
				{
					//Add new player
					Tank new_tank;
					new_tank.image = tank_images[player_vector.size()];
					new_tank.x = 100 + 100 * player_vector.size(); 
					new_tank.y = 400;
					player_vector.push_back(new_tank);
					Sleep(100);
				}
			}

			//delete Player Button
			if(mouse_position.x > 530 && mouse_position.x < 733
				&& mouse_position.y > 38 && mouse_position.y < 77)
			{
				//delete player
				if(player_vector.size() > 0)
				{
					player_vector.pop_back();
				}
				Sleep(100);
			}

			//Start game Button
			if(mouse_position.x > 31 && mouse_position.x < 276
				&& mouse_position.y > 38 && mouse_position.y < 77)
			{
				//Start game
			}

			//Human left button
			if(mouse_position.x > 300 && mouse_position.x < 342
				&& mouse_position.y > 153 && mouse_position.y < 185)
			{
			
			}

			//Human right button
			if(mouse_position.x > 455 && mouse_position.x < 497
				&& mouse_position.y > 38 && mouse_position.y < 77)
			{
			
			}

			//difficulty left button
			if(mouse_position.x > 300 && mouse_position.x < 342
				&& mouse_position.y > 96 && mouse_position.y < 130)
			{
			
			}

			//difficulty right button
			if(mouse_position.x > 455 && mouse_position.x < 497
				&& mouse_position.y > 96 && mouse_position.y < 130)
			{
			
			}
		}
		//530 773
		//96 133
		stringstream stream;
		stream << mouse_position.x;
		stream << "     ";
		stream << mouse_position.y;
		glPrint(stream.str().c_str(),20,20,1,1,1);
	} 
	else
	if(game_state == SELECT_WEAPONS)
	{
		select_weapons_image->DrawImageScaled(400,300,800,600,-1);
	}
	else
	if(game_state == IN_GAME)
	{
	}

 }