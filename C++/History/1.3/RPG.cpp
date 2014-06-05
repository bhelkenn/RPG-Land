#include<iostream>
#include<string>
#include<sstream> //used for GetInt() flavor function
#include<fstream> //used for saving and loading player data
#include<time.h> //used for various RNG events, like creature encounters and town placement
#include<windows.h> //used for Clear() flavor function
using namespace std;

//flavor functions
void Pause() {
	cout << "Press ENTER to continue...";
	char c[15];
	cin.getline(c, 15);
}
void Clear() {
	HANDLE						hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO	csbi;
	DWORD						count;
	DWORD						cellCount;
	COORD						homeCoords = {0, 0};

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) {return;}

	//Get the number of cells in the current buffer
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) {return;}
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	//Fill the entire buffer with spaces
	if (!FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count)) {return;}

	//Fill the entire buffer with the current colors and attributes
	if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count)) {return;}

	//Move the cursor home
	SetConsoleCursorPosition(hStdOut, homeCoords);
}
int GetInt() {
	int num = 0;
	string input;
	while (true) {
		getline(cin, input);
		//convert from string to number safely.
		stringstream ss(input);
		if (ss >> num) {break;}
		cout << "Invalid entry, please try again: ";
	}
	return num;
}
char GetChar() {
	char c = 0;
	string input;
	while (true) {
		getline(cin, input);
		//verifies there's just a single char
		if (input.length() == 1) {
			c = input[0];
			break;
		}
		cout << "Invalid entry, please try again: ";
	}
	return c;
}

//damage calculation
int DamageCalculation(double atk, double def, double max_health, double crit, string name) {
	const double DMG_VARIANCE = 0.3;
	const double STARTING_POINT = 0.1; //starting point. Ex. if ATK = 90, dmg = 90 * STARTING_POINT
	
	double adj_atk = atk / atk; //Since ATK is the variable around which damage is determined, it's forced to 1.
	double dmg = atk * STARTING_POINT;
	double shift = (atk - def) / atk; //adjusts shift to match the same ratio as adj_atk
	
	//shifts damage according to disparity between ATK and DEF
	if (shift == 0) {} //no need to do anything, stays at starting point
	else {
		if (shift < 0) {
			shift *= -1.0;
			dmg *= 1.0 - shift;
		}
		else {dmg += dmg * shift;}
	}
	//establish variance
	int skew = (int)(dmg * (1 - DMG_VARIANCE));
	int result;
	if (skew != (int)dmg) {result = rand() % ((int)dmg - skew) + skew;}
	else {result = skew;}
	
	if (result < 1) {result = 1;} //forces damage to actually occur
	//checks for crits
	if (rand() % 101 < crit) {
		cout << name << " landed a critical hit!" << endl;
		result *= 2;
	}
	return result;
}

class Item {
public:
	Item() {}
	
	void set_item(string item_type, string item_subtype, int item_level) {
		quantity = 1;
		type = item_type;
		subtype = item_subtype;
		level = item_level;
		init_item();
	}
	
	void inc_quantity() {quantity++;}
	void dec_quantity() {quantity--;}

	//GET FUNCTIONS
	string get_name() {return name;}
	string get_type() {return type;}
	string get_subtype() {return subtype;}
	int get_sell_value() {return sell_value;}
	int get_buy_value() {return buy_value;}
	int get_level() {return level;}
	int get_quantity() {return quantity;}
	int get_damage() {return (int)(damage_percentage * 100.0);}
	
	//LOAD FUNCTIONS
	void load_name(char *c) {
		string s = c;
		name = s;
	}
	void load_type(char *c) {
		string s = c;
		type = s;
	}
	void load_subtype(char *c) {
		string s = c;
		subtype = s;
	}
	void load_sell_value(int x) {sell_value = x;}
	void load_buy_value(int x) {buy_value = x;}
	void load_level(int x) {level = x;}
	void load_quantity(int x) {quantity = x;}
	
	//BOOLS
	bool equipable() {
		if (type == "Armor" || 
			type == "Weapon" || 
			type == "Shield" || 
			type == "Offhand") {return true;}
		else {return false;}
	}
	bool broken() {
		if (damage_percentage == 1.0) {return true;}
		else {return false;}
	}
	
	//returns value of bonus given by an item
	int stat_value() {
		if (subtype == "Sword") {return level_multiplier(5);}
		else if (subtype == "Dagger") {return level_multiplier(3);}
		else if (subtype == "Staff") {return level_multiplier(2);}
		else if (subtype == "Cloth") {return level_multiplier(3);}
		else if (subtype == "Leather") {return level_multiplier(5);}
		else if (subtype == "Mail") {return level_multiplier(8);}
		else if (type == "Shield") {return level_multiplier(15);}
		else if (type == "Offhand") {return level_multiplier(2);}
		else {return 0;}
	}
	int attrib_value() {return level_multiplier(1);}
	
	//damage and repair
	void damage(int x) {
		damage_percentage += (x / 100.0);
		if (damage_percentage > 1.0) {damage_percentage = 1.0;}
	}
	void repair(int x) {
		damage_percentage -= (x / 100.0);
		if (damage_percentage < 0) {damage_percentage = 0;}
	}
	
private:
	//VARIABLES
	string name, type, subtype;
	int sell_value, buy_value, level, quantity;
	double damage_percentage; //from 0 to 1, determining the amount of repair needed
	
	void init_item() {
		//known: type, subtype, level
		//assumed quantity of 1
		
		//known types: "Armor", "Weapon", "Shield", "Offhand", "Potion", "Room", "Trash", "Boss Drop"
		//known subtypes: "Mail", "Leather", "Cloth", "Sword", "Dagger", "Staff", "Orb", "Health", "Mana", "none", "broken tooth", "hairball",
		//"string", "tail fluff", "lint", "Fractured Tooth", "Poop Nugget", "Torn Claw", "Flea Collar", "Golden Apple"
		//known levels: 1, 2, 3, 4, 5
		//conditions to init name and buy/sell value based on knowns
		
		string s; //used to assemble names
		//name assembly
		if (type == "Armor") {
			if (level > 0) { //validates as a "real" item
				//assemble prefix
				if (level == 1) {s = "Crappy ";}
				else if (level == 2) {s = "Cheap ";}
				else if (level == 3) {s = "Basic ";}
				else if (level == 4) {s = "Expensive ";}
				else if (level == 5) {s = "Luxurious ";}
				else {cout << "Not a valid level (Armor -> name)" << endl;}
			
				//add subtype to prefix
				if (subtype == "Mail") {s += subtype + " ";}
				else if (subtype == "Leather") {s += subtype + " ";}
				else if (subtype == "Cloth") {s += subtype + " ";}
				else {cout << "Not a valid subtype (Armor -> name)" << endl;}
			
				//add type to the end
				s += type;
			
				//this is the name of the item
				name = s;
			}
			//for unequipped slots
			else {name = "empty";}
		}
		else if (type == "Weapon") {
			if (level > 0) { //validates as a "real" item
				//since prefix will change based on the subtype, will need
				//to determine the subtype before beginning name assembly
			
				//determine subtype first
				if (subtype == "Sword" || subtype == "Dagger") {
					//assemble prefix based on subtype
					if (level == 1) {s = "Copper ";}
					else if (level == 2) {s = "Bronze ";}
					else if (level == 3) {s = "Iron ";}
					else if (level == 4) {s = "Steel ";}
					else if (level == 5) {s = "Diamond ";}
					else {cout << "Not a valid level (Sword/Dagger -> name)" << endl;}
					s += subtype;
				}
				else if (subtype == "Staff") {
					//assemble prefix based on subtype
					if (level == 1) {s = "Foam ";}
					else if (level == 2) {s = "Rubber ";}
					else if (level == 3) {s = "Wooden ";}
					else if (level == 4) {s = "Metal ";}
					else if (level == 5) {s = "Mage's ";}
					else {cout << "Not a valid level (Staff -> name)" << endl;}
					s += subtype;
				}
				else {cout << "Not a valid subtype (Weapon -> name)" << endl;}
			
				//since there is no need to specify these as weapons....
				//this is the name of the item
				name = s;
			}
			//for unequipped slots
			else {name = "empty";}
		}
		else if (type == "Shield") {
			if (level > 0) { //validates as a "real" item
				//assemble prefix
				if (level == 1) {s = "Plastic ";}
				else if (level == 2) {s = "Wooden ";}
				else if (level == 3) {s = "Iron ";}
				else if (level == 4) {s = "Steel ";}
				else if (level == 5) {s = "Jade ";}
				else {cout << "Not a valid level (Shield -> name)" << endl;}
			
				//add type
				s += type;
			
				//this is the name
				name = s;
			}
			//for unequipped slots
			else {name = "empty";}
		}
		else if (type == "Offhand") {
			if (level > 0) { //validates as a "real" item
				//assemble prefix
				if (level == 1) {s = "Foam ";}
				else if (level == 2) {s = "Rubber ";}
				else if (level == 3) {s = "Wooden ";}
				else if (level == 4) {s = "Metal ";}
				else if (level == 5) {s = "Mage's ";}
				else {cout << "Not a valid level (Offhand -> name)" << endl;}
				s += subtype;
				
				//since there is no need to specify these as offhands....
				//this is the name
				name = s;
			}
			else {name = "empty";}
		}
		else if (type == "Potion") {
			//assemble prefix
			if (level == 1) {s = "Crappy ";}
			else if (level == 2) {s = "Cheap ";}
			else if (level == 3) {s = "Basic ";}
			else if (level == 4) {s = "Expensive ";}
			else if (level == 5) {s = "Luxurious ";}
			else {cout << "Not a valid level (Potion -> name)" << endl;}
			
			//add subtype to prefix
			if (subtype == "Health") {s += subtype + " ";}
			else if (subtype == "Mana") {s += subtype + " ";}
			else {cout << "Not a valid subtype (Potion -> name)" << endl;}
			
			//add type to the end
			s += type;
			
			//this is the name
			name = s;
		}
		else if (type == "Room") {
			//assemble prefix
			if (level == 1) {s = "Crappy ";}
			else if (level == 2) {s = "Cheap ";}
			else if (level == 3) {s = "Basic ";}
			else if (level == 4) {s = "Expensive ";}
			else if (level == 5) {s = "Luxurious ";}
			else {cout << "Not a valid level (Room -> name)" << endl;}
			
			//add type
			s += type;
			
			//this is the name
			name = s;
		}
		else if (type == "Trash") {
			//assemble prefix
			if (level == 1) {s = "Slimy ";}
			else if (level == 2) {s = "Pungent ";}
			else if (level == 3) {s = "Greasy ";}
			else if (level == 4) {s = "Rotten ";}
			else if (level == 5) {s = "Putrid ";}
			else {cout << "Not a valid level (Trash -> name)" << endl;}
			
			//add subtype
			s += subtype;
			
			//this is the name
			name = s;
		}
		else if (type == "Boss Drop") {name = subtype;}
		else {cout << "Not a valid item type (name)" << endl;}
		
		//buy value assembly
		if (type == "Armor") {
			//further classify by subtype
			if (subtype == "Mail") {buy_value = 45 * level;}
			else if (subtype == "Leather") {buy_value = 30 * level;}
			else if (subtype == "Cloth") {buy_value = 15 * level;}
			else {cout << "Not a valid subtype (Armor -> buy value)" << endl;}
		}
		else if (type == "Weapon") {
			//further classify by subtype
			if (subtype == "Sword") {buy_value = 100 * level;}
			else if (subtype == "Dagger") {buy_value = 50 * level;}
			else if (subtype == "Staff") {buy_value = 75 * level;}
			else {cout << "Not a valid subtype (Weapon -> buy value)" << endl;}
		}
		else if (type == "Shield") {
			//no subtypes to filter by
			buy_value = 30 * level;
		}
		else if (type == "Offhand") {
			//no subtypes to filter by
			buy_value = 30 * level;
		}
		else if (type == "Potion") {
			//no need to classify by subtype
			//health and mana potions should cost the same
			buy_value = 5 * level;
		}
		else if (type == "Room") {
			//no subtypes to filter by
			buy_value = 10 * level;
		}
		else if (type == "Trash") {
			if (subtype == "broken tooth") {buy_value = 6 * level;}
			else if (subtype == "hairball") {buy_value = 4 * level;}
			else if (subtype == "string") {buy_value = 8 * level;}
			else if (subtype == "tail fluff") {buy_value = 16 * level;}
			else if (subtype == "lint") {buy_value = 10 * level;}
			else {cout << "Not a valid subtype (Trash -> buy value)" << endl;}
		}
		else if (type == "Boss Drop") {buy_value = 200 * level;}
		else {cout << "Not a valid item type (buy value)" << endl;}
		
		//sell value is always half of the buy value...
		sell_value = buy_value / 2;
		damage_percentage = 0;
	}
	
	int level_multiplier(int x) {return x * 2 * level;}
};

class Spell {
public:
	Spell() {}
	
	void set_spell(string spell_name, string spell_type, int mag, int spell_cost, int player_level) {
		name = spell_name;
		type = spell_type;
		level = player_level;
		magnitude = mag;
		cost = spell_cost;
	}
	
	void inc_level() {
		level++;
		update_spell();
	}
	
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
	
	void update_spell() {
		magnitude /= (level - 1);
		magnitude *= level;
		cost /= (level - 1);
		cost *= level;
	}
};

//creature classes
class Creature {
public:
	Creature () {}
	
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
	virtual Item get_loot() {
		cout << "You've looted " << loot.get_name() << " from the " << name << "!" << endl;
		return loot;
	}
	bool get_has_loot() {return has_loot;}
	
