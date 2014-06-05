#include<iostream>
#include<string>
#include<time.h> //used for combat variance
#include"AchievementDatabase.h"
#include"DamageCalculation.h"
#include"Flavor.h"
#include"GlobalVars.h"
#include"Item.h"
#include"Quest.h"
#include"Spell.h"
using namespace std;

#ifndef PLAYER_H
#define PLAYER_H

//player classes
class Player {
public:
	Player() {}
	
	//INITIALIZATION
	void set_name();
	void set_class();
	void set_quests();
	void init_achievements();
	
	//these are referenced within base class even though they vary based on class type
	//Hence, these are set as "virtual". Cannot be set as "pure virtual" since the base
	//class is instantiated.
	virtual void set_player();
	virtual void update_stats() {}
	virtual void init_spellbook() {}
	virtual void restore_mana(int) {}
	virtual void restore_mana_to_max() {}
	virtual int get_current_mana() {return 0;}
	virtual void level_up();
	virtual void view_stats();
	virtual void view_mana() {}
	//COMBAT
	virtual int cast_spell(bool) {return 0;}
	virtual bool get_did_cast_spell() {return true;}
	virtual void lower_def() {}
	virtual void boost_block() {}
	virtual void lower_block() {}
	virtual void boost_atk() {}
	virtual void lower_atk() {}
	//EQUIPMENT MANIPULATION
	virtual void unequip_item(Item) {}
	virtual void equip_item(Item) {}
	
	void equipment_menu();
	
	//HEALTH MANIPULATION
	void restore_health_to_max() {current_health = max_health;}
	void restore_health(int);
	void lose_health(int x) {current_health -= x;}
	
	//INVENTORY MANIPULATION
	void inc_inventory(Item);
	void dec_inventory(Item);

	//COMBAT
	void take_damage(int, int, int, string);
	void dies();

	void repair(int x) {equipment[x].repair(100);}
	
	//VIEW FUNCTIONS
	void view_attrib_description();
	void view_inventory();
	void view_equipment();
	Item* view_equipable_items();
	void view_gold() {cout << "You currently have " << gold << " gold." << endl;}
	void view_quest_log();
	void view_health() {cout << "Your health: " << current_health << "/" << max_health << endl;}
	void view_achievements();
	
	//GOLD-EXP MANIPULATION
	void remove_gold(int x) {gold -= x;}
	void add_gold(int);
	void gain_exp(int);
	void lose_exp(int);
	void earn_explore_bonus(int);
	
	//QUESTING
	void quest_calculation_and_acceptance();
	bool quest_complete();
	void complete_active_quest() {boss_quest.complete_quest();}
	void turn_in_quest();
	void check_quest_at_town_entrance();
	
	//ACHIEVEMENTS
	void achievement_menu();
	void earned_achievements();
	void unearned_achievements();
	void earn_achievement(Achievement&);
	Achievement target_achievement(string achieve_type, int achieve_rank) {return game_achievements.target_achievement(achieve_type, achieve_rank);}
	
	//attribute overview and manipulation
	void attribs_menu();
	
	//LEVELING
	void allocate_points();
	
	//BOOLS
	bool use_item();
	bool is_dead();
	
	//GET FUNCTIONS
	bool* get_boss_quests_complete() {return boss_quests_complete;}
	int* get_base_attribs();
	int* get_exp_gold_inv_count();
	int get_exp() {return exp;}
	int get_gold() {return gold;}
	int get_level() {return level;}
	int get_atk() {return atk;}
	int get_def() {return def;}
	int get_max_health() {return max_health;}
	int get_max_mana() {return max_mana;}
	int get_current_health() {return current_health;}
	int get_items_in_inventory() {return items_in_inventory;}
	int get_accuracy() {return accuracy;}
	int get_crit() {return crit;}
	int get_boosted() {return boosted;}
	int get_equipment_slots() {return EQUIPMENT_SLOTS;}
	int get_base_attrib_count() {return BASE_ATTRIB_COUNT;}
	int get_misc_count() {return MISC_COUNT;}
	string get_name() {return name;}
	string get_player_class() {return player_class;}
	Quest get_boss_quest() {return boss_quest;}
	AchievementDatabase get_game_achievements() {return game_achievements;}
	Item* get_inventory() {return inventory;}
	Item* get_equipment() {return equipment;}
	
