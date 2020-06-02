#include "main.hpp"

struct Entity
{
	Position4 mapPosition;
	unsigned char ch;
	TCODColor color;
	std::string name;

	Entity(Position4 pos, int ch, std::string name, TCODColor color);
	virtual ~Entity() {};

	virtual void update();
	virtual void render(const Pane& pane) const;

protected:
	Position3 renderPosition;
};

struct Creature : public Entity
{
	float baseMoveTime;

	short int health;
	Armor equippedArmor;

	int containerIndex;
	int itemIndex;

	std::vector<std::shared_ptr<Container>> inventory; //needs to be pointer
	std::shared_ptr<Item> selectedItem; //needs to be pointer

	Position3 stance; //used for faction alliances

	Creature(Position4 pos, int ch, std::string name, Position3 stance, int health, Armor armor);
	virtual ~Creature() {};

	virtual void takeDamage(int damage);

	virtual void update();
	virtual void render(const Pane& pane) const;

protected:
	float moveSpeed;
	Clock moveClock;

	double angle;

	//movement
	virtual void move();
	void changeStanceUp();
	void changeStanceDown();

	//world interact
	virtual void pickUpItem(); //should eventually chenge to pure
	virtual void dropItem();

	//actions
	virtual void reload();
	virtual void changeFireMode();
	virtual void equipArmor();
	virtual void useMelee();

	virtual void updateTools();

	friend struct ActionManager;
};

struct Player : public Creature
{
	bool hasSecondChance;

	TCODColor backgroundColor;

	Player(Position4 pos); //player constructor that takes a position

	void takeDamage(int damage);

	void update(); //updates the player
	void render(const Pane& pane) const; //renders the player

private:
	//movement
	void move();

	//world interact
	void pickUpItem();
	void dropItem();

	//actions
	void reload();
	void changeFireMode();
	void equipArmor();
	void useMelee();

	void updateTools();

	void moveSelectorUp(); //moves the selector up on the inventory
	void moveSelectorDown(); //moves the selector down on the inventory
	void filterIndexes(); //filters the container and item indexes to make sure they are in range

	friend struct ActionManager;
};

namespace ep
{
	struct inventory
	{
		inline static const std::vector<std::shared_ptr<Container>> testInventory = 
		{
			std::make_shared<Container>(ep::container::smallBackpack(0, 0, 0, 
			{
				std::make_shared<Item>(ep::item::knife(0, 0, 0)),
				std::make_shared<Item>(ep::item::sip45(0, 0, 0)),
				std::make_shared<Item>(ep::item::cal45Magazine7(0, 0, 0)),
				std::make_shared<Item>(ep::item::cal45Magazine7(0, 0, 0))
			}))
		};
	};
}