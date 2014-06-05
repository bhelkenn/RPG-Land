#include"Player.h"
using namespace std;

//CLASS DESCRIPTIONS
void Player::warrior_description() {
	cout << "Warriors are melee fighters highly trained in the art "
	<< "of weaponry. They are unmatched in one-on-one physical combat, "
	<< "have unusually high stamina, and can wear any type of armor. They "
	<< "are the only class that can use shields.\n" << endl;
	
	cout << "A warrior's primary attribute is Strength." << endl;
}
void Player::rogue_description() {
	cout << "Rogues are highly trained in the art of subterfuge and stealth. "
	<< "They specialize in taking their enemy by surprise, and dealing "
	<< "copious amounts of damage by exploiting this advantage. Rogues cannot "
	<< "wear heavy armor or use shields.\n" << endl;
	
	cout << "A rogue's primary attribute is Agility." << endl;
}
void Player::mage_description() {
	cout << "Mages are masters of the elements and arcane spells. They use "
	<< "both offensive and defensive spells to both decimate their foes and "
	<< "protect and heal themselves. To assist them in channelling these potent "
	<< "magics, mages elect to wear flowing robes of magically-imbued cloth and "
	<< "eschew more traditional forms of protection.\n" << endl;
	
	cout << "A mage's primary attribute is Intellect." << endl;
}

//CONSTRUCTORS
Warrior::Warrior(string player_name) {
	name = player_name;
	player_class = "Warrior";
	set_descriptions();
	init_attribs();
	points = 0;
	base_str += CLASS_BONUS;
	base_sta += CLASS_BONUS;
	boosted = 0;
}
Rogue::Rogue(string player_name) {
	name = player_name;
	player_class = "Rogue";
	set_descriptions();
	init_attribs();
	points = 0;
	base_agi += CLASS_BONUS;
	boosted = 0;
}
Mage::Mage(string player_name) {
	name = player_name;
	player_class = "Mage";
	set_descriptions();
	init_attribs();
	points = 0;
	base_int += CLASS_BONUS;
	boosted = 0;
}

