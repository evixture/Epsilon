#include "main.hpp"


//Tile struct
struct Tile
{
	int ch;
	TCODColor fgcol;
	TCODColor bgcol;
	int maxHeight;

	bool explored;
	bool walkable;
	bool transparent;
	bool destructible;

	Tile(int ch, TCODColor fgcol, TCODColor bgcol, int maxHeight, bool walkable, bool transparent, bool destructible);

};

//Map Class
class Map
{
public:
	int mapW;
	int mapH;
	int lookHeight;

	const char* debugTextMap;

	//main map core
	std::shared_ptr<TCODMap> fovMap;
	std::vector<const char*> textMapList;
	std::vector<std::shared_ptr<Tile>> tileList;

	//main entity core
	std::shared_ptr<Entity> player;
	std::vector<std::shared_ptr<Entity>> entityList;

	Map(int w, int h);

	bool isInFov(int x, int y);

	bool isExplored(int x, int y);

	void computeFov();

	TCODColor getBgColor(int x, int y);
	TCODColor getFgColor(int x, int y);
	int getCh(int x, int y);
	bool getTransparency(int x, int y);
	bool getWalkability(int x, int y);

	void updateProperties(std::shared_ptr<Window> window);

	void createMap(const char* filePath);

	void update(std::shared_ptr<Window> window);

	void render(std::shared_ptr<Window> window);
};
