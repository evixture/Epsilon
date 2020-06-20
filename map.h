#include "main.hpp"

/*
	World
	|	Map
*/

struct Map
{
	std::string filePath;

	std::string name;
	unsigned short int totalFloors;
	int width;
	int height;

	std::shared_ptr<Player> player;
	std::shared_ptr<Creature> global;
																									
	std::vector<std::shared_ptr<Creature>> creatureList;			
	std::vector<std::shared_ptr<Item>> mapItemList;				
	std::vector<std::shared_ptr<Container>> mapContainerList;	

	std::vector<std::vector< std::shared_ptr<Block >>> levelList;
	std::vector<std::shared_ptr<TCODMap>> fovMapList;

	Map(std::string filePath);

	void addCreature	(std::shared_ptr<Creature> creature);			
	void addItem		(std::shared_ptr<Item> item);						
	void addContainer	(std::shared_ptr<Container> container);
	void refreshFOV		(int floor);

	bool getTransparency			(Position4& position) const;
	bool getWalkability				(Position4 position, bool checkCreatures) const;
	bool getSolidity				(Position4& position) const;
	bool inMapBounds				(Position3& position) const;

	std::shared_ptr<Block> getBlock(Position3 position) const;

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

struct World
{
	int xOffset; //CHECK the x offset of the rendered portion of the map
	int yOffset; //CHECK the y offset of the rendered portion of the map

	std::shared_ptr<Map> debugmap;

	std::vector<std::shared_ptr<Sound>> soundList;
	std::vector<std::shared_ptr<Sound>> soundBuffer;

	World();

	TCODColor	getBgColor		(Position3& position) const;

	bool		isInPlayerFov	(Position4 position) const; 
	bool		isExplored		(Position3& position) const;

	void addSound(Sound sound);
	void updateBlock(Position3 blockPosition, bool checkCreatures);	

	void update();
	void render(const Pane& pane) const;

private:
	int getOffset		(int playerx, int mapw, int renderw);

	void computeFov(Position4 mapPosition);

	void updateEntities();

	void renderTiles(const Pane& pane) const;
	void renderWorldItems(const Pane& pane) const;
	void renderCreatures(const Pane& pane) const;
};