//INITIALIZATION
void Player::set_name() {
	cout << "What is your name? ";
	char c[30];
	cin.getline(c, 30);
	name = c;
	cout << "Hi, " << name << "!" << endl;
	Pause();
	Clear();
}
void Player::set_class() {
	//introduction to classes
	cout << "In RPG Land, there are three classes you can play. These three "
	<< "classes are commonly known as Warriors, Rogues, and Mages." << endl;
	Pause();
	set_class_start:
	Clear();
	//print out the options to choose from
	cout << "Please choose a class to learn more about:\n" << endl;
	int count = 1;
	cout << count << ". Warrior" << endl;
	count++;
	cout << count << ". Rogue" << endl;
	count++;
	cout << count << ". Mage" << endl;
	
	//switch "choice", confirmation "decision"
	int choice;
	char decision;
	while (true) {
		choice = GetInt();
		switch (choice) {
			//warrior choice
			case 1:
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
			case 2:
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
			case 3:
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
			default:
				cout << "Please enter a number between 1 and " << count << ": ";
				break;
		}
	}
	set_class_confirm:
	cout << "\nYou've chosen the " << player_class << " class! Congratulations!\n" << endl;
	Pause();
}
void Player::set_quests() {
	Quest quest;
	boss_quest = quest;
	load_quest_active(false);
}
void Player::init_new_player() {
	level = 1;
	exp_to_next_level = LEVEL_INCREMENT * level * level;
	exp = items_in_inventory = 0;
	gold = 300;
	points = 10;
	for (int i = 0; i < MAX_LEVEL; i++) {boss_quests_complete[i] = false;}
	set_quests();
	achievement_count = 0;
	achievement_points = 0;
}
void Player::set_player() {
	set_name();
	set_class();
}
void Warrior::set_player() {
	init_new_player();
	update_stats();
	attribs_menu();
	current_health = max_health;
	init_equipment();
}
void Rogue::set_player() {
	init_new_player();
	update_stats();
	attribs_menu();
	current_health = max_health;
	init_equipment();
}
void Mage::set_player() {
	init_new_player();
	update_stats();
	attribs_menu();
	current_health = max_health;
	current_mana = max_mana;
	init_equipment();
	init_spellbook();
}
void Warrior::init_equipment() {
	equipment[0].set_item("Armor", "Cloth", 0);
	equipment[1].set_item("Weapon", "Staff", 0);
	equipment[2].set_item("Shield", "Shield", 0);
}
void Rogue::init_equipment() {
	equipment[0].set_item("Armor", "Cloth", 0);
	equipment[1].set_item("Weapon", "Staff", 0);
	equipment[2].set_item("Weapon", "Staff", 0);
}
void Mage::init_equipment() {
	equipment[0].set_item("Armor", "Cloth", 0);
	equipment[1].set_item("Weapon", "Staff", 0);
	equipment[2].set_item("Offhand", "Orb", 0);
}
void Mage::init_spellbook() {
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
void Player::init_achievements() {
	achievement_count = 0;
	achievement_points = 0;
}

//provides descriptions of the four base attributes
void Warrior::set_descriptions() {
	description_strength = "Strength adds to your attack rating. ";
	description_strength += "One point of STR equates to two ATK points.";
	description_agility = "Agility adds to your attack rating, as well as your defense rating.";
	description_agility += "One point of AGI equates to one ATK point and two DEF points.";
	description_intellect = "Intellect has no effect. Putting points to INT would be idiotic.";
	description_stamina = "Stamina adds to your maximum health. ";
	description_stamina += "One point of STA equates to two health points.";
}
void Rogue::set_descriptions() {
	description_strength = "Strength adds to your attack rating. ";
	description_strength += "One point of STR equates to one ATK point.";
	description_agility = "Agility adds to your attack rating, as well as your defense rating.";
	description_agility += "One point of AGI equates to two ATK points and one DEF point.";
	description_intellect = "Intellect has no effect. Putting points to INT would be idiotic.";
	description_stamina = "Stamina adds to your maximum health. ";
	description_stamina += "One point of STA equates to two health points.";
}
void Mage::set_descriptions() {
	description_strength = "Strength adds to your attack rating. ";
	description_strength += "One point of STR equates to one ATK point.";
	description_agility = "Agility adds to your attack rating, as well as your defense rating.";
	description_agility += "One point of AGI equates to one ATK point and two DEF points.";
	description_intellect = "Intellect adds to your spellpower and mana pool. ";
	description_intellect += "One point of INT equates to two mana points and one-fifth points of spellpower.";
	description_stamina = "Stamina adds to your maximum health. ";
	description_stamina += "One point of STA equates to two health points.";
}

//establishes stats and attributes, and takes bonuses into consideration
void Warrior::update_stats() {
	//update equipment bonuses
	block = equipment[2].get_block_bonus(); //will correct to 0 if no equipment
	if (equipment[0].get_level() > 0 && equipment[2].get_level() > 0) {
		if (equipment[0].broken() && equipment[2].broken()) {equip_def_bonus = 0;}
		else if (equipment[0].broken()) {equip_def_bonus = equipment[2].get_def_bonus();}
		else if (equipment[2].broken()) {equip_def_bonus = equipment[0].get_def_bonus();}
		else {equip_def_bonus = equipment[0].get_def_bonus() + equipment[2].get_def_bonus();}
	}
	else if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
		equip_def_bonus = equipment[0].get_def_bonus();
	}
	else if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
		equip_def_bonus	= equipment[2].get_def_bonus();
	}
	else {equip_def_bonus = 0;}
	
	if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
		if (equipment[1].get_subtype() == "Sword") {equip_atk_bonus = equipment[1].get_atk_bonus();}
		else {equip_atk_bonus = equipment[1].get_atk_bonus();}
	}
	else {equip_atk_bonus = 0;}
	
	//establish attribute bonuses
	str_bonus = agi_bonus = int_bonus = sta_bonus = 0; //initialize
	if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
		if (equipment[0].get_subtype() == "Mail") {str_bonus += equipment[0].get_str_bonus();}
		else if (equipment[0].get_subtype() == "Leather") {agi_bonus += equipment[0].get_agi_bonus();}
		else if (equipment[0].get_subtype() == "Cloth") {int_bonus += equipment[0].get_int_bonus();}
		sta_bonus += equipment[0].get_sta_bonus();
	}
	if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
		if (equipment[1].get_subtype() == "Sword") {str_bonus += equipment[1].get_str_bonus();}
		else if (equipment[1].get_subtype() == "Dagger") {agi_bonus += equipment[1].get_agi_bonus();}
		else if (equipment[1].get_subtype() == "Staff") {int_bonus += equipment[1].get_int_bonus();}
		sta_bonus += equipment[1].get_sta_bonus();
	}
	if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
		str_bonus += equipment[2].get_str_bonus();
		sta_bonus += equipment[2].get_sta_bonus();
	}
	
	//update attributes
	strength = base_str + str_bonus + equip_atk_bonus / ATTRIB_BONUS;
	agility = base_agi + agi_bonus;
	intellect = base_int + int_bonus;
	stamina = base_sta + sta_bonus;
	
	//sets ATK and DEF
	atk = strength * 2 + agility + equip_atk_bonus;
	def = agility * 2 + equip_def_bonus;
	
	//sets max health and mana
	max_health = 10 + stamina * 2;
	max_mana = 0;
	
	//sets crit chance
	crit = 5;
	
	//sets dodge and parry chances
	dodge = 5;
	parry = 5;
	
	//sets accuracy
	accuracy = 90;
}
void Rogue::update_stats() {
	//update equipment bonuses
	if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {equip_def_bonus = equipment[0].get_def_bonus();}
	else {equip_def_bonus = 0;}
	
	const int ATK_BONUS = 2;
	if (equipment[1].get_level() > 0 && equipment[2].get_level() > 0) {
		if (equipment[1].broken() && equipment[2].broken()) {equip_atk_bonus = 0;}
		else if (equipment[1].broken()) {
			if (equipment[2].get_subtype() == "Dagger") {equip_atk_bonus = equipment[2].get_atk_bonus() * ATK_BONUS;}
			else {equip_atk_bonus = equipment[2].get_atk_bonus();}
		}
		else if (equipment[2].broken()) {
			if (equipment[1].get_subtype() == "Dagger") {equip_atk_bonus = equipment[1].get_atk_bonus() * ATK_BONUS;}
			else {equip_atk_bonus = equipment[1].get_atk_bonus();}
		}
		else {
			int weapon_gain1, weapon_gain2;
			if (equipment[1].get_subtype() == "Dagger") {weapon_gain1 = equipment[1].get_atk_bonus() * ATK_BONUS;}
			else {weapon_gain1 = equipment[1].get_atk_bonus();}
			if (equipment[2].get_subtype() == "Dagger") {weapon_gain2 = equipment[2].get_atk_bonus() * ATK_BONUS;}
			else {weapon_gain2 = equipment[2].get_atk_bonus();}
			equip_atk_bonus = weapon_gain1 + weapon_gain2;
		}
	}
	else if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
		if (equipment[1].get_subtype() == "Dagger") {equip_atk_bonus = equipment[1].get_atk_bonus() * ATK_BONUS;}
		else {equip_atk_bonus = equipment[1].get_atk_bonus();}
	}
	else if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
		if (equipment[2].get_subtype() == "Dagger") {equip_atk_bonus = equipment[2].get_atk_bonus() * ATK_BONUS;}
		else {equip_atk_bonus = equipment[2].get_atk_bonus();}
	}
	else {equip_atk_bonus = 0;}
	
	//establish attribute bonuses
	str_bonus = agi_bonus = int_bonus = sta_bonus = 0; //initialize
	if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
		if (equipment[0].get_subtype() == "Leather") {agi_bonus += equipment[0].get_agi_bonus();}
		else if (equipment[0].get_subtype() == "Cloth") {int_bonus += equipment[0].get_int_bonus();}
		sta_bonus += equipment[0].get_sta_bonus();
	}
	
	if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
		if (equipment[1].get_subtype() == "Sword") {str_bonus += equipment[1].get_str_bonus();}
		else if (equipment[1].get_subtype() == "Dagger") {agi_bonus += equipment[1].get_agi_bonus();}
		else if (equipment[1].get_subtype() == "Staff") {int_bonus += equipment[1].get_int_bonus();}
		sta_bonus += equipment[1].get_sta_bonus();
	}
	
	if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
		if (equipment[2].get_subtype() == "Sword") {str_bonus += equipment[2].get_str_bonus();}
		else if (equipment[2].get_subtype() == "Dagger") {agi_bonus += equipment[2].get_agi_bonus();}
		else if (equipment[2].get_subtype() == "Staff") {int_bonus += equipment[2].get_int_bonus();}
		sta_bonus += equipment[2].get_sta_bonus();
	}
	
	//update attributes
	strength = base_str + str_bonus;
	agility = base_agi + agi_bonus + equip_atk_bonus / ATTRIB_BONUS;
	intellect = base_int + int_bonus;
	stamina = base_sta + sta_bonus;
	
	//sets ATK and DEF
	atk = agility * 2 + strength + equip_atk_bonus;
	def = agility + equip_def_bonus;
	
	//sets max health and mana
	max_health = 10 + stamina * 2;
	max_mana = 0;
	
	const int CRIT_LEVEL_BONUS = 3;
	const int PARRY_WEAPON_BONUS = 5;
	//sets crit and parry chance
	if (equipment[1].get_level() > 0 && equipment[2].get_level() > 0) {
		if (equipment[1].broken() && equipment[2].broken()) {
			crit = 10;
			parry = 5;
		}
		else if (equipment[1].broken()) {
			crit = 10 + CRIT_LEVEL_BONUS * level;
			parry = 5 + PARRY_WEAPON_BONUS;
		}
		else if (equipment[2].broken()) {
			crit = 10 + CRIT_LEVEL_BONUS * level;
			parry = 5 + PARRY_WEAPON_BONUS;
		}
		else {
			crit = 10 + CRIT_LEVEL_BONUS * 2 * level;
			parry = 5 + PARRY_WEAPON_BONUS * 2;
		}
	}
	else if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
		crit = 10 + CRIT_LEVEL_BONUS * level;
		parry = 5 + PARRY_WEAPON_BONUS;
	}
	else if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
		crit = 10 + CRIT_LEVEL_BONUS * level;
		parry = 5 + PARRY_WEAPON_BONUS;
	}
	else {
		crit = 10;
		parry = 5;
	}
	
	//sets dodge chance and accuracy
	dodge = 10;
	accuracy = 90;
}
void Mage::update_stats() {
	//update equipment bonuses
	if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {equip_def_bonus = equipment[0].get_def_bonus();}
	else {equip_def_bonus = 0;}
	
	if (equipment[1].get_level() > 0 && equipment[2].get_level() > 0) {
		if (equipment[1].broken() && equipment[2].broken()) {equip_atk_bonus = equip_mana_bonus = 0;}
		else if (equipment[1].broken()) {equip_mana_bonus = equipment[2].get_mana_bonus();}
		else if (equipment[2].broken()) {
			equip_mana_bonus = equipment[1].get_mana_bonus();
			equip_atk_bonus = equipment[1].get_atk_bonus();
		}
		else {
			int weapon_gain = equipment[1].get_atk_bonus();
			int offhand_gain = equipment[2].get_mana_bonus();
			if (equipment[1].get_subtype() == "Staff") {
				equip_mana_bonus = weapon_gain + offhand_gain;
			}
			else {equip_mana_bonus = offhand_gain;}
			equip_atk_bonus = weapon_gain;
		}
	}
	else if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
		equip_mana_bonus = equipment[1].get_mana_bonus();
		equip_atk_bonus = equipment[1].get_atk_bonus();
	}
	else if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
		equip_mana_bonus = equipment[2].get_mana_bonus();
	}
	else {equip_atk_bonus = equip_mana_bonus = 0;}
	
	//establish attribute bonuses
	str_bonus = agi_bonus = int_bonus = sta_bonus = 0; //initialize
	if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
		int_bonus += equipment[0].get_int_bonus();
		sta_bonus += equipment[0].get_sta_bonus();
	}
	
	if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
		if (equipment[1].get_subtype() == "Sword") {str_bonus += equipment[1].get_str_bonus();}
		else if (equipment[1].get_subtype() == "Dagger") {agi_bonus += equipment[1].get_agi_bonus();}
		else if (equipment[1].get_subtype() == "Staff") {int_bonus += equipment[1].get_int_bonus();}
		sta_bonus += equipment[1].get_sta_bonus();
	}
	
	if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
		int_bonus += equipment[2].get_int_bonus();
		sta_bonus += equipment[2].get_sta_bonus();
	}
	
	//update attributes
	strength = base_str + str_bonus;
	agility = base_agi + agi_bonus;
	intellect = base_int + int_bonus + equip_mana_bonus / ATTRIB_BONUS;
	stamina = base_sta + sta_bonus;
	
	//sets ATK and DEF
	atk = strength + agility + equip_atk_bonus;
	def = agility * 2 + equip_def_bonus;
	
	//sets max health and mana
	max_health = 10 + stamina * 2;
	max_mana = intellect * 2 + equip_mana_bonus;
	
	//sets crit chance
	crit = 5;
	
	//sets dodge and parry chances
	dodge = 5;
	parry = 5;
	
	//sets accuracy
	accuracy = 90;
	
	//updates spellpower
	spellpower = intellect / ATTRIB_BONUS;
	for (int i = 0; i < SPELL_COUNT; i++) {
		int x = spellbook[i].get_magnitude();
		spellbook[i].load_magnitude(x + spellpower);
	}
}

