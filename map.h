#include "main.hpp"

//struct Level : public TCODMap
//{
//	std::vector<std::shared_ptr<Tile>> tileList;
//
//	Level(int fovmapW, int fovmapH);
//};

//struct Map
//{
//	const char* filePath;
//
//	const char* mapName;
//	int floorNum;
//	int totalFloors;
//	int mapW;
//	int mapH;
//
//	//std::vector<std::shared_ptr<Tile>> tileList;
//	std::vector<Level> levelList;
//
//	Map(const char* cFilePath);
//};

struct Map
{
	const char* filePath;

	const char* mapName;
	int floorNumber;
	int totalFloors;
	int mapW;
	int mapH;

	std::vector<std::vector< Tile >> levelList;
	//std::shared_ptr< std::vector< std::shared_ptr< std::vector< std::shared_ptr< Tile >>>>> levelList;

	Map(const char* filePath);
};

//World Class  LEVELS AND ENTS
class World
{
public:
	//move to ent
	int lookHeight;

	//std::vector<std::shared_ptr<Map>> mapList;
	std::shared_ptr<Map> debugmap;

	//std::shared_ptr<Map> currentMap;

	//main map core
	TCODMap fovMap;

	//main entity core
	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Entity>> entityList;

	World();

	bool isInFov(int x, int y, int level);

	bool isExplored(int x, int y, int level);

	void computeFov();

	TCODColor getBgColor(int x, int y, int level);
	TCODColor getFgColor(int x, int y, int level);
	int getCh(int x, int y, int level);
	bool getTransparency(int x, int y, int level);
	bool getWalkability(int tx, int ty, int level);

	void updateProperties();

	//void createMap(Map mapFile);

	void update(std::shared_ptr<Pane> window);

	void render(std::shared_ptr<Pane> window);
};