	//LOAD FUNCTIONS
	void load_inventory(Item*&);
	void load_quest_active(bool active) {quest_active = active;}
	void load_boss_quest_completion(bool*&);
	void load_player(int*&, int*&, bool*&, Item*&);
	void load_achievements(Achievement*&, int);
	
protected:
	//VARIABLES
	static const int EQUIPMENT_SLOTS = 3;
	static const int BASE_ATTRIB_COUNT = 4;
	static const int MISC_COUNT = 3;
	Item equipment[EQUIPMENT_SLOTS];
	Item *inventory;
	Quest boss_quest;
	AchievementDatabase game_achievements;
	string name, player_class;
	string description_strength, description_agility, description_intellect, description_stamina;
	int base_attribs[BASE_ATTRIB_COUNT]; //used for load / save functionality
	int exp_gold_inv_count[MISC_COUNT]; //used for load / save functionality
	int base_str, base_agi, base_int, base_sta; //baseline attributes
	int str_bonus, agi_bonus, int_bonus, sta_bonus; //equipment attribute bonuses
	int strength, agility, intellect, stamina; //fully adjusted attributes
	int points, exp, items_in_inventory, gold, level;
	int atk, def, max_health, max_mana, current_health;
	int equip_atk_bonus, equip_def_bonus;
	int crit, dodge, parry, block, accuracy, exp_to_next_level;
	int boosted;
	int achievement_count, achievement_points;
	static const int ATTRIB_BONUS = 5;
	static const int CLASS_BONUS = 10;
	static const int LEVEL_INCREMENT = 50;
	static const int DMG_VARIANCE = 15; //read as % variance
	bool boss_quests_complete[MAX_LEVEL];
	bool quest_active;
	
	//CLASS DESCRIPTIONS
	void warrior_description();
	void rogue_description();
	void mage_description();
	
	//INITIALIZATION
	void init_attribs() {base_str = base_agi = base_int = base_sta = 10;}
	void init_new_player();

	void view_attributes();
	
	//INVENTORY MANIPULATION
	void add_to_inventory(Item);
	void remove_from_inventory(Item);
	int num_of_equipable_items();
};
class Warrior: public Player {
public:
	Warrior(string);
	
	void set_player();
	
	//COMBAT
	void boost_block();
	void lower_block();
	
	//establishes stats and attributes, and takes bonuses into consideration
	void update_stats();
	
	//EQUIPMENT MANIPULATION
	void unequip_item(Item);
	void equip_item(Item);
	
private:
	void init_equipment();
	
	//provides descriptions of the four base attributes
	void set_descriptions();
};
class Rogue: public Player {
public:
	Rogue(string);
	
	void set_player();
	
	//COMBAT
	void boost_atk();
	void lower_atk();
	
	//establishes stats and attributes, and takes bonuses into consideration
	void update_stats();
	
	//EQUIPMENT MANIPULATION
	void unequip_item(Item);
	void equip_item(Item);
	
private:
	void init_equipment();
	
	//provides descriptions of the four base attributes
	void set_descriptions();
};
class Mage: public Player {
public:
	Mage(string);
	
	//INITIALIZATION
	void set_player();
	void init_spellbook();
	
	//establishes stats and attributes, and takes bonuses into consideration
	void update_stats();
	
	//EQUIPMENT MANIPULATION
	void unequip_item(Item);
	void equip_item(Item);
	
	//VIEW FUNCTIONS
	void view_stats();
	void view_mana() {cout << "Your mana: " << current_mana << "/" << max_mana << endl << endl;}
	
	//MANA MANIPULATION
	void restore_mana(int);
	void restore_mana_to_max() {current_mana = max_mana;}
	void lose_mana(int x) {current_mana -= x;}
	
	//COMBAT
	void boost_def(int);
	void lower_def();
	int cast_spell(bool);
	
	void level_up();
	
	//GET FUNCTIONS
	bool get_did_cast_spell() {return did_cast_spell;}
	Spell* get_spellbook() {return spellbook;}
	int get_current_mana() {return current_mana;}
	
private:
	//VARIABLES
	static const int SPELL_COUNT = 3;
	Spell spellbook[SPELL_COUNT];
	int current_mana, equip_mana_bonus, spellpower;
	bool did_cast_spell;

	
private:
	void init_equipment();
	
	void view_spells();
	
	//provides descriptions of the four base attributes
	void set_descriptions();
};

#endif