//EQUIPMENT MANIPULATION
void Player::equipment_menu() {
	Item *inv;
	
	equipment_menu_start:
	view_equipment();
	
	cout << "\nWhat would you like to do?\n" << endl;
	
	int count = 1;
	cout << count << ". Equip an item" << endl;
	count++;
	cout << count << ". Unequip an item" << endl;
	count++;
	cout << count << ". View detailed information about an item" << endl;
	count++;
	cout << count << ". Leave menu" << endl;
	
	int equipable_items_count = num_of_equipable_items();
	bool no_equipment = true;
	for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
		if (equipment[i].get_level() > 0) {no_equipment = false;}
	}
	
	int choice;
	while (true) {
		choice = GetInt();
		switch (choice) {
			case 1:
				if (equipable_items_count > 0) {
					int equip_choice;
					while (true) {
						view_equipment();
						cout << endl;
						inv = view_equipable_items();
						cout << equipable_items_count + 1 << ". Cancel request" << endl;
						cout << "\nWhich item would you like to equip?" << endl;
						equip_choice = GetInt();
						equip_choice--;
						if (equip_choice < equipable_items_count && equip_choice >= 0) {
							if (inv[equip_choice].get_level() <= level) {
								equip_item(inv[equip_choice]);
								goto equipment_menu_start;
							}
							else {
								cout << "You aren't at a sufficient level to equip this!" << endl;
								Pause();
								Clear();
							}
						}
						else if (equip_choice == equipable_items_count) {goto equipment_menu_start;}
						else {cout << "Please enter a number between 1 and " << equipable_items_count + 1 << ": ";}
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
			case 2:
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
					if (unequip_choice < EQUIPMENT_SLOTS && unequip_choice >= 0) {
						unequip_item(equipment[unequip_choice]);
						goto equipment_menu_start;
					}
					else if (unequip_choice == EQUIPMENT_SLOTS) {
						goto equipment_menu_start;
					}
				}
				break;
			case 3:
				if (no_equipment) {
					cout << "You have nothing equipped; there is no detail to view." << endl;
					Pause();
					goto equipment_menu_start;
				}
				view_equipment();
				cout << EQUIPMENT_SLOTS + 1 << ". Cancel request" << endl;
				cout << "\nWhich item would you like to view detailed information about?" << endl;
				int info_choice;
				info_choice = GetInt();
				info_choice--;
				equipment[info_choice].view_stats(player_class);
				goto equipment_menu_start;
				break;
			case 4:
				return;
				break;
			default:
				cout << "Please enter a number between 1 and " << count << ": ";
				break;
		}
	}
}
void Warrior::unequip_item(Item item) {
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
			equipment[2].set_item("Shield", "Shield", 0);
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
void Warrior::equip_item(Item item) {
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
void Rogue::unequip_item(Item item) {
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
void Rogue::equip_item(Item item) {
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
void Mage::unequip_item(Item item) {
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
void Mage::equip_item(Item item) {
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

//HEALTH AND MANA MANIPULATION
void Player::restore_health(int x) {
	current_health += x;
	if (current_health > max_health) {restore_health_to_max();}
}
void Mage::restore_mana(int x) {
	current_mana += x;
	if (current_mana > max_mana) {restore_mana_to_max();}
}

//INVENTORY MANIPULATION
void Player::inc_inventory(Item item) {
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
void Player::dec_inventory(Item item) {
	for (int i = 0; i < items_in_inventory; i++) {
		if (inventory[i].get_name() == item.get_name()) {
			inventory[i].dec_quantity();
			if (inventory[i].get_quantity() == 0) {
				remove_from_inventory(inventory[i]);
			}
		}
	}
}
void Player::add_to_inventory(Item item) {
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
void Player::remove_from_inventory(Item item) {
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
int Player::num_of_equipable_items() {
	int count = 0;
	for (int i = 0; i < items_in_inventory; i++) {
		if (inventory[i].equipable()) {count++;}
	}
	return count;
}
bool Player::use_item() {
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
	use_item_choice:
	Clear();
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
		if (choice < count && choice >= 0) {
			//determine change to health / mana
			if (potions[choice].get_level() <= level) {
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
			}
			else {
				cout << "You aren't at a sufficient level to use this item!" << endl;
				Pause();
				goto use_item_choice;
			}
			Pause();
			//decrement item from inventory
			dec_inventory(potions[choice]);
			return true;
		}
		else if (choice == count) {return false;} //cancels out
		else {cout << "Please enter a number between 1 and " << count + 1 << ": ";}
	}
}

//COMBAT
void Player::take_damage(int attack, int accuracy, int critical, string creature_name) {
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
bool Player::is_dead() {
	if (current_health <= 0) {return true;}
	else {return false;}
}
void Player::dies() {
	cout << "You've died!" << endl;
	lose_exp(50);
	for (int i = 0; i < EQUIPMENT_SLOTS; i++) {equipment[i].damage(20);}
	Pause();
	if (exp < LEVEL_INCREMENT * (level - 1)) {exp = LEVEL_INCREMENT * (level - 1);}
	
	while (boosted > 0) {
		if (player_class == "Warrior") {lower_block();}
		else if (player_class == "Rogue") {lower_atk();}
		else if (player_class == "Mage") {lower_def();}
	}
}
void Warrior::boost_block() {
	block += 30;
	boosted = 1;
	cout << "You've boosted your block chance by 30%!" << endl;
}
void Warrior::lower_block() {
	block -= 30;
	boosted = 0;
}
void Rogue::boost_atk() {
	atk *= 2;
	boosted = 1;
	cout << "You've doubled your ATK!" << endl;
}
void Rogue::lower_atk() {
	atk /= 2;
	boosted = 0;
}
void Mage::boost_def(int x) {
	def += x;
	boosted += 1;
}
void Mage::lower_def() {
	def -= spellbook[2].get_magnitude();
	boosted -= 1;
}
int Mage::cast_spell(bool in_combat) {
	cast_spell_start:
	Clear();
	cout << "What spell would you like to cast?" << endl;
	view_spells();
	cout << SPELL_COUNT + 1 << ". Cancel request (go to the previous menu)" << endl;
	
	int spell_dmg = 0;
	int choice;
	while (true) {
		choice = GetInt();
		choice--;
		if (choice < SPELL_COUNT && choice >= 0) {
			if (spellbook[choice].get_cost() > current_mana) {
				cout << "You don't have enough mana to cast this. YOU NEED MORE JUICE!!!" << endl;
				Pause();
				goto cast_spell_start;
			}
			else {
				if (spellbook[choice].get_type() == "Offensive") {
					if (in_combat == true) {
						cout << "You cast " << spellbook[choice].get_name() << "!" << endl;
						spell_dmg = spellbook[choice].get_magnitude();
						lose_mana(spellbook[choice].get_cost());
						did_cast_spell = true;
					}
					else {
						cout << "You can't cast " << spellbook[choice].get_name() << " out of combat!" << endl;
						Pause();
						goto cast_spell_start;
					}
				}
				else if (spellbook[choice].get_type() == "Healing") {
					cout << "You cast " << spellbook[choice].get_name() << "!" << endl;
					restore_health(spellbook[choice].get_magnitude());
					cout << "You've restored " << spellbook[choice].get_magnitude() << " health!" << endl;
					Pause();
					lose_mana(spellbook[choice].get_cost());
					did_cast_spell = true;
				}
				else if (spellbook[choice].get_type() == "Defensive") {
					if (in_combat == true) {
						cout << "You cast " << spellbook[choice].get_name() << "!" << endl;
						boost_def(spellbook[choice].get_magnitude());
						cout << "You've boosted your DEF by " << spellbook[choice].get_magnitude() << endl;
						Pause();
						lose_mana(spellbook[choice].get_cost());
						did_cast_spell = true;
					}
					else {
						cout << "You can't cast " << spellbook[choice].get_name() << " out of combat!" << endl;
						Pause();
						goto cast_spell_start;
					}
				}
				return spell_dmg;
			}
		}
		else if (choice == SPELL_COUNT) {
			did_cast_spell = false;
			return spell_dmg;
		}
		else {cout << "Please enter a number between 1 and " << SPELL_COUNT + 1 << ": ";}
	}
}

//VIEW FUNCTIONS
void Player::view_attrib_description() {
	int choice;
	while (true) {
		Clear();
		cout << "What attribute would you like to learn more about?\n" << endl;
		int count = 1;
		cout << count << ". Strength" << endl;
		count++;
		cout << count << ". Agility" << endl;
		count++;
		cout << count << ". Intellect" << endl;
		count++;
		cout << count << ". Stamina" << endl;
		count++;
		cout << count << ". Return to the previous menu" << endl;
		
		view_attrib_description_choice:
		choice = GetInt();
		switch (choice) {
			case 1:
				cout << "STRENGTH:" << endl;
				cout << description_strength << endl;
				Pause();
				continue;
				break;
			case 2:
				cout << "AGILITY:" << endl;
				cout << description_agility << endl;
				Pause();
				continue;
				break;
			case 3:
				cout << "INTELLECT:" << endl;
				cout << description_intellect << endl;
				Pause();
				continue;
				break;
			case 4:
				cout << "STAMINA" << endl;
				cout << description_stamina << endl;
				Pause();
				continue;
				break;
			case 5:
				return;
				break;
			default:
				cout << "Please enter a number between 1 and " << count << ": ";
				goto view_attrib_description_choice;
				break;
		}
	}
}
void Player::view_inventory() {
	view_inventory_start:
	Clear();
	if (items_in_inventory == 0) {
		cout << "You have no items in your inventory." << endl;
		Pause();
		return;
	}
	else {
		cout << "You have the following items in your inventory:" << endl;
		for (int i = 0; i < items_in_inventory; i++) {
			cout << i + 1 << ". " << inventory[i].get_name() << " (Requires Level " << inventory[i].get_level() << ") x" << inventory[i].get_quantity();
			if (inventory[i].equipable()) {cout << " - can be equipped" << endl;}
			else {cout << endl;}
		}
	}
	cout << endl;
	view_gold();
	
	cout << "\nWould you like to view detailed information about an item (Y/N)? ";
	char decision;
	while (true) {
		decision = GetChar();
		if (decision == 'Y' || decision == 'y') {
			cout << "\nWhich item would you like to view detailed information about?" << endl;
			int info_choice;
			while (true) {
				info_choice = GetInt();
				info_choice--;
				if (info_choice < items_in_inventory && info_choice >= 0) {
					inventory[info_choice].view_stats(player_class);
					goto view_inventory_start;
				}
				else {cout << "Please enter a number between 1 and " << items_in_inventory << ": ";}
			}
		}
		else if (decision == 'N' || decision == 'n') {return;}
		else {cout << "Please enter Y or N: ";}
	}
}
void Player::view_equipment() {
	Clear();
	cout << "You have the following equipped:" << endl;
	for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
		cout << i + 1 << ". " << equipment[i].get_type() << " slot: " << equipment[i].get_name();
		if (equipment[i].broken()) {cout << " - BROKEN";}
		else if (equipment[i].get_level() > 0) {cout << " - " << 100 - equipment[i].get_damage() << "/100";}
		cout << endl;
	}
}
Item* Player::view_equipable_items() {
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
void Player::view_quest_log() {
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
	Pause();
}
void Player::view_attributes() {
	Clear();
	cout << "Your current attributes are: " << endl;
	cout << "Strength: " << strength << endl;
	cout << "Agility: " << agility << endl;
	cout << "Intellect: " << intellect << endl;
	cout << "Stamina: " << stamina << endl;
}
void Player::view_stats() {
	Clear();
	cout << "Your current stats are: " << endl;
	cout << "ATK: " << atk << endl;
	cout << "DEF: " << def << endl;
	cout << "Health: " << current_health << "/" << max_health << endl;
	cout << "\nExperience: " << exp << "/" << exp_to_next_level << endl;
	Pause();
}
void Mage::view_stats() {
	Clear();
	cout << "Your current stats are: " << endl;
	cout << "ATK: " << atk << endl;
	cout << "DEF: " << def << endl;
	cout << "Health: " << current_health << "/" << max_health << endl;
	cout << "Mana: " << current_mana << "/" << max_mana << endl;
	cout << "Spellpower: " << spellpower << endl;
	cout << "\nExperience: " << exp << "/" << exp_to_next_level << endl;
	Pause();
}
void Mage::view_spells() {
	for (int i = 0; i < SPELL_COUNT; i++) {
		cout << i + 1 << ". " << spellbook[i].get_name() << " - " << spellbook[i].get_cost() << " mana" << endl;
	}
}
void Player::view_achievements() {
	cout << achievement_points << "/" << game_achievements.get_total_points() << " achievement score\n" << endl;
	cout << achievement_count << "/" << game_achievements.get_total_count() << " achievements earned\n" << endl;
}

//GOLD-EXP MANIPULATION
void Player::add_gold(int x) {
	gold += x;
	cout << "You've collected " << x << " gold!" << endl;
}
void Player::gain_exp(int exp_gain) {
	exp += exp_gain;
	cout << "You've gained " << exp_gain << " experience!" << endl;
	if (exp >= exp_to_next_level && level < MAX_LEVEL) {level_up();}
}
void Player::lose_exp(int exp_loss) {
	exp -= exp_loss;
	cout << "You've lost " << exp_loss << " experience!" << endl;
}
void Player::earn_explore_bonus(int area) {
	cout << "You've explored the entire area!" << endl;
	gain_exp(area);
	add_gold(area * 2);
	Pause();
}

//QUESTING
void Player::quest_calculation_and_acceptance() {
	for (int i = 0; i < level; i++) {
		if (boss_quests_complete[i] == false && quest_active == false) {//haven't completed it yet
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
bool Player::quest_complete() {
	if (boss_quest.get_complete()) {return true;}
	else {return false;}
}
void Player::turn_in_quest() {
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
void Player::check_quest_at_town_entrance() {
	if (quest_active && quest_complete()) {
		turn_in_quest();
		Pause();
		Clear();
		cout << "Now that that's done... ";
	}
}

//ACHIEVEMENTS
void Player::achievement_menu() {
	achievement_menu_start:
	Clear();
	view_achievements();
	
	cout << "What would you like to do?" << endl;
	int count = 1;
	cout << count << ". View earned achievements" << endl;
	count++;
	cout << count << ". View unearned achievements" << endl;
	count++;
	cout << count << ". Return to previous menu" << endl;
	
	int choice = GetInt();
	switch (choice) {
		case 1:
			game_achievements.view_earned_achievements();
			goto achievement_menu_start;
			break;
		case 2:
			game_achievements.view_unearned_achievements();
			goto achievement_menu_start;
			break;
		case 3:
			return;
			break;
	}
}
void Player::earn_achievement(Achievement &achieve) {
	if (game_achievements.check_if_earned(achieve) == false) {
		game_achievements.earn_achievement(achieve);
		cout << "ACHIEVEMENT EARNED: " << achieve.get_name() << "!" << endl;
		cout << "Points earned: " << achieve.get_points() << endl;
		achievement_count += 1;
		achievement_points += achieve.get_points();
	}
}

//attribute overview and manipulation
void Player::attribs_menu() {
	Clear();
	if (points > 0) {
		cout << "You have unspent points!\n" << endl;
		Pause();
	}
	attribs_menu_start:
	view_attributes();
	
	cout << "\nWhat would you like to do? " << endl;
	if (points > 0) {cout << "(A)llocate points to attributes" << endl;}
	else if (points == 0) {cout << "(R)eturn without doing anything" << endl;}
	else {cout << "points out of bounds (attribs_menu)" << endl;}
	cout << "(L)earn more about an attribute and how it affects your class" << endl;
	
	char choice;
	while (true) {
		choice = GetChar();
		if (points > 0) {
			switch (choice) {
				case 'L':
				case 'l':
					view_attrib_description();
					goto attribs_menu_start;
					break;
				case 'A':
				case 'a':
					allocate_points();
					goto attribs_menu_start;
					break;
				default:
					cout << "Please enter L or A: ";
					break;
			}
		}
		else if (points == 0) {
			switch (choice) {
				case 'L':
				case 'l':
					view_attrib_description();
					goto attribs_menu_start;
					break;
				case 'R':
				case 'r':
					return;
					break;
				default:
					cout << "Please enter L or R: ";
					break;
			}
		}
		else {cout << "points out of bounds (attribs_menu)" << endl;}
	}
}

//LEVELING
void Player::allocate_points() {
	allocate_points_start:
	while (points > 0) {
		//overview
		view_attributes();
	
		cout << "\nYou have " << points << " points remaining. ";
		cout << "Please select an attribute to add to:" << endl;
		
		//list of available attributes to add to
		int count = 1;
		cout << count << ". Strength" << endl;
		count++;
		cout << count << ". Agility" << endl;
		count++;
		cout << count << ". Intellect" << endl;
		count++;
		cout << count << ". Stamina" << endl;
		int choice;
		while (true) {
			choice = GetInt();
			switch (choice) {
				case 1:
					base_str++;
					update_stats();
					points--;
					goto allocate_points_start;
					break;
				case 2:
					base_agi++;
					update_stats();
					points--;
					goto allocate_points_start;
					break;
				case 3:
					base_int++;
					update_stats();
					points--;
					goto allocate_points_start;
					break;
				case 4:
					base_sta++;
					update_stats();
					points--;
					goto allocate_points_start;
					break;
				default:
					cout << "Please enter a number between 1 and " << count << ": ";
					break;
			}
		}
	}
	view_attributes();
	cout << "\nAll available points have been allocated." << endl;
	Pause();
}
void Player::level_up() {
	cout << "You've leveled up! ";
	level++;
	if (level == MAX_LEVEL) {exp_to_next_level = 1000000;}
	else {exp_to_next_level = LEVEL_INCREMENT * level * level;}
	cout << "You are now Level " << level << "!" << endl;
	points = 5;
	Pause();
	attribs_menu();
}
void Mage::level_up() {
	Player::level_up();
	for (int i = 0; i < SPELL_COUNT; i++) {spellbook[i].inc_level();}
}

//GET FUNCTIONS
int* Player::get_base_attribs() {
	base_attribs[0] = base_str;
	base_attribs[1] = base_agi;
	base_attribs[2] = base_int;
	base_attribs[3] = base_sta;
	return base_attribs;
}
int* Player::get_exp_gold_inv_count() {
	exp_gold_inv_count[0] = exp;
	exp_gold_inv_count[1] = gold;
	exp_gold_inv_count[2] = items_in_inventory;
	return exp_gold_inv_count;
}

//LOAD FUNCTIONS
void Player::load_inventory(Item* &items) {
	inventory = new Item[items_in_inventory];
	for (int i = 0; i < items_in_inventory; i++) {inventory[i] = items[i];}
}
void Player::load_player(int* &saved_base_attribs, int* &saved_misc, bool* &saved_boss_quests, Item* &saved_equipment) {
	//load base attributes
	for (int i = 0; i < BASE_ATTRIB_COUNT; i++) {base_attribs[i] = saved_base_attribs[i];}
	base_str = base_attribs[0];
	base_agi = base_attribs[1];
	base_int = base_attribs[2];
	base_sta = base_attribs[3];
	
	//load exp, gold, health and mana meters, and number of items in inventory
	for (int i = 0; i < MISC_COUNT; i++) {exp_gold_inv_count[i] = saved_misc[i];}
	exp = exp_gold_inv_count[0];
	gold = exp_gold_inv_count[1];
	items_in_inventory = exp_gold_inv_count[2];
	
	//load boss quest completion history
	for (int i = 0; i < MAX_LEVEL; i++) {boss_quests_complete[i] = saved_boss_quests[i];}
	
	//load equipment
	for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
		equipment[i].set_item(saved_equipment[i].get_type(), saved_equipment[i].get_subtype(), saved_equipment[i].get_level());
	}
	
	//load level
	level = 1;
	exp_to_next_level = LEVEL_INCREMENT * level * level;
	while (exp >= exp_to_next_level && level < MAX_LEVEL) {
		level++;
		exp_to_next_level = LEVEL_INCREMENT * level * level;
	}
	if (level == MAX_LEVEL) {exp_to_next_level = 1000000;}
}
void Player::load_achievements(Achievement* &earned, int earned_count) {
	achievement_count = earned_count;
	achievement_points = 0;
	
	for (int i = 0; i < earned_count; i++) {achievement_points += earned[i].get_points();}
}