	void det_creature(int player_level) {
		//creature determination can be any level up to current level
		level = rand() % player_level + 1;
	
		//determines type of creature encountered
		int x = rand() % 101;
		if (x <= 65) {type = "Balanced";} //bunny encounter - 65% chance
		else if (x > 65 && x <= 90) {type = "Offensive";} //ferret encounter - 25% chance
		else {type = "Defensive";} //midget encounter - 10% chance
		
		//initialize now that type is established
		stat_pool = 150 + 100 * (level - 1);
		set_type();
		has_loot = generate_loot();
	}
	
	//COMBAT
	void take_damage(int attack, int acc, int critical, string player_name) {
		if (rand() % 101 < dodge) {
			cout << name << " dodged your attack!" << endl;
			return;
		}
		if (rand() % 101 > acc) {
			cout << "Your attack missed!" << endl;
			return;
		}
		else {
			int x = DamageCalculation((double)attack, (double)def, (double)max_health, (double)critical, player_name);
			if (x > 0) {
				cout << name << " takes " << x << " damage!" << endl;
				health -= x;
			}
			else {cout << name << " took no damage." << endl;}
		}
		
	}
	void take_spell_dmg(int magnitude, int acc, int critical) {
		if (rand() % 101 < dodge) {
			cout << name << " dodged your attack!" << endl;
			return;
		}
		if (rand() % 101 > acc) {
			cout << "Your attack missed!" << endl;
			return;
		}
		else {
			//establish base damage
			int dmg = magnitude;
			//check for critical hits
			if (rand() % 101 < critical) {
				cout << "Your spell landed a critical hit!" << endl;
				dmg *= 2;
			}
			int skew = (int)(dmg * (1 - DMG_VARIANCE / 100.0));
			if (skew != dmg) {dmg = rand() % (dmg - skew) + skew;}
			else {dmg = skew;}
			cout << name << " takes " << dmg << " damage!" << endl;
			health -= dmg;
		}
	}
	bool is_dead() {
		if (health <= 0) {return true;}
		else {return false;}
	}
	
protected:
	//VARIABLES
	Item loot;
	string name, type;
	int exp, atk, def, health, max_health, stat_pool, level, creature_acc, dodge, parry, crit, money, escape_chance;
	static const int DMG_VARIANCE = 15; //read as % variance
	bool has_loot;

	virtual bool generate_loot() {
		const int NUM = 5;
		Item items[NUM];
		
		string subtypes[NUM] = {"broken tooth", "hairball", "string", "tail fluff", "lint"};
		string type = "Trash";
		
		for (int i = 0; i < NUM; i++) {items[i].set_item(type, subtypes[i], level);}
		
		int x = rand() % 101;
		if (x <= 100 && x > 95) {loot = items[4];}
		else if (x <= 95 && x > 85) {loot = items[3];}
		else if (x <= 85 && x > 70) {loot = items[2];}
		else if (x <= 70 && x > 50) {loot = items[1];}
		else if (x <= 50 && x > 25) {loot = items[0];}
		else {return false;}
		return true;
	}
	
	//HELPER FUNCTIONS TO ESTABLISH STANCE
	virtual void defensive() {
		creature_acc = 85;
		atk = stat_pool / 3;
		def = stat_pool * 2 / 3;
		max_health = stat_pool;
		dodge = 10;
		parry = 10;
		crit = 2;
		escape_chance = 95;
	}
	virtual void offensive() {
		creature_acc = 95;
		atk = stat_pool * 2 / 3;
		def = stat_pool / 3;
		max_health = stat_pool / 4;
		dodge = 2;
		parry = 2;
		crit = 10;
		escape_chance = 85;
	}
	virtual void balanced() {
		creature_acc = 90;
		atk = stat_pool / 2;
		def = stat_pool / 2;
		max_health = stat_pool / 2;
		dodge = 5;
		parry = 5;
		crit = 5;
		escape_chance = 90;
	}
	
	virtual void set_type() {
		string s;
		if (level >= 1) {s += "Cute ";}
		if (level >= 2) {s += "Fluffy ";}
		if (level >= 3) {s += "Crabby ";}
		if (level >= 4) {s += "Chubby ";}
		if (level == 5) {s += "DANGEROUS ";}
		
		atk = def = 0;
		if (type == "Offensive") {
			s += "ferret";
			name = s;
			exp = 10 * level;
			offensive();
		}
		else if (type == "Defensive") {
			s += "midget";
			name = s;
			exp = 15 * level;
			defensive();
		}
		else if (type == "Balanced") {
			s += "bunny";
			name = s;
			exp = 5 * level;
			balanced();
		}
		else {cout << "Not a valid type (Creature -> set_type)" << endl;}
		money = exp * 3;
		health = max_health;
	}
};
class Boss: public Creature {
public:
	Boss() {}
	Boss(int boss_level) {
		level = boss_level;
		creature_acc = 100;
		stat_pool = 150 + 100 * (level - 1);
		set_type();
		exp = 30 * level;
		money = exp * 5;
		has_loot = generate_loot();
	}
	
	//GET FUNCTIONS
	Item get_loot() {
		cout << "You've looted the " << loot.get_name() << " from " << name << "!" << endl;
		return loot;
	}
	string get_flavor() {return attack_flavor;}

private:
	//VARIABLES
	string attack_flavor;
	
	bool generate_loot() {
		const int NUM = 5;
		Item items[NUM];
		
		string subtypes[NUM] = {"Fractured Tooth", "Poop Nugget", "Torn Claw", "Flea Collar", "Golden Apple"};
		string type = "Boss Drop";
		
		for (int i = 0; i < NUM; i++) {items[i].set_item(type, subtypes[i], i + 1);}
		
		loot = items[level - 1];
		return true;
	}
	
	//HELPER FUNCTIONS TO ESTABLISH STANCE
	void defensive() {
		atk = stat_pool / 3;
		def = stat_pool * 2 / 3;
		max_health = stat_pool;
		dodge = 25;
		parry = 25;
		crit = 5;
		escape_chance = 90;
	}
	void offensive() {
		atk = stat_pool * 2 / 3;
		def = stat_pool / 3;
		max_health = stat_pool / 4;
		dodge = 5;
		parry = 5;
		crit = 25;
		escape_chance = 70;
	}
	void balanced() {
		atk = stat_pool / 2;
		def = stat_pool / 2;
		max_health = stat_pool / 2;
		dodge = 10;
		parry = 10;
		crit = 10;
		escape_chance = 80;
	}
	
	void set_type() {
		if (level == 1) {
			name = "Omar, the Ancient";
			attack_flavor = "Omar breathes on you. His putrid stench overwhelms you!";
			balanced();
		}
		else if (level == 2) {
			name = "Clarissa, the Gaseous";
			attack_flavor = "Clarissa flatulates. Your eyes water from her gaseous assault!";
			defensive();
		}
		else if (level == 3) {
			name = "Simba, the Feline";
			attack_flavor = "Simba claws you viciously, drawing blood. He meows affectionately at you!";
			offensive();
		}
		else if (level == 4) {
			name = "Ninja, the Labrador";
			attack_flavor = "Ninja tackles you and begins licking you with his slobbery tongue. Get off!";
			balanced();
		}
		else if (level == 5) {
			name = "Bob, your Uncle";
			attack_flavor = "Uncle Bob starts telling a story about bobbing for apples. The horror!";
			balanced();
		}
		else {cout << "Not a valid level (Boss -> set_type())" << endl;}
		max_health *= 3;
		health = max_health;
	}
};

class Quest {
public:
	Quest() {complete = false;}
	Quest(int quest_level) {
		level = quest_level;
		exp = 50 * level;
		gold = 100 * level;
		Boss boss(level);
		set_quest(boss.get_name());
	}
	
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
	int level, exp, gold;
	bool complete;
	
	void set_quest(string boss_name) {
		name = "WANTED: " + boss_name;
		description = "As you start to leave town, a woman comes running and screaming towards you from just outside the entrance to town. ";
		description += "As she nears, you notice that she is disheveled and manic, her clothes torn. You step into her path and stop her. ";
		description += "She stops, gasping, and tells you of the villanous " + boss_name + " that attempted to destroy her virtue. You promise ";
		description += "to bring swift and brutal justice to " + boss_name + " and return to the fair maiden. She swoons in your arms..... Yeah, ";
		description += "it's probably best to start bringing justice.";
		complete = false;
	}
};

//player classes
class Player {
public:
	Player() {}
	
	//INITIALIZATION
	void set_name() {
		cout << "What is your name? ";
		char c[30];
		cin.getline(c, 30);
		name = c;
		cout << "Hi, " << name << "!" << endl;
		Pause();
		Clear();
	}
	void set_class() {
		//switch "choice", confirmation "decision"
		char choice, decision;
		
		//introduction to classes
		cout << "In RPG Land, there are three classes you can play. These three "
		<< "classes are commonly known as Warriors, Rogues, and Mages." << endl;
		Pause();
		set_class_start:
		Clear();
		//print out the options to choose from
		cout << "Please choose a class to learn more about:" << endl;
		cout << "\n1. Warrior" << endl;
		cout << "2. Rogue" << endl;
		cout << "3. Mage" << endl;
		
		while (true) {
			choice = GetChar();
			switch (choice) {
				//warrior choice
				case '1':
					warrior_description();
					cout << "Would you like to choose the Warrior (Y/N)? ";
					while (true) {
						decision = GetChar();
						if (decision == 'Y' || decision == 'y') {
							player_class = "Warrior";
							goto set_class_confirm;
						}
						else if (decision == 'N' || decision == 'n') {goto set_class_start;}
						else {cout << "Please enter either Y or N: ";}
					}
					break;
				//rogue choice
				case '2':
					rogue_description();
					cout << "Would you like to choose the Rogue (Y/N)? ";
					while (true) {
						decision = GetChar();
						if (decision == 'Y' || decision == 'y') {
							player_class = "Rogue";
							goto set_class_confirm;
						}
						else if (decision == 'N' || decision == 'n') {goto set_class_start;}
						else {cout << "Please enter either Y or N: ";}
					}
					break;
				//mage choice
				case '3':
					mage_description();
					cout << "Would you like to choose the Mage (Y/N)? ";
					while (true) {
						decision = GetChar();
						if (decision == 'Y' || decision == 'y') {
							player_class = "Mage";
							goto set_class_confirm;
						}
						else if (decision == 'N' || decision == 'n') {goto set_class_start;}
						else {cout << "Please enter either Y or N: ";}
					}
					break;
			}
		}
		set_class_confirm:
		cout << "\nYou've chosen the " << player_class << " class! Congratulations!\n" << endl;
		Pause();
	}
	void set_quests() {
		Quest quest;
		boss_quest = quest;
		load_quest_active(false);
	}
	
	//these are referenced within base class even though they vary based on class type
	//Hence, these are set as "virtual". Cannot be set as "pure virtual" since the base
	//class is instantiated.
	virtual void set_player() {
		set_name();
		set_class();
	}
	virtual void update_stats() {}
	virtual void init_spellbook() {}
	virtual void restore_mana(int x) {}
	virtual void restore_mana_to_max() {}
	virtual void cast_healing_spell() {}
	virtual void cast_spell(Creature& creature) {}
	virtual int get_current_mana() {return 0;}
	virtual bool get_did_cast_spell() {return true;}
	virtual void level_up() {
		if (exp >= exp_to_next_level && level < MAX_LEVEL) {
			cout << "You've leveled up! ";
			level++;
			exp_to_next_level = LEVEL_INCREMENT * level * level;
			cout << "You are now Level " << level << "!" << endl;
			points = level - 1;
			Pause();
			allocate_points();
		}
	}
	virtual void view_stats() {
		Clear();
		cout << "Your current stats are: " << endl;
		cout << "ATK: " << atk << endl;
		cout << "DEF: " << def << endl;
		cout << "Health: " << current_health << "/" << max_health << endl;
		cout << "\nExperience: " << exp << "/" << exp_to_next_level << endl;
	}
	virtual void lower_def() {}
	//EQUIPMENT MANIPULATION
	virtual void unequip_item(Item item) {}
	virtual void equip_item(Item item) {}
	
