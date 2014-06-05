#include<iostream>
#include<string>
#include<cmath> /* used for determining damage in creature combat */
#include<fstream> /* used for saving and loading player data */
#include<time.h> /* used for various RNG events, like creature encounters and town placement */
using namespace std;

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
	
	bool equipable() {
		if (type == "Armor" || type == "Weapon" || type == "Shield") {return true;}
		else {return false;}
	}
	
	//returns value of bonus given by an item
	int item_stat_value() {
		if (subtype == "Sword") {return level_multiplier(5);}
		else if (subtype == "Dagger") {return level_multiplier(3);}
		else if (subtype == "Staff") {return level_multiplier(2);}
		else if (subtype == "Cloth") {return level_multiplier(3);}
		else if (subtype == "Leather") {return level_multiplier(5);}
		else if (subtype == "Mail") {return level_multiplier(8);}
		else if (type == "Shield") {return level_multiplier(15);}
		else {return 0;}
	}
	
private:
	string name, type, subtype;
	int sell_value, buy_value, level, quantity;
	
	void init_item() {
		//known: type, subtype, level
		//assumed quantity of 1
		
		//known types: "Armor", "Weapon", "Shield", "Potion", "Room", "Trash"
		//known subtypes: "Mail", "Leather", "Cloth", "Sword", "Dagger", "Staff", "Health",
		//"Mana", "none", "broken tooth", "hairball", "string", "tail fluff", "lint"
		//known levels: 1, 2, 3, 4, 5
		//conditions to init name and buy/sell value based on knowns
		
		//name assembly
		if (type == "Armor") {
			if (level != 0) { /* validates as a "real" item */
				string s; /*used to assemble the name*/
			
				//assemble prefix
				if (level == 1) {s = "Crappy ";}
				else if (level == 2) {s = "Cheap ";}
				else if (level == 3) {s = "Basic ";}
				else if (level == 4) {s = "Expensive ";}
				else if (level == 5) {s = "Luxurious ";}
				else {cout << "Not a valid level" << endl;}
			
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
			if (level != 0) { /* validates as a "real" item */
				string s; /*used to assemble the name*/
			
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
					else {cout << "Not a valid level" << endl;}
					s += subtype;
				}
				else if (subtype == "Staff") {
					//assemble prefix based on subtype
					if (level == 1) {s = "Foam ";}
					else if (level == 2) {s = "Rubber ";}
					else if (level == 3) {s = "Wooden ";}
					else if (level == 4) {s = "Metal ";}
					else if (level == 5) {s = "Mage's ";}
					else {cout << "Not a valid level" << endl;}
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
			if (level != 0) { /* validates as a "real" item */
				string s; /*used to assemble the name*/
				
				//assemble prefix
				if (level == 1) {s = "Plastic ";}
				else if (level == 2) {s = "Wooden ";}
				else if (level == 3) {s = "Iron ";}
				else if (level == 4) {s = "Steel ";}
				else if (level == 5) {s = "Jade ";}
				else {cout << "Not a valid level" << endl;}
			
				//add type
				s += type;
			
				//this is the name
				name = s;
			}
			//for unequipped slots
			else {name = "empty";}
		}
		else if (type == "Potion") {
			string s; /*used to assemble the name*/
			
			//assemble prefix
			if (level == 1) {s = "Crappy ";}
			else if (level == 2) {s = "Cheap ";}
			else if (level == 3) {s = "Basic ";}
			else if (level == 4) {s = "Expensive ";}
			else if (level == 5) {s = "Luxurious ";}
			else {cout << "Not a valid level" << endl;}
			
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
			string s; /*used to assemble the name*/
			
			//assemble prefix
			if (level == 1) {s = "Crappy ";}
			else if (level == 2) {s = "Cheap ";}
			else if (level == 3) {s = "Basic ";}
			else if (level == 4) {s = "Expensive ";}
			else if (level == 5) {s = "Luxurious ";}
			else {cout << "Not a valid level" << endl;}
			
			//add type
			s += type;
			
			//this is the name
			name = s;
		}
		else if (type == "Trash") {
			string s; /*used to assemble the name*/
			
			//assemble prefix
			if (level == 1) {s = "Slimy ";}
			else if (level == 2) {s = "Pungent ";}
			else if (level == 3) {s = "Greasy ";}
			else if (level == 4) {s = "Rotten ";}
			else if (level == 5) {s = "Putrid ";}
			else {cout << "Not a valid level" << endl;}
			
			//add subtype
			s += subtype;
			
			//this is the name
			name = s;
		}
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
			buy_value = 50 * level;
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
		else {cout << "Not a valid item type (buy value)" << endl;}
		
		//sell value is always half of the buy value...
		sell_value = buy_value / 2;
	}
	
	int level_multiplier(int x) {return pow(x * 2, level);}
};

class Spell {
public:
	Spell() {}
	
	void set_spell(string spell_name, string spell_type, int mag, int player_level) {
		name = spell_name;
		type = spell_type;
		level = player_level;
		magnitude = mag;
		cost = magnitude * 2 * level;
	}
	
	void inc_level() {level++;}
	
	//GET FUNCTIONS
	int get_magnitude() {return magnitude;}
	int get_cost() {return cost;}
	string get_name() {return name;}
	string get_type() {return type;}
	
	//LOAD FUNCTIONS
	void load_magnitude(int x) {magnitude = x;}
	
private:
	string name, type;
	int magnitude, cost, level;
};

class Creature {
public:
	Creature () {}
	Creature(string cr_type, int cr_level) {
		level = cr_level;
		type = cr_type;
		stat_pool = 60 * level;
		set_type();
		money = exp * 2;
		has_loot = generate_loot();
	}
	
	//GET FUNCTIONS
	string get_name() {return name;}
	int get_exp() {return exp;}
	int get_atk() {return atk;}
	int get_def() {return def;}
	int get_health() {return health;}
	int get_accuracy() {return creature_acc;}
	int get_crit() {return crit;}
	int get_money() {return money;}
	Item get_loot() {
		cout << "You've looted " << loot.get_name()
		<< " from the " << name << "!" << endl;
		return loot;
	}
	bool get_has_loot() {return has_loot;}
	
	void take_damage(int attack, int acc, int critical) {
		if (rand() % 101 < dodge) {
			cout << name << " dodged your attack!" << endl;
			return;
		}
		if (rand() % 101 > acc) {
			cout << "Your attack missed!" << endl;
			return;
		}
		else {
			int x;
			if (attack < def && attack > 0) {x = 1;}
			else if (attack == 0) {x = 0;}
			else {
				//establish base damage
				double dmg = 10 * (pow(1.25, attack / (def * 1.0)) - 1) * level;
				//check for critical strikes
				if (rand() % 101 < critical) {
					cout << "You landed a critical hit!" << endl;
					dmg *= 2;
				}
				x = dmg;
			}
			
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
		if (rand() % 101 < parry) {
			cout << name << " parried your attack!" << endl;
			return;
		}
		if (rand() % 101 > acc) {
			cout << "Your attack missed!" << endl;
			return;
		}
		else if (rand() % 101 <= acc) {
			//establish base damage
			int dmg = magnitude;
			//check for critical hits
			if (rand() % 101 < critical) {
				cout << "Your spell landed a critical hit!" << endl;
				dmg *= 2;
			}
			cout << name << " takes " << dmg << " damage!" << endl;
			health -= magnitude;
		}
	}
	
	bool is_dead() {
		if (health <= 0) {return true;}
		else {return false;}
	}
	
private:
	string name, type;
	int exp, atk, def, health, stat_pool, level, creature_acc, dodge, parry, crit, money;
	Item loot;
	bool has_loot;

	bool generate_loot() {
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
	
	void set_type() {
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
			atk = stat_pool * 2 / 3;
			def = stat_pool / 3;
			health = stat_pool / 3;
			creature_acc = 95;
			dodge = 2;
			parry = 2;
			crit = 10;
		}
		else if (type == "Defensive") {
			s += "midget";
			name = s;
			exp = 15 * level;
			atk = stat_pool / 3;
			def = stat_pool * 2 / 3;
			health = stat_pool / 1;
			health *= 2;
			creature_acc = 85;
			dodge = 10;
			parry = 10;
			crit = 2;
		}
		else if (type == "Balanced") {
			s += "bunny";
			name = s;
			exp = 5 * level;
			atk = stat_pool / 2;
			def = stat_pool / 2;
			health = stat_pool / 2;
			creature_acc = 90;
			dodge = 5;
			parry = 5;
			crit = 5;
		}
		else {cout << "Not a valid type (Creature -> set_type)" << endl;}
	}
};

class Player {
public:
	Player() {}
	
	//initialize player for the first time
	void set_player() {
		init_attribs();
		set_name();
		set_class();
		init_stats();
		allocate_points();
		current_health = max_health;
		current_mana = max_mana;
		items_in_inventory = 0;
		equipment = new Item[3];
		equipment[0].set_item("Armor", "Cloth", 0);
		equipment[1].set_item("Weapon", "Staff", 0);
		equipment[2].set_item("Shield", "none", 0);
		if (player_class == "Mage") {init_spellbook();}
		level_up();
	}
	
	//checks if player has an inventory initialized
	bool has_inventory() {
		if (items_in_inventory > 0) {return true;}
		else {return false;}
	}
	
	//determine player's name
	void set_name() {
		cout << "What is your name? ";
		char c[30];
		cin.getline(c, 30);
		name = c;
		cout << "Hi, " << name << "!" << endl;
		system("PAUSE");
		system("CLS");
	}
	
	//determines the class the player would like to play as
	void set_class() {
		//switch "choice", confirmation "decision"
		char choice, decision;
		
		//introduction to classes
		cout << "In RPG Land, there are three classes you can play. These three "
		<< "classes are commonly known as Warriors, Rogues, and Mages." << endl;
		system("PAUSE");
		set_class_start:
		system("CLS");
		//print out the options to choose from
		cout << "Please choose a class to learn more about:" << endl;
		cout << "\n1. Warrior" << endl;
		cout << "2. Rogue" << endl;
		cout << "3. Mage" << endl;
		
		cin >> choice;
		
		while (true) {
			switch (choice) {
				//warrior choice
				case '1':
					warrior_description();
					cout << "Would you like to choose the Warrior (Y/N)? ";
					cin >> decision;
					
					while (true) {
						if (decision == 'Y' || decision == 'y') {
							player_class = "Warrior";
							warrior_bonus();
							goto set_class_confirm;
						}
						else if (decision == 'N' || decision == 'n') {goto set_class_start;}
						else {
							cout << "Please enter either Y or N: ";
							cin >> decision;
						}
					}
					break;
				//rogue choice
				case '2':
					rogue_description();
					cout << "Would you like to choose the Rogue (Y/N)? ";
					cin >> decision;
					
					while (true) {
						if (decision == 'Y' || decision == 'y') {
							player_class = "Rogue";
							rogue_bonus();
							goto set_class_confirm;
						}
						else if (decision == 'N' || decision == 'n') {goto set_class_start;}
						else {
							cout << "Please enter either Y or N: ";
							cin >> decision;
						}
					}
					break;
				//mage choice
				case '3':
					mage_description();
					cout << "Would you like to choose the Mage (Y/N)? ";
					cin >> decision;
					
					while (true) {
						if (decision == 'Y' || decision == 'y') {
							player_class = "Mage";
							mage_bonus();
							goto set_class_confirm;
						}
						else if (decision == 'N' || decision == 'n') {goto set_class_start;}
						else {
							cout << "Please enter either Y or N: ";
							cin >> decision;
						}
					}
					break;
				default:
					cout << "Invalid entry." << endl;
					cin >> choice;
			}
		}
		set_class_confirm:
		cout << "\nYou've chosen the " << player_class << " class! Congratulations!" << endl << endl;
		system("PAUSE");
	}
	
	//allocates remaining points as desired
	void allocate_points() {
		allocate_points_start:
		while (points > 0) {
			char choice;
			
			//overview
			view_attributes();
		
			cout << "\nYou have " << points << " points remaining. ";
			while (true) {
				cout << "Please select an attribute to add to:" << endl;
			
				//list of available attributes to add to
				cout << "1. Strength" << endl;
				cout << "2. Agility" << endl;
				cout << "3. Intellect" << endl;
				cout << "4. Stamina" << endl;
				cin >> choice;
				switch (choice) {
					case '1':
						base_str++;
						init_stats();
						points--;
						goto allocate_points_start;
						break;
					case '2':
						base_agi++;
						init_stats();
						points--;
						goto allocate_points_start;
						break;
					case '3':
						base_int++;
						init_stats();
						points--;
						goto allocate_points_start;
						break;
					case '4':
						base_sta++;
						init_stats();
						points--;
						goto allocate_points_start;
						break;
					default:
						cout << "Invalid entry. ";
						cin >> choice;
				}
			}
		}
		view_attributes();
		cout << "\nAll available points have been allocated." << endl;
		system("PAUSE");
	}

	void view_attributes() {
		system("CLS");
		cout << "Your current attributes are: " << endl;
		cout << "Strength: " << strength << endl;
		cout << "Agility: " << agility << endl;
		cout << "Intellect: " << intellect << endl;
		cout << "Stamina: " << stamina << endl;
	}
	
	void view_stats() {
		system("CLS");
		cout << "Your current stats are: " << endl;
		cout << "ATK: " << atk << endl;
		cout << "DEF: " << def << endl;
		cout << "Health: " << current_health << "/" << max_health << endl;
		if (player_class == "Mage") {
			cout << "Mana: " << current_mana << "/" << max_mana << endl;
			cout << "Spellpower: " << spellpower << endl;
		}
		cout << "\nExperience: " << exp << "/" << exp_to_next_level << endl;
		
	}
	
	void view_inventory() {
		system("CLS");
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
	}
	
	void view_equipment() {
		system("CLS");
		cout << "You have the following equipped:" << endl;
		for (int i = 0; i < 3; i++) {
			cout << equipment[i].get_type() << " slot: "
			<< equipment[i].get_name() << endl;
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
	
	void restore_health_to_max() {current_health = max_health;}
	void restore_mana_to_max() {current_mana = max_mana;}

	void restore_health(int x) {
		current_health += x;
		if (current_health > max_health) {restore_health_to_max();}
	}
	void restore_mana(int x) {
		current_mana += x;
		if (current_mana > max_mana) {restore_mana_to_max();}
	}
	
	void lose_health(int x) {current_health -= x;}
	void lose_mana(int x) {current_mana -= x;}
	
	void boost_def(int x) {def += x;}
	
	//incrementing an item in inventory
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
	
	//decrementing an item in inventory
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

	//modifiers to gold
	void remove_gold(int x) {gold -= x;}
	void add_gold(int x) {gold += x;}
	
	void gain_exp(int cr_exp) {
		exp += cr_exp;
		cout << "You've gained " << cr_exp << " experience!" << endl;
		level_up();
	}
	
	bool use_item() {
		system("CLS");
		Item *potions;
		int count = 0;
		
		//establish number of potions in inventory
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].get_type() == "Potion") {count++;}
		}
		
		//what if you have no items?
		if (count == 0) {
			cout << "You don't have any useable items!" << endl;
			system("PAUSE");
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
		cout << "What potion would you like to use:" << endl << endl;
		for (int i = 0; i < count; i++) {
			cout << i + 1 << ". " << potions[i].get_name() << " x"
			<< potions[i].get_quantity() << endl;
		}
		//adds an option to cancel
		cout << count + 1 << ". Cancel request (go back to the previous screen)" << endl;
		
		int choice;
		cin >> choice;
		
		while (true) {
			//decrement by one for index
			choice--;
			if (choice < count) {
				//determine change to health / mana
				if (potions[choice].get_subtype() == "Health") {
					int gain = 20 * potions[choice].get_level();
					restore_health(gain);
					cout << "You've used a " << potions[choice].get_name() << " and restored "
					<< gain << " health!" << endl;
				}
				else if (potions[choice].get_subtype() == "Mana") {
					int gain = 20 * potions[choice].get_level();
					restore_mana(gain);
					cout << "You've used a " << potions[choice].get_name() << " and restored "
					<< gain << " mana!" << endl;
				}
				system("PAUSE");
				//decrement item from inventory
				dec_inventory(potions[choice]);
				return true;
			}
			else if (choice == count) {return false;} /* cancels out */
			else {
				cout << "Invalid entry.";
				cin >> choice;
			}
		}
	}

	Creature cast_spell(Creature creature) {
		system("CLS");
		cout << "What spell would you like to cast?" << endl;
		view_spells();
		cout << num_of_spells + 1 << ". Cancel request (go to the previous menu)" << endl;
		
		int choice;
		cin >> choice;
		choice--;
		
		while (true) {
			if (choice < num_of_spells) {
				cout << "You cast " << spellbook[choice].get_name() << "!" << endl;
				if (spellbook[choice].get_type() == "Offensive") {
					creature.take_spell_dmg(spellbook[choice].get_magnitude(), acc, crit);
				}
				else if (spellbook[choice].get_type() == "Healing") {
					restore_health(spellbook[choice].get_magnitude());
					cout << "You've restored " << spellbook[choice].get_magnitude() << " health!" << endl;
					system("PAUSE");
				}
				else if (spellbook[choice].get_type() == "Defensive") {
					boost_def(spellbook[choice].get_magnitude());
					cout << "You've boosted your DEF by " << spellbook[choice].get_magnitude() << endl;
					system("PAUSE");
				}
				lose_mana(spellbook[choice].get_cost());
				did_cast_spell = true;
				return creature;
			}
			else if (choice == num_of_spells) {
				did_cast_spell = false;
				return creature;
			}
			else {
				cout << "Invalid entry.";
				cin >> choice;
			}
		}
	}
	
	void cast_healing_spell() {
		if (player_class == "Mage") {
			for (int i = 0; i < num_of_spells; i++) {
				if (spellbook[i].get_type() == "Healing") {
					system("CLS");
					cout << "Would you like to cast " << spellbook[i].get_name() << " (Y/N)? ";
					char choice;
					cin >> choice;
					
					while (true) {
						if (choice == 'Y' || choice == 'y') {
							cout << "You cast " << spellbook[i].get_name() << "!" << endl;
							restore_health(spellbook[choice].get_magnitude());
							cout << "You've restored " << spellbook[choice].get_magnitude() << " health!" << endl;
							system("PAUSE");
							lose_mana(spellbook[choice].get_cost());
							return;
						}
						else if (choice == 'N' || choice == 'n') {
							return;
						}
						else {
							cout << "Invalid entry.";
							cin >> choice;
						}
					}
				}
			}
		}
		else {
			cout << "You don't have any spells to cast. You're not a Mage, " << name << "!" << endl;
			system("PAUSE");
		}
	}
	
	void take_damage(int attack, int accuracy, int critical) {
		if (rand() % 101 < block) {
			cout << "You block the creature's attack!" << endl;
			return;
		}
		if (rand() % 101 < dodge) {
			cout << "You dodge the creature's attack!" << endl;
			return;
		}
		if (rand() % 101 < parry) {
			cout << "You parry the creature's attack!" << endl;
			return;
		}
		if (rand() % 101 > accuracy) {
			cout << "The creature missed you!" << endl;
			return;
		}
		else {
			int x;
			if (attack < def && attack > 0) {x = 1;}
			else if (attack == 0) {x = 0;}
			else {
				//establish base damage
				double dmg = 10 * (pow(1.25, attack / (def * 1.0)) - 1) * level;
				//check for crits
				if (rand() % 101 < critical) {
					cout << "The creature landed a critical hit!" << endl;
					dmg *= 2;
				}
				x = dmg;
			}
			if (x > 0) {
				cout << "You take " << x << " damage!" << endl;
				lose_health(x);
			}
			else {cout << "You didn't take any damage." << endl;}
		}
	}
	
	bool is_dead() {
		if (current_health <= 0) {return true;}
		else {return false;}
	}
	
	void dies() {
		cout << "You've died!" << endl;
		int exp_loss = 50;
		exp -= exp_loss;
		cout << "You've lost " << exp_loss << " experience!" << endl;
		system("PAUSE");
		if (exp < LEVEL_INCREMENT * (level - 1)) {exp = LEVEL_INCREMENT * (level - 1);}
	}
	
	//useful for viewing your current gold, either in the field or in a shop
	void view_gold() {cout << "You currently have " << gold << " gold." << endl;}
	
	void view_spells() {
		for (int i = 0; i < num_of_spells; i++) {
			cout << i + 1 << ". " << spellbook[i].get_name() << endl;
		}
	}
	
	//unequip items from slot
	void unequip_item(Item item) {
		if (item.get_type() == "Armor") {
			int armor_loss = item.item_stat_value();
			if (player_class == "Warrior") {
				equip_def_bonus -= armor_loss;
				armor_rating = equip_def_bonus;
			}
			else if (player_class == "Rogue") {
				equip_def_bonus -= armor_loss;
				armor_rating = equip_def_bonus;
			}
			else if (player_class == "Mage") {
				equip_def_bonus -= armor_loss;
				armor_rating = equip_def_bonus;
			}
			equipment[0].set_item("Armor", "Cloth", 0);
			inc_inventory(item);
			init_stats();
			cout << "\nYou've removed " << item.get_name() << "." << endl;
			system("PAUSE");
		}
		else if (item.get_type() == "Weapon") {
			int weapon_loss = item.item_stat_value();
			if (player_class == "Warrior") {
				if (item.get_subtype() == "Sword") {weapon_loss *= 2;}
			}
			else if (player_class == "Rogue") {
				if (item.get_subtype() == "Dagger") {weapon_loss *= 2;}
			}
			else if (player_class == "Mage") {
				if (item.get_subtype() == "Staff") {
					weapon_loss *= 2;
					equip_mana_bonus -= weapon_loss;
				}
			}
			equip_atk_bonus -= weapon_loss;
			equipment[1].set_item("Weapon", "Staff", 0);
			inc_inventory(item);
			init_stats();
			cout << "\nYou've removed " << item.get_name() << "." << endl;
			system("PAUSE");
		}
		else if (item.get_type() == "Shield") {
			int shield_loss = item.item_stat_value();
			equip_def_bonus -= shield_loss;
			armor_rating = equip_def_bonus;
			equipment[2].set_item("Shield", "none", 0);
			inc_inventory(item);
			init_stats();
			cout << "\nYou've removed " << item.get_name() << "." << endl;
			system("PAUSE");
		}
	}
	
	//equip item in slot
	void equip_item(Item item) {
		for (int i = 0; i < 3; i++) {
			if (item.get_name() == equipment[i].get_name()) {return;}
		}
		
		if (item.get_type() == "Armor") {
			int armor_gain;
			if (player_class == "Warrior") {
				if (equipment[0].get_level() != 0) {unequip_item(equipment[0]);}
				equipment[0] = item;
				armor_gain = equipment[0].item_stat_value();
				equip_def_bonus += armor_gain;
				armor_rating = equip_def_bonus;
				init_stats();
				dec_inventory(item);
				cout << "\nYou've equipped " << item.get_name() << "." << endl;
			}
			else if (player_class == "Rogue") {
				if (item.get_subtype() == "Mail") {
					cout << "You cannot equip mail armor." << endl;
				}
				else {
					if (equipment[0].get_level() != 0) {unequip_item(equipment[0]);}
					equipment[0] = item;
					armor_gain = equipment[0].item_stat_value();
					equip_def_bonus += armor_gain;
					armor_rating = equip_def_bonus;
					init_stats();
					dec_inventory(item);
					cout << "\nYou've equipped " << item.get_name() << "." << endl;
				}
			}
			else if (player_class == "Mage") {
				if (item.get_subtype() == "Cloth") {
					if (equipment[0].get_level() != 0) {unequip_item(equipment[0]);}
					equipment[0] = item;
					armor_gain = equipment[0].item_stat_value();
					equip_def_bonus += armor_gain;
					armor_rating = equip_def_bonus;
					init_stats();
					dec_inventory(item);
					cout << "\nYou've equipped " << item.get_name() << "." << endl;
				}
				else {
					cout << "You cannot equip leather or mail armor." << endl;
				}
			}
			system("PAUSE");
		}
		else if (item.get_type() == "Weapon") {
			int weapon_gain;
			if (equipment[1].get_level() != 0) {unequip_item(equipment[1]);}
			equipment[1] = item;
			weapon_gain = equipment[1].item_stat_value();
			if (player_class == "Warrior") {
				if (equipment[1].get_subtype() == "Sword") {weapon_gain *= 2;}
			}
			else if (player_class == "Rogue") {
				if (equipment[1].get_subtype() == "Dagger") {weapon_gain *= 2;}
			}
			else if (player_class == "Mage") {
				if (equipment[1].get_subtype() == "Staff") {
					weapon_gain *= 2;
					equip_mana_bonus += weapon_gain;
				}
			}
			equip_atk_bonus += weapon_gain;
			init_stats();
			dec_inventory(item);
			cout << "\nYou've equipped " << item.get_name() << "." << endl;
			system("PAUSE");
		}
		else if (item.get_type() == "Shield") {
			int shield_gain;
			if (player_class == "Warrior") {
				if (equipment[2].get_level() != 0) {unequip_item(equipment[2]);}
				equipment[2] = item;
				shield_gain = equipment[2].item_stat_value();
				equip_def_bonus += shield_gain;
				armor_rating = equip_def_bonus;
				block = 5;
				init_stats();
				dec_inventory(item);
				cout << "\nYou've equipped " << item.get_name() << "." << endl;
			}
			else {cout << "You cannot equip shields." << endl;}
			system("PAUSE");
		}
	}
	
	//equipment menu
	void equipment_menu() {
		int count;
		char choice;
		Item *inv;
		
		equipment_menu_start:
		view_equipment();
		
		cout << "\nWhat would you like to do?" << endl;
		cout << "1. Equip an item" << endl;
		cout << "2. Unequip an item" << endl;
		cout << "3. Leave menu" << endl;
		
		cin >> choice;
		count = num_of_equipable_items();
		while (true) {
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
						cin >> equip_choice;
						while (true) {
							if (equip_choice <= count) {
								equip_item(inv[equip_choice - 1]);
								goto equipment_menu_start;
							}
							else {
								cout << "Invalid entry";
								cin >> equip_choice;
							}
						}
					}
					else {
						cout << "\nYou have no equippable items." << endl << endl;
						system("PAUSE");
						goto equipment_menu_start;
					}
					delete[] inv;
					inv = 0;
					break;
				case '2':
				case 'U':
				case 'u':
					if (equipment[0].get_level() == 0 && equipment[1].get_level() == 0 && equipment[2].get_level() == 0) {
						cout << "You have nothing equipped; you cannot unequip anything." << endl;
						goto equipment_menu_start;
					}
					view_equipment();
					cout << "Cancel request" << endl;
					cout << "\nWhich equipment would you like to unequip?" << endl;
					char unequip_choice;
					cin >> unequip_choice;
					while (true) {
						switch (unequip_choice) {
							case '1':
							case 'A':
							case 'a':
								unequip_item(equipment[0]);
								goto equipment_menu_start;
								break;
							case '2':
							case 'W':
							case 'w':
								unequip_item(equipment[1]);
								goto equipment_menu_start;
								break;
							case '3':
							case 'S':
							case 's':
								unequip_item(equipment[2]);
								goto equipment_menu_start;
								break;
							case '4':
							case 'C':
							case 'c':
								goto equipment_menu_start;
								break;
							default:
								cout << "Invalid entry.";
								cin >> unequip_choice;
						}
					}
					goto equipment_menu_start;
					break;
				case '3':
				case 'L':
				case 'l':
					return;
					break;
				default:
					cout << "Invalid entry.";
					cin >> choice;
			}
		}
	}
	
	//establishes ATK, DEF, max_health, and mana levels, and takes bonuses into consideration
	void init_stats() {
		//update attributes
		strength = base_str + equip_atk_bonus / ATTRIB_BONUS;
		if (player_class == "Rogue") {agility = base_agi + equip_atk_bonus / ATTRIB_BONUS;}
		else {agility = base_agi + equip_def_bonus / ATTRIB_BONUS;}
		intellect = base_int + equip_mana_bonus / ATTRIB_BONUS;
		stamina = base_sta + equip_health_bonus / ATTRIB_BONUS;
		
		//sets ATK
		if (player_class == "Warrior") {atk = strength * 2 + agility + equip_atk_bonus;}
		else if (player_class == "Rogue") {atk = agility * 2 + strength + equip_atk_bonus;}
		else {atk = strength + agility + equip_atk_bonus;}
		
		//sets DEF
		if (player_class == "Rogue") {def = agility + armor_rating;}
		else {def = agility * 2 + armor_rating;}
		
		//sets max_health
		if (player_class == "Warrior") {max_health = 10 + stamina * 2 + equip_health_bonus;}
		else {max_health = 10 + stamina + equip_health_bonus;}
		
		//sets max_mana
		if (player_class == "Mage") {max_mana = intellect * 5 + equip_mana_bonus;}
		else {max_mana = 0;}
		
		//sets crit chance
		crit = 5;
		
		//sets dodge, parry, and block chances
		dodge = 5;
		if (player_class == "Rogue") {dodge = 10;}
		parry = 5;
		
		//sets accuracy
		acc = 90;
		
		//updates spellpower
		if (player_class == "Mage") {
			spellpower = intellect / ATTRIB_BONUS;
			for (int i = 0; i < num_of_spells; i++) {
				int x = spellbook[i].get_magnitude();
				spellbook[i].load_magnitude(x + spellpower);
			}
		}
	}

	//initializes spellbook
	void init_spellbook() {
		const int NUM = 3;
		string names[NUM] = {"Fireball", "Salve", "Steelskin"};
		string types[NUM] = {"Offensive", "Healing", "Defensive"};
		int magnitudes[NUM] = {25, 40, 50};
		
		num_of_spells = NUM;
		spellbook = new Spell[num_of_spells];
		for (int i = 0; i < num_of_spells; i++) {
			spellbook[i].set_spell(names[i], types[i], magnitudes[i], level);
		}
	}
	
	//level-up logic
	void level_up() {
		int max_level = 10;
		exp_to_next_level = LEVEL_INCREMENT * level * level;
		if (exp >= exp_to_next_level && level < max_level) {
			cout << "You've leveled up! ";
			level++;
			exp_to_next_level = LEVEL_INCREMENT * level * level;
			for (int i = 0; i < num_of_spells; i++) {
				spellbook[i].inc_level();
			}
			cout << "You are now Level " << level << "!" << endl;
			points += level - 1;
			system("PAUSE");
			allocate_points();
		}
	}
	
	//GET FUNCTIONS
	int get_base_str() {return base_str;}
	int get_base_agi() {return base_agi;}
	int get_base_int() {return base_int;}
	int get_base_sta() {return base_sta;}
	int get_points() {return points;}
	int get_exp() {return exp;}
	int get_gold() {return gold;}
	int get_armor_rating() {return armor_rating;}
	int get_level() {return level;}
	int get_atk() {return atk;}
	int get_def() {return def;}
	int get_max_health() {return max_health;}
	int get_max_mana() {return max_mana;}
	int get_current_mana() {return current_mana;}
	int get_current_health() {return current_health;}
	int get_equip_atk_bonus() {return equip_atk_bonus;}
	int get_equip_def_bonus() {return equip_def_bonus;}
	int get_equip_health_bonus() {return equip_health_bonus;}
	int get_equip_mana_bonus() {return equip_mana_bonus;}
	int get_items_in_inventory() {return items_in_inventory;}
	int get_accuracy() {return acc;}
	int get_crit() {return crit;}
	string get_name() {return name;}
	string get_player_class() {return player_class;}
	Item *get_inventory() {return inventory;}
	Item *get_equipment() {return equipment;}
	Spell *get_spellbook() {return spellbook;}
	bool get_did_cast_spell() {return did_cast_spell;}
	
	//LOAD FUNCTIONS
	void load_base_str(int x) {base_str = x;}
	void load_base_agi(int x) {base_agi = x;}
	void load_base_int(int x) {base_int = x;}
	void load_base_sta(int x) {base_sta = x;}
	void load_points(int x) {points = x;}
	void load_exp(int x) {exp = x;}
	void load_gold(int x) {gold = x;}
	void load_armor_rating(int x) {armor_rating = x;}
	void load_level(int x) {level = x;}
	void load_atk(int x) {atk = x;}
	void load_def(int x) {def = x;}
	void load_max_health(int x) {max_health = x;}
	void load_max_mana(int x) {max_mana = x;}
	void load_current_mana(int x) {current_mana = x;}
	void load_current_health(int x) {current_health = x;}
	void load_equip_atk_bonus(int x) {equip_atk_bonus = x;}
	void load_equip_def_bonus(int x) {equip_def_bonus = x;}
	void load_equip_health_bonus(int x) {equip_health_bonus = x;}
	void load_equip_mana_bonus(int x) {equip_mana_bonus = x;}
	void load_items_in_inventory(int x) {items_in_inventory = x;}
	void load_name(char *c) {name = c;}
	void load_player_class(char *c) {player_class = c;}
	void load_inventory(Item *items) {
		inventory = new Item[items_in_inventory];
		for (int i = 0; i < items_in_inventory; i++) {
			inventory[i] = items[i];
		}
	}
	void load_equipment(Item *items) {equipment = items;}
	
private:
	int base_str, base_agi, base_int, base_sta;
	int strength, agility, intellect, stamina;
	int points, exp, items_in_inventory, gold, armor_rating, level, num_of_spells;
	int atk, def, max_health, max_mana, current_mana, current_health;
	int equip_atk_bonus, equip_def_bonus, equip_health_bonus, equip_mana_bonus;
	int crit, dodge, parry, block, acc;
	int exp_to_next_level, spellpower;
	bool did_cast_spell;
	string name, player_class;
	Item *inventory, *equipment;
	Spell *spellbook;
	static const int ATTRIB_BONUS = 5;
	static const int CLASS_BONUS = 10;
	static const int LEVEL_INCREMENT = 50;

	//CLASS DESCRIPTIONS
	void warrior_description() {
		cout << "Warriors are melee fighters highly trained in the art "
		<< "of weaponry. They are unmatched in one-on-one physical combat, "
		<< "have unusually high stamina, and can wear any type of armor. They "
		<< "are the only class that can use shields." << endl << endl;
		
		cout << "A warrior's primary attribute is Strength." << endl;
	}
	void rogue_description() {
		cout << "Rogues are highly trained in the art of subterfuge and stealth. "
		<< "They specialize in taking their enemy by surprise, and dealing "
		<< "copious amounts of damage by exploiting this advantage. Rogues cannot "
		<< "wear heavy armor or use shields." << endl << endl;
		
		cout << "A rogue's primary attribute is Agility." << endl;
	}
	void mage_description() {
		cout << "Mages are masters of the elements and arcane spells. They use "
		<< "both offensive and defensive spells to both decimate their foes and "
		<< "protect and heal themselves. To assist them in channelling these potent "
		<< "magics, mages elect to wear flowing robes of magically-imbued cloth and "
		<< "eschew more traditional forms of protection." << endl << endl;
		
		cout << "A mage's primary attribute is Intellect." << endl;
	}
	
	//CLASS PRESET BONUSES
	void warrior_bonus() {
		base_str += CLASS_BONUS;
		base_sta += CLASS_BONUS;
	}
	void rogue_bonus() {base_agi += CLASS_BONUS;}
	void mage_bonus() {base_int += CLASS_BONUS;}
	
	//establishes pre-customization baseline for attributes
	void init_attribs() {
		base_str = base_agi = base_int = base_sta = 10;
		equip_atk_bonus = equip_def_bonus = equip_health_bonus = equip_mana_bonus = 0;
		points = 20;
		exp = items_in_inventory = armor_rating = spellpower = 0;
		level = 1;
		gold = 300;
	}

	int num_of_equipable_items() {
		int count = 0;
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].equipable()) {count++;}
		}
		return count;
	}

	//adding a new item to inventory
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

	//removing an item from inventory
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
};

