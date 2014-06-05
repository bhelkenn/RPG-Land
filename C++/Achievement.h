#include<iostream>
#include<string>
#include"GlobalVars.h"
#include"Quest.h"
using namespace std;

#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H

class Achievement {
public:
	Achievement() {obtained = false;}
	
	void init_achievement(string, int);
	
	
	//GET FUNCTIONS
	bool get_obtained() {return obtained;}
	int get_points() {return points;}
	int get_rank() {return rank;}
	string get_type() {return type;}
	string get_name() {return name;}
	string get_description() {return description;}
	
	//LOAD FUNCTIONS
	void load_rank(int x) {rank = x;}
	void load_type(char* c) {type = c;}
	
	void earn_achievement() {obtained = true;}
	
private:
	//VARIABLES
	string type, name, description;
	int points, rank;
	bool obtained;
	
	void set_points();
};

#endif