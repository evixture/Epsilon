#include "main.hpp"

struct Entity //generic entity base for use on map
{
	Position mapPosition; //the position of the entity on the map
	int ch; //character representation of the character
	const TCODColor color; //foreground color of the entity
	std::string name; //string name of entity

	int height; //height of the entity

	Entity(Position pos, int ch, std::string name, TCODColor color); //entity constructor that takes position, character, string name, and color

	virtual void update(); //virtual update entity
	virtual void render(const std::shared_ptr<Pane> &pane) const; //virtual render entity

protected:
	Position renderPosition; //position of the entity on the map pane
};

struct Creature : public Entity //creature base used for player and other beings
{
	double angle; //angle that the creature us facing

	int health; //the health of the creature
	int armor; //the armor of the creature

	int containerIndex; //the index of the selected item's container in the inventory
	int itemIndex; //the index of the selected item in the inventory

	std::vector<std::shared_ptr<Container>> inventory; //contains all of the items and containers of the creature

	std::shared_ptr<Item> selectedItem; //the item that is currently selected
	std::shared_ptr<MagazineData> selectedMagazine; //the selected magazine of the selected item

	std::shared_ptr<MagazineData> nullMagazine; //a generic magazine that is used then a firearm has no actual magazine

	Creature(Position pos, int ch, std::string name, TCODColor color, int health, int armor); //creature constructor that takes a position, character, string name, color, health, and armor

	virtual void moveSelectorUp() {}; //moves the selector up on the inventory
	virtual void moveSelectorDown() {}; //moves the selector down on the inventory
	virtual void pickUpItem() {}; //picks up the item on the ground
	virtual void dropItem() {}; //drops the selected item
	virtual void filterIndexes() {}; //filters the container and item indexes to make sure they are in range
	virtual void reload() {}; //reloads the selected item

	virtual void update(); //virtual updates the creature
	void virtual render(const std::shared_ptr<Pane>& pane) const; // virtual renders the creature
};

struct Player : public Creature //player derived creature that the player interacts through
{
	Player(Position pos); //player constructor that takes a position

	void moveSelectorUp(); //moves the selector up on the inventory
	void moveSelectorDown(); //moves the selector down on the inventory
	void filterIndexes(); //filters the container and item indexes to make sure they are in range

	void pickUpItem(); //picks up the item on the ground
	void dropItem(); //drops the selected item
	void reload(); //reloads the selected item
	void changeFireMode();

	void update(); //updates the player
	void render(const std::shared_ptr<Pane>& pane) const; //renders the player
};