	void equipment_menu() {
		Item *inv;
		
		equipment_menu_start:
		view_equipment();
		
		cout << "\nWhat would you like to do?" << endl;
		cout << "1. Equip an item" << endl;
		cout << "2. Unequip an item" << endl;
		cout << "3. Leave menu" << endl;
		
		int count = num_of_equipable_items();
		bool no_equipment = true;
		char choice;
		while (true) {
			choice = GetChar();
			switch (choice) {
				case '1':
				case 'E':
				case 'e':
					if (count > 0) {
						view_equipment();
						cout << endl;
						inv = view_equipable_items();
						cout << "\nWhich item would you like to equip?" << endl;
						int equip_choice;
						while (true) {
							equip_choice = GetInt();
							equip_choice--;
							if (equip_choice <= count) {
								equip_item(inv[equip_choice]);
								goto equipment_menu_start;
							}
						}
					}
					else {
						cout << "\nYou have no equippable items.\n" << endl;
						Pause();
						goto equipment_menu_start;
					}
					delete[] inv;
					inv = 0;
					break;
				case '2':
				case 'U':
				case 'u':
					for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
						if (equipment[i].get_level() > 0) {no_equipment = false;}
					}
					if (no_equipment) {
						cout << "You have nothing equipped; you cannot unequip anything." << endl;
						Pause();
						goto equipment_menu_start;
					}
					view_equipment();
					cout << EQUIPMENT_SLOTS + 1 << ". Cancel request" << endl;
					cout << "\nWhich equipment would you like to unequip?" << endl;
					int unequip_choice;
					while (true) {
						unequip_choice = GetInt();
						unequip_choice--;
						if (unequip_choice < EQUIPMENT_SLOTS) {
							unequip_item(equipment[unequip_choice]);
							goto equipment_menu_start;
						}
						else if (unequip_choice == EQUIPMENT_SLOTS) {
							goto equipment_menu_start;
						}
					}
					break;
				case '3':
				case 'L':
				case 'l':
					return;
					break;
			}
		}
	}
	
	//HEALTH MANIPULATION
	void restore_health_to_max() {current_health = max_health;}
	void restore_health(int x) {
		current_health += x;
		if (current_health > max_health) {restore_health_to_max();}
	}
	void lose_health(int x) {current_health -= x;}
	
	//INVENTORY MANIPULATION
	void inc_inventory(Item item) {
		bool in_inventory = false;
		int index;
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].get_name() == item.get_name()) {
				in_inventory = true;
				index = i;
			}
		}
		if (in_inventory) {inventory[index].inc_quantity();}
		else {add_to_inventory(item);}
	}
	void dec_inventory(Item item) {
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].get_name() == item.get_name()) {
				inventory[i].dec_quantity();
				if (inventory[i].get_quantity() == 0) {
					remove_from_inventory(inventory[i]);
				}
			}
		}
	}

	//COMBAT
	void take_damage(int attack, int accuracy, int critical, string creature_name) {
		if (rand() % 101 < block && equipment[2].broken() == false) {
			cout << "You block " << creature_name << "'s attack!" << endl;
			equipment[2].damage(1); //only applies to Warriors, so damages shield slot
			return;
		}
		if (rand() % 101 < dodge) {
			cout << "You dodge " << creature_name << "'s attack!" << endl;
			return;
		}
		if (rand() % 101 < parry) {
			cout << "You parry " << creature_name << "'s attack!" << endl;
			if (player_class == "Rogue") {
				if (equipment[1].broken() && equipment[2].broken()) {}
				else if (equipment[1].broken()) {equipment[2].damage(1);}
				else if (equipment[2].broken()) {equipment[1].damage(1);}
				else {
					int x = rand() % 2 + 1; //either 1 or 2
					equipment[x].damage(1); //randomly damage a weapon
				}
			}
			else {
				if (equipment[1].broken() == false) {equipment[1].damage(1);} //damage weapon slot
			}
			return;
		}
		if (rand() % 101 > accuracy) {
			cout << creature_name << " missed you!" << endl;
			return;
		}
		else {
			int x = DamageCalculation((double)attack, (double)def, (double)max_health, (double)critical, creature_name);
			if (x > 0) {
				cout << "You take " << x << " damage!" << endl;
				lose_health(x);
				equipment[0].damage(1); //always Armor slot
			}
			else {cout << "You didn't take any damage." << endl;}
		}
	}
	void dies() {
		cout << "You've died!" << endl;
		lose_exp(50);
		for (int i = 0; i < EQUIPMENT_SLOTS; i++) {equipment[i].damage(20);}
		Pause();
		if (exp < LEVEL_INCREMENT * (level - 1)) {exp = LEVEL_INCREMENT * (level - 1);}
	}

	void repair(int x) {
		equipment[x].repair(100);
	}
	
	//VIEW FUNCTIONS
	void view_attributes() {
		Clear();
		cout << "Your current attributes are: " << endl;
		cout << "Strength: " << strength << endl;
		cout << "Agility: " << agility << endl;
		cout << "Intellect: " << intellect << endl;
		cout << "Stamina: " << stamina << endl;
	}
	void view_inventory() {
		Clear();
		if (items_in_inventory == 0) {
			cout << "You have no items in your inventory." << endl;
		}
		else {
			cout << "You have the following items in your inventory:" << endl;
			for (int i = 0; i < items_in_inventory; i++) {
				cout << inventory[i].get_name() << " x" << inventory[i].get_quantity();
				if (inventory[i].equipable()) {cout << " - can be equipped" << endl;}
				else {cout << endl;}
			}
		}
		cout << endl;
		view_gold();
	}
	void view_equipment() {
		Clear();
		cout << "You have the following equipped:" << endl;
		for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
			cout << i + 1 << ". " << equipment[i].get_type() << " slot: " << equipment[i].get_name();
			if (equipment[i].broken()) {cout << " - BROKEN";}
			else if (equipment[i].get_level() > 0) {cout << " - " << 100 - equipment[i].get_damage() << "/100";}
			cout << endl;
		}
	}
	Item* view_equipable_items() {
		int count = 0;
		Item *inv;
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].equipable()) {count++;}
		}
		inv = new Item[count];
		int j = 0;
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].equipable()) {
				inv[j] = inventory[i];
				j++;
			}
		}
		cout << "You have the following equipable items:" << endl;
		for (int i = 0; i < count; i++) {
			cout << i + 1 << ". " << inv[i].get_name()
			<< " x" << inv[i].get_quantity()<< endl;
		}
		return inv;
	}
	void view_gold() {cout << "You currently have " << gold << " gold." << endl;}
	void view_quest_log() {
		Clear();
		if (quest_active) {
			cout << boss_quest.get_name() << endl << endl;
			cout << "Description:" << endl;
			cout << boss_quest.get_description() << endl;
			cout << "\nReward for completing:" << endl;
			cout << boss_quest.get_exp() << " experience" << endl;
			cout << boss_quest.get_gold() << " gold" << endl;
			cout << "\nCompleted: ";
			if (boss_quest.get_complete() == false) {cout << "No" << endl;}
			else {cout << "Yes" << endl;}
		}
		else {cout << "You don't have an active quest." << endl;}
	}
	
	//GOLD-EXP MANIPULATION
	void remove_gold(int x) {gold -= x;}
	void add_gold(int x) {
		gold += x;
		cout << "You've collected " << x << " gold!" << endl;
	}
	void gain_exp(int exp_gain) {
		exp += exp_gain;
		cout << "You've gained " << exp_gain << " experience!" << endl;
		level_up();
	}
	void lose_exp(int exp_loss) {
		exp -= exp_loss;
		cout << "You've lost " << exp_loss << " experience!" << endl;
	}
	void earn_explore_bonus(int area) {
		cout << "You've explored the entire area!" << endl;
		gain_exp(area);
		add_gold(area * 2);
		Pause();
	}
	
	//QUESTING
	void quest_calculation_and_acceptance() {
		for (int i = 0; i < level; i++) {
			if (boss_quests_complete[i] == false) {//haven't completed it yet
				Quest proposed_quest(i + 1);
				cout << proposed_quest.get_description() << endl;
				cout << "\nQuest accepted!" << endl;
				Pause();
				boss_quest = proposed_quest;
				load_quest_active(true);
				return; //prevents accepting additional boss quests, which would overwrite previous ones
			}
		} //if the end is reached with an accepted quest, it means they're all done up to current level
	}
	bool quest_complete() {
		if (boss_quest.get_complete()) {return true;}
		else {return false;}
	}
	void complete_active_quest() {boss_quest.complete_quest();}
	void turn_in_quest() {
		Boss boss(boss_quest.get_level());
		cout << "A woman rushes over to you, and it takes you a moment to realize that she is the same person "
		<< "who told you about " << boss.get_name() << ". She's cleaned up, and is looking much more attractive." << endl;
		cout << "Damsel: 'Did you get that foul " << boss.get_name() << "?' You nod, and her face splits into a beautiful, "
		<< "rather seductive grin." << endl;
		cout << "Damsel: 'Great, let me repay you for your kind deed...'" << endl;
		cout << "\nQuest complete! You've earned the following rewards:" << endl;
		cout << boss_quest.get_exp() << " experience" << endl;
		cout << boss_quest.get_gold() << " gold" << endl;
		gain_exp(boss_quest.get_exp());
		add_gold(boss_quest.get_gold());
		load_quest_active(false);
		boss_quests_complete[boss_quest.get_level() - 1] = true;
	}
	
	//LEVELING
	void allocate_points() {
		allocate_points_start:
		while (points > 0) {
			
			//overview
			view_attributes();
		
			cout << "\nYou have " << points << " points remaining. ";
			char choice;
			while (true) {
				cout << "Please select an attribute to add to:" << endl;
			
				//list of available attributes to add to
				cout << "1. Strength" << endl;
				cout << "2. Agility" << endl;
				cout << "3. Intellect" << endl;
				cout << "4. Stamina" << endl;
				choice = GetChar();
				switch (choice) {
					case '1':
						base_str++;
						update_stats();
						points--;
						goto allocate_points_start;
						break;
					case '2':
						base_agi++;
						update_stats();
						points--;
						goto allocate_points_start;
						break;
					case '3':
						base_int++;
						update_stats();
						points--;
						goto allocate_points_start;
						break;
					case '4':
						base_sta++;
						update_stats();
						points--;
						goto allocate_points_start;
						break;
				}
			}
		}
		view_attributes();
		cout << "\nAll available points have been allocated." << endl;
		Pause();
	}

	//BOOLS
	bool use_item() {
		Clear();
		Item *potions;
		int count = 0;
		
		//establish number of potions in inventory
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].get_type() == "Potion") {count++;}
		}
		
		//what if you have no items?
		if (count == 0) {
			cout << "You don't have any useable items!" << endl;
			Pause();
			return false;
		}
		
		//set temporary array with correct size and fill it
		potions = new Item[count];
		int index = 0;
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].get_type() == "Potion") {
				potions[index] = inventory[i];
				index++;
			}
		}
		
		//choose a potion to use
		cout << "What potion would you like to use:\n" << endl;
		for (int i = 0; i < count; i++) {
			cout << i + 1 << ". " << potions[i].get_name() << " x"
			<< potions[i].get_quantity() << endl;
		}
		//adds an option to cancel
		cout << count + 1 << ". Cancel request (go back to the previous screen)" << endl;
		
		int choice;
		while (true) {
			//decrement by one for index
			choice = GetInt();
			choice--;
			if (choice < count) {
				//determine change to health / mana
				if (potions[choice].get_subtype() == "Health") {
					int gain = 30 * potions[choice].get_level();
					restore_health(gain);
					cout << "You've used a " << potions[choice].get_name() << " and restored "
					<< gain << " health!" << endl;
				}
				else if (potions[choice].get_subtype() == "Mana") {
					if (player_class == "Mage") {
						int gain = 30 * potions[choice].get_level();
						restore_mana(gain);
						cout << "You've used a " << potions[choice].get_name() << " and restored "
						<< gain << " mana!" << endl;
					}
					else {
						cout << "You've used a " << potions[choice].get_name()
						<< ", but it had no effect..." << endl;
					}
				}
				Pause();
				//decrement item from inventory
				dec_inventory(potions[choice]);
				return true;
			}
			else if (choice == count) {return false;} //cancels out
		}
	}
	bool is_dead() {
		if (current_health <= 0) {return true;}
		else {return false;}
	}
	bool has_inventory() {
		if (items_in_inventory > 0) {return true;}
		else {return false;}
	}

	//GET FUNCTIONS
	int get_base_str() {return base_str;}
	int get_base_agi() {return base_agi;}
	int get_base_int() {return base_int;}
	int get_base_sta() {return base_sta;}
	int get_points() {return points;}
	int get_exp() {return exp;}
	int get_gold() {return gold;}
	int get_level() {return level;}
	int get_atk() {return atk;}
	int get_def() {return def;}
	int get_max_health() {return max_health;}
	int get_max_mana() {return max_mana;}
	int get_current_health() {return current_health;}
	int get_equip_atk_bonus() {return equip_atk_bonus;}
	int get_equip_def_bonus() {return equip_def_bonus;}
	int get_equip_health_bonus() {return equip_health_bonus;}
	int get_items_in_inventory() {return items_in_inventory;}
	int get_accuracy() {return acc;}
	int get_crit() {return crit;}
	int get_boosted() {return boosted;}
	int get_max_level() {return MAX_LEVEL;}
	string get_name() {return name;}
	string get_player_class() {return player_class;}
	Item* get_inventory() {return inventory;}
	Item* get_equipment() {return equipment;}
	bool* get_boss_quests_complete() {return boss_quests_complete;}
	bool get_quest_active() {return quest_active;}
	Quest get_boss_quest() {return boss_quest;}
	
	//LOAD FUNCTIONS
	void load_base_str(int x) {base_str = x;}
	void load_base_agi(int x) {base_agi = x;}
	void load_base_int(int x) {base_int = x;}
	void load_base_sta(int x) {base_sta = x;}
	void load_points(int x) {points = x;}
	void load_exp(int x) {exp = x;}
	void load_gold(int x) {gold = x;}
	void load_atk(int x) {atk = x;}
	void load_def(int x) {def = x;}
	void load_max_health(int x) {max_health = x;}
	void load_max_mana(int x) {max_mana = x;}
	void load_current_health(int x) {current_health = x;}
	void load_equip_atk_bonus(int x) {equip_atk_bonus = x;}
	void load_equip_def_bonus(int x) {equip_def_bonus = x;}
	void load_equip_health_bonus(int x) {equip_health_bonus = x;}
	void load_items_in_inventory(int x) {items_in_inventory = x;}
	void load_name(char *c) {name = c;}
	void load_player_class(char *c) {player_class = c;}
	void load_inventory(Item *items) {
		inventory = new Item[items_in_inventory];
		for (int i = 0; i < items_in_inventory; i++) {
			inventory[i] = items[i];
		}
	}
	void load_equipment(Item* &items) {
		for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
			equipment[i].set_item(items[i].get_type(), items[i].get_subtype(), items[i].get_level()); //init_item
		}
	}
	void load_level(int xp) {
		level = 1;
		exp_to_next_level = LEVEL_INCREMENT * level * level;
		for (; xp >= exp_to_next_level && level < MAX_LEVEL; level++) {
			exp_to_next_level = LEVEL_INCREMENT * level * level;
		}
	}
	void load_quest_active(bool active) {quest_active = active;}
	
