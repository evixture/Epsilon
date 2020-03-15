#include "main.hpp"

struct Action //handles item actions
{
	enum class Type {DROP, RELOAD, CHANGEFIREMODE, EQUIP, MELEE} type; //main type of action, enum for easy comparison

	std::string name; //the string name of the action so it can be rendered in action pane
	std::function<void()> action; //the function that is called when activated

	Action(std::string name, std::function<void()> action, Type actionType); //action constructor that takes string name, function, and action type

	void update();
};

struct ActionManager //manages all of the actions of an item
{
	std::vector<std::shared_ptr<Action>> actionList; //list of actions for an item
	unsigned char actionIndex; //the index of the selected item in the action list
	std::shared_ptr<Action> selectedAction; //the selected action of the item

	ActionManager(std::vector<std::shared_ptr<Action>> actionList); //constructor of action manager that takes a list of actions

	void update();

	void moveSelectorUp(); //moves the action index down in value
	void moveSelectorDown(); //moves the action index up in value

	void doAction(); //calls the action of the selected action
};

struct Item //an item that a creature can hold and interact with
{
	enum class ItemType {HAND, NORMAL, FIREARM, MAGAZINE, ARMOR, MELEE} type;

	unsigned char size; //size that the item tekes up in the inventory
	double distToEnt; //the distance from the player to the item, used to highlight the item when the player is in proximity
	
	TCODColor barColor; //color of the bar to render on the side of the inventory

	Position4 mapPosition; //the position of the item on the map

	std::shared_ptr<Block> tile; //the tile component used when the item is on the map
	std::shared_ptr<Tool> tool; //the tool component used when the item is in the player's inventory

	Creature* owner; 
	std::shared_ptr<ActionManager> actionManager; //used to activate more advanced interactions with the item

	void createActionManager(Creature* owner); //makes an action manager action list based on the item type

	Item(int size, std::shared_ptr<Block> tile, std::shared_ptr<Tool> tool, Position4 position, Creature* owner, ItemType type); //item constructor that takes a size, tile, too, position, and a player used for action manager

	virtual std::shared_ptr<MagazineData> getMagazineData(); //used to get the important data of the magazine, returns generic magazine when called from item
	virtual void changeBarColor(); //updates the inventory bar color

	void updateTool(Position4& mapPosition, int mx, int my, bool isHeld); //updates tool, used when in player inventory
	void renderTool(const std::shared_ptr<Pane>& pane) const; //renders tool, used then in player inventory

	void updateTile(); //used to update tile, used when on the map
	void renderTile(const std::shared_ptr<Pane>& pane) const; //renders the tile, used when on the map

private:
	Position4 tileRenderPosition; //the position of the item on the map window
};

struct MagazineItem : public Item //magazine derived class of base item
{
	std::shared_ptr<MagazineData> magazineData; //contains all of the important data of the magazine

	MagazineItem(Item item, std::shared_ptr<MagazineData> magazineData); //magazine item constructor that takes an item and magazine data

	std::shared_ptr<MagazineData> getMagazineData(); //returns the magazine data of this class

	void changeBarColor();
};

struct Container //container, used to hold items in the inventory
{
	unsigned char itemCapacity; //the maximum units of items the container can hold 
	unsigned char currentSize; //the current size used up of all of the items in the container

	std::shared_ptr<Item> containerItem; //item representation of the container

	std::vector<std::shared_ptr<Item>> itemList; //the list of all of the items in the container

	Container(int itemCapacity, std::shared_ptr<Item> containerItem); //container constructor that takes a capacity and a container item

	bool addItem(std::shared_ptr<Item> item); //adds an item to the container
};

namespace ep
{
	/*
ITEM SIZES
	1 UNIT
		COMPACT PISTOLS
	2 UNITS
		COMPACT SMGS
		PISTOLS
	3 UNITS
		STANDARD RIFLES
		LARGE SMGS
	4 UNITS
		MACHINE GUNS
		LARGE RIFLES
	5 UNITS
		SNIPER RIFLES
*/

	struct item
	{
		inline static Item smallBackpack(int x, int y, int level, Creature* owner)
		{
			return Item(2, std::make_shared<Block>(ep::block::item::smallBackpack), std::make_shared<Tool>(ep::tool::smallBackpack), Position4(x, y, 0, level), owner, Item::ItemType::NORMAL);
		}

		inline static Item hands(int x, int y, int level, Creature* owner)
		{
			return Item(1, std::make_shared<Block>(ep::block::item::defBlock), std::make_shared<Melee>(ep::tool::hands), Position4(x, y, 0, level), owner, Item::ItemType::HAND);
		}

		inline static Item L1R3Armor(int x, int y, int level, Creature* owner)
		{
			return Item(2, std::make_shared<Block>(ep::block::item::L1R3Armor), std::make_shared<Armor>(ep::tool::L1R3Armor), Position4(x, y, 0, level), owner, Item::ItemType::ARMOR);
		}

		inline static Item sip45(int x, int y, int level, Creature* owner)
		{
			return Item(2, std::make_shared<Block>(ep::block::item::sip45), std::make_shared<Firearm>(ep::tool::sip45), Position4(x, y, 0, level), owner, Item::ItemType::FIREARM);
		}

		inline static Item cal45Magazine7(int x, int y, int level, Creature* owner)
		{
			return Item(1, std::make_shared<Block>(ep::block::item::cal45Magazine7), std::make_shared<Tool>(ep::tool::cal45magazine7), Position4(x, y, 0, level), owner, Item::ItemType::NORMAL);
		}

		inline static Item sir556(int x, int y, int level, Creature* owner)
		{
			return Item(3, std::make_shared<Block>(ep::block::item::sir556), std::make_shared<Firearm>(ep::tool::sir556), Position4(x, y, 0, level), owner, Item::ItemType::FIREARM);
		}

		inline static Item cal556Magazine30(int x, int y, int level, Creature* owner)
		{
			return Item(1, std::make_shared<Block>(ep::block::item::cal556Magazine30), std::make_shared<Tool>(ep::tool::cal556magazine30), Position4(x, y, 0, level), owner, Item::ItemType::NORMAL);
		}

		inline static Item knife(int x, int y, int level, Creature* owner)
		{
			return Item(1, std::make_shared<Block>(ep::block::item::knife), std::make_shared<Melee>(ep::tool::knife), Position4(x, y, 0, level), owner, Item::ItemType::MELEE);
		}
	};

	struct container
	{
		inline static Container smallBackpack(int x, int y, int level, Creature* owner)
		{
			return Container(5, std::make_shared<Item>(ep::item::smallBackpack(x, y, level, owner)));
		}

		inline static Container hands(int x, int y, int level, Creature* owner)
		{
			return Container(0, std::make_shared<Item>(ep::item::hands(x, y, level, owner)));
		}
	};

	struct magazine
	{
		inline static MagazineItem cal45Magazine7(int x, int y, int level, Creature* owner)
		{
			return MagazineItem(ep::item::cal45Magazine7(x, y, level, owner), std::make_shared<MagazineData>(MagazineData::AmmoType::FOURTYFIVEACP, 7, 7));
		}

		inline static MagazineItem cal556Magazine30(int x, int y, int level, Creature* owner)
		{
			return MagazineItem(ep::item::cal556Magazine30(x, y, level, owner), std::make_shared<MagazineData>(MagazineData::AmmoType::FIVEPOINTFIVESIX, 30, 30));
		}
	};
}