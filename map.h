#include "main.hpp"

struct MapFile
{
	const char* filePath;

	int mapW;
	int mapH;

	int textLength;

	int getMapTextLength();

	MapFile(const char* filePath, int mapWidth, int mapHeight);
};

//Map Class
class Map
{
public:
	//int mapW;
	//int mapH;
	int lookHeight;

	MapFile debugmap;

	//main map core
	std::shared_ptr<TCODMap> fovMap;
	//make mapfile shared ptr
	std::vector<const char*> textMapList;
	std::vector<std::shared_ptr<Tile>> tileList;

	//main entity core
	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Entity>> entityList;

	Map();

	bool isInFov(int x, int y);

	bool isExplored(int x, int y);

	void computeFov();
	
	TCODColor getBgColor(int x, int y);
	TCODColor getFgColor(int x, int y);
	int getCh(int x, int y);
	bool getTransparency(int x, int y);
	bool getWalkability(int tx, int ty);

	void updateProperties(std::shared_ptr<Window> window);

	void createMap(MapFile mapFile);

	void update(std::shared_ptr<Window> window);

	void render(std::shared_ptr<Window> window);
};
