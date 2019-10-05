#include "main.hpp"

#define WORLD engine->gui->mapPane->world

struct Tile
{
	int ch;
	int height;

	TCODColor fgcol;
	TCODColor bgcol;

	bool explored;
	bool walkable;
	bool transparent;

	Tile(int ch, TCODColor fgcol, TCODColor bgcol, int height, bool walkable, bool transparent);

	//bad performance
	virtual void render(int x, int y, std::shared_ptr<Pane> pane);
	
	virtual void update();
};

struct Destructible : public Tile
{
	TCODColor destFgColor;
	TCODColor destBgColor;
	TCODColor destCh;

	Destructible(int ch, TCODColor fgcol, TCODColor bgcol, int height, bool walkable, bool transparent);

	virtual void render(int x, int y, std::shared_ptr<Pane> pane);

	virtual void update();
};
