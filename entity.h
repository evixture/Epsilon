#include "main.hpp"

struct Entity
{
	Position4 mapPosition;
	unsigned char ch;
	TCODColor color;
	std::string name;

	Entity(Position4 pos, int ch, std::string name, TCODColor color);

	virtual void update();
	virtual void render(const Pane& pane) const;

protected:
	Position3 renderPosition;
};

struct Creature : public Entity
{
	float baseMoveTime;
	float moveSpeed;
	Clock moveClock;

	double angle;

	short int health;
	Armor equippedArmor;

	int containerIndex;
	int itemIndex;

	std::vector<std::shared_ptr<Container>> inventory;

	std::shared_ptr<Item> selectedItem;

	Creature(Position4 pos, int ch, std::string name, TCODColor color, int health, Armor armor);

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

	virtual void update();
	virtual void render(const Pane& pane) const;
};

struct Player : public Creature
{
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
	void render(const Pane& pane) const; //renders the player

private:
	void moveSelectorUp(); //moves the selector up on the inventory
	void moveSelectorDown(); //moves the selector down on the inventory
	void filterIndexes(); //filters the container and item indexes to make sure they are in range
};
