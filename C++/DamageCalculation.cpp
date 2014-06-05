#include"DamageCalculation.h"
using namespace std;

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