class Shop {
public:
	//top level menu
	Player menu(Player player) {
		system("CLS");
		cout << entrance_flavor << endl;
		menu_start:
		cout << menu_flavor << endl;
	
		cout << "\n1. Buy / Browse goods" << endl;
		cout << "2. Sell" << endl;
		cout << "3. Leave shop" << endl;
	
		char choice;
		cin >> choice;
		while (true) {
			switch (choice) {
				case '1':
				case 'B':
				case 'b':
					player = buy(player);
					system("CLS");
					goto menu_start;
					break;
				case '2':
				case 'S':
				case 's':
					player = sell(player);
					system("CLS");
					goto menu_start;
					break;
				case '3':
				case 'L':
				case 'l':
					cout << leave_flavor << endl;
					system("PAUSE");
					return player;
					break;
				default:
					cout << "Invalid entry." << endl;
					cin >> choice;
			}
		}
	}

protected:
	Item *inventory;
	int size_of_inventory, level;
	string display_inventory_flavor, sell_flavor;
	string entrance_flavor, leave_flavor, menu_flavor;
	
	//shop inventory
	void display_inventory() {
		system("CLS");
		cout << display_inventory_flavor << endl << endl;
		for (int i = 0; i < size_of_inventory; i++) {
			cout << i + 1 << ". " << inventory[i].get_name() << " - "
			<< inventory[i].get_buy_value() << " gold" << endl;
		}
	}

