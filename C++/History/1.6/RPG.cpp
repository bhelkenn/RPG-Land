#include<time.h> //used for calculating creature encounter and escape chances
#include"AchievementDatabase.h"
#include"Creature.h"
#include"DamageCalculation.h"
#include"Flavor.h"
#include"Item.h"
#include"Map.h"
#include"Player.h"
#include"Quest.h"
#include"SaveLoad.h"
#include"Shop.h"
#include"Spell.h"
using namespace std;

//FUNCTION PROTOTYPES
void player_menu(Player*&);
void creature_killed(Player*&, Creature&);
void boss_killed(Player*&, Map&, Boss&);
int fight_display_menu(Player*&);
void fight_creature(Player*&);
void fight_boss(Player*&, Map&);
void move_results(Player*&, Map&, int, string);
void wild_menu(Player*&);
void town_menu(Player*&);
void quit_game();

//MENUS + helper functions
void player_menu(Player* &player) {
	player_menu_start:
	Clear();

	cout << "Player menu:\n" << endl;
	cout << "What would you like to do?\n" << endl;
	
	int count = 1;
	cout << count << ".  View attributes" << endl;
	count++;
	cout << count << ".  View stats" << endl;
	count++;
	cout << count << ".  View inventory" << endl;
	count++;
	cout << count << ".  View quest log" << endl;
	count++;
	cout << count << ".  View achievements" << endl;
	count++;
	cout << count << ".  Use item" << endl;
	if (player->get_player_class() == "Mage") {
		count++;
		cout << count << ".  Cast a spell" << endl;
	}
	count++;
	cout << count << ".  Equipment" << endl;
	count++;
	cout << count << ".  Load / Save Player" << endl;
	count++;
	if (player->get_player_class() == "Mage") {cout << count << ". Exit menu" << endl;}
	else {cout << count << ".  Exit menu" << endl;}
	count++;
	cout << count << ". Quit game" << endl;
	
	int choice;
	while (true) {
		choice = GetInt();
		if (player->get_player_class() == "Mage") {
			switch (choice) {
				case 1:
					player->attribs_menu();
					goto player_menu_start;
					break;
				case 2:
					player->view_stats();
					goto player_menu_start;
					break;
				case 3:
					player->view_inventory();
					goto player_menu_start;
					break;
				case 4:
					player->view_quest_log();
					goto player_menu_start;
					break;
				case 5:
					player->achievement_menu();
					goto player_menu_start;
					break;
				case 6:
					player->use_item();
					goto player_menu_start;
					break;
				case 7:
					player->cast_spell(false);
					goto player_menu_start;
					break;
				case 8:
					player->equipment_menu();
					goto player_menu_start;
					break;
				case 9:
					save_or_load(player);
					goto player_menu_start;
					break;
				case 10:
					return;
					break;
				case 11:
					quit_game();
					goto player_menu_start;
					break;
				default:
					cout << "Please enter a number between 1 and " << count << ": ";
					break;
			}
		}
		else {
			switch (choice) {
				case 1:
					player->attribs_menu();
					goto player_menu_start;
					break;
				case 2:
					player->view_stats();
					goto player_menu_start;
					break;
				case 3:
					player->view_inventory();
					goto player_menu_start;
					break;
				case 4:
					player->view_quest_log();
					goto player_menu_start;
					break;
				case 5:
					player->achievement_menu();
					goto player_menu_start;
					break;
				case 6:
					player->use_item();
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
					quit_game();
					goto player_menu_start;
					break;
				default:
					cout << "Please enter a number between 1 and " << count << ": ";
					break;
			}
		}
	}
}
void creature_killed(Player* &player, Creature &creature) {
	Item *loot = creature.get_loot();
	int loot_count = creature.get_loot_count();
	while (player->get_boosted() > 0) {
		if (player->get_player_class() == "Warrior") {player->lower_block();}
		else if (player->get_player_class() == "Rogue") {player->lower_atk();}
		else if (player->get_player_class() == "Mage") {player->lower_def();}
	}
	cout << "You've killed the " << creature.get_name() << "!" << endl;
	player->gain_exp(creature.get_exp());
	player->add_gold(creature.get_money());
	if (creature.get_has_loot()) {
		for (int i = 0; i < loot_count; i++) {
			cout << "You've looted " << loot[i].get_name() << " from the " << creature.get_name() << "!" << endl;
			player->inc_inventory(loot[i]);
		}
	}
	Pause();
}
void boss_killed(Player* &player, Map &map, Boss &boss) {
	Item *loot = boss.get_loot();
	int loot_count = boss.get_loot_count();
	while (player->get_boosted() > 0) {
		if (player->get_player_class() == "Warrior") {player->lower_block();}
		else if (player->get_player_class() == "Rogue") {player->lower_atk();}
		else if (player->get_player_class() == "Mage") {player->lower_def();}
	}
	cout << "\nYou've killed " << boss.get_name() << "!" << endl;
	player->earn_achievement(player->target_achievement("Quest", boss.get_level()));
	player->gain_exp(boss.get_exp());
	player->add_gold(boss.get_money());
	for (int i = 0; i < loot_count; i++) {
		cout << "You've looted " << loot[i].get_name() << " from " << boss.get_name() << "!" << endl;
		player->inc_inventory(loot[i]);
	}
	map.killed_boss();
	player->complete_active_quest();
	Pause();
}
int fight_display_menu(Player* &player) {
	Clear();
	cout << "What would you like to do?" << endl;
	
	int count = 1;
	cout << count << ". Attack" << endl;
	count++;
	if (player->get_player_class() == "Warrior") {cout << count << ". Use Shield Wall ability" << endl;}
	else if (player->get_player_class() == "Rogue") {cout << count << ". Use Bloodlust ability" << endl;}
	else if (player->get_player_class() == "Mage") {cout << count << ". Cast a spell" << endl;}
	count++;
	cout << count << ". Use item" << endl;
	count++;
	cout << count << ". Run away\n" << endl;
	
	player->view_health();
	player->view_mana();
	
	return count;
}
void fight_creature(Player* &player) {
	Creature creature;
	creature.det_creature(player->get_level());
	
	cout << "You've encountered a " << creature.get_name() << "!\n" << endl;
	Pause();
	
	fight_creature_start:
	int count = fight_display_menu(player);
	
	cout << creature.get_name() << "'s health: " << creature.get_health() << endl;
	
	int spell_dmg;
	int choice;
	while (true) {
		choice = GetInt();
		switch (choice) {
			case 1:
				//damage creature
				creature.take_damage(player->get_atk(), player->get_accuracy(), player->get_crit(), player->get_name());
				if (creature.is_dead()) {
					creature_killed(player, creature);
					return;
				}
				//damage player
				player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
				if (player->is_dead()) {
					player->dies();
					return;
				}
				Pause();
				goto fight_creature_start;
				break;
			case 2:
				if (player->get_player_class() == "Warrior") {
					if (player->get_boosted() == 0) {player->boost_block();}
					else {
						cout << "This ability is already active!" << endl;
						Pause();
						goto fight_creature_start;
					}
					//damage player
					player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
				}
				else if (player->get_player_class() == "Rogue") {
					if (player->get_boosted() == 0) {player->boost_atk();}
					else {
						cout << "This ability is already active!" << endl;
						Pause();
						goto fight_creature_start;
					}
					//damage player
					player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
				}
				else if (player->get_player_class() == "Mage") {
					spell_dmg = player->cast_spell(true);
					if (player->get_did_cast_spell()) {
						//damage creature
						creature.take_spell_dmg(spell_dmg, player->get_accuracy(), player->get_crit());
						if (creature.is_dead()) {
							creature_killed(player, creature);
							return;
						}
						//damage player
						player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
					}
				}
				goto fight_creature_start;
				break;
			case 3:
				if (player->use_item()) {
					//damage player
					player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
				}
				goto fight_creature_start;
				break;
			case 4:
				if (rand() % 101 < creature.get_escape_chance()) {
					cout << "You flee like a coward! Were you that scared of a " << creature.get_name() << "?" << endl;
					Pause();
					return;
				}
				else {
					cout << "You tried to run, but the " << creature.get_name() << " bit your ankle and you fell over. LAME." << endl;
					Pause();
					//damage player
					player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
					goto fight_creature_start;
				}
				break;
			default:
				cout << "Please enter a number between 1 and " << count << ": ";
				break;
		}
	}
}
void fight_boss(Player* &player, Map &map) {
	Boss boss(map.get_level());
	cout << "You've encountered " << boss.get_name() << "!\n" << endl;
	Pause();
	
	fight_boss_start:
	int count = fight_display_menu(player);
	
	cout << boss.get_name() << "'s health: " << boss.get_health() << endl;
	
	int spell_dmg;
	int choice;
	while (true) {
		choice = GetInt();
		switch (choice) {
			case 1:
				//damage boss
				boss.take_damage(player->get_atk(), player->get_accuracy(), player->get_crit(), player->get_name());
				if (boss.is_dead()) {
					boss_killed(player, map, boss);
					return;
				}
				//damage player
				cout << boss.get_flavor() << endl;
				player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
				if (player->is_dead()) {
					player->dies();
					return;
				}
				Pause();
				goto fight_boss_start;
				break;
			case 2:
				if (player->get_player_class() == "Warrior") {
					if (player->get_boosted() == 0) {player->boost_block();}
					else {
						cout << "This ability is already active!" << endl;
						Pause();
						goto fight_boss_start;
					}
					//damage player
					cout << boss.get_flavor() << endl;
					player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
				}
				else if (player->get_player_class() == "Rogue") {
					if (player->get_boosted() == 0) {player->boost_atk();}
					else {
						cout << "This ability is already active!" << endl;
						Pause();
						goto fight_boss_start;
					}
					//damage player
					cout << boss.get_flavor() << endl;
					player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
				}
				else if (player->get_player_class() == "Mage") {
					spell_dmg = player->cast_spell(true);
					if (player->get_did_cast_spell()) {
						//damage boss
						boss.take_spell_dmg(spell_dmg, player->get_accuracy(), player->get_crit());
						if (boss.is_dead()) {
							boss_killed(player, map, boss);
							return;
						}
						//damage player
						cout << boss.get_flavor() << endl;
						player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
					}
				}
				goto fight_boss_start;
				break;
			case 3:
				if (player->use_item()) {
					//damage player
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
			case 4:
				if (rand() % 101 < boss.get_escape_chance()) {
					cout << "You flee like a coward! Were you that scared of " << boss.get_name() << "?" << endl;
					Pause();
					return;
				}
				else {
					cout << "You tried to run, but " << boss.get_name() << " bit your ankle and you fell over. LAME." << endl;
					Pause();
					//damage player
					player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
				}
				Pause();
				goto fight_boss_start;
				break;
			default:
				cout << "Please enter a number between 1 and " << count << ": ";
				break;
		}
	}
}
void move_results(Player* &player, Map &map, int position, char direction) {
	int creature_chance = 35;
	if (position == map.get_boss() && map.get_boss_present()) {
		Dungeon instance(map.get_level());
		instance.move();
		fight_boss(player, map);
		player->update_stats();
	}
	else if (rand() % 101 < creature_chance) {
		fight_creature(player);
		player->update_stats();
	}
	else {
		if (direction == 'N') {cout << map.get_north_flavor() << endl;}
		if (direction == 'W') {cout << map.get_west_flavor() << endl;}
		if (direction == 'E') {cout << map.get_east_flavor() << endl;}
		if (direction == 'S') {cout << map.get_south_flavor() << endl;}
		Pause();
	}
	if (map.explored_map() && map.get_explore_bonus_earned() == false) {
		player->earn_explore_bonus(map.get_area());
		map.set_explore_bonus_earned();
	}
}
void wild_menu(Player* &player) {
	Map map(player->get_boss_quest().get_level()); //initialize the map based on level of quest boss
	int position = map.get_town(); //start position in at the town
	
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
		//validation
		if (choice == 'n') {choice = 'N';}
		if (choice == 'w') {choice = 'W';}
		if (choice == 'e') {choice = 'E';}
		if (choice == 's') {choice = 'S';}
		
		if (choice == 'N' || choice == 'W' || choice == 'E' || choice == 'S') {
			if (map.can_move(position, choice)) {
				position = map.move_player(position, choice);
				if (position == map.get_town()) {
					map.enter_town(position);
					return;
				}
				move_results(player, map, position, choice);
				goto wild_menu_after_move;
			}
			else {
				cout << "You can't move ";
				if (choice == 'N') {cout << "north";}
				else if (choice == 'W') {cout << "west";}
				else if (choice == 'E') {cout << "east";}
				else if (choice == 'S') {cout << "south";}
				else {cout << "ERROR (choice not expected value (wild_menu)" << endl;}
				cout << ". Please try another direction: ";
			}
		}
		else if (choice == '1') {return;}
		else if (choice == '2') {
			player_menu(player);
			goto wild_menu_start;
		}
		else {cout << "Please enter N, W, E, S, 1, or 2: ";}
	}
}
void town_menu(Player* &player) {
	town_menu_start:
	Clear();
	
	cout << "You have entered town.\n" << endl;
	
	player->check_quest_at_town_entrance();
	
	Blacksmith town_blacksmith(player->get_level());
	Inn town_inn(player->get_level());
	
	cout << "What would you like to do?\n" << endl;
	
	int count = 1;
	cout << count << ". Go to the blacksmith" << endl;
	count++;
	cout << count << ". Go to the inn" << endl;
	count++;
	cout << count << ". Leave the town (enter the wild)" << endl;
	count++;
	cout << count << ". Player menu" << endl;
	
	int choice;
	while (true) {
		choice = GetInt();
		switch (choice) {
			case 1:
				town_blacksmith.menu(player);
				goto town_menu_start;
				break;
			case 2:
				town_inn.menu(player);
				goto town_menu_start;
				break;
			case 3:
				player->quest_calculation_and_acceptance();
				wild_menu(player);
				goto town_menu_start;
				break;
			case 4:
				player_menu(player);
				goto town_menu_start;
				break;
			default:
				cout << "Please enter a number between 1 and " << count << ": ";
		}
	}
}
void quit_game() {
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
			return;
		}
	}
}

int main() {
	srand((unsigned int)time(0)); //just set once to ensure random generation
	cout << "Welcome to:" << endl;
	cout << ".########.########..######....##.........###...##....#.########." << endl;
	cout << ".##.....#.##.....#.##....##...##........##.##..###...#.##.....##" << endl;
	cout << ".##.....#.##.....#.##.........##.......##...##.####..#.##.....##" << endl;
	cout << ".########.########.##...###...##......##.....#.##.##.#.##.....##" << endl;
	cout << ".##...##..##.......##....##...##......########.##..###.##.....##" << endl;
	cout << ".##....##.##.......##....##...##......##.....#.##...##.##.....##" << endl;
	cout << ".##.....#.##........######....#######.##.....#.##....#.########." << endl;
	
	cout << "\nThe land of pun and misadventure!\n" << endl;
	Pause();
	Player *player = new Player;
	choose_player(player);
	
	town_menu(player);
	
	return 0;
}