protected:
	//VARIABLES
	static const int EQUIPMENT_SLOTS = 3;
	Item equipment[EQUIPMENT_SLOTS];
	Item *inventory;
	Quest boss_quest;
	string name, player_class;
	int base_str, base_agi, base_int, base_sta; //baseline attributes
	int str_bonus, agi_bonus, int_bonus, sta_bonus; //equipment attribute bonuses
	int strength, agility, intellect, stamina; //fully adjusted attributes
	int points, exp, items_in_inventory, gold, level;
	int atk, def, max_health, max_mana, current_health;
	int equip_atk_bonus, equip_def_bonus, equip_health_bonus;
	int crit, dodge, parry, block, acc, exp_to_next_level;
	int boosted;
	static const int ATTRIB_BONUS = 5;
	static const int CLASS_BONUS = 10;
	static const int LEVEL_INCREMENT = 50;
	static const int DMG_VARIANCE = 15; //read as % variance
	static const int MAX_LEVEL = 5;
	bool boss_quests_complete[MAX_LEVEL];
	bool quest_active;
	
	//CLASS DESCRIPTIONS
	void warrior_description() {
		cout << "Warriors are melee fighters highly trained in the art "
		<< "of weaponry. They are unmatched in one-on-one physical combat, "
		<< "have unusually high stamina, and can wear any type of armor. They "
		<< "are the only class that can use shields.\n" << endl;
		
		cout << "A warrior's primary attribute is Strength." << endl;
	}
	void rogue_description() {
		cout << "Rogues are highly trained in the art of subterfuge and stealth. "
		<< "They specialize in taking their enemy by surprise, and dealing "
		<< "copious amounts of damage by exploiting this advantage. Rogues cannot "
		<< "wear heavy armor or use shields.\n" << endl;
		
		cout << "A rogue's primary attribute is Agility." << endl;
	}
	void mage_description() {
		cout << "Mages are masters of the elements and arcane spells. They use "
		<< "both offensive and defensive spells to both decimate their foes and "
		<< "protect and heal themselves. To assist them in channelling these potent "
		<< "magics, mages elect to wear flowing robes of magically-imbued cloth and "
		<< "eschew more traditional forms of protection.\n" << endl;
		
		cout << "A mage's primary attribute is Intellect." << endl;
	}
	
	//establishes pre-customization baseline for attributes
	void init_attribs() {base_str = base_agi = base_int = base_sta = 10;}

	//INVENTORY MANIPULATION
	void add_to_inventory(Item item) {
		//create temp array with current number of items in inventory
		Item *tmp_array;
		tmp_array = new Item[items_in_inventory];
		for (int i = 0; i < items_in_inventory; i++) {
			tmp_array[i] = inventory[i];
		}
		
		//increment the items in inventory counter by one
		items_in_inventory++;
		
		//set inventory to the new size and copy current items back in
		inventory = new Item[items_in_inventory];
		for (int i = 0; i < items_in_inventory - 1; i++) {
			inventory[i] = tmp_array[i];
		}
		
		//purge temp array to prevent memory leaks
		delete[] tmp_array;
		tmp_array = 0;
		
		//add the new item to the last (new) index in inventory
		inventory[items_in_inventory - 1] = item;
	}
	void remove_from_inventory(Item item) {
		//create temp array with one less index
		Item *tmp_array;
		tmp_array = new Item[items_in_inventory - 1];
		int count = 0;
		
		//moves over the items to be kept
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].get_name() != item.get_name()) {
				tmp_array[count] = inventory[i];
				count++;
			}
		}
		
		//decrement the items in inventory counter by one
		items_in_inventory--;
		
		//set inventory to the new size and copy updated items back in
		inventory = new Item[items_in_inventory];
		for (int i = 0; i < items_in_inventory; i++) {
			inventory[i] = tmp_array[i];
		}
		
		//purge temp array to prevent memory leaks
		delete[] tmp_array;
		tmp_array = 0;
	}
	int num_of_equipable_items() {
		int count = 0;
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].equipable()) {count++;}
		}
		return count;
	}
};
class Warrior: public Player {
public:
	Warrior(string player_name) {
		name = player_name;
		player_class = "Warrior";
		init_attribs();
		base_str += CLASS_BONUS;
		base_sta += CLASS_BONUS;
		boosted = 0;
	}
	
	void set_player() {
		level = 1;
		exp_to_next_level = LEVEL_INCREMENT * level * level;
		exp = items_in_inventory = 0;
		gold = 300;
		points = 20;
		update_stats();
		allocate_points();
		current_health = max_health;
		init_equipment();
		for (int i = 0; i < MAX_LEVEL; i++) {boss_quests_complete[i] = false;}
	}
	
	//establishes stats and attributes, and takes bonuses into consideration
	void update_stats() {
		//update equipment bonuses
		block = 5 * equipment[2].get_level(); //will correct to 0 if no equipment
		if (equipment[0].get_level() > 0 && equipment[2].get_level() > 0) {
			if (equipment[0].broken() && equipment[2].broken()) {equip_def_bonus = 0;}
			else if (equipment[0].broken()) {equip_def_bonus = equipment[2].stat_value();}
			else if (equipment[2].broken()) {equip_def_bonus = equipment[0].stat_value();}
			else {equip_def_bonus = equipment[0].stat_value() + equipment[2].stat_value();}
		}
		else {equip_def_bonus = 0;}
		
		const int WEAPON_BONUS = 2;
		if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			int weapon_gain = equipment[1].stat_value();
			if (equipment[1].get_subtype() == "Sword") {weapon_gain *= WEAPON_BONUS;}
			equip_atk_bonus = weapon_gain;
		}
		else {equip_atk_bonus = 0;}
		
