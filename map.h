#include "main.hpp"

struct Tile
{
	TCODColor bgcol;
	TCODColor destcol;

	bool blocksMove;
	bool blocksLight;
	bool destructible;

	//add args?
	Tile();

	~Tile();

	void destroy();

	void dispColor();

	void render();
};

class Map
{
public:

	int w;
	int h;
	int conw;
	int conh;

	TCODConsole* mapWin;

	std::vector<std::shared_ptr<Tile>> tiles;

	std::shared_ptr<TCODMap> tcodMap;

	Map(int conw, int conh, int w, int h);
	~Map();

	void setWall(int x, int y);

	void setWindow(int x, int y);

	void setDest(int x, int y);

	bool canWalk(int x, int y);

	bool blocksLight(int x, int y);

	//void update();

	void render();
};

//TODO : make map class and destructible tiles
//TODO : figure out how to turn a string to a vector of tiles
//TODO : teleporting tiles and map floors
//TODO : map scrolling