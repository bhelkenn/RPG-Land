#include<string>
using namespace std;

#ifndef SPELL_H
#define SPELL_H

class Spell {
public:
	Spell() {}
	
	void set_spell(string, string, int, int, int);
	
	void inc_level();
	
	//GET FUNCTIONS
	int get_magnitude() {return magnitude;}
	int get_cost() {return cost;}
	string get_name() {return name;}
	string get_type() {return type;}
	
	//LOAD FUNCTIONS
	void load_magnitude(int x) {magnitude = x;}
	
private:
	//VARIABLES
	string name, type;
	int magnitude, cost, level;
	
	void update_spell();
};

#endif