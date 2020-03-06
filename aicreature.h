#include "main.hpp"

struct AICreature : public Creature
{
	TCODPath path;

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

	void updateTools();

	//need to include functions that only calls from selected item?

	void update(); //virtual updates the creature
	void render(const std::shared_ptr<Pane>& pane) const; // virtual renders the creature

};