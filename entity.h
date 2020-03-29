#include "main.hpp"

struct Entity //generic entity base for use on map
{
	Position4 mapPosition; //the position of the entity on the map
	unsigned char ch; //character representation of the character
	TCODColor color; //foreground color of the entity
	std::string name; //string name of entity

	Entity(Position4 pos, int ch, std::string name, TCODColor color); //entity constructor that takes position, character, string name, and color

	virtual void update(); //virtual update entity
	virtual void render(const std::shared_ptr<Pane> &pane) const; //virtual render entity

protected:
	Position3 renderPosition; //position of the entity on the map pane
};

struct Creature : public Entity //creature base used for player and other beings
{
	float baseMoveTime; //time before the player moves a unit
	float moveSpeed;
	Clock moveClock;

	double angle; //angle that the creature us facing

	short int health; //the health of the creature
	Armor equippedArmor; //the armor of the creature

	int containerIndex; //the index of the selected item's container in the inventory
	int itemIndex; //the index of the selected item in the inventory

	std::vector<std::shared_ptr<Container>> inventory; //contains all of the items and containers of the creature

	std::shared_ptr<Item> selectedItem; //the item that is currently selected
	//std::shared_ptr<MagazineData> selectedMagazine; //the selected magazine of the selected item

	//std::shared_ptr<MagazineData> nullMagazine; //a generic magazine that is used then a firearm has no actual magazine

	Creature(Position4 pos, int ch, std::string name, TCODColor color, int health, Armor armor); //creature constructor that takes a position, character, string name, color, health, and armor

	//movement
	virtual void move();
	void changeStanceUp();
	void changeStanceDown();

	//world interact
	virtual void pickUpItem(); //should eventually chenge to pure
	virtual void dropItem();

	virtual void takeDamage(int damage);

	//actions
	virtual void reload();
	virtual void changeFireMode();
	virtual void equipArmor();
	virtual void useMelee();

	virtual void updateTools();

	//need to include functions that only calls from selected item?

	virtual void update(); //virtual updates the creature
	virtual void render(const std::shared_ptr<Pane>& pane) const; // virtual renders the creature
};

struct Player : public Creature //player derived creature that the player interacts through
{
	char xMoveDist; //the amount that the player moves in the x dimension
	char yMoveDist; //the amount that the player moves in the y dimension

	bool hasSecondChance;

	TCODColor backgroundColor;

	Player(Position4 pos); //player constructor that takes a position

	//movement
	void move();

	//world interact
	void pickUpItem(); //picks up the item on the ground
	void dropItem(); //drops the selected item

	void takeDamage(int damage);

	//actions
	void reload(); //reloads the selected item
	void changeFireMode();
	void equipArmor();
	void useMelee();

	void updateTools();

	void update(); //updates the player
	void render(const std::shared_ptr<Pane>& pane) const; //renders the player

private:
	void moveSelectorUp(); //moves the selector up on the inventory
	void moveSelectorDown(); //moves the selector down on the inventory
	void filterIndexes(); //filters the container and item indexes to make sure they are in range
};
