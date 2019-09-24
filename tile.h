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

	//inline void render(int x, int y, std::shared_ptr<Pane> pane);
	
};
