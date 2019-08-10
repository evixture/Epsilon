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

	Tile(TCODColor bgcol, bool blocksMove, bool blocksLight, bool destructible);

	~Tile();

	void destroy();

	void dispColor();

	void render();
};

struct TextMap
{
	const char* filePath;

	TextMap(const char* filePath);

	~TextMap();

	void textToVector(std::vector<std::shared_ptr<Tile>>& vector);
};

class Map
{
public:

	//map and console dimensions
	int mapw;
	int maph;
	int conw;
	int conh;

	//map console and tiles
	TCODConsole* mapWin;
	std::vector<std::shared_ptr<Tile>> tileList;
	std::shared_ptr<TCODMap> tcodMap;
	std::shared_ptr<TextMap> textMap;

	//player init
	std::shared_ptr<Entity> player;
	std::vector<std::shared_ptr<Entity>> entityList;

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