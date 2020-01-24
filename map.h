#include "main.hpp"

struct Map //map class that takes a text file and converts it to a vector of tiles
{
	std::string filePath; //string path to the text file

	std::string name; //string name of the map
	int totalFloors; //the total number of floors of the map
	int width; //the width of the map
	int height; //the height of the map

	std::vector<std::vector< std::shared_ptr<Tile >>> levelList; //list of list of tiles, list of floors->list of tiles

	Map(std::string filePath); //map constructor that takes a string file path

	std::shared_ptr<Tile> getTileFromCode(std::string code);
};

struct World
{
	int xOffset; //the x offset of the rendered portion of the map
	int yOffset; //the y offset of the rendered portion of the map

	std::shared_ptr<Map> debugmap;
	std::shared_ptr<TCODMap> fovMap; //the part of the map used to calculate the fov

	std::shared_ptr<Player> player; //the player

	std::vector<std::shared_ptr<Creature>> creatureList; //list of all entities on the map
	std::vector<std::shared_ptr<Item>> mapItemList; //list of all items on the map
	std::vector<std::shared_ptr<Container>> mapContainerList; //list of all containers on the map

	World(); //world contructor that takes nothing

	void addCreature(std::shared_ptr<Creature> creature);
	void addItem(std::shared_ptr<Item> item);
	void addContainer(std::shared_ptr<Container> container);

	std::shared_ptr<Tile> getTile	(Position3 position) const; //returns the tile at specific coordinates
	TCODColor getBgColor			(Position3 position) const; //gets the background color of the tile at specific coordinates

	bool    getTransparency			(Position4 position) const; //gets the transparency of a tile
	bool    getWalkability			(Position4 position) const; //gets the walkability of a tile
	bool	getSolidity				(Position4 position) const;
	bool	inMapBounds				(Position3 position) const; //checks if the coordinates are in the map bounds
	bool	isInFov					(Position3 position) const; //returns true if the coordinates are in fov
	bool	isExplored				(Position3 position) const; //checks if a tile has been explored previously

	void update(); //updates the map

	void render(const std::shared_ptr<Pane> &pane) const; //renders the map

private:
	int getOffset		(int playerx, int mapw, int renderw); //returns the map offset

	void computeFov();  //computes the fov map
	void updateProperties(); //updates tcod map properties

	void updateEntities(); //updates all entities

	void renderTiles(const std::shared_ptr<Pane>& pane) const; //renderes all of the tiles
	void renderCreatures(const std::shared_ptr<Pane>& pane) const; //renders all of the entities
};
