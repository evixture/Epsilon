#include "main.hpp"

/*
	Container
	|	Magazine Item
	|	L	Item
	|		|	Action Manager
	|		|	L	Action


	OWNERSHIP
		item
		tool
		tile?
		bullet
*/
struct Action
{
	enum class Type {DROP, RELOAD, CHANGEFIREMODE, EQUIP, MELEE} type;

	std::string name;

	Action(std::string name, Type actionType);

	void update();

	bool operator == (const Action& compAction);
};

struct ActionManager
{
	std::vector<Action> actionList;
	Action selectedAction;

	ActionManager(std::vector<Action> actionList);

	void update();

	void moveSelectorUp();
	void moveSelectorDown();

	void doAction(Creature* newOwner);

private:
	unsigned char actionIndex;
};

struct Item
{
	const Creature* owner; //creature and its pointer cannot be modified

	enum class ItemType {HAND, NORMAL, FIREARM, MAGAZINE, ARMOR, MELEE} type;

	unsigned char size;
	double distToEnt;
	
	TCODColor barColor;

	Position4 mapPosition;
	Position3 lastKnownMapPosition;

	std::shared_ptr<Block> block;
	std::shared_ptr<Tool> tool;

	std::shared_ptr<ActionManager> actionManager;

	Item(const Creature* creature, int size, std::shared_ptr<Block> block, std::shared_ptr<Tool> tool, Position4 position, ItemType type);
	virtual ~Item() {};

	virtual MagazineData& getMagazineData();
	virtual void changeBarColor();

	void updateTool(Position4& mapPosition, int xMouse, int yMouse, bool isHeld);
	void renderTool(const Pane& pane) const;

	void updateTile();
	void renderTile(const Pane& pane) const;

private:
	void createActionManager();
	Position4 tileRenderPosition; //replace with position2
};

struct MagazineItem : public Item
{
	MagazineData magazineData;

	MagazineItem(Item item, MagazineData magazineData);

	MagazineData& getMagazineData();

	void changeBarColor();
};

struct Container
{
	unsigned char itemCapacity;
	unsigned char currentSize;

	std::shared_ptr<Item> item;

	std::vector<std::shared_ptr<Item>> itemList;

	Container(int itemCapacity, std::shared_ptr<Item> item);
	Container(int itemCapacity, std::shared_ptr<Item> item, std::vector<std::shared_ptr<Item>> itemList);

	bool addItem(std::shared_ptr<Item> item);
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
		inline static Item smallBackpack(const Creature* owner, int x, int y, int level)
		{
			return Item(owner, 2, std::make_shared<Block>(ep::block::item::smallBackpack), std::make_shared<Tool>(ep::tool::smallBackpack(owner)), Position4(x, y, 3, level), Item::ItemType::NORMAL);
		}

		inline static Item hands(const Creature* owner, int x, int y, int level)
		{
			return Item(owner, 1, std::make_shared<Block>(ep::block::item::defBlock), std::make_shared<Melee>(ep::tool::hands(owner)), Position4(x, y, 3, level), Item::ItemType::HAND);
		}

		inline static Item L1R3Armor(const Creature* owner, int x, int y, int level)
		{
			return Item(owner, 2, std::make_shared<Block>(ep::block::item::L1R3Armor), std::make_shared<Armor>(ep::tool::L1R3Armor(owner)), Position4(x, y, 3, level), Item::ItemType::ARMOR);
		}

		inline static Item sip45(const Creature* owner, int x, int y, int level)
		{
			return Item(owner, 2, std::make_shared<Block>(ep::block::item::sip45), std::make_shared<Firearm>(ep::tool::sip45(owner)), Position4(x, y, 3, level), Item::ItemType::FIREARM);
		}

		inline static Item cal45Magazine7(const Creature* owner, int x, int y, int level)
		{
			return Item(owner, 1, std::make_shared<Block>(ep::block::item::cal45Magazine7), std::make_shared<Tool>(ep::tool::cal45magazine7(owner)), Position4(x, y, 3, level), Item::ItemType::NORMAL);
		}

		inline static Item sir556(const Creature* owner, int x, int y, int level)
		{
			return Item(owner, 3, std::make_shared<Block>(ep::block::item::sir556), std::make_shared<Firearm>(ep::tool::sir556(owner)), Position4(x, y, 3, level), Item::ItemType::FIREARM);
		}

		inline static Item cal556Magazine30(const Creature* owner, int x, int y, int level)
		{
			return Item(owner, 1, std::make_shared<Block>(ep::block::item::cal556Magazine30), std::make_shared<Tool>(ep::tool::cal556magazine30(owner)), Position4(x, y, 3, level), Item::ItemType::NORMAL);
		}

		inline static Item knife(const Creature* owner, int x, int y, int level)
		{
			return Item(owner, 1, std::make_shared<Block>(ep::block::item::knife), std::make_shared<Melee>(ep::tool::knife(owner)), Position4(x, y, 3, level), Item::ItemType::MELEE);
		}
	};

	struct container
	{
		inline static Container smallBackpack(const Creature* owner, int x, int y, int level)
		{
			return Container(5, std::make_shared<Item>(ep::item::smallBackpack(owner, x, y, level)));
		}

		inline static Container smallBackpack(const Creature* owner, int x, int y, int level, std::vector<std::shared_ptr<Item>> itemList)
		{
			return Container(5, std::make_shared<Item>(ep::item::smallBackpack(owner, x, y, level)), itemList);
		}

		inline static Container hands(const Creature* owner, int x, int y, int level)
		{
			return Container(0, std::make_shared<Item>(ep::item::hands(owner, x, y, level)));
		}
	};

	struct magazineItem
	{
		inline static MagazineItem cal45Magazine7(const Creature* owner, int x, int y, int level)
		{
			return MagazineItem(ep::item::cal45Magazine7(owner, x, y, level), MagazineData(MagazineData::AmmoType::FOURTYFIVEACP, 7, 7));
		}

		inline static MagazineItem cal556Magazine30(const Creature* owner, int x, int y, int level)
		{
			return MagazineItem(ep::item::cal556Magazine30(owner, x, y, level), MagazineData(MagazineData::AmmoType::FIVEPOINTFIVESIX, 30, 30));
		}
	};
}