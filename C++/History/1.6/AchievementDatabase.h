#include"Achievement.h"
using namespace std;

#ifndef ACHIEVEMENTDATABASE_H
#define ACHIEVEMENTDATABASE_H

class AchievementDatabase {
public:
	AchievementDatabase();
	
	Achievement target_achievement(string, int);
	void earn_achievement(Achievement&);
	bool check_if_earned(Achievement&);
	
	//GET FUNCTIONS
	int get_total_count() {return total_count;}
	int get_total_points() {return total_points;}
	int get_earned_count() {return earned_count;}
	Achievement* get_earned_achievements();
	
	//VIEW FUNCTIONS
	void view_unearned_achievements();
	void view_earned_achievements();
	
	void load_achievements(Achievement*&, int);
private:
	//VARIABLES
	Achievement *all_achievements;
	int total_count, total_points, earned_count;
	
	//INITIALIZATION
	void set_total_count();
	void set_total_points();
	void init_all_achievements();
};

#endif