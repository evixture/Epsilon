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

	//aggression
	float aggression;
	//caution
	//fear
	//stealth

	AICreature(Creature creature, TCODMap* fovMap);

	void move();

	//world interact
	void pickUpItem(); //should eventually chenge to pure
	void dropItem();

	//actions
	void reload();
	void changeFireMode();
	void equipArmor();
	void useMelee();
	void takeDamage(int damage);

	void updateTools();

	//need to include functions that only calls from selected item?

	void update(); //virtual updates the creature
	void render(const std::shared_ptr<Pane>& pane) const; // virtual renders the creature

private:

	void behave();
	void act();

	bool inEffectiveRange();

	void decayInterest();
	void reactToSounds();

	Clock interestDecayClock;
	Clock reactionFireClock;
};