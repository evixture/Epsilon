#include "main.hpp"

struct Action //handles item actions
{
	enum class Type {DROP, RELOAD, CHANGEFIREMODE, EQUIP} type; //main type of action, enum for easy comparison

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
	enum class ItemType {NODROP, NORMAL, FIREARM, MAGAZINE, ARMOR} type;

	unsigned char size; //size that the item tekes up in the inventory
	double distToEnt; //the distance from the player to the item, used to highlight the item when the player is in proximity
	
	TCODColor barColor;

	Position4 mapPosition; //the position of the item on the map

	std::shared_ptr<Block> tile; //the tile component used when the item is on the map
	std::shared_ptr<Tool> tool; //the tool component used when the item is in the player's inventory

	Player* owner;
	std::shared_ptr<ActionManager> actionManager; //used to activate more advanced interactions with the item

	void createActionManager(Player* owner); //makes an action manager action list based on the item type

	Item(int size, std::shared_ptr<Block> tile, std::shared_ptr<Tool> tool, Position4 position, Player* owner, ItemType type); //item constructor that takes a size, tile, too, position, and a player used for action manager

	virtual std::shared_ptr<MagazineData> getMagazineData(); //used to get the important data of the magazine, returns generic magazine when called from item
	virtual void changeBarColor();

	void updateTool(Position4 mapPosition, int mx, int my, double angle, bool isHeld); //updates tool, used when in player inventory
	void renderTool(const std::shared_ptr<Pane>& pane) const; //renders tool, used then in player inventory

	void updateTile(); //used to update tile, used when on the map
	void renderTile(const std::shared_ptr<Pane>& pane) const; //renders the tile, used when on the map

private:
	Position4 renderPosition; //the position of the item on the map window
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

