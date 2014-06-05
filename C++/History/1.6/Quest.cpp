#include"Quest.h"
using namespace std;

//CONSTRUCTORS
Quest::Quest(int quest_level) {
	level = quest_level;
	exp = 50 * level;
	gold = 100 * level;
	set_boss_names();
	set_quest();
	complete = false;
}

//INITIALIZATION
void Quest::set_boss_names() {
	for (int i = 0; i < MAX_LEVEL; i++) {
		Boss boss(i + 1);
		boss_names[i] = boss.get_name();
	}
}
void Quest::set_quest() {
	name = "WANTED: " + boss_names[level - 1];
	if (level == 1) {
		description = "As you start to leave town, a woman comes running and screaming towards you from just outside the entrance to town. ";
		description += "As she nears, you notice that she is disheveled and manic, her clothes torn. You step into her path and stop her. ";
		description += "She stops, gasping, and tells you of the villanous " + boss_names[0] + " that attempted to destroy her virtue. You promise ";
		description += "to bring swift and brutal justice to " + boss_names[0] + " and return to the fair maiden. She swoons in your arms..... Yeah, ";
		description += "it's probably best to start bringing justice.";
	}
	else if (level >= 2) {
		description = "You've licked your wounds and recovered after your fight with " + boss_names[level - 2] + ". As you start to leave ";
		description += "town again, the woman from your previous adventure comes running towards you again. She tells you of the villanous ";
		description += boss_names[level - 1] + " that is now terrorizing damsels everywhere. You promise once more to bring swift and brutal ";
		description += "justice to " + boss_names[level - 1] + " and return to her. She hugs you, elated... It's probably best to get out there.";
	}
}