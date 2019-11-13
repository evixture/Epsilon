#include "main.hpp"

struct Tile
{
	const char* tag;

	int ch;
	int height;

	TCODColor foregroundColor;
	TCODColor backgroundColor;

	bool explored;
	bool walkable;	

	Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int height, bool walkable);

	Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int height, bool walkable, const char* tag);

	virtual bool getDestroyed();

	virtual void interact();

	void render(int x, int y, const std::shared_ptr<Pane>& pane) const;
};

struct Destructible : public Tile
{
	bool destroyed;
	int strength;

	Destructible(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int height, bool walkable, int strength);

	bool getDestroyed();

	void interact();
};

struct Stair : public Tile
{
	int moveDist;
	
	Stair(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int height, bool walkable, int moveDist);

	void interact();
};
