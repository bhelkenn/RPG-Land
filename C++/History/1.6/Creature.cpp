#include"Creature.h"
using namespace std;

//CONSTRUCTORS
Boss::Boss(int boss_level) {
	level = boss_level;
	creature_acc = 100;
	stat_pool = 150 + 100 * (level - 1);
	set_type();
	exp = 30 * level;
	money = exp * 5;
	has_loot = generate_loot();
}

//INITIALIZATION	
void Creature::set_type() {
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
		s += "turtle";
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
void Boss::set_type() {
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
bool Creature::generate_loot() {
	const int TRASH = 5;
	const int POTION = 2;
	const int EQUIPMENT = 8;
	int trash_chance = 35;
	int potion_chance = 15;
	int equipment_chance = 10;
	loot_count = 0;
	
	string trash[TRASH] = {"broken tooth", "hairball", "string", "tail fluff", "lint"};
	string potion[POTION] = {"Health", "Mana"};
	string equipment_type[EQUIPMENT] = {"Armor", "Armor", "Armor", "Weapon", "Weapon", "Weapon", "Offhand", "Shield"};
	string equipment_subtype[EQUIPMENT] = {"Mail", "Leather", "Cloth", "Sword", "Dagger", "Staff", "Orb", "Shield"};
	
	Item *tmp;
	//trash
	for (int i = 0; i < TRASH; i++) {
		if (rand() % 101 < trash_chance) {
			if (loot_count > 0) {
				tmp = new Item[loot_count];
				for (int j = 0; j < loot_count; j++) {tmp[j] = loot[j];}
			}
			loot_count++;
			loot = new Item[loot_count];
			if (loot_count + 1 > 0) {
				for (int k = 0; k < loot_count - 1; k++) {loot[k] = tmp[k];}
			}
			loot[loot_count - 1].set_item("Trash", trash[i], level);
		}
	}
	//potions
	for (int i = 0; i < POTION; i++) {
		if (rand() % 101 < potion_chance) {
			if (loot_count > 0) {
				tmp = new Item[loot_count];
				for (int j = 0; j < loot_count; j++) {tmp[j] = loot[j];}
			}
			loot_count++;
			loot = new Item[loot_count];
			if (loot_count + 1 > 0) {
				for (int k = 0; k < loot_count - 1; k++) {loot[k] = tmp[k];}
			}
			loot[loot_count - 1].set_item("Potion", potion[i], level);
		}
	}
	//equipment
	for (int i = 0; i < EQUIPMENT; i++) {
		if (rand() % 101 < equipment_chance) {
			if (loot_count > 0) {
				tmp = new Item[loot_count];
				for (int j = 0; j < loot_count; j++) {tmp[j] = loot[j];}
			}
			loot_count++;
			loot = new Item[loot_count];
			if (loot_count + 1 > 0) {
				for (int k = 0; k < loot_count - 1; k++) {loot[k] = tmp[k];}
			}
			loot[loot_count - 1].set_item(equipment_type[i], equipment_subtype[i], level);
		}
	}
	if (loot_count > 0) {return true;}
	else {return false;}
}
bool Boss::generate_loot() {
	const int BOSS = 5;
	const int TRASH = 5;
	const int POTION = 2;
	const int EQUIPMENT = 8;
	int trash_chance = 50;
	int potion_chance = 20;
	int equipment_chance = 30;
	
	string boss[BOSS] = {"Fractured Tooth", "Poop Nugget", "Torn Claw", "Flea Collar", "Golden Apple"};
	string trash[TRASH] = {"broken tooth", "hairball", "string", "tail fluff", "lint"};
	string potion[POTION] = {"Health", "Mana"};
	string equipment_type[EQUIPMENT] = {"Armor", "Armor", "Armor", "Weapon", "Weapon", "Weapon", "Offhand", "Shield"};
	string equipment_subtype[EQUIPMENT] = {"Mail", "Leather", "Cloth", "Sword", "Dagger", "Staff", "Orb", "Shield"};
	
	//boss
	loot_count = 1;
	loot = new Item[loot_count];
	loot[loot_count - 1].set_item("Boss Drop", boss[level - 1], level);
	Item *tmp;
	//trash
	for (int i = 0; i < TRASH; i++) {
		if (rand() % 101 < trash_chance) {
			tmp = new Item[loot_count];
			for (int j = 0; j < loot_count; j++) {tmp[j] = loot[j];}
			
			loot_count++;
			loot = new Item[loot_count];
			
			for (int k = 0; k < loot_count - 1; k++) {loot[k] = tmp[k];}
			
			loot[loot_count - 1].set_item("Trash", trash[i], level);
		}
	}
	//potions
	for (int i = 0; i < POTION; i++) {
		if (rand() % 101 < potion_chance) {
			tmp = new Item[loot_count];
			for (int j = 0; j < loot_count; j++) {tmp[j] = loot[j];}
			
			loot_count++;
			loot = new Item[loot_count];
			
			for (int k = 0; k < loot_count - 1; k++) {loot[k] = tmp[k];}
			
			loot[loot_count - 1].set_item("Potion", potion[i], level);
		}
	}
	//equipment
	for (int i = 0; i < EQUIPMENT; i++) {
		if (rand() % 101 < equipment_chance) {
			tmp = new Item[loot_count];
			for (int j = 0; j < loot_count; j++) {tmp[j] = loot[j];}
			
			loot_count++;
			loot = new Item[loot_count];
			
			for (int k = 0; k < loot_count - 1; k++) {loot[k] = tmp[k];}
			
			loot[loot_count - 1].set_item(equipment_type[i], equipment_subtype[i], level);
		}
	}
	return true;
}

void Creature::det_creature(int player_level) {
	//creature determination can be any level up to current level
	level = rand() % player_level + 1;
	
	//determines type of creature encountered
	int x = rand() % 101;
	if (x <= 65) {type = "Balanced";} //balanced encounter - 65% chance
	else if (x > 65 && x <= 90) {type = "Offensive";} //offensive encounter - 25% chance
	else {type = "Defensive";} //defensive encounter - 10% chance
		
	//initialize now that type is established
	stat_pool = 150 + 100 * (level - 1);
	set_type();
	has_loot = generate_loot();
}

//COMBAT
void Creature::take_damage(int attack, int acc, int critical, string player_name) {
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
void Creature::take_spell_dmg(int magnitude, int acc, int critical) {
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
bool Creature::is_dead() {
	if (health <= 0) {return true;}
	else {return false;}
}

//STANCE FUNCTIONS
void Creature::defensive() {
	creature_acc = 85;
	atk = stat_pool / 3;
	def = stat_pool * 2 / 3;
	max_health = stat_pool;
	dodge = 10;
	parry = 10;
	crit = 2;
	escape_chance = 85;
}
void Boss::defensive() {
	atk = stat_pool / 3;
	def = stat_pool * 2 / 3;
	max_health = stat_pool;
	dodge = 25;
	parry = 25;
	crit = 5;
	escape_chance = 40;
}
void Creature::offensive() {
	creature_acc = 95;
	atk = stat_pool * 2 / 3;
	def = stat_pool / 3;
	max_health = stat_pool / 4;
	dodge = 2;
	parry = 2;
	crit = 10;
	escape_chance = 65;
}
void Boss::offensive() {
	atk = stat_pool * 2 / 3;
	def = stat_pool / 3;
	max_health = stat_pool / 4;
	dodge = 5;
	parry = 5;
	crit = 25;
	escape_chance = 20;
}
void Creature::balanced() {
	creature_acc = 90;
	atk = stat_pool / 2;
	def = stat_pool / 2;
	max_health = stat_pool / 2;
	dodge = 5;
	parry = 5;
	crit = 5;
	escape_chance = 75;
}
void Boss::balanced() {
	atk = stat_pool / 2;
	def = stat_pool / 2;
	max_health = stat_pool / 2;
	dodge = 10;
	parry = 10;
	crit = 10;
	escape_chance = 30;
}