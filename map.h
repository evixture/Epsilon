#include "main.hpp"

struct Map
{
	std::string filePath;

	std::string mapName;
	int totalFloors;
	int mapWidth;
	int mapHeight;

	std::vector<std::vector< std::shared_ptr<Tile >>> levelList;

	Map(std::string filePath);
};

//World Class
class World
{
public:
	int xOffset;
	int yOffset;

	std::shared_ptr<Map> debugmap;
	std::shared_ptr<TCODMap> fovMap;

	std::vector<std::shared_ptr<Entity>> entityList;
	std::shared_ptr<Player> player;

	std::vector<std::shared_ptr<Item>> mapItemList;
	std::vector<std::shared_ptr<Container>> mapContainerList;

	World();

	std::shared_ptr<Tile> getTile	(int x, int y, int level) const;
	TCODColor getBgColor			(int x, int y, int level) const;

	bool    getTransparency		(int x, int y, int level, int height) const;
	bool    getWalkability		(int x, int y, int level) const;
	bool	inMapBounds			(int x, int y, int level) const;
	bool	isInFov				(int x, int y, int level) const;
	bool	isExplored			(int x, int y, int level) const;

	void update();

	void render(const std::shared_ptr<Pane> &pane) const;

private:

	int getOffset		(int playerx, int mapw, int renderw);

	void computeFov();
	void updateProperties();

	void updateEntities();

	void renderTiles(const std::shared_ptr<Pane>& pane) const;
	void renderMouse(const std::shared_ptr<Pane>& pane) const;
	void renderEntities(const std::shared_ptr<Pane>& pane) const;
};
