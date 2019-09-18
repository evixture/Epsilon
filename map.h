#include "main.hpp"

struct Map
{
	const char* filePath;

	const char* mapName;
	int totalFloors;
	int mapW;
	int mapH;

	std::vector<std::vector< Tile >> levelList;

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
	std::shared_ptr<TCODMap> fovMap;

	//main entity core
	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Entity>> entityList;

	World();

	TCODColor getBgColor(int x, int y, int level);
	TCODColor getFgColor(int x, int y, int level);
	int getCh(int x, int y, int level);
	bool getTransparency(int x = 0, int y = 0, int level = 0);
	bool getWalkability(int tx = 0, int ty = 0, int level = 0);

	bool isInFov(int x, int y, int level);
	bool isExplored(int x, int y, int level);
	void computeFov();
	void updateProperties();

	void update(std::shared_ptr<Pane> window);

	void render(std::shared_ptr<Pane> window);
};
