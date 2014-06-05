#include<iostream>
#include<string>
#include<time.h> //used for boss and town placement
#include"Flavor.h"
using namespace std;

#ifndef MAP_H
#define MAP_H

//MAPS AND DUNGEONS
class Map {
public:
	Map(int);
	
	void init_map();
	
	void draw_map();
	
	int move_player(int, char);
	
	void enter_town(int);
	
	bool explored_map();
	
	//verifies that player can move in a particular direction
	bool can_move(int, char);
	
	//GET FUNCTIONS
	string get_north_flavor() {return north_flavor;}
	string get_west_flavor() {return west_flavor;}
	string get_east_flavor() {return east_flavor;}
	string get_south_flavor() {return south_flavor;}
	int get_town() {return town;}
	int get_boss() {return boss;}
	int get_area() {return area;}
	int get_level() {return level;}
	bool get_explore_bonus_earned() {return explore_bonus_earned;}
	bool get_boss_present() {return boss_present;}
	
	//SET FUNCTIONS
	void set_explore_bonus_earned() {explore_bonus_earned = true;}
	
	void killed_boss() {boss_present = false;}
	
private:
	//VARIABLES
	string north_flavor, west_flavor, east_flavor, south_flavor;
	char *map;
	char space;
	int level, size, town, area, boss;
	bool explore_bonus_earned, boss_present;
	
	//SET FUNCTIONS
	void set_town();
	void set_boss();
};
class Dungeon {
public:
	Dungeon(int);
	
	void move();
	
private:
	static const int STAGES = 3;
	string stage[STAGES];
	string success, deadend, fork, entrance, end;
	int complexity, level;
	
	void init_flavor();
};


#endif