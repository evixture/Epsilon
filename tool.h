#include "main.hpp"

struct Tool
{
	TCODColor color;
	int ch;
	
	int toolx;
	int tooly;

	int dx;
	int dy;

	const char* name;

	Tool(const char* name, TCODColor color);

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

	int moveWait;
	int moveCap;

	bool hitWall;

	BLine travel;

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

	int fireWait;
	float baseFireCap;
	int fireCap;

	int reloadTimer;
	float baseReloadTimer;
	int reloadWait;

	std::vector<std::shared_ptr<Bullet>> bulletList;

	Weapon(const char* name, TCODColor color, int ammoCap, int numberMags, float fireRate, float reloadSpeed);

	void update(int x, int y, int mx, int my, double angle);
	void render(const std::shared_ptr<Pane>& pane) const;
};

struct Item
{
	int size;

	Position mapPosition;
	Position renderPosition;

	std::shared_ptr<Tile> tile;
	std::shared_ptr<Tool> tool;

	Item(int size, std::shared_ptr<Tile> tile, std::shared_ptr<Tool> tool, int x, int y, int level);

	void updateTool(int x, int y, int mx, int my, double angle);
	void renderTool(const std::shared_ptr<Pane>& pane) const;

	void updateTool();
	void renderTile(const std::shared_ptr<Pane>& pane) const;
};


//struct Item// : public Container
//{
//	//bool useTile;
//	float distToCreature;
//
//	TCODColor tileBackgoundColor;
//
//	//Position mapPosition;
//	//Position renderPosition;
//
//	std::shared_ptr<Tile> tile;
//	std::shared_ptr<Tool> tool;
//
//	Item(const char* name, std::shared_ptr<Tile> tile, std::shared_ptr<Tool> tool, bool useTile);
//
//	void update(int x, int y, int mx, int my, double angle);
//	void render(const std::shared_ptr<Pane>& pane) const;
//};

struct Container
{
	int itemCapacity;
	int currentSize;
	const char* name;

	std::vector<std::shared_ptr<Item>> itemList;

	Container(const char* name, int itemCapacity);

	bool addItem(std::shared_ptr<Item> item);
};