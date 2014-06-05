#include<string>
#include"Creature.h"
#include"GlobalVars.h"
using namespace std;

#ifndef QUEST_H
#define QUEST_H

class Quest {
public:
	Quest() {complete = false;}
	Quest(int);
	
	//GET FUNCTIONS
	bool get_complete() {return complete;}
	int get_exp() {return exp;}
	int get_gold() {return gold;}
	int get_level() {return level;}
	string get_name() {return name;}
	string get_description() {return description;}
	
	void complete_quest() {complete = true;}
	
private:
	string description, name;
	string boss_names[MAX_LEVEL];
	int level, exp, gold;
	bool complete;
	
	void set_boss_names();
	
	void set_quest();
};

#endif