#include "main.hpp"

struct Map
{
	const char* filePath;

	const char* mapName;
	int totalFloors;
	int mapWidth;
	int mapHeight;

	std::vector<std::vector< std::shared_ptr<Tile >>> levelList;

	Map(const char* filePath);
};

//World Class  LEVELS AND ENTS
class World
{
public:
	//move to ent
	int lookHeight;

	int xOffset;
	int yOffset;

	//std::vector<std::shared_ptr<Map>> mapList;
	std::shared_ptr<Map> debugmap;
	//std::shared_ptr<Map> currentMap;

	std::shared_ptr<TCODMap> fovMap;

	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Entity>> entityList;

	World();

	std::shared_ptr<Tile> getTile	(int x, int y, int level) const;
	TCODColor getBgColor			(int x, int y, int level) const;
	//TCODColor getFgColor			(int x, int y, int level) const;
	//int       getCh					(int x, int y, int level) const;
	bool      getTransparency		(int x, int y, int level, int height) const;
	bool      getWalkability		(int x, int y, int level) const;
	int       getHeight				(int x, int y, int level) const;

	bool inMapBounds(int x, int y, int level) const;
	int getOffset(int playerx, int mapw, int renderw);

	bool isInFov(int x, int y, int level) const;
	bool isExplored(int x, int y, int level) const;
	void computeFov();
	void updateProperties();

	void update();

	//make const
	void render(const std::shared_ptr<Pane> &pane) const;
};
