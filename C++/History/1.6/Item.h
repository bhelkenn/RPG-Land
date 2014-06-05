#include<iostream>
#include<string>
#include"Flavor.h"
using namespace std;

#ifndef ITEM_H
#define ITEM_H

class Item {
public:
	Item() {}
	
	void set_item(string, string, int);
	
	void inc_quantity() {quantity++;}
	void dec_quantity() {quantity--;}

	//GET FUNCTIONS
	string get_name() {return name;}
	string get_type() {return type;}
	string get_subtype() {return subtype;}
	string get_description() {return description;} //used to view a room
	int get_sell_value() {return sell_value;}
	int get_buy_value() {return buy_value;}
	int get_level() {return level;}
	int get_block_bonus() {return block_bonus;}
	int get_atk_bonus() {return atk_bonus;}
	int get_def_bonus() {return def_bonus;}
	int get_mana_bonus() {return mana_bonus;}
	int get_quantity() {return quantity;}
	int get_damage() {return (int)(damage_percentage * 100.0);}
	int get_str_bonus() {return str_bonus;}
	int get_agi_bonus() {return agi_bonus;}
	int get_int_bonus() {return int_bonus;}
	int get_sta_bonus() {return sta_bonus;}
	
	//LOAD FUNCTIONS
	void load_type(char *c) {type = c;}
	void load_subtype(char *c) {subtype = c;}
	void load_level(int x) {level = x;}
	void load_quantity(int x) {quantity = x;}
	
	//BOOLS
	bool equipable();
	bool broken();
	
	//VIEW FUNCTIONS
	void view_stats(string);
	
	//damage and repair
	void damage(int);
	void repair(int);
	
private:
	//VARIABLES
	string name, type, subtype, description;
	int sell_value, buy_value, level, quantity, attrib_bonus, stat_bonus;
	int def_bonus, atk_bonus, mana_bonus, block_bonus, rogue_atk_bonus;
	int str_bonus, agi_bonus, int_bonus, sta_bonus;
	double damage_percentage; //from 0 to 1, determining the amount of repair needed
	
	void init_item();
	
	int level_multiplier(int x) {return x * 2 * level;}
};

#endif