	//buying menu
	Player buy(Player player) {
		buy_start:
		display_inventory();
		cout << size_of_inventory + 1 << ". Exit menu" << endl << endl;		
		player.view_gold();
		
		int choice;
		cin >> choice;
		while (true) {
			if (choice - 1 < size_of_inventory) {
				if (player.get_gold() >= inventory[choice - 1].get_buy_value()) {
					player.inc_inventory(inventory[choice - 1]);
					player.remove_gold(inventory[choice - 1].get_buy_value());
					cout << "You've purchased " << inventory[choice - 1].get_name()
					<< " for " << inventory[choice - 1].get_buy_value() << " gold." << endl;
				}
				else {cout << "You don't have enough gold." << endl;}
				system("PAUSE");
				goto buy_start;
			}
			else if (choice - 1 == size_of_inventory) {return player;}
			else {
				cout << "Invalid entry" << endl;
				cin >> choice;
			}
		}
		return player;
	}

	//selling menu
	Player sell(Player player) {
		sell_menu_start:
		system("CLS");
		if (player.get_items_in_inventory() == 0) {
			cout << "You don't have any items to sell." << endl;
			system("PAUSE");
			return player;
		}
		else {
			Item *player_inventory = player.get_inventory();
			cout << sell_flavor << endl << endl;
			for (int i = 0; i < player.get_items_in_inventory(); i++) {
				cout << i + 1 << ". " << player_inventory[i].get_name() << " x" << player_inventory[i].get_quantity()
				<< " - " << player_inventory[i].get_sell_value() << " gold" << endl;
			}
			cout << player.get_items_in_inventory() + 1 << ". Exit menu" << endl << endl;
			
			int choice;
			cin >> choice;
			bool quit = false;
			while (quit == false) {
				if (choice - 1 <= player.get_items_in_inventory()) {
					if (choice - 1 < player.get_items_in_inventory()) {
						cout << "You would like to sell " << player_inventory[choice - 1].get_name()
						<< " for " << player_inventory[choice - 1].get_sell_value() << " gold. Is that "
						<< "acceptable (Y/N)?" << endl;
						char decision;
						cin >> decision;
						while (true) {
							switch (decision) {
								case 'Y':
								case 'y':
									player.dec_inventory(player_inventory[choice - 1]);
									player.add_gold(player_inventory[choice - 1].get_sell_value());
									cout << "You have sold " << player_inventory[choice - 1].get_name()
									<< " for " << player_inventory[choice - 1].get_sell_value()
									<< " gold. " << endl << endl;
									system("PAUSE");
									goto sell_menu_start;
									break;
								case 'N':
								case 'n':
									goto sell_menu_start;
									break;
								default:
									cout << "Please enter Y or N: " << endl;
									cin >> decision;
							}
						}
					}
					else {return player;}
				}
				else {
					cout << "Invalid entry" << endl;
					cin >> choice;
				}
			}
			//purge pointer to prevent memory leak
			delete[] player_inventory;
			player_inventory = 0;
		}
		return player;
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

private:
	//initializes inventory
	void init_inventory() {
		size_of_inventory = 7;
		inventory = new Item[size_of_inventory];
		inventory[0].set_item("Weapon", "Sword", level);
		inventory[1].set_item("Weapon", "Dagger", level);
		inventory[2].set_item("Weapon", "Staff", level);
		inventory[3].set_item("Armor", "Cloth", level);
		inventory[4].set_item("Armor", "Leather", level);
		inventory[5].set_item("Armor", "Mail", level);
		inventory[6].set_item("Shield", "none", level);
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

private:
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
	Player stay(Player player) {
		system("CLS");
		cout << "Innkeeper: 'Our " << room.get_name() << " is available to rent for " << room.get_buy_value()
		<< " gold. We also offer 'specials' for some of our more...distinguished guests.'" << endl << endl;
		player.view_gold();
		cout << "\nWould you like to stay the night (Y/N)? ";
		
		char choice;
		cin >> choice;
		while (true) {
			switch (choice) {
				case 'Y':
				case 'y':
					if (player.get_gold() >= room.get_buy_value()) {
						player.remove_gold(room.get_buy_value());
						player.restore_health_to_max();
						player.restore_mana_to_max();
						system("CLS");
						cout << "You've stayed in the " << room.get_name() << ", and your health and mana have "
						<< "recovered fully. Anything else, such as certain people sneaking into your room in the middle "
						<< "of the night, you'd rather not think about too much." << endl;
						system("PAUSE");
						return player;
					}
					else {
						cout << "You don't have enough gold." << endl;
						system("PAUSE");
						return player;
					}
					break;
				case 'N':
				case 'n':
					cout << "You decided not to stay the night. Perhaps it had something to do with how the innkeeper has been "
					<< "staring at you this entire time...." << endl;
					system("PAUSE");
					return player;
					break;
				default:
					cout << "Please enter Y or N: ";
					cin >> choice;
			}
		}
		return player;
	}
};

class Map {
public:
	Map() {
		size = 5; /*number of spaces in a row*/
		space = 'O';
		init_map();
		set_town();
	}
	
	void init_map() {
		map = new char[size * size];
		for (int i = 0; i < size * size; i++) {
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
			system("PAUSE");
		}
	}
	
	//verifies that player can move in a particular direction
	bool can_move_north(int position) {
		if (position > size - 1) {return true;}
		else {return false;}
	}
	bool can_move_south(int position) {
		if (position < (size * size) - size) {return true;}
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
	
	//get initial position in the wild
	int get_town() {return town;}
	
private:
	char space;
	char *map;
	int size, town;
	
	void set_town() {
		town = rand() % (size * size);
		map[town] = 'T';
	}
};

bool saves_exist() {
	ifstream file;
	file.open("save_games.txt");
	if (file.is_open()) {return true;}
	else {return false;}
}

void save_game(Player player) {
	string filename;
	fstream file;
	save_game_start:
	system("CLS");
	cout << "To create a save game, enter a name: ";
	cin >> filename;
	
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
		cin >> decision;
		bool repeat = true;
		while (repeat) {
			if (decision == 'Y' || decision == 'y') {repeat = false;}
			else if (decision == 'N' || decision == 'n') {goto save_game_start;}
			else {
				cout << "Invalid entry.";
				cin >> decision;
			}
		}
	}
	else {
		file.open("save_games.txt", fstream::out |  fstream::app);
		file << filename << endl;
		file.close();
	}
	
	cout << "\nSaving game...." << endl;
	
	//attributes and stats
	file.open(filename + "_int.txt", fstream::out);
	file << player.get_base_str() << endl;
	file << player.get_base_agi() << endl;
	file << player.get_base_int() << endl;
	file << player.get_base_sta() << endl;
	file << player.get_points() << endl;
	file << player.get_exp() << endl;
	file << player.get_gold() << endl;
	file << player.get_armor_rating() << endl;
	file << player.get_level() << endl;
	file << player.get_atk() << endl;
	file << player.get_def() << endl;
	file << player.get_max_health() << endl;
	file << player.get_max_mana() << endl;
	file << player.get_items_in_inventory() << endl;
	file.close();
	file.open(filename + "_char.txt", fstream::out);
	file << player.get_name().c_str() << endl;
	file << player.get_player_class().c_str() << endl;
	file.close();
	
	//inventory - only if there is any
	if (player.has_inventory()) {
		Item *item = player.get_inventory();
		for (int i = 0; i < player.get_items_in_inventory(); i++) {
			file.open(filename + "_char.txt", fstream::out | fstream::app);
			file << item[i].get_type() << endl;
			file << item[i].get_subtype() << endl;
			file.close();
			file.open(filename + "_int.txt", fstream::out | fstream::app);
			file << item[i].get_level() << endl;
			file << item[i].get_quantity() << endl;
			file.close();
		}
		delete[] item;
		item = 0;
	}
	
	//equipment
	Item *equipment = player.get_equipment();
	for (int i = 0; i < 3; i++) {
		file.open(filename + "_char.txt", fstream::out | fstream::app);
		file << equipment[i].get_type() << endl;
		file << equipment[i].get_subtype() << endl;
		file.close();
		file.open(filename + "_int.txt", fstream::out | fstream::app);
		file << equipment[i].get_level() << endl;
		file.close();
	}
	
	cout << filename << " has been saved." << endl;
	system("PAUSE");
}

Player load_game(Player player) {
	string filename;
	ifstream file;
	int choice;
	system("CLS");
	cout << "Please choose a game to load:" << endl;
	
	//display list of saved games
	file.open("save_games.txt");
	string line;
	string *saves;
	saves = new string[3];
	for (int i = 1; file.good(); i++) {
		getline(file, line);
		if (line != "") {
			cout << i << ". " << line << endl;
			saves[i - 1] = line;
		}
	}
	file.close();
	
	cin >> choice;
	
	filename = saves[choice - 1];
	cout << "\nLoading " << filename << "...." << endl;
	
	//attributes and stats
	int x;
	file.open(filename + "_int.txt");
	for (int i = 0; file.good(); i++) {
		file >> x;
		if (i == 0) {player.load_base_str(x);}
		else if (i == 1) {player.load_base_agi(x);}
		else if (i == 2) {player.load_base_int(x);}
		else if (i == 3) {player.load_base_sta(x);}
		else if (i == 4) {player.load_points(x);}
		else if (i == 5) {player.load_exp(x);}
		else if (i == 6) {player.load_gold(x);}
		else if (i == 7) {player.load_armor_rating(x);}
		else if (i == 8) {player.load_level(x);}
		else if (i == 9) {player.load_atk(x);}
		else if (i == 10) {player.load_def(x);}
		else if (i == 11) {player.load_max_health(x);}
		else if (i == 12) {player.load_max_mana(x);}
		else if (i == 13) {player.load_items_in_inventory(x);}
	}
	file.close();
	
	file.open(filename + "_char.txt");
	char *c;
	c = new char[20];
	for (int i = 0; file.good(); i++) {
		file.getline(c, 20, '\n');
		if (i == 0) {player.load_name(c);}
		else if (i == 1) {player.load_player_class(c);}
	}
	delete[] c;
	c = 0;
	file.close();
	
	//inventory
	int int_length, char_length; /*number of inventory lines*/
	if (player.has_inventory()) {
		cout << "Loading inventory..." << endl;
		char_length = player.get_items_in_inventory() * 2;
		int_length = player.get_items_in_inventory() * 2;
		Item *inv;
		inv = new Item[player.get_items_in_inventory()];
		
		int j = 0; /*index of inventory*/
		char *c_inv;
		c_inv = new char[30];
		for (j = 0; j < player.get_items_in_inventory(); j++) {
			file.open(filename + "_char.txt");
			for (int i = 0; file.good(); i++) {
				file.getline(c_inv, 30, '\n');
				if ((i >= 2) && (i < 2 + char_length)) { /*ensures it stays within inventory bounds*/
					if ((i - 2) % char_length == 2 * j) {inv[j].load_type(c_inv);}
					if ((i - 2) % char_length == (2 * j) + 1) {inv[j].load_subtype(c_inv);}
				}
			}
			file.close();
		}
		delete[] c_inv;
		c_inv = 0;
		
		int x_inv;
		for (j = 0; j < player.get_items_in_inventory(); j++) {
			file.open(filename + "_int.txt");
			for (int i = 0; file.good(); i++) {
				file >> x_inv;
				if ((i >= 14) && (i < 14 + int_length)) { /*ensures it stays within inventory bounds*/
					if ((i - 14) % int_length == 2 * j) {inv[j].load_level(x_inv);}
					if ((i - 14) % int_length == (2 * j) + 1) {inv[j].load_quantity(x_inv);}
				}
			}
			file.close();
		}

		for (int i = 0; i < player.get_items_in_inventory(); i++) {
			int num = inv[i].get_quantity(); /*backup quantity*/
			inv[i].set_item(inv[i].get_type(), inv[i].get_subtype(), inv[i].get_level()); /*init_item*/
			inv[i].load_quantity(num); /*restore original quantity*/
		}
		
		player.load_inventory(inv);
		delete[] inv;
		inv = 0;
	}
	//if no inventory
	else {
		char_length = 0;
		int_length = 0;
	}
	
	//equipment
	cout << "Loading equipment..." << endl;
	Item *equipment;
	equipment = new Item[3];
	
	//initialize empty equipment at first
	equipment[0].set_item("Armor", "Cloth", 0);
	equipment[1].set_item("Weapon", "Staff", 0);
	equipment[2].set_item("Shield", "none", 0);
	player.load_equipment(equipment);
	
	//copy in equipment details
	char *c_equip;
	c_equip = new char[30];
	int j = 0; /*index of equipment*/
	for (j = 0; j < 3; j++) {
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
	for (j = 0; j < 3; j++) {
		file.open(filename + "_int.txt");
		for (int i = 0; file.good(); i++) {
			file >> x_equip;
			if (i >= 14 + int_length) {
				if ((i - 14 - int_length) % 4 == j) {equipment[j].load_level(x_equip);}
			}
		}
		file.close();
	}
	
	//initialize proper equipment
	for (int i = 0; i < 3; i++) {
		equipment[i].set_item(equipment[i].get_type(), equipment[i].get_subtype(), equipment[i].get_level()); /*init_item*/
	}
	
	//init_stats for empty gear
	player.load_equip_atk_bonus(0);
	player.load_equip_def_bonus(0);
	player.load_equip_health_bonus(0);
	player.load_equip_mana_bonus(0);
	player.init_stats();
	
	//EQUIP proper gear, which calls init_stats()
	for (int i = 0; i < 3; i++) {player.equip_item(equipment[i]);}
	
	if (player.get_player_class() == "Mage") {player.init_spellbook();}
	player.restore_health_to_max();
	player.restore_mana_to_max();
	player.level_up();
	player.init_stats();
	
	cout << "Your game has been loaded." << endl;
	system("PAUSE");

	return player;
}

Player load_or_create_player_menu(Player player) {
	if (saves_exist()) {
		cout << "What would you like to do?" << endl;
		cout << "1. Load player" << endl;
		cout << "2. New player" << endl;
		char choice;
		cin >> choice;
		bool repeat = true;
		while (repeat) {
			switch (choice) {
				case '1':
				case 'L':
				case 'l':
					player = load_game(player);
					repeat = false;
					break;
				case '2':
				case 'N':
				case 'n':
					cin.ignore();
					player.set_player();
					repeat = false;
					break;
				default:
					cout << "Invalid entry.";
					cin >> choice;
			}
		}
	}
	else {player.set_player();}
	return player;
}

Player save_or_load(Player player) {
	save_or_load_start:
	system("CLS");
	cout << "What would you like to do?" << endl;
	cout << "1. Save player" << endl;
	cout << "2. Load player" << endl;
	cout << "3. Exit menu" << endl;
	char choice;
	cin >> choice;
	
	while (true) {
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
				player = load_game(player);
				goto save_or_load_start;
				break;
			case '3':
			case 'X':
			case 'x':
			case 'E':
			case 'e':
				return player;
				break;
			default:
				cout << "Invalid entry.";
				cin >> choice;
		}
	}
	return player;
}

Player player_menu(Player player) {
	player_menu_start:
	system("CLS");

	cout << "Player menu:" << endl << endl;
	cout << "What would you like to do? " << endl;
	cout << "1. View attributes" << endl;
	cout << "2. View stats" << endl;
	cout << "3. View inventory" << endl;
	cout << "4. Use item" << endl;
	cout << "5. Cast healing spell" << endl;
	cout << "6. Equipment" << endl;
	cout << "7. Load / Save Player" << endl;
	cout << "8. Exit menu" << endl;
	cout << "9. Quit game" << endl;
	
	char choice;
	cin >> choice;
	
	while (true) {
		switch (choice) {
			case '1':
				player.view_attributes();
				system("PAUSE");
				goto player_menu_start;
				break;
			case '2':
				player.view_stats();
				system("PAUSE");
				goto player_menu_start;
				break;
			case '3':
				player.view_inventory();
				system("PAUSE");
				goto player_menu_start;
				break;
			case '4':
				player.use_item();
				goto player_menu_start;
				break;
			case '5':
			case 'H':
			case 'h':
				player.cast_healing_spell();
				goto player_menu_start;
				break;
			case '6':
				player.equipment_menu();
				goto player_menu_start;
				break;
			case '7':
				save_or_load(player);
				goto player_menu_start;
				break;
			case '8':
			case 'X':
			case 'x':
				return player;
				break;
			case '9':
			case 'Q':
			case 'q':
				system("CLS");
				cout << "Are you sure you sure you want to quit? Don't be that guy, man. "
				<< "Are you really that much of a pathetic, lowlife loser (Y/N)? ";
				char decision;
				cin >> decision;
				while (true) {
					if (decision == 'Y' || decision == 'y') {exit(0);}
					else if (decision == 'N' || decision == 'n') {
						cout << "That's the spirit, champ!" << endl;
						system("PAUSE");
						goto player_menu_start;
					}
					else {
						cout << "Invalid entry.";
						cin >> decision;
					}
				}
				break;
			default:
				cout << "Invalid entry. ";
				cin >> choice;
		}
	}
	return player;
}

Creature det_creature(Player player) {
	Creature bunny("Balanced", player.get_level());
	Creature ferret("Offensive", player.get_level());
	Creature midget("Defensive", player.get_level());
	
	//determines type of creature encountered
	if (rand() % 101 <= 65) {return bunny;} /*bunny encounter - 65% chance*/
	else if (rand() % 101 > 65 && rand() % 101 <= 90) {return ferret;} /*ferret encounter - 25% chance*/
	else {return midget;} /*midget encounter - 10% chance*/
}

Player fight_menu(Player player) {
	Creature creature;
	creature = det_creature(player);
	cout << "You've encountered a " << creature.get_name() << "!" << endl << endl;
	system("PAUSE");
	
	fight_menu_start:
	system("CLS");
	cout << "What would you like to do?" << endl;
	cout << "1. Attack" << endl;
	cout << "2. Cast a spell" << endl;
	cout << "3. Use item" << endl;
	cout << "4. Run away" << endl << endl;
	cout << "Your health: " << player.get_current_health() << "/" << player.get_max_health() << endl;
	cout << "Your mana: " << player.get_current_mana() << "/" << player.get_max_mana() << endl << endl;
	cout << creature.get_name() << "'s health: " << creature.get_health() << endl;
	char choice;
	cin >> choice;
	
	while (true) {
		switch (choice) {
			case '1':
			case 'A':
			case 'a':
				creature.take_damage(player.get_atk(), player.get_accuracy(), player.get_crit());
				if (creature.is_dead()) {
					cout << "You've killed the " << creature.get_name() << "!" << endl;
					player.gain_exp(creature.get_exp());
					player.add_gold(creature.get_money());
					cout << "You've looted " << creature.get_money() << " gold!" << endl;
					if (creature.get_has_loot()) {player.inc_inventory(creature.get_loot());}
					system("PAUSE");
					return player;
				}
				player.take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit());
				if (player.is_dead()) {
					player.dies();
					return player;
				}
				system("PAUSE");
				goto fight_menu_start;
				break;
			case '2':
			case 'C':
			case 'c':
			case 'S':
			case 's':
				if (player.get_player_class() == "Mage") {
					creature = player.cast_spell(creature);
					if (player.get_did_cast_spell()) {
						if (creature.is_dead()) {
							cout << "You've killed the " << creature.get_name() << "!" << endl;
							player.gain_exp(creature.get_exp());
							player.add_gold(creature.get_money());
							cout << "You've looted " << creature.get_money() << " gold!" << endl;
							if (creature.get_has_loot()) {player.inc_inventory(creature.get_loot());}
							system("PAUSE");
							return player;
						}
						player.take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit());
						if (player.is_dead()) {
							player.dies();
							return player;
						}
						system("PAUSE");
					}
				}
				else {
					cout << "You don't have any spells to cast. You're not a Mage, "
					<< player.get_name() << "!" << endl;
					system("PAUSE");
				}
				goto fight_menu_start;
				break;
			case '3':
			case 'U':
			case 'u':
				if (player.use_item()) {
					player.take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit());
					if (player.is_dead()) {
						player.dies();
						return player;
					}
					system("PAUSE");
				}
				goto fight_menu_start;
				break;
			case '4':
			case 'R':
			case 'r':
				cout << "You flee like a coward! Were you that scared of a " << creature.get_name() << "?" << endl;
				system("PAUSE");
				return player;
				break;
			default:
				cout << "Invalid entry.";
				cin >> choice;
		}
	}
}

