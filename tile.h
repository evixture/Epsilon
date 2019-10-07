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
	//bool transparent;
	bool destructible;
	bool destroyed;

	Tile(int ch, TCODColor fgcol, TCODColor bgcol, int height, bool walkable, bool destructible);

	void destroy(int destCH, TCODColor destBGCOL, TCODColor destFGCOL, bool destWALK);

	//bad performance
	void render(int x, int y, const std::shared_ptr<Pane>& pane) const;
	
	void update();
};

//struct Destructible : public Tile
//{
//	TCODColor destFgColor;
//	TCODColor destBgColor;
//	TCODColor destCh;
//
//	Destructible(int ch, TCODColor fgcol, TCODColor bgcol, int height, bool walkable, bool transparent);
//
//	virtual void render(int x, int y, const std::shared_ptr<Pane>& pane) const;
//
//	virtual void update();
//};