		if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
			equip_health_bonus = equipment[0].stat_value() / ATTRIB_BONUS;
		}
		else {equip_health_bonus = 0;}
		
		//establish attribute bonuses
		str_bonus = agi_bonus = int_bonus = sta_bonus = 0; //initialize
		if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
			if (equipment[0].get_subtype() == "Mail") {
				str_bonus += equipment[0].attrib_value();
				sta_bonus += equipment[0].attrib_value();
			}
			else if (equipment[0].get_subtype() == "Leather") {
				agi_bonus += equipment[0].attrib_value();
			}
			else if (equipment[0].get_subtype() == "Cloth") {
				int_bonus += equipment[0].attrib_value();
			}
		}
		if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			if (equipment[1].get_subtype() == "Sword") {
				str_bonus += equipment[1].attrib_value();
				sta_bonus += equipment[1].attrib_value();
			}
			else if (equipment[1].get_subtype() == "Dagger") {
				agi_bonus += equipment[1].attrib_value();
			}
			else if (equipment[1].get_subtype() == "Staff") {
				int_bonus += equipment[1].attrib_value();
			}
		}
		if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
			str_bonus += equipment[2].attrib_value();
			sta_bonus += equipment[2].attrib_value();
		}
		
		//update attributes
		strength = base_str + str_bonus + equip_atk_bonus / ATTRIB_BONUS;
		agility = base_agi + agi_bonus + equip_def_bonus / ATTRIB_BONUS;
		intellect = base_int + int_bonus;
		stamina = base_sta + sta_bonus + equip_health_bonus / ATTRIB_BONUS;
		
		//sets ATK and DEF
		atk = strength * 2 + agility + equip_atk_bonus;
		def = agility * 2 + equip_def_bonus;
		
		//sets max health and mana
		max_health = 10 + stamina * 2 + equip_health_bonus;
		max_mana = 0;
		
		//sets crit chance
		crit = 5;
		
		//sets dodge and parry chances
		dodge = 5;
		parry = 5;
		
		//sets accuracy
		acc = 90;
	}
	
	//EQUIPMENT MANIPULATION
	void unequip_item(Item item) {
		if (item.get_level() > 0) {
			if (item.get_type() == "Armor") {
				equipment[0].set_item("Armor", "Cloth", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Weapon") {
				equipment[1].set_item("Weapon", "Staff", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Shield") {
				equipment[2].set_item("Shield", "none", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
		}
		else {
			cout << "You don't have anything to unequip from that slot." << endl;
			Pause();
		}
	}
	void equip_item(Item item) {
		//failures
		if (item.get_type() == "Offhand") {
			cout << "You cannot equip offhand items" << endl;
			Pause();
			return;
		}
		//assuming no failures...
		else if (item.get_type() == "Armor") {
			if (equipment[0].get_level() > 0) {unequip_item(equipment[0]);}
			equipment[0] = item;
		}
		else if (item.get_type() == "Weapon") {
			if (equipment[1].get_level() > 0) {unequip_item(equipment[1]);}
			equipment[1] = item;
		}
		else if (item.get_type() == "Shield") {
			if (equipment[2].get_level() > 0) {unequip_item(equipment[2]);}
			equipment[2] = item;
		}
		update_stats();
		dec_inventory(item);
		cout << "\nYou've equipped " << item.get_name() << "." << endl;
		Pause();
	}

private:
	void init_equipment() {
		equipment[0].set_item("Armor", "Cloth", 0);
		equipment[1].set_item("Weapon", "Staff", 0);
		equipment[2].set_item("Shield", "none", 0);
	}
};
class Rogue: public Player {
public:
	Rogue(string player_name) {
		name = player_name;
		player_class = "Rogue";
		init_attribs();
		base_agi += CLASS_BONUS;
		boosted = 0;
	}
	
	void set_player() {
		level = 1;
		exp_to_next_level = LEVEL_INCREMENT * level * level;
		exp = items_in_inventory = 0;
		gold = 300;
		points = 20;
		update_stats();
		allocate_points();
		current_health = max_health;
		init_equipment();
		for (int i = 0; i < MAX_LEVEL; i++) {boss_quests_complete[i] = false;}
	}
	
	//establishes stats and attributes, and takes bonuses into consideration
	void update_stats() {
		//update equipment bonuses
		if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
			equip_def_bonus = equipment[0].stat_value();
			equip_health_bonus = equipment[0].stat_value() / ATTRIB_BONUS;
		}
		else {equip_def_bonus = equip_health_bonus = 0;}
		
		const int WEAPON_BONUS = 5;
		if (equipment[1].get_level() > 0 && equipment[2].get_level() > 0) {
			if (equipment[1].broken() && equipment[2].broken()) {equip_atk_bonus = 0;}
			else if (equipment[1].broken()) {
				int weapon_gain = equipment[2].stat_value();
				if (equipment[2].get_subtype() == "Dagger") {weapon_gain *= WEAPON_BONUS;}
				equip_atk_bonus = weapon_gain;
			}
			else if (equipment[2].broken()) {
				int weapon_gain = equipment[1].stat_value();
				if (equipment[1].get_subtype() == "Dagger") {weapon_gain *= WEAPON_BONUS;}
				equip_atk_bonus = weapon_gain;
			}
			else {
				int weapon_gain1 = equipment[1].stat_value();
				if (equipment[1].get_subtype() == "Dagger") {weapon_gain1 *= WEAPON_BONUS;}
				int weapon_gain2 = equipment[2].stat_value();
				if (equipment[2].get_subtype() == "Dagger") {weapon_gain2 *= WEAPON_BONUS;}
				equip_atk_bonus = weapon_gain1 + weapon_gain2;
			}
		}
		else if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			int weapon_gain = equipment[1].stat_value();
			if (equipment[1].get_subtype() == "Dagger") {weapon_gain *= WEAPON_BONUS;}
			equip_atk_bonus = weapon_gain;
		}
		else if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
			int weapon_gain = equipment[2].stat_value();
			if (equipment[2].get_subtype() == "Dagger") {weapon_gain *= WEAPON_BONUS;}
			equip_atk_bonus = weapon_gain;
		}
		else {equip_atk_bonus = 0;}
		
		//establish attribute bonuses
		str_bonus = agi_bonus = int_bonus = sta_bonus = 0; //initialize
		if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
			if (equipment[0].get_subtype() == "Leather") {
				agi_bonus += equipment[0].attrib_value();
			}
			else if (equipment[0].get_subtype() == "Cloth") {
				int_bonus += equipment[0].attrib_value();
			}
		}
		if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			if (equipment[1].get_subtype() == "Sword") {
				str_bonus += equipment[1].attrib_value();
				sta_bonus += equipment[1].attrib_value();
			}
			else if (equipment[1].get_subtype() == "Dagger") {
				agi_bonus += equipment[1].attrib_value();
			}
			else if (equipment[1].get_subtype() == "Staff") {
				int_bonus += equipment[1].attrib_value();
			}
		}
		if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
			if (equipment[2].get_subtype() == "Sword") {
				str_bonus += equipment[2].attrib_value();
				sta_bonus += equipment[2].attrib_value();
			}
			else if (equipment[2].get_subtype() == "Dagger") {
				agi_bonus += equipment[2].attrib_value();
			}
			else if (equipment[2].get_subtype() == "Staff") {
				int_bonus += equipment[2].attrib_value();
			}
		}
		
		//update attributes
		strength = base_str + str_bonus;
		agility = base_agi + agi_bonus + equip_atk_bonus / ATTRIB_BONUS;
		intellect = base_int + int_bonus;
		stamina = base_sta + sta_bonus + equip_health_bonus / ATTRIB_BONUS;
		
		//sets ATK and DEF
		atk = agility * 2 + strength + equip_atk_bonus;
		def = agility + equip_def_bonus;
		
		//sets max health and mana
		max_health = 10 + stamina + equip_health_bonus;
		max_mana = 0;
		
		//sets crit chance
		if (equipment[1].get_level() > 0 && equipment[2].get_level() > 0) {
			if (equipment[1].broken() && equipment[2].broken()) {crit = 10;}
			else if (equipment[1].broken() || equipment[2].broken()) {crit = 10 + 3 * level;}
			else {crit = 10 + 6 * level;}
		}
		else if (equipment[1].get_level() > 0 || equipment[2].get_level() > 0) {
			if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {crit = 10 + 3 * level;}
			else if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {crit = 10 + 3 * level;}
			else {crit = 10;}
		}
		else {crit = 10;}
		
		//sets dodge and parry chances
		dodge = 10;
		if (equipment[1].get_level() > 0 && equipment[2].get_level() > 0) {
			if (equipment[1].broken() && equipment[2].broken()) {parry = 5;}
			else if (equipment[1].broken() || equipment[2].broken()) {parry = 10;}
			else {parry = 15;}
		}
		else if (equipment[1].get_level() > 0 || equipment[2].get_level() > 0) {
			if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {parry = 10;}
			else if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {parry = 10;}
			else {parry = 5;}
		}
		else {parry = 5;}
		
		//sets accuracy
		acc = 90;
	}

	//EQUIPMENT MANIPULATION
	void unequip_item(Item item) {
		if (item.get_level() > 0) {
			if (item.get_type() == "Armor") {
				equipment[0].set_item("Armor", "Cloth", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Weapon" && equipment[1].get_name() == item.get_name() && equipment[2].get_name() == item.get_name()) {
				equipment[2].set_item("Weapon", "Staff", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Weapon" && equipment[1].get_name() == item.get_name()) {
				equipment[1].set_item("Weapon", "Staff", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Weapon" && equipment[2].get_name() == item.get_name()) {
				equipment[2].set_item("Weapon", "Staff", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
		}
		else {
			cout << "You don't have anything to unequip from that slot." << endl;
			Pause();
		}
	}
	void equip_item(Item item) {
		//failures
		if (item.get_type() == "Offhand") {
			cout << "You cannot equip offhand items" << endl;
			Pause();
			return;
		}
		else if (item.get_type() == "Shield") {
			cout << "You cannot equip shields." << endl;
			Pause();
			return;
		}
		else if (item.get_type() == "Armor") {
			if (item.get_subtype() == "Mail") {
				cout << "You cannot equip mail armor." << endl;
				Pause();
				return;
			}
			//assuming no failures...
			if (equipment[0].get_level() > 0) {unequip_item(equipment[0]);}
			equipment[0] = item;
		}
		//assuming no failures...
		else if (item.get_type() == "Weapon") {
			if (equipment[1].get_level() > 0 && equipment[2].get_level() > 0) {
				unequip_item(equipment[2]);
				equipment[2] = item;
			}
			else if (equipment[1].get_level() > 0) {equipment[2] = item;}
			else if (equipment[2].get_level() > 0) {equipment[1] = item;}
			else {equipment[1] = item;}
		}
		update_stats();
		dec_inventory(item);
		cout << "\nYou've equipped " << item.get_name() << "." << endl;
		Pause();
	}
	
private:
	void init_equipment() {
		equipment[0].set_item("Armor", "Cloth", 0);
		equipment[1].set_item("Weapon", "Staff", 0);
		equipment[2].set_item("Weapon", "Staff", 0);
	}
};
class Mage: public Player {
public:
	Mage(string player_name) {
		name = player_name;
		player_class = "Mage";
		init_attribs();
		base_int += CLASS_BONUS;
		boosted = 0;
	}
	
	void set_player() {
		level = 1;
		exp_to_next_level = LEVEL_INCREMENT * level * level;
		exp = items_in_inventory = 0;
		gold = 300;
		points = 20;
		update_stats();
		allocate_points();
		current_health = max_health;
		current_mana = max_mana;
		init_equipment();
		init_spellbook();
		for (int i = 0; i < MAX_LEVEL; i++) {boss_quests_complete[i] = false;}
	}
	
	void init_spellbook() {
		const int LOWEST_MAG = 20; //starting point for magnitude algorithm
		const int LOWEST_COST = 15; //starting point for cost algorithm
		const int MAG_STEP = 30; //jump increment between magnitudes
		string names[SPELL_COUNT] = {	"Fireball", 				"Salve", 					"Steelskin"};
		string types[SPELL_COUNT] = {	"Offensive", 				"Healing", 					"Defensive"};
		int magnitudes[SPELL_COUNT] = {	LOWEST_MAG + MAG_STEP * 1, 	LOWEST_MAG + MAG_STEP * 0, 	LOWEST_MAG + MAG_STEP * 2};
		int costs[SPELL_COUNT] = {		LOWEST_COST,				LOWEST_COST * 2,			LOWEST_COST * 3};
		
		for (int i = 0; i < SPELL_COUNT; i++) {
			spellbook[i].set_spell(names[i], types[i], magnitudes[i], costs[i], level);
		}
	}
	
	//establishes stats and attributes, and takes bonuses into consideration
	void update_stats() {
		//update equipment bonuses
		if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
			equip_def_bonus = equipment[0].stat_value();
			equip_health_bonus = equipment[0].stat_value() / ATTRIB_BONUS;
		}
		else {equip_def_bonus = equip_health_bonus = 0;}
		
		const int WEAPON_BONUS = 2;
		if (equipment[1].get_level() > 0 && equipment[2].get_level() > 0) {
			if (equipment[1].broken() && equipment[2].broken()) {equip_atk_bonus = equip_mana_bonus = 0;}
			else if (equipment[1].broken()) {equip_mana_bonus = equipment[2].stat_value();}
			else if (equipment[2].broken()) {
				int weapon_gain = equipment[1].stat_value();
				if (equipment[1].get_subtype() == "Staff") {
					weapon_gain *= WEAPON_BONUS;
					equip_mana_bonus = weapon_gain;
				}
				equip_atk_bonus = weapon_gain;
			}
			else {
				int weapon_gain = equipment[1].stat_value();
				int offhand_gain = equipment[2].stat_value();
				if (equipment[1].get_subtype() == "Staff") {
					weapon_gain *= WEAPON_BONUS;
					equip_mana_bonus = weapon_gain + offhand_gain;
				}
				else {equip_mana_bonus = offhand_gain;}
				equip_atk_bonus = weapon_gain;
			}
		}
		else if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			int weapon_gain = equipment[1].stat_value();
			if (equipment[1].get_subtype() == "Staff") {
				weapon_gain *= WEAPON_BONUS;
				equip_mana_bonus = weapon_gain;
			}
			equip_atk_bonus = weapon_gain;
		}
		else if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
			equip_mana_bonus = equipment[2].stat_value();
		}
		else {equip_atk_bonus = equip_mana_bonus = 0;}
		
		//establish attribute bonuses
		str_bonus = agi_bonus = int_bonus = sta_bonus = 0; //initialize
		if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {int_bonus += equipment[0].attrib_value();}
		if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			if (equipment[1].get_subtype() == "Sword") {
				str_bonus += equipment[1].attrib_value();
				sta_bonus += equipment[1].attrib_value();
			}
			else if (equipment[1].get_subtype() == "Dagger") {agi_bonus += equipment[1].attrib_value();}
			else if (equipment[1].get_subtype() == "Staff") {int_bonus += equipment[1].attrib_value();}
		}
		if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {int_bonus += equipment[2].attrib_value();}
		
		//update attributes
		strength = base_str + str_bonus + equip_atk_bonus / ATTRIB_BONUS;
		agility = base_agi + agi_bonus + equip_def_bonus / ATTRIB_BONUS;
		intellect = base_int + int_bonus + equip_mana_bonus / ATTRIB_BONUS;
		stamina = base_sta + sta_bonus + equip_health_bonus / ATTRIB_BONUS;
		
		//sets ATK and DEF
		atk = strength + agility + equip_atk_bonus;
		def = agility * 2 + equip_def_bonus;
		
		//sets max health and mana
		max_health = 10 + stamina + equip_health_bonus;
		max_mana = intellect * 2 + equip_mana_bonus;
		
		//sets crit chance
		crit = 5;
		
		//sets dodge and parry chances
		dodge = 5;
		parry = 5;
		
		//sets accuracy
		acc = 90;
		
		//updates spellpower
		spellpower = intellect / ATTRIB_BONUS;
		for (int i = 0; i < SPELL_COUNT; i++) {
			int x = spellbook[i].get_magnitude();
			spellbook[i].load_magnitude(x + spellpower);
		}
	}
	
	//EQUIPMENT MANIPULATION
	void unequip_item(Item item) {
		if (item.get_level() > 0) {
			if (item.get_type() == "Armor") {
				equipment[0].set_item("Armor", "Cloth", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Weapon") {
				equipment[1].set_item("Weapon", "Staff", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Offhand") {
				equipment[2].set_item("Offhand", "Orb", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
		}
		else {
			cout << "You don't have anything to unequip from that slot." << endl;
			Pause();
		}
	}
	void equip_item(Item item) {
		//failures
		if (item.get_type() == "Shield") {
			cout << "You cannot equip shields." << endl;
			Pause();
			return;
		}
		else if (item.get_type() == "Armor") {
			if (item.get_subtype() != "Cloth") {
				cout << "You cannot equip leather or mail armor." << endl;
				Pause();
				return;
			}
			//assuming no failures...
			if (equipment[0].get_level() > 0) {unequip_item(equipment[0]);}
			equipment[0] = item;
		}
		//assuming no failures...
		else if (item.get_type() == "Weapon") {
			if (equipment[1].get_level() > 0) {unequip_item(equipment[1]);}
			equipment[1] = item;
		}
		else if (item.get_type() == "Offhand") {
			if (equipment[2].get_level() > 0) {unequip_item(equipment[2]);}
			equipment[2] = item;
		}
		update_stats();
		dec_inventory(item);
		cout << "\nYou've equipped " << item.get_name() << "." << endl;
		Pause();
	}
	
	//VIEW FUNCTIONS
	void view_stats() {
		Clear();
		cout << "Your current stats are: " << endl;
		cout << "ATK: " << atk << endl;
		cout << "DEF: " << def << endl;
		cout << "Health: " << current_health << "/" << max_health << endl;
		cout << "Mana: " << current_mana << "/" << max_mana << endl;
		cout << "Spellpower: " << spellpower << endl;
		cout << "\nExperience: " << exp << "/" << exp_to_next_level << endl;
		
	}
	
	//MANA MANIPULATION
	void restore_mana(int x) {
		current_mana += x;
		if (current_mana > max_mana) {restore_mana_to_max();}
	}
	void restore_mana_to_max() {current_mana = max_mana;}
	void lose_mana(int x) {current_mana -= x;}
	
	//COMBAT
	void boost_def(int x) {
		def += x;
		boosted += 1;
	}
	void lower_def() {
		def -= spellbook[2].get_magnitude();
		boosted -= 1;
	}
	void cast_spell(Creature& creature) {
		cast_spell_start:
		Clear();
		cout << "What spell would you like to cast?" << endl;
		view_spells();
		cout << SPELL_COUNT + 1 << ". Cancel request (go to the previous menu)" << endl;
		
		int choice;
		while (true) {
			choice = GetInt();
			choice--;
			if (choice < SPELL_COUNT) {
				if (spellbook[choice].get_cost() > current_mana) {
					cout << "You don't have enough mana to cast this. YOU NEED MORE JUICE!!!" << endl;
					Pause();
					goto cast_spell_start;
				}
				else {
					cout << "You cast " << spellbook[choice].get_name() << "!" << endl;
					if (spellbook[choice].get_type() == "Offensive") {
						creature.take_spell_dmg(spellbook[choice].get_magnitude(), acc, crit);
					}
					else if (spellbook[choice].get_type() == "Healing") {
						restore_health(spellbook[choice].get_magnitude());
						cout << "You've restored " << spellbook[choice].get_magnitude() << " health!" << endl;
						Pause();
					}
					else if (spellbook[choice].get_type() == "Defensive") {
						boost_def(spellbook[choice].get_magnitude());
						cout << "You've boosted your DEF by " << spellbook[choice].get_magnitude() << endl;
						Pause();
					}
					lose_mana(spellbook[choice].get_cost());
					did_cast_spell = true;
					return;
				}
			}
			else if (choice == SPELL_COUNT) {
				did_cast_spell = false;
				return;
			}
		}
	}
	void cast_healing_spell() {
		cast_healing_spell_start:
		Clear();
		cout << "What spell would you like to cast?" << endl;
		view_spells();
		cout << SPELL_COUNT + 1 << ". Cancel request (go to the previous menu)" << endl;
		
		int choice;
		while (true) {
			choice = GetInt();
			choice--;
			if (choice < SPELL_COUNT) {
				if (spellbook[choice].get_type() == "Healing") {
					if (spellbook[choice].get_cost() > current_mana) {
						cout << "You don't have enough mana to cast this. YOU NEED MORE JUICE!!!" << endl;
						Pause();
						goto cast_healing_spell_start;
					}
					cout << "You cast " << spellbook[choice].get_name() << "!" << endl;
					restore_health(spellbook[choice].get_magnitude());
					cout << "You've restored " << spellbook[choice].get_magnitude() << " health!" << endl;
					lose_mana(spellbook[choice].get_cost());
					Pause();
					return;
				}
				else {
					cout << "You can't cast " << spellbook[choice].get_name() << " out of combat!" << endl;
					Pause();
					goto cast_healing_spell_start;
				}
			}
		}
	}

	void level_up() {
		if (exp >= exp_to_next_level && level < MAX_LEVEL) {
			Player::level_up();
			for (int i = 0; i < SPELL_COUNT; i++) {spellbook[i].inc_level();}
		}
	}
	
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
	void init_equipment() {
		equipment[0].set_item("Armor", "Cloth", 0);
		equipment[1].set_item("Weapon", "Staff", 0);
		equipment[2].set_item("Offhand", "Orb", 0);
	}
	
	void view_spells() {
		for (int i = 0; i < SPELL_COUNT; i++) {
			cout << i + 1 << ". " << spellbook[i].get_name() << " - " << spellbook[i].get_cost() << " mana" << endl;
		}
	}
};

//shop definition and subtypes
class Shop {
protected:
	//VARIABLES
	Item *inventory;
	string display_inventory_flavor, sell_flavor, entrance_flavor, leave_flavor, menu_flavor;
	int size_of_inventory, level;
	
	//shop inventory
	void display_inventory() {
		Clear();
		cout << display_inventory_flavor << endl << endl;
		for (int i = 0; i < size_of_inventory; i++) {
			cout << i + 1 << ". " << inventory[i].get_name() << " - "
			<< inventory[i].get_buy_value() << " gold" << endl;
		}
	}

	//buying menu
	void buy(Player* &player) {
		buy_start:
		display_inventory();
		cout << size_of_inventory + 1 << ". Exit menu\n" << endl;		
		player->view_gold();
		
		int choice;
		while (true) {
			choice = GetInt();
			choice--;
			if (choice < size_of_inventory) {
				if (player->get_gold() >= inventory[choice].get_buy_value()) {
					player->inc_inventory(inventory[choice]);
					player->remove_gold(inventory[choice].get_buy_value());
					cout << "You've purchased " << inventory[choice].get_name()
					<< " for " << inventory[choice].get_buy_value() << " gold." << endl;
				}
				else {cout << "You don't have enough gold." << endl;}
				Pause();
				goto buy_start;
			}
			else if (choice == size_of_inventory) {return;}
		}
	}

	//selling menu
	void sell(Player* &player) {
		sell_menu_start:
		Clear();
		if (player->get_items_in_inventory() == 0) {
			cout << "You don't have any items to sell." << endl;
			Pause();
			return;
		}
		else {
			Item *inv = player->get_inventory();
			cout << sell_flavor << endl << endl;
			for (int i = 0; i < player->get_items_in_inventory(); i++) {
				cout << i + 1 << ". " << inv[i].get_name() << " x" << inv[i].get_quantity()
				<< " - " << inv[i].get_sell_value() << " gold" << endl;
			}
			cout << player->get_items_in_inventory() + 1 << ". Exit menu\n" << endl;
			
			int choice;
			while (true) {
				choice = GetInt();
				choice--;
				if (choice <= player->get_items_in_inventory()) {
					if (choice < player->get_items_in_inventory()) {
						cout << "You would like to sell " << inv[choice].get_name()
						<< " for " << inv[choice].get_sell_value() << " gold. Is that "
						<< "acceptable (Y/N)?" << endl;
						char decision;
						while (true) {
							decision = GetChar();
							switch (decision) {
								case 'Y':
								case 'y':
									player->dec_inventory(inv[choice]);
									player->add_gold(inv[choice].get_sell_value());
									cout << "You have sold " << inv[choice].get_name() << " for "
									<< inv[choice].get_sell_value() << " gold.\n" << endl;
									Pause();
									goto sell_menu_start;
									break;
								case 'N':
								case 'n':
									goto sell_menu_start;
									break;
								default:
									cout << "Please enter Y or N: " << endl;\
							}
						}
					}
					else {return;}
				}
			}
			//purge pointer to prevent memory leak
			delete[] inv;
			inv = 0;
		}
	}
};
class Blacksmith: public Shop {
public:
	Blacksmith(int shop_level) {
		level = shop_level;
		init_inventory();
		display_inventory_flavor = "Blacksmith: 'Here's what I have to offer:'";
		sell_flavor = "Blacksmith: 'What would you like to sell to me?' ";
		entrance_flavor = "You enter the blacksmith. The proprietor notices you enter.";
		menu_flavor = "Blacksmith: 'Welcome to my shop! What would you like?'";
		leave_flavor = "You decide to leave the blacksmith's shop. ";
		leave_flavor += "The proprietor scowls at your retreating back.";
	}

	//top level menu
	void menu(Player* &player) {
		Clear();
		cout << entrance_flavor << endl;
		menu_start:
		cout << menu_flavor << endl;
	
		cout << "\n1. Buy / Browse goods" << endl;
		cout << "2. Sell" << endl;
		cout << "3. Repair" << endl;
		cout << "4. Leave blacksmith" << endl;
	
		char choice;
		while (true) {
			choice = GetChar();
			switch (choice) {
				case '1':
				case 'B':
				case 'b':
					buy(player);
					Clear();
					goto menu_start;
					break;
				case '2':
				case 'S':
				case 's':
					sell(player);
					Clear();
					goto menu_start;
					break;
				case '3':
				case 'R':
				case 'r':
					repair(player);
					Clear();
					goto menu_start;
					break;
				case '4':
				case 'L':
				case 'l':
					cout << leave_flavor << endl;
					Pause();
					return;
					break;
			}
		}
	}

private:
	//initializes inventory
	void init_inventory() {
		size_of_inventory = 8;
		inventory = new Item[size_of_inventory];
		inventory[0].set_item("Weapon", "Sword", level);
		inventory[1].set_item("Weapon", "Dagger", level);
		inventory[2].set_item("Weapon", "Staff", level);
		inventory[3].set_item("Armor", "Cloth", level);
		inventory[4].set_item("Armor", "Leather", level);
		inventory[5].set_item("Armor", "Mail", level);
		inventory[6].set_item("Shield", "none", level);
		inventory[7].set_item("Offhand", "Orb", level);
	}
	
	void repair(Player* &player) {
		repair_start:
		Clear();
		cout << "Which item would you like to repair?\n" << endl;
		player->view_equipment();
		cout << "4. Cancel" << endl;
		
		Item *equipment = player->get_equipment();
		char decision;
		int cost;
		int choice;
		while (true) {
			choice = GetInt();
			choice--;
			cost = (int)((equipment[choice].get_damage() / 100.0) * equipment[choice].get_buy_value());
			if (choice < 3) {
				cout << "It will cost " << cost << " to repair " << equipment[choice].get_name() << ". ";
				cout << "Is this acceptable (Y/N)? ";
				decision = GetChar();
				if (decision == 'Y' || decision == 'y') {
					player->repair(choice);
					player->remove_gold(cost);
					cout << equipment[choice].get_name() << " has been repaired." << endl;
					Pause();
					goto repair_start;
				}
				else if (decision == 'N' || decision == 'n') {goto repair_start;}
			}
			else if (choice == 3) {return;}
		}
	}
};
class Inn: public Shop {
public:
	Inn(int shop_level) {
		level = shop_level;
		init_inventory();
		init_room();
		display_inventory_flavor = "Innkeeper: 'Here's what I have to offer:'";
		sell_flavor = "Innkeeper: 'What would you like to do to - I mean, sell me?' ";
		entrance_flavor = "You enter the inn. A buxom maiden notices your entrance and sashays over.";
		menu_flavor = "Innkeeper: 'Welcome to my inn!'";
		leave_flavor = "You decide to leave, possibly put off by the innkeeper's come-hither stares. ";
		leave_flavor += "She pouts after you as you leave.";
	}
	
	//top level menu
	void menu(Player* &player) {
		Clear();
		cout << entrance_flavor << endl;
		menu_start:
		cout << menu_flavor << endl;
		cout << "What would you like to do?" << endl;
		
		cout << "\n1. Buy / Browse goods" << endl;
		cout << "2. Sell" << endl;
		cout << "3. Stay the night" << endl;
		cout << "4. Leave inn" << endl;
		
		char choice;
		while (true) {
			choice = GetChar();
			switch (choice) {
				case '1':
				case 'B':
				case 'b':
					buy(player);
					Clear();
					goto menu_start;
					break;
				case '2':
				case 'S':
				case 's':
					sell(player);
					Clear();
					goto menu_start;
					break;
				case '3':
					stay(player);
					Clear();
					goto menu_start;
					break;
				case '4':
				case 'L':
				case 'l':
					cout << leave_flavor << endl;
					Pause();
					return;
					break;
			}
		}
	}

private:
	//VARIABLES
	Item room;
	
	//initializes inventory
	void init_inventory() {
		size_of_inventory = 2;
		inventory = new Item[size_of_inventory];
		inventory[0].set_item("Potion", "Health", level);
		inventory[1].set_item("Potion", "Mana", level);
	}
	
	//establishes room for rent
	void init_room() {room.set_item("Room", "none", level);}
	
	//room menu
	void stay(Player* &player) {
		Clear();
		cout << "Innkeeper: 'Our " << room.get_name() << " is available to rent for " << room.get_buy_value()
		<< " gold. We also offer 'specials' for some of our more...distinguished guests.'\n" << endl;
		player->view_gold();
		cout << "\nWould you like to stay the night (Y/N)? ";
		
		char choice;
		while (true) {
			choice = GetChar();
			switch (choice) {
				case 'Y':
				case 'y':
					if (player->get_gold() >= room.get_buy_value()) {
						player->remove_gold(room.get_buy_value());
						player->restore_health_to_max();
						if (player->get_player_class() == "Mage") {player->restore_mana_to_max();}
						Clear();
						cout << "You've stayed in the " << room.get_name() << ", and your health and mana have "
						<< "recovered fully. Anything else, such as certain people sneaking into your room in the middle "
						<< "of the night, you'd rather not think about too much." << endl;
						Pause();
						return;
					}
					else {
						cout << "You don't have enough gold." << endl;
						Pause();
						return;
					}
					break;
				case 'N':
				case 'n':
					cout << "You decided not to stay the night. Perhaps it had something to do with how the innkeeper has been "
					<< "staring at you this entire time...." << endl;
					Pause();
					return;
					break;
				default:
					cout << "Please enter Y or N: ";
			}
		}
	}
};

class Map {
public:
	Map(int player_level) {
		level = player_level;
		size = 3 + level; //number of spaces in a row
		area = size * size;
		space = 'O';
		init_map();
		set_town();
		set_boss();
		explore_bonus_earned = false;
	}
	
	void init_map() {
		map = new char[area];
		for (int i = 0; i < area; i++) {
			map[i] = space;
		}
	}
	
	void draw_map() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				cout << map[i * size + j] << " ";
			}
			cout << endl;
		}
	}
	
	int move_player(int start, char direction) {
		int finish;
		if (direction == 'N') {finish = start - size;}
		if (direction == 'W') {finish = start - 1;}
		if (direction == 'E') {finish = start + 1;}
		if (direction == 'S') {finish = start + size;}
		
		if (map[start] != 'T') {map[start] = 'X';}
		map[finish] = '*';
		return finish;
	}
	
	void enter_town(int position) {
		if (position == town) {
			cout << "You stumble back to the entrance of town. You can't believe you managed to survive out there "
			<< "amidst nature's chaos!" << endl;
			Pause();
		}
	}
	
	bool explored_map() {
		for (int i = 0; i < area; i++) {
			if (map[i] == 'O') {return false;}
		}
		return true; //only is reached if none of the map coords are 'O'
	}
	
	//verifies that player can move in a particular direction
	bool can_move_north(int position) {
		if (position > size - 1) {return true;}
		else {return false;}
	}
	bool can_move_south(int position) {
		if (position < (area) - size) {return true;}
		else {return false;}
	}
	bool can_move_west(int position) {
		if (position % size > 0) {return true;}
		else {return false;}
	}
	bool can_move_east(int position) {
		if (position % size != size - 1) {return true;}
		else {return false;}
	}
	
	//GET FUNCTIONS
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
	char *map;
	char space;
	int level, size, town, area, boss;
	bool explore_bonus_earned, boss_present;
	
	//SET FUNCTIONS
	void set_town() {
		town = rand() % area;
		map[town] = 'T';
	}
	void set_boss() {
		boss = rand() % area;
		while (boss == town) {boss = rand() % area;}
		boss_present = true;
	}
};

//LOAD-SAVE FUNCTIONS
bool saves_exist() {
	ifstream file;
	file.open("save_games.txt");
	if (file.is_open()) {return true;}
	else {return false;}
}
void save_game(Player* &player) {
	string filename;
	fstream file;
	save_game_start:
	Clear();
	cout << "To create a save game, enter a name: ";
	getline(cin, filename);
	
	string filename_short = filename;
	switch (player->get_level()) {
		case 1:
			filename += ", Level 1";
			break;
		case 2:
			filename += ", Level 2";
			break;
		case 3:
			filename += ", Level 3";
			break;
		case 4:
			filename += ", Level 4";
			break;
		case 5:
			filename += ", Level 5";
			break;
	}
	
	//save name of game to a separate file
	file.open("save_games.txt", fstream::in);
	bool save_exists = false;
	char line[30];
	for (int i = 0; file.good(); i++) {
		file.getline(line, 30, '\n');
		if (line == filename) {save_exists = true;}
	}
	file.close();
	if (save_exists) {
		cout << "A game already exists with this name. Overwrite (Y/N)? ";
		char decision;
		while (true) {
			decision = GetChar();
			if (decision == 'Y' || decision == 'y') {break;}
			else if (decision == 'N' || decision == 'n') {goto save_game_start;}
		}
	}
	else {
		file.open("save_games.txt", fstream::out |  fstream::app);
		file << filename_short << ", Level " << player->get_level() << endl;
		file.close();
	}
	
	cout << "\nSaving game...." << endl;
	
	//attributes and stats
	file.open(filename_short + "_int.txt", fstream::out);
	file << player->get_base_str() << endl;
	file << player->get_base_agi() << endl;
	file << player->get_base_int() << endl;
	file << player->get_base_sta() << endl;
	file << player->get_exp() << endl;
	file << player->get_gold() << endl;
	file << player->get_max_health() << endl;
	file << player->get_max_mana() << endl;
	file << player->get_items_in_inventory() << endl;
	file.close();
	file.open(filename_short + "_char.txt", fstream::out);
	file << player->get_name().c_str() << endl;
	file << player->get_player_class().c_str() << endl;
	file.close();
	
	//quests - always MAX_LEVEL number
	const int MAX_LEVEL = player->get_max_level();
	bool *boss_quest_completion = player->get_boss_quests_complete();
	file.open(filename_short + "_int.txt", fstream::out | fstream::app);
	for (int i = 0; i < MAX_LEVEL; i++) {
		if (boss_quest_completion[i]) {file << 1 << endl;}
		else {file << 0 << endl;}
	}
	file.close();
	
	//inventory - only if there is any
	if (player->has_inventory()) {
		Item *item = player->get_inventory();
		for (int i = 0; i < player->get_items_in_inventory(); i++) {
			file.open(filename_short + "_char.txt", fstream::out | fstream::app);
			file << item[i].get_type() << endl;
			file << item[i].get_subtype() << endl;
			file.close();
			file.open(filename_short + "_int.txt", fstream::out | fstream::app);
			file << item[i].get_level() << endl;
			file << item[i].get_quantity() << endl;
			file.close();
		}
	}
	
	//equipment
	const int EQUIPMENT_SLOTS = 3;
	Item *equipment = new Item[EQUIPMENT_SLOTS];
	equipment = player->get_equipment();
	for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
		file.open(filename_short + "_char.txt", fstream::out | fstream::app);
		file << equipment[i].get_type() << endl;
		file << equipment[i].get_subtype() << endl;
		file.close();
		file.open(filename_short + "_int.txt", fstream::out | fstream::app);
		file << equipment[i].get_level() << endl;
		file.close();
	}
	
	cout << filename << " has been saved." << endl;
	Pause();
}
void load_game(Player* &player) {
	string filename;
	ifstream file;
	Clear();
	cout << "Please choose a game to load:" << endl;
	
	//display list of saved games
	string line;
	string saves[3];
	string saves_short[3];
	file.open("save_games.txt");
	for (int i = 0; file.good(); i++) {
		getline(file, line);
		if (line != "") {
			cout << i + 1 << ". " << line << endl;
			saves[i] = line;
		}
	}
	file.close();
	file.open("save_games.txt");
	for (int i = 0; file.good(); i++) {
		string junk;
		getline(file, line, ',');
		getline(file, junk);
		if (line != "") {saves_short[i] = line;}
	}
	file.close();
	
	int choice = GetInt();
	choice--;
	filename = saves_short[choice];
	cout << "\nLoading " << filename << "...." << endl;
	
	//attributes and stats
	file.open(filename + "_char.txt");
	char c[20];
	for (int i = 0; file.good(); i++) {
		file.getline(c, 20, '\n');
		if (i == 0) {player->load_name(c);}
		else if (i == 1) {player->load_player_class(c);}
	}
	file.close();
	
	if (player->get_player_class() == "Warrior") {player = new Warrior(player->get_name());}
	else if (player->get_player_class() == "Rogue") {player = new Rogue(player->get_name());}
	else if (player->get_player_class() == "Mage") {player = new Mage(player->get_name());}
	else {cout << "Oops! (load_game)" << endl;}
	
	file.open(filename + "_int.txt");
	int x;
	for (int i = 0; file.good(); i++) {
		file >> x;
		if (i == 0) {player->load_base_str(x);}
		else if (i == 1) {player->load_base_agi(x);}
		else if (i == 2) {player->load_base_int(x);}
		else if (i == 3) {player->load_base_sta(x);}
		else if (i == 4) {player->load_exp(x);}
		else if (i == 5) {player->load_gold(x);}
		else if (i == 6) {player->load_max_health(x);}
		else if (i == 7) {player->load_max_mana(x);}
		else if (i == 8) {player->load_items_in_inventory(x);}
	}
	file.close();
	
	//quests - always MAX_LEVEL number
	const int MAX_LEVEL = player->get_max_level();
	bool *boss_quest_completion = player->get_boss_quests_complete();
	file.open(filename + "_int.txt");
	for (int i = 0; file.good(); i++) {
		file >> x;
		for (int j = 0; j < MAX_LEVEL; j++) {
			if (i == 9 + j) {
				if (x == 1) {boss_quest_completion[j] = true;}
				else {boss_quest_completion[j] = false;}
			}
		}
	}
	file.close();
	
	//inventory
	int int_length, char_length; //number of inventory lines
	if (player->has_inventory()) {
		char_length = player->get_items_in_inventory() * 2;
		int_length = player->get_items_in_inventory() * 2;
		Item *inv;
		inv = new Item[player->get_items_in_inventory()];
		
		int j = 0; //index of inventory
		char c_inv[30];
		for (j = 0; j < player->get_items_in_inventory(); j++) {
			file.open(filename + "_char.txt");
			for (int i = 0; file.good(); i++) {
				file.getline(c_inv, 30, '\n');
				if ((i >= 2) && (i < 2 + char_length)) { //ensures it stays within inventory bounds
					if ((i - 2) % char_length == 2 * j) {inv[j].load_type(c_inv);}
					if ((i - 2) % char_length == (2 * j) + 1) {inv[j].load_subtype(c_inv);}
				}
			}
			file.close();
		}
		
		int x_inv;
		for (j = 0; j < player->get_items_in_inventory(); j++) {
			file.open(filename + "_int.txt");
			for (int i = 0; file.good(); i++) {
				file >> x_inv;
				if ((i >= (9 + MAX_LEVEL)) && (i < (9 + MAX_LEVEL) + int_length)) { //ensures it stays within inventory bounds
					if ((i - (9 + MAX_LEVEL)) % int_length == 2 * j) {inv[j].load_level(x_inv);}
					if ((i - (9 + MAX_LEVEL)) % int_length == (2 * j) + 1) {inv[j].load_quantity(x_inv);}
				}
			}
			file.close();
		}

		for (int i = 0; i < player->get_items_in_inventory(); i++) {
			int num = inv[i].get_quantity(); //backup quantity
			inv[i].set_item(inv[i].get_type(), inv[i].get_subtype(), inv[i].get_level()); //init_item
			inv[i].load_quantity(num); //restore original quantity
		}
		
		player->load_inventory(inv);
		delete[] inv;
		inv = 0;
	}
	//if no inventory
	else {
		char_length = 0;
		int_length = 0;
	}
	
	//equipment
	const int EQUIPMENT_SLOTS = 3;
	Item *equipment = new Item[EQUIPMENT_SLOTS];
	
	//copy in equipment details
	char c_equip[30];
	int j = 0; //index of equipment
	for (j = 0; j < EQUIPMENT_SLOTS; j++) {
		file.open(filename + "_char.txt");
		for (int i = 0; file.good(); i++) {
			file.getline(c_equip, 30, '\n');
			if (i >= 2 + char_length) {
				if ((i - 2 - char_length) % 7 == 2 * j) {equipment[j].load_type(c_equip);}
				if ((i - 2 - char_length) % 7 == (2 * j) + 1) {equipment[j].load_subtype(c_equip);}
			}
		}
		file.close();
	}
	int x_equip;
	for (j = 0; j < EQUIPMENT_SLOTS; j++) {
		file.open(filename + "_int.txt");
		for (int i = 0; file.good(); i++) {
			file >> x_equip;
			if (i >= (9 + MAX_LEVEL) + int_length) {
				if ((i - (9 + MAX_LEVEL) - int_length) % 4 == j) {equipment[j].load_level(x_equip);}
			}
		}
		file.close();
	}
	
	//initialize proper equipment
	player->load_equipment(equipment);
	
	player->load_level(player->get_exp());
	player->update_stats(); //also handles equipment bonuses
	player->restore_health_to_max();
	if (player->get_player_class() == "Mage") {
		player->restore_mana_to_max();
		player->init_spellbook();
		player->update_stats();
	}
	player->set_quests();
	cout << "Your game has been loaded." << endl;
	Pause();
}
void load_or_create_player_menu(Player* &player) {
	if (saves_exist()) {
		cout << "What would you like to do?" << endl;
		cout << "1. Load player" << endl;
		cout << "2. New player" << endl;
		char choice;
		while (true) {
			choice = GetChar();
			switch (choice) {
				case '1':
				case 'L':
				case 'l':
					load_game(player);
					return;
					break;
				case '2':
				case 'N':
				case 'n':
					player->set_player();
					if (player->get_player_class() == "Warrior") {
						player = new Warrior(player->get_name());
						player->set_player();
					}
					else if (player->get_player_class() == "Rogue") {
						player = new Rogue(player->get_name());
						player->set_player();
					}
					else if (player->get_player_class() == "Mage") {
						player = new Mage(player->get_name());
						player->set_player();
					}
					else {cout << "Oops! (load_or_create_player_menu)" << endl;}
					return;
					break;
			}
		}
	}
	else {
		player->set_player();
		if (player->get_player_class() == "Warrior") {
			player = new Warrior(player->get_name());
			player->set_player();
		}
		else if (player->get_player_class() == "Rogue") {
			player = new Rogue(player->get_name());
			player->set_player();
		}
		else if (player->get_player_class() == "Mage") {
			player = new Mage(player->get_name());
			player->set_player();
		}
		else {cout << "Oops! (load_or_create_player_menu)" << endl;}
	}
	player->set_quests();
}
void save_or_load(Player* &player) {
	save_or_load_start:
	Clear();
	cout << "What would you like to do?" << endl;
	cout << "1. Save player" << endl;
	cout << "2. Load player" << endl;
	cout << "3. Exit menu" << endl;
	
	char choice;
	while (true) {
		choice = GetChar();
		switch (choice) {
			case '1':
			case 'S':
			case 's':
				save_game(player);
				goto save_or_load_start;
				break;
			case '2':
			case 'L':
			case 'l':
				load_game(player);
				goto save_or_load_start;
				break;
			case '3':
			case 'X':
			case 'x':
			case 'E':
			case 'e':
				return;
				break;
		}
	}
}

