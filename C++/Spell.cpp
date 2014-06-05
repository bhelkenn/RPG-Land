#include"Spell.h"
using namespace std;

//INITIALIZATION
void Spell::set_spell(string spell_name, string spell_type, int mag, int spell_cost, int player_level) {
	name = spell_name;
	type = spell_type;
	level = player_level;
	magnitude = mag;
	cost = spell_cost;
}

//LEVELING
void Spell::inc_level() {
	level++;
	update_spell();
}
void Spell::update_spell() {
	magnitude /= (level - 1);
	magnitude *= level;
	cost /= (level - 1);
	cost *= level;
}