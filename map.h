#include "main.hpp"

struct Map //map class deals with the map and creatures and the data used for the world
{
	std::string filePath; //string path to the text file

	std::string name; //string name of the map
	unsigned short int totalFloors; //the total number of floors of the map
	int width; //the width of the map
	int height; //the height of the map

	std::shared_ptr<Player> player; //the player													
																									
	std::vector<std::shared_ptr<Creature>> creatureList; //list of all entities on the map			
	std::vector<std::shared_ptr<Item>> mapItemList; //list of all items on the map					
	std::vector<std::shared_ptr<Container>> mapContainerList; //list of all containers on the map	

	std::vector<std::vector< std::shared_ptr<Block >>> levelList; //list of list of tiles, list of floors->list of tiles
	std::vector<std::shared_ptr<TCODMap>> fovMapList; //the part of the map used to calculate the fov

	Map(std::string filePath); //map constructor that takes a string file path

	void addCreature(std::shared_ptr<Creature> creature); //adds a creature to the current map				
	void addItem(std::shared_ptr<Item> item); //adds an item to the current map							
	void addContainer(std::shared_ptr<Container> container); //adds a container to the current map	

	void refreshFOV(int floor);

	bool    getTransparency			(Position4& position) const; //gets the transparency of a tile
	bool    getWalkability			(Position4 position, bool checkCreatures) const; //gets the walkability of a tile
	bool	getSolidity				(Position4& position) const;
	bool	inMapBounds				(Position3& position) const; //checks if the coordinates are in the map bounds

	std::shared_ptr<Block> getBlock(Position3 position) const; //returns the tile at specific coordinates

private:
	bool getMapName(pugi::xml_node& dataNode);
	bool getMapLevels(pugi::xml_node& dataNode);
	bool getMapHeight(pugi::xml_node& dataNode);
	bool getMapWidth(pugi::xml_node& dataNode);

	bool createBlockMap(pugi::xml_node& dataNode);

	bool getCreatures(pugi::xml_node& dataNode);
	bool getItems(pugi::xml_node& dataNode);
	bool getContainers(pugi::xml_node& dataNode);

	std::shared_ptr<Block> getTileFromCode(std::string code);
};

struct World //world struct that deals with rendering and updating the map
{
	int xOffset; //CHECK the x offset of the rendered portion of the map
	int yOffset; //CHECK the y offset of the rendered portion of the map

	std::shared_ptr<Map> debugmap; //main map used for debugging

	std::vector<std::shared_ptr<Sound>> soundList;
	std::vector<std::shared_ptr<Sound>> soundBuffer;

	World(); //world contructor that takes nothing

	
	TCODColor getBgColor			(Position3& position) const; //gets the background color of the tile at specific coordinates

	bool	isInPlayerFov			(Position4 position) const; //returns true if the coordinates are in fov
	bool	isExplored				(Position3& position) const; //checks if a tile has been explored previously

	//int addSound(Sound sound); //should eventually give params
	void updateBlock(Position3 blockPosition, bool checkCreatures);	

	void update(); //updates the map

	void render(const std::shared_ptr<Pane> &pane) const; //renders the map

private:
	//std::shared_ptr<Audio> soundManager;

	int getOffset		(int playerx, int mapw, int renderw); //returns the map offset

	void computeFov(Position4 mapPosition);  //computes the fov map

	void updateEntities(); //updates all entities

	void renderTiles(const std::shared_ptr<Pane>& pane) const; //renderes all of the tiles
	void renderCreatures(const std::shared_ptr<Pane>& pane) const; //renders all of the entities
};
