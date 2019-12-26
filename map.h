#include "main.hpp"

struct Map //map class that takes a text file and converts it to a vector of tiles
{
	std::string filePath; //string path to the text file

	std::string mapName; //string name of the map
	int totalFloors; //the total number of floors of the map
	int mapWidth; //the width of the map
	int mapHeight; //the height of the map

	std::vector<std::vector< std::shared_ptr<Tile >>> levelList; //list of list of tiles, list of floors->list of tiles

	Map(std::string filePath); //map constructor that takes a string file path
};

struct World
{
	int xOffset; //the x offset of the rendered portion of the map
	int yOffset; //the y offset of the rendered portion of the map

	std::shared_ptr<Map> debugmap;
	std::shared_ptr<TCODMap> fovMap; //the part of the map used to calculate the fov

	std::shared_ptr<Player> player; //the player

	std::vector<std::shared_ptr<Item>> mapItemList; //list of all items on the map
	std::vector<std::shared_ptr<Container>> mapContainerList; //list of all containers on the map

	World(); //world contructor that takes nothing

	std::shared_ptr<Tile> getTile	(int x, int y, int level) const; //returns the tile at specific coordinates
	TCODColor getBgColor			(int x, int y, int level) const; //gets the background color of the tile at specific coordinates

	bool    getTransparency		(int x, int y, int level, int height) const; //gets the transparency of a tile
	bool    getWalkability		(int x, int y, int level) const; //gets the walkability of a tile
	bool	inMapBounds			(int x, int y, int level) const; //checks if the coordinates are in the map bounds
	bool	isInFov				(int x, int y, int level) const; //returns true if the coordinates are in fov
	bool	isExplored			(int x, int y, int level) const; //checks if a tile has been explored previously

	void update(); //updates the map

	void render(const std::shared_ptr<Pane> &pane) const; //renders the map

private:
	std::vector<std::shared_ptr<Entity>> entityList; //list of all entities on the map

	int getOffset		(int playerx, int mapw, int renderw); //returns the map offset

	void computeFov();  //computes the fov map
	void updateProperties(); //updates tcod map properties

	void updateEntities(); //updates all entities

	void renderTiles(const std::shared_ptr<Pane>& pane) const; //renderes all of the tiles
	void renderMouse(const std::shared_ptr<Pane>& pane) const; //renders the mouse pointer
	void renderEntities(const std::shared_ptr<Pane>& pane) const; //renders all of the entities
};
