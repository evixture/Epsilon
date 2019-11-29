#include "main.hpp"

struct Tool
{
	TCODColor color;
	int ch;
	
	int toolx;
	int tooly;

	int sourcex;
	int sourcey;

	int dx;
	int dy;

	const char* name;

	Tool(const char* name, TCODColor color, int ch);

	virtual void updateToolPosition(double angle);
	virtual void update(int x, int y, int mx, int my, double angle);

	virtual void render(const std::shared_ptr<Pane>& pane) const;

};

struct Bullet
{
	int ch;

	int bulletx;
	int bullety;

	int tox;
	int toy;

	int xbound;
	int ybound;

	Clock moveClock;

	bool hitWall;

	BLine travel;

	//Position renderPosition;

	Bullet(int ch, int startx, int starty, int dx, int dy, int xbound, int ybound);

	void update();
	void render(const std::shared_ptr<Pane>& pane) const;
};



//change name
struct Weapon : public Tool
{
	int numberMags;

	int ammoCap;
	int ammoAmount;

	float baseFireCap;
	Clock fireClock;

	float baseReloadTimer;
	Clock reloadClock;

	std::vector<std::shared_ptr<Bullet>> bulletList;

	Weapon(const char* name, TCODColor color, int ammoCap, int numberMags, float fireRate, float reloadSpeed);

	void updateWeaponChar(double angle);
	void update(int x, int y, int mx, int my, double angle);

	void render(const std::shared_ptr<Pane>& pane) const;
};

struct Item
{
	int size;
	double distToEnt;

	Position mapPosition;
	Position renderPosition;

	std::shared_ptr<Tile> tile;
	std::shared_ptr<Tool> tool;

	Item(int size, std::shared_ptr<Tile> tile, std::shared_ptr<Tool> tool, Position position);

	void updateTool(int x, int y, int mx, int my, double angle, int level);
	void renderTool(const std::shared_ptr<Pane>& pane) const;

	void updateTile();
	void renderTile(const std::shared_ptr<Pane>& pane) const;
};

//struct Magazine : public Item
//{
//	enum AmmoType { NINEMILLIMETER, FIVEPOINTFIVESIX } ammoType;
//
//	int ammoCapacity;
//	int currentAmmo;
//
//	Magazine()
//};

struct Container
{
	int itemCapacity;
	int currentSize;

	std::shared_ptr<Item> containerItem;

	std::vector<std::shared_ptr<Item>> itemList;

	Container(int itemCapacity, std::shared_ptr<Item> containerItem);

	bool addItem(std::shared_ptr<Item> item);
};