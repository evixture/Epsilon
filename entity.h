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
	double angle; //angle that the creature us facing

	short int health; //the health of the creature
	Armor equippedArmor; //the armor of the creature

	int containerIndex; //the index of the selected item's container in the inventory
	int itemIndex; //the index of the selected item in the inventory

	std::vector<std::shared_ptr<Container>> inventory; //contains all of the items and containers of the creature

	std::shared_ptr<Item> selectedItem; //the item that is currently selected
	std::shared_ptr<MagazineData> selectedMagazine; //the selected magazine of the selected item

	std::shared_ptr<MagazineData> nullMagazine; //a generic magazine that is used then a firearm has no actual magazine

	Creature(Position4 pos, int ch, std::string name, TCODColor color, int health, Armor armor); //creature constructor that takes a position, character, string name, color, health, and armor

	//virtual void moveSelectorUp() = 0; //moves the selector up on the inventory
	//virtual void moveSelectorDown() = 0; //moves the selector down on the inventory
	//virtual void pickUpItem() = 0; //picks up the item on the ground
	//virtual void dropItem() = 0; //drops the selected item
	//virtual void filterIndexes() = 0; //filters the container and item indexes to make sure they are in range
	//virtual void reload() = 0; //reloads the selected item
	//void takeDamage(Bullet* bullet);

	virtual void update(); //virtual updates the creature
	void virtual render(const std::shared_ptr<Pane>& pane) const; // virtual renders the creature
};

struct Player : public Creature //player derived creature that the player interacts through
{
	float baseMoveTime; //time before the player moves a unit

	Player(Position4 pos); //player constructor that takes a position

	void move();
	void changeStanceUp();
	void changeStanceDown();

	void moveSelectorUp(); //moves the selector up on the inventory
	void moveSelectorDown(); //moves the selector down on the inventory
	void filterIndexes(); //filters the container and item indexes to make sure they are in range

	void pickUpItem(); //picks up the item on the ground
	void dropItem(); //drops the selected item

	void reload(); //reloads the selected item
	void changeFireMode();
	void equipArmor();


	void update(); //updates the player
	void render(const std::shared_ptr<Pane>& pane) const; //renders the player

private:
	float moveSpeed;
	Clock moveClock;

	char moveXSpeed; //the amount that the player moves in the x dimension
	char moveYSpeed; //the amount that the player moves in the y dimension
};
