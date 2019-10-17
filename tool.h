#include "main.hpp"

struct Container
{
	bool active;

	Container();

	virtual void update();
	virtual void render();
};

struct Tool : public Container
{
	TCODColor color;
	int ch;
	
	int toolx;
	int tooly;

	int dx;
	int dy;

	Tool(TCODColor color);

	virtual void update(int x, int y, int mx, int my, double angle);
	virtual void render(const std::shared_ptr<Pane>& pane) const;

};

struct Bullet
{
	int bulletx;
	int bullety;

	int tox;
	int toy;

	int xbound;
	int ybound;

	bool hitWall;

	BLine travel;

	Bullet(int startx, int starty, int dx, int dy, int xbound, int ybound);

	void update();
	void render(const std::shared_ptr<Pane>& pane) const;
};

//struct Magazine
//{
//	int maxCapacity;
//	int currentCapacity;
//};

//change name
struct Weapon : public Tool
{
	int numberMags;

	int ammoCap;
	int ammoAmount;

	int fireWait;
	int fireCap;

	int reloadTimer;
	int reloadWait;

	std::vector<std::shared_ptr<Bullet>> bulletList;

	Weapon(TCODColor color, int ammoCap, int numberMags, int fireRate, int reloadSpeed);

	//void weaponFire(int endx, int endy, std::shared_ptr<Pane> pane, int xbound, int ybound);

	void update(int x, int y, int mx, int my, double angle);
	void render(const std::shared_ptr<Pane>& pane) const;
};


struct Item : public Container
{
	bool onMap;

	std::shared_ptr<Tile> tile;
	std::shared_ptr<Tool> tool;

	Item(std::shared_ptr<Tile> tile, std::shared_ptr<Tool> tool);

	void update();
	void render();
};