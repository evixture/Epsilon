#include "main.hpp"

struct MapFile
{
	const char* filePath;

	int textLength;

	int getMapTextLenght(const char* filepath);

	//MapFile();

	MapFile(const char* filePath);
};

//Map Class
class Map
{
public:
	int mapW;
	int mapH;
	int lookHeight;

	MapFile debugmap = MapFile("data/maps/debugmap.txt");

	//main map core
	std::shared_ptr<TCODMap> fovMap;
	//make mapfile shared ptr
	std::vector<const char*> textMapList;
	std::vector<std::shared_ptr<Tile>> tileList;

	//main entity core
	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Entity>> entityList;

	Map(int w, int h);

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