//MENUS + helper functions
void player_menu(Player* &player) {
	player_menu_start:
	Clear();

	cout << "Player menu:\n" << endl;
	cout << "What would you like to do? " << endl;
	cout << "1.  View attributes" << endl;
	cout << "2.  View stats" << endl;
	cout << "3.  View inventory" << endl;
	cout << "4.  View quest log" << endl;
	cout << "5.  Use item" << endl;
	if (player->get_player_class() == "Mage") {
		cout << "6.  Cast healing spell" << endl;
		cout << "7.  Equipment" << endl;
		cout << "8.  Load / Save Player" << endl;
		cout << "9.  Exit menu" << endl;
		cout << "10. Quit game" << endl;
	}
	else {
		cout << "6.  Equipment" << endl;
		cout << "7.  Load / Save Player" << endl;
		cout << "8.  Exit menu" << endl;
		cout << "9.  Quit game" << endl;
	}
	
	int choice;
	if (player->get_player_class() == "Mage") {
		while (true) {
			choice = GetInt();
			switch (choice) {
				case 1:
					player->view_attributes();
					Pause();
					goto player_menu_start;
					break;
				case 2:
					player->view_stats();
					Pause();
					goto player_menu_start;
					break;
				case 3:
					player->view_inventory();
					Pause();
					goto player_menu_start;
					break;
				case 4:
					player->view_quest_log();
					Pause();
					goto player_menu_start;
					break;
				case 5:
					player->use_item();
					goto player_menu_start;
					break;
				case 6:
					if (player->get_player_class() == "Mage") {player->cast_healing_spell();}
					goto player_menu_start;
					break;
				case 7:
					player->equipment_menu();
					goto player_menu_start;
					break;
				case 8:
					save_or_load(player);
					goto player_menu_start;
					break;
				case 9:
					return;
					break;
				case 10:
					Clear();
					cout << "Are you sure you sure you want to quit? Don't be that guy, man. "
					<< "Are you really that much of a pathetic, lowlife loser (Y/N)? ";
					char decision;
					while (true) {
						decision = GetChar();
						if (decision == 'Y' || decision == 'y') {exit(0);}
						else if (decision == 'N' || decision == 'n') {
							cout << "That's the spirit, champ!" << endl;
							Pause();
							goto player_menu_start;
						}
					}
					break;
			}
		}
	}
	else {
		while (true) {
			choice = GetInt();
			switch (choice) {
				case 1:
					player->view_attributes();
					Pause();
					goto player_menu_start;
					break;
				case 2:
					player->view_stats();
					Pause();
					goto player_menu_start;
					break;
				case 3:
					player->view_inventory();
					Pause();
					goto player_menu_start;
					break;
				case 4:
					player->view_quest_log();
					Pause();
					goto player_menu_start;
					break;
				case 5:
					player->use_item();
					goto player_menu_start;
					break;
				case 6:
					player->equipment_menu();
					goto player_menu_start;
					break;
				case 7:
					save_or_load(player);
					goto player_menu_start;
					break;
				case 8:
					return;
					break;
				case 9:
					Clear();
					cout << "Are you sure you sure you want to quit? Don't be that guy, man. "
					<< "Are you really that much of a pathetic, lowlife loser (Y/N)? ";
					char decision;
					while (true) {
						decision = GetChar();
						if (decision == 'Y' || decision == 'y') {exit(0);}
						else if (decision == 'N' || decision == 'n') {
							cout << "That's the spirit, champ!" << endl;
							Pause();
							goto player_menu_start;
						}
					}
					break;
			}
		}
	}
}
void fight_creature(Player* &player) {
	Creature creature;
	creature.det_creature(player->get_level());
	
	cout << "You've encountered a " << creature.get_name() << "!\n" << endl;
	Pause();
	
	fight_creature_start:
	Clear();
	cout << "What would you like to do?" << endl;
	cout << "1. Attack" << endl;
	if (player->get_player_class() == "Mage") {
		cout << "2. Cast a spell" << endl;
		cout << "3. Use item" << endl;
		cout << "4. Run away\n" << endl;
	}
	else {
		cout << "2. Use item" << endl;
		cout << "3. Run away\n" << endl;
	}
	cout << "Your health: " << player->get_current_health() << "/" << player->get_max_health() << endl;
	if (player->get_player_class() == "Mage") {
		cout << "Your mana: " << player->get_current_mana() << "/" << player->get_max_mana() << endl << endl;
	}
	cout << creature.get_name() << "'s health: " << creature.get_health() << endl;
	
	char choice;
	if (player->get_player_class() == "Mage") {
		while (true) {
			choice = GetChar();
			switch (choice) {
				case '1':
				case 'A':
				case 'a':
					creature.take_damage(player->get_atk(), player->get_accuracy(), player->get_crit(), player->get_name());
					if (creature.is_dead()) {
						while (player->get_boosted() > 0) {
							player->lower_def();
						}
						cout << "You've killed the " << creature.get_name() << "!" << endl;
						player->gain_exp(creature.get_exp());
						player->add_gold(creature.get_money());
						if (creature.get_has_loot()) {player->inc_inventory(creature.get_loot());}
						Pause();
						return;
					}
					player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
					goto fight_creature_start;
					break;
				case '2':
				case 'C':
				case 'c':
				case 'S':
				case 's':
					player->cast_spell(creature);
					if (player->get_did_cast_spell()) {
						if (creature.is_dead()) {
							while (player->get_boosted() > 0) {
								player->lower_def();
							}
							cout << "You've killed the " << creature.get_name() << "!" << endl;
							player->gain_exp(creature.get_exp());
							player->add_gold(creature.get_money());
							if (creature.get_has_loot()) {player->inc_inventory(creature.get_loot());}
							Pause();
							return;
						}
						player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
					}
					goto fight_creature_start;
					break;
				case '3':
				case 'U':
				case 'u':
					if (player->use_item()) {
						player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
					}
					goto fight_creature_start;
					break;
				case '4':
				case 'R':
				case 'r':
					if (rand() % 101 < creature.get_escape_chance()) {
						cout << "You flee like a coward! Were you that scared of a " << creature.get_name() << "?" << endl;
						Pause();
						return;
					}
					else {
						cout << "You tried to run, but the " << creature.get_name() << " bit your ankle and you fell over. LAME." << endl;
						Pause();
						player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
						goto fight_creature_start;
					}
					break;
			}
		}
	}
	else {
		while (true) {
			choice = GetChar();
			switch (choice) {
				case '1':
				case 'A':
				case 'a':
					creature.take_damage(player->get_atk(), player->get_accuracy(), player->get_crit(), player->get_name());
					if (creature.is_dead()) {
						cout << "You've killed the " << creature.get_name() << "!" << endl;
						player->gain_exp(creature.get_exp());
						player->add_gold(creature.get_money());
						if (creature.get_has_loot()) {player->inc_inventory(creature.get_loot());}
						Pause();
						return;
					}
					player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
					goto fight_creature_start;
					break;
				case '2':
				case 'U':
				case 'u':
					if (player->use_item()) {
						player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
					}
					goto fight_creature_start;
					break;
				case '3':
				case 'R':
				case 'r':
					if (rand() % 101 < creature.get_escape_chance()) {
						cout << "You flee like a coward! Were you that scared of a " << creature.get_name() << "?" << endl;
						Pause();
						return;
					}
					else {
						cout << "You tried to run, but the " << creature.get_name() << " bit your ankle and you fell over. LAME." << endl;
						Pause();
						player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
						goto fight_creature_start;
					}
					break;
			}
		}
	}
}
void fight_boss(Player* &player, Map& map) {
	Boss boss(map.get_level());
	cout << "You've encountered " << boss.get_name() << "!\n" << endl;
	Pause();
	
	fight_boss_start:
	Clear();
	cout << "What would you like to do?" << endl;
	cout << "1. Attack" << endl;
	if (player->get_player_class() == "Mage") {
		cout << "2. Cast a spell" << endl;
		cout << "3. Use item" << endl;
		cout << "4. Run away\n" << endl;
	}
	else {
		cout << "2. Use item" << endl;
		cout << "3. Run away\n" << endl;
	}
	cout << "Your health: " << player->get_current_health() << "/" << player->get_max_health() << endl;
	if (player->get_player_class() == "Mage") {
		cout << "Your mana: " << player->get_current_mana() << "/" << player->get_max_mana() << endl << endl;
	}
	cout << boss.get_name() << "'s health: " << boss.get_health() << endl;
	
	char choice;
	if (player->get_player_class() == "Mage") {
		while (true) {
			choice = GetChar();
			switch (choice) {
				case '1':
				case 'A':
				case 'a':
					boss.take_damage(player->get_atk(), player->get_accuracy(), player->get_crit(), player->get_name());
					if (boss.is_dead()) {
						cout << "You've killed " << boss.get_name() << "!" << endl;
						player->gain_exp(boss.get_exp());
						player->add_gold(boss.get_money());
						player->inc_inventory(boss.get_loot());
						map.killed_boss();
						player->complete_active_quest();
						Pause();
						return;
					}
					cout << boss.get_flavor() << endl;
					player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
					goto fight_boss_start;
					break;
				case '2':
				case 'C':
				case 'c':
				case 'S':
				case 's':
					player->cast_spell(boss);
					if (player->get_did_cast_spell()) {
						if (boss.is_dead()) {
							cout << "You've killed " << boss.get_name() << "!" << endl;
							player->gain_exp(boss.get_exp());
							player->add_gold(boss.get_money());
							player->inc_inventory(boss.get_loot());
							map.killed_boss();
							player->complete_active_quest();
							Pause();
							return;
						}
						cout << boss.get_flavor() << endl;
						player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
					}
					goto fight_boss_start;
					break;
				case '3':
				case 'U':
				case 'u':
					if (player->use_item()) {
						cout << boss.get_flavor() << endl;
						player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
					}
					goto fight_boss_start;
					break;
				case '4':
				case 'R':
				case 'r':
					if (rand() % 101 < boss.get_escape_chance()) {
						cout << "You flee like a coward! Were you that scared of " << boss.get_name() << "?" << endl;
						Pause();
						return;
					}
					else {
						cout << "You tried to run, but " << boss.get_name() << " bit your ankle and you fell over. LAME." << endl;
						Pause();
						player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
						goto fight_boss_start;
					}
					break;
			}
		}
	}
	else {
		while (true) {
			choice = GetChar();
			switch (choice) {
				case '1':
				case 'A':
				case 'a':
					boss.take_damage(player->get_atk(), player->get_accuracy(), player->get_crit(), player->get_name());
					if (boss.is_dead()) {
						cout << "You've killed " << boss.get_name() << "!" << endl;
						player->gain_exp(boss.get_exp());
						player->add_gold(boss.get_money());
						player->inc_inventory(boss.get_loot());
						map.killed_boss();
						player->complete_active_quest();
						Pause();
						return;
					}
					cout << boss.get_flavor() << endl;
					player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
					goto fight_boss_start;
					break;
				case '2':
				case 'U':
				case 'u':
					if (player->use_item()) {
						cout << boss.get_flavor() << endl;
						player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
					}
					goto fight_boss_start;
					break;
				case '3':
				case 'R':
				case 'r':
					if (rand() % 101 < boss.get_escape_chance()) {
						cout << "You flee like a coward! Were you that scared of " << boss.get_name() << "?" << endl;
						Pause();
						return;
					}
					else {
						cout << "You tried to run, but " << boss.get_name() << " bit your ankle and you fell over. LAME." << endl;
						Pause();
						player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
						goto fight_boss_start;
					}
					break;
			}
		}
	}
}
void move_results(Player* &player, Map& map, int position, string flavor) {
	int creature_chance = 35;
	if (position == map.get_boss() && map.get_boss_present()) {
		fight_boss(player, map);
		player->update_stats();
	}
	else if (rand() % 101 < creature_chance) {
		fight_creature(player);
		player->update_stats();
	}
	else {
		cout << flavor << endl;
		Pause();
	}
	if (map.explored_map() && map.get_explore_bonus_earned() == false) {
		player->earn_explore_bonus(map.get_area());
		map.set_explore_bonus_earned();
	}
}
void wild_menu(Player* &player) {
	Map map(player->get_boss_quest().get_level()); //initialize the map based on level of quest boss
	int position = map.get_town();
	string north_flavor = "You move north; so far, the coast seems clear. Who's really scared of bunnies, anyway?";
	string west_flavor = "You move west; so far, nothing has eaten you. Flowers can't eat you, right?";
	string east_flavor = "You move east; so far, everything is fine. Grass just makes you itch, after all.";
	string south_flavor = "You move south; so far, all you see are bees buzzing around the flowers. Oh man, BEES!?";
	
	wild_menu_start:
	
	Clear();
	cout << "You have entered the wild! It's a large and scary meadow, "
	<< "filled with gross things and cute creatures.\n" << endl;
	Pause();
	
	wild_menu_after_move:
	//handles death situations (returns to town)
	if (player->is_dead()) {
		player->restore_health_to_max();
		if (player->get_player_class() == "Mage") {player->restore_mana_to_max();}
		return;
	}
	Clear();
	cout << "Map:" << endl;
	map.draw_map();
	
	cout << "What would you like to do?" << endl;
	cout << "(N) Move north" << endl;
	cout << "(W) Move west" << endl;
	cout << "(E) Move east" << endl;
	cout << "(S) Move south" << endl;
	cout << "1. Return to town" << endl;
	cout << "2. Player menu" << endl;
	
	char choice;
	while (true) {
		choice = GetChar();
		switch (choice) {
			case 'N':
			case 'n':
				if (map.can_move_north(position)) {
					position = map.move_player(position, 'N');
					if (position == map.get_town()) {
						map.enter_town(position);
						return;
					}
					move_results(player, map, position, north_flavor);
					goto wild_menu_after_move;
				}
				else {cout << "You can't move north. Please try another direction: ";}
				break;
			case 'W':
			case 'w':
				if (map.can_move_west(position)) {
					position = map.move_player(position, 'W');
					if (position == map.get_town()) {
						map.enter_town(position);
						return;
					}
					move_results(player, map, position, west_flavor);
					goto wild_menu_after_move;
				}
				else {cout << "You can't move west. Please try another direction: ";}
				break;
			case 'E':
			case 'e':
				if (map.can_move_east(position)) {
					position = map.move_player(position, 'E');
					if (position == map.get_town()) {
						map.enter_town(position);
						return;
					}
					move_results(player, map, position, east_flavor);
					goto wild_menu_after_move;
				}
				else {cout << "You can't move east. Please try another direction: ";}
				break;
			case 'S':
			case 's':
				if (map.can_move_south(position)) {
					position = map.move_player(position, 'S');
					if (position == map.get_town()) {
						map.enter_town(position);
						return;
					}
					move_results(player, map, position, south_flavor);
					goto wild_menu_after_move;
				}
				else {cout << "You can't move south. Please try another direction: ";}
				break;
			case '1':
				return;
				break;
			case '2':
				player_menu(player);
				goto wild_menu_start;
				break;
		}
	}
}
void town_menu(Player* &player) {
	town_menu_start:
	Clear();
	Blacksmith town_blacksmith(player->get_level());
	Inn town_inn(player->get_level());
	
	cout << "You have entered town.\n" << endl;
	
	if (player->get_quest_active() && player->quest_complete()) {
		player->turn_in_quest();
		Pause();
		Clear();
		cout << "Now that that's done... ";
	}
	
	cout << "What would you like to do?" << endl;
	cout << "1. Go to the blacksmith" << endl;
	cout << "2. Go to the inn" << endl;
	cout << "3. Leave the town (enter the wild)" << endl;
	cout << "4. Player menu" << endl;
	
	char choice;
	while (true) {
		choice = GetChar();
		switch (choice) {
			case '1':
				town_blacksmith.menu(player);
				goto town_menu_start;
				break;
			case '2':
				town_inn.menu(player);
				goto town_menu_start;
				break;
			case '3':
				player->quest_calculation_and_acceptance();
				wild_menu(player);
				goto town_menu_start;
				break;
			case '4':
				player_menu(player);
				goto town_menu_start;
				break;
			default:
				cout << "Invalid input ";
		}
	}
}

int main() {
	srand((unsigned int)time(0)); //just set once to ensure random generation
	
	cout << "Welcome to RPG Land!\n" << endl;
	Player *player = new Player;
	load_or_create_player_menu(player);
	
	town_menu(player);
	
	return 0;
}