Player wild_menu(Player player) {
	Map map;
	int position = map.get_town();
	int creature_chance = 35;
	
	wild_menu_start:
	
	system("CLS");
	cout << "You have entered the wild! It's a large and scary meadow, "
	<< "filled with gross things and cute creatures." << endl << endl;
	system("PAUSE");
	
	wild_menu_after_move:
	//handles death situations (returns to town)
	if (player.is_dead()) {
		player.restore_health_to_max();
		player.restore_mana_to_max();
		return player;
	}
	system("CLS");
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
	cin >> choice;
	
	while (true) {
		switch (choice) {
			case 'N':
			case 'n':
				if (map.can_move_north(position)) {
					position = map.move_player(position, 'N');
					if (rand() % 101 < creature_chance) {player = fight_menu(player);}
					else {
						cout << "You move north; so far, the coast seems clear. Who's really scared of bunnies, anyway?" << endl;
						system("PAUSE");
					}
					if (position == map.get_town()) {
						map.enter_town(position);
						return player;
					}
					goto wild_menu_after_move;
				}
				else {
					cout << "You can't move north. Please try another direction: ";
					cin >> choice;
				}
				break;
			case 'W':
			case 'w':
				if (map.can_move_west(position)) {
					position = map.move_player(position, 'W');
					if (rand() % 101 < creature_chance) {player = fight_menu(player);}
					else {
						cout << "You move west; so far, nothing has eaten you. Flowers can't eat you, right?" << endl;
						system("PAUSE");
					}
					if (position == map.get_town()) {
						map.enter_town(position);
						return player;
					}goto wild_menu_after_move;
				}
				else {
					cout << "You can't move west. Please try another direction: ";
					cin >> choice;
				}
				break;
			case 'E':
			case 'e':
				if (map.can_move_east(position)) {
					position = map.move_player(position, 'E');
					if (rand() % 101 < creature_chance) {player = fight_menu(player);}
					else {
						cout << "You move east; so far, everything is fine. Grass just makes you itch, after all." << endl;
						system("PAUSE");
					}
					if (position == map.get_town()) {
						map.enter_town(position);
						return player;
					}goto wild_menu_after_move;
				}
				else {
					cout << "You can't move east. Please try another direction: ";
					cin >> choice;
				}
				break;
			case 'S':
			case 's':
				if (map.can_move_south(position)) {
					position = map.move_player(position, 'S');
					if (rand() % 101 < creature_chance) {player = fight_menu(player);}
					else {
						cout << "You move south; so far, all you see are bees buzzing around the flowers. Oh man, BEES!?" << endl;
						system("PAUSE");
					}
					if (position == map.get_town()) {
						map.enter_town(position);
						return player;
					}goto wild_menu_after_move;
				}
				else {
					cout << "You can't move south. Please try another direction: ";
					cin >> choice;
				}
				break;
			case '1':
				return player;
				break;
			case '2':
				player = player_menu(player);
				goto wild_menu_start;
				break;
			default:
				cout << "Invalid entry.";
				cin >> choice;
		}
	}
	return player;
}

Player town_menu(Player player) {
	town_menu_start:
	system("CLS");
	Blacksmith town_blacksmith(player.get_level());
	Inn town_inn(player.get_level());
	
	cout << "You have entered town." << endl << endl;
	cout << "What would you like to do?" << endl;
	cout << "1. Go to the blacksmith" << endl;
	cout << "2. Go to the inn" << endl;
	cout << "3. Leave the town (enter the wild)" << endl;
	cout << "4. Player menu" << endl;
	
	char choice;
	cin >> choice;
	
	while (true) {
		switch (choice) {
			case '1':
				player = town_blacksmith.menu(player);
				goto town_menu_start;
				break;
			case '2':
				player = town_inn.menu(player);
				goto town_menu_start;
				break;
			case '3':
				player = wild_menu(player);
				goto town_menu_start;
				break;
			case '4':
				player = player_menu(player);
				goto town_menu_start;
				break;
			default:
				cout << "Invalid input" << endl;
				cin >> choice;
		}
	}
	return player;
}

int main() {
	srand(time(0)); /*just set once to ensure random generation*/
	
	Player player;
	
	cout << "Welcome to RPG Land!" << endl << endl;
	player = load_or_create_player_menu(player);

	town_menu(player);
	
	return 0;
}