#include "main.hpp"
struct Tile
{
	int x;
	int y;

	TCODColor bgcol;

	bool blocksMove;
	bool blocksLight;
	bool destructible;

	//add args?
	Tile();

	~Tile();

	void destroy();

	TCODColor disColor();

	//void render();
};

class Map
{
public:

	int w;
	int h;

	std::vector<Tile> tiles;

	Map(int x, int y);
	~Map();

	bool canWalk(int x, int y);

	bool blocksLight(int x, int y);

	//void update();

	void render();
};

//TODO : make map class and destructible tiles
//TODO : figure out how to turn a string to a vector of tiles
//TODO : teleporting tiles and map floors
//TODO : map scrolling