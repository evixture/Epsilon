#include "main.hpp"

//struct Item
//{
//
//};

struct Tool// : public Container
{
	TCODColor color;
	int ch;
	
	int toolx;
	int tooly;

	int dx;
	int dy;

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

//struct Container
//{
//	std::vector<>
//
//	Container();
//};