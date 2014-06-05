#include<iostream>
#include<string>
#include<time.h> //used in creature combat and loot generation
#include"Item.h"
#include"DamageCalculation.h"
using namespace std;

#ifndef CREATURE_H
#define CREATURE_H

class Creature {
public:
	Creature() {}
	
	//GET FUNCTIONS
	string get_name() {return name;}
	int get_exp() {return exp;}
	int get_atk() {return atk;}
	int get_def() {return def;}
	int get_health() {return health;}
	int get_accuracy() {return creature_acc;}
	int get_crit() {return crit;}
	int get_money() {return money;}
	int get_escape_chance() {return escape_chance;}
	int get_loot_count() {return loot_count;}
	Item *get_loot() {return loot;}
	bool get_has_loot() {return has_loot;}
	
	void det_creature(int);
	
	//COMBAT
	void take_damage(int, int, int, string);
	void take_spell_dmg(int, int, int);
	bool is_dead();
	
protected:
	//VARIABLES
	Item* loot;
	string name, type;
	int exp, atk, def, health, max_health, stat_pool, level;
	int creature_acc, dodge, parry, crit, money, escape_chance, loot_count;
	static const int DMG_VARIANCE = 15; //read as % variance
	bool has_loot;

	virtual bool generate_loot();
	
	//HELPER FUNCTIONS TO ESTABLISH STANCE
	virtual void defensive();
	virtual void offensive();
	virtual void balanced();
	
	virtual void set_type();
};
class Boss: public Creature {
public:
	Boss() {}
	Boss(int);
	
	//GET FUNCTIONS
	int get_level() {return level;}
	string get_flavor() {return attack_flavor;}

private:
	//VARIABLES
	string attack_flavor;
	
	bool generate_loot();
	
	//HELPER FUNCTIONS TO ESTABLISH STANCE
	void defensive();
	void offensive();
	void balanced();
	
	void set_type();
};

#endif