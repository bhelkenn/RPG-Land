#include"Map.h"
using namespace std;

//CONSTRUCTORS
Map::Map(int player_level) {
	level = player_level;
	size = 3 + level; //number of spaces in a row
	area = size * size;
	space = 'O';
	init_map();
	set_town();
	set_boss();
	explore_bonus_earned = false;
	north_flavor = "You move north; so far, the coast seems clear. Who's really scared of bunnies, anyway?";
	west_flavor = "You move west; so far, nothing has eaten you. Flowers can't eat you, right?";
	east_flavor = "You move east; so far, everything is fine. Grass just makes you itch, after all.";
	south_flavor = "You move south; so far, all you see are bees buzzing around the flowers. Oh man, BEES!?";
}
Dungeon::Dungeon(int dungeon_level) {
	level = dungeon_level;
	complexity = STAGES * level;
	stage[0] = "You can't see anything in front of you, but you know your quarry lies in wait further in.";
	stage[1] = "As you move further in, you notice a lightening of the darkness surrounding you. You must be on the right track.";
	stage[2] = "You're able to start making shapes out of the darkness. You can see the floor, parts of the walls. You feel like you're getting close.";
	fork = "You come across what appears to be a fork. You pick a direction at random.";
	end = "Abruptly, you break into a large room. A strange light comes from the creature ";
	end += "before you, as it rears to attack!";
	init_flavor();
}

//INITIALIZATION
void Map::init_map() {
	map = new char[area];
	for (int i = 0; i < area; i++) {
		map[i] = space;
	}
}
void Map::draw_map() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << map[i * size + j] << " ";
		}
		cout << endl;
	}
}
void Dungeon::init_flavor() {
	if (level == 1) {
		entrance = "You enter a dark passage, hearing a distant grunting.";
		success = "You move forward, and encounter no resistance or barriers. ";
		success += "You hear groaning further down the passage. ";
		deadend = "As you move forward, you come up against a wall. It's a dead-end! ";
		deadend += "The echoes from the moans and grunts disorient you further as you attempt to backtrack.";
	}
	else if (level == 2) {
		entrance = "You enter a dank cave. The smell is almost overwhelming!";
		success = "You move forward, and encounter no resistance or barriers. ";
		success += "You smell something approaching that of manure. You believe it's coming ";
		success += "from the direction you just traveled.";
		deadend = "As you move forward, you come up against a wall. It's a dead-end! ";
		deadend += "Another wave of staggering stench buffets you. You close your eyes against it, ";
		deadend += "even though it's pitch-black. You back up, bumping into a wall, and trace it back ";
		deadend += "to the last fork.";
	}
	else if (level == 3) {
		entrance = "You enter a pitch-black grotto. You hearing mewling deeper in.";
		success = "You move forward, and encounter no resistance or barriers. ";
		success += "Mewls come from deeper in the grotto. You squint, trying to see something in the blackness.";
		deadend += "Following a particularly loud yowl, you slam against a wall and stagger. You shake your ";
		deadend += "and back up.";
	}
	else if (level == 4) {
		entrance = "You enter a dim cavern. You can barely see anything in front of you.";
		success = "Rumbles sound from the deep parts of the cavern. You edge forward warily.";
		deadend += "As you inch forward, the floor abruptly gives way. You fall into the depression. ";
		deadend += "You climb back out, brushing yourself off.";
	}
	else if (level == 5) {
		entrance = "You enter an utterly dark and humid hallway. You hear a distant laugh further in.";
		success = "You hear what sounds like a party further down the hallway. As you move forward, ";
		success += "you think you detect a lightening of the gloom around you.";
		deadend += "The hallway curves around, abruptly ending. You back up to the fork.";
	}
	else {cout << "Not a valid level (Dungeon)" << endl;}
}

//SET FUNCTIONS
void Map::set_town() {
	town = rand() % area;
	map[town] = 'T';
}
void Map::set_boss() {
	boss = rand() % area;
	while (boss == town) {boss = rand() % area;}
	boss_present = true;
}

//MOVING
int Map::move_player(int start, char direction) {
	int finish;
	if (direction == 'N') {finish = start - size;}
	if (direction == 'W') {finish = start - 1;}
	if (direction == 'E') {finish = start + 1;}
	if (direction == 'S') {finish = start + size;}
	
	if (map[start] != 'T') {map[start] = 'X';}
	map[finish] = '*';
	return finish;
}
//verifies that player can move in a particular direction
bool Map::can_move(int position, char direction) {
	if (direction == 'N') {
		if (position > size - 1) {return true;}
		else {return false;}
	}
	else if (direction == 'S') {
		if (position < area - size) {return true;}
		else {return false;}
	}
	else if (direction == 'W') {
		if (position % size > 0) {return true;}
		else {return false;}
	}
	else if (direction == 'E') {
		if (position % size != size - 1) {return true;}
		else {return false;}
	}
	else {
		cout << "Invalid direction chosen" << endl;
		return false;
	}
}
void Dungeon::move() {
	int success_chance = 70;
	int progress = 0;
	
	Clear();
	cout << entrance << endl;
	Pause();
	while (progress <= complexity) {
		cout << fork << endl;
		if (rand() % 101 < success_chance) {
			cout << success << endl;
			Pause();
			for (int i = 0; i < STAGES; i++) {
				if (progress == level * i) {
					Clear();
					cout << stage[i] << endl;
					Pause();
				}
			}
			progress++;
		}
		else {
			cout << deadend << endl;
			Pause();
		}
	}
	cout << end << endl;
}

void Map::enter_town(int position) {
	if (position == town) {
		cout << "You stumble back to the entrance of town. You can't believe you managed to survive out there "
		<< "amidst nature's chaos!" << endl;
		Pause();
	}
}

bool Map::explored_map() {
	for (int i = 0; i < area; i++) {
		if (map[i] == 'O') {return false;}
	}
	return true; //only is reached if none of the map coords are 'O'
}