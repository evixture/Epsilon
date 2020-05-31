#include "main.hpp"

struct AICreature : public Creature
{
	TCODColor debugBGColor;

	//pathfinding and movement
	TCODPath path;
	int pathStep;

	Position3 pathfindPosition; //end point of the path
	Position3 lookPosition; //where the ai looks, will also be used for aiming
	Position3 focusPosition; //last known location of the player

	int moveSpeedMode;

	//interest
	float interestDecay;
	float visualInterest;
	float soundInterest;
	float soundInterestChange;

	float aggression;
	//caution	: effects allowable distance and agression floor, range from -1 to 1
	//	attitude, opinion
	//fear		: effects fleeing battle, needs node system set up, range from 0 to 1
	//stealth	: prob not needed but need for stances??

	AICreature(Creature creature, TCODMap* fovMap);

	void takeDamage(int damage);

	//need to include functions that only calls from selected item?

	void update(); //virtual updates the creature
	void render(const Pane& pane) const; // virtual renders the creature

private:
	bool inFov;

	void behave();
	void act();

	bool inEffectiveRange();

	float calcVisInt;
	void decayInterest();
	void reactToSounds();

	void move();

	//world interact
	void pickUpItem(); //should eventually change to pure
	void dropItem();

	//actions
	void reload();
	void changeFireMode();
	void equipArmor();
	void useMelee();

	void updateTools();

	Clock interestDecayClock;
	Clock reactionFireClock;

	friend struct ActionManager;
};