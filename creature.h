#include "main.hpp"


struct Creature : public Entity
{
	Position2 targetPosition;

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
	virtual bool pickUpItem(); //should eventually chenge to pure
	virtual void dropItem();
	virtual void throwItem();

	//actions
	virtual bool reload();
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

	Player(Position4 pos);

	void takeDamage(int damage);

	void update(); //updates the player
	void render(const Pane& pane) const; //renders the player

private:
	//movement
	void move();

	//world interact
	bool pickUpItem();
	void dropItem();
	void throwItem();

	//actions
	bool reload();
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
	struct inventory //all inventories need to have hands as the first item in the list
	{
		inline static std::vector<std::shared_ptr<Container>> testInventory(const Creature* owner)
		{
			return {
			std::make_shared<Container>(ep::container::hands(owner, 0, 0, 0)),
			std::make_shared<Container>(ep::container::smallBackpack(owner, 0, 0, 0,
			{
				std::make_shared<Item>(ep::item::sip45(owner, 0, 0, 0)),
				std::make_shared<MagazineItem>(ep::magazineItem::cal45Magazine7(owner, 0, 0, 0)),
				std::make_shared<MagazineItem>(ep::magazineItem::cal45Magazine7(owner, 0, 0, 0)),
				std::make_shared<Item>(ep::item::knife(owner, 0, 0, 0))
			})),
			std::make_shared<Container>(ep::container::smallBackpack(owner, 0, 0, 0,
			{
				std::make_shared<Item>(ep::item::sir556(owner, 0, 0, 0)),
				std::make_shared<MagazineItem>(ep::magazineItem::cal556Magazine30(owner, 0, 0, 0)),
				std::make_shared<MagazineItem>(ep::magazineItem::cal556Magazine30(owner, 0, 0, 0)),
			}))};
		}
	};
}