#ifndef TANK_H
#define TANK_H

#include <vector>
#include "../../../Code/Image.h"
using namespace std;

class Tank
{
public:
	int health;
	float turret_angle;
	int turret_power;
	// weapons
	Image* image;
	int difficulty;
	bool is_human;
	int x;
	int y;
};

extern vector<Tank> player_vector;

#endif