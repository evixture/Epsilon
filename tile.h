#include "main.hpp"

struct Tile
{
	unsigned char ch; //the character to render on the map
	TCODColor foregroundColor; //foreground color of the tile
	TCODColor backgroundColor; //background color of the tile
	unsigned short int deceleration; //the amount that a bullet is slowed down by when it passes through it

	Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int deceleration);
};

struct Block //tile class used for the map and items
{
	enum class Tag {STATIC, DESTRUCTIBLE, STAIR} tag; //type and functionality of the tile
	enum TileHeights {FLOOR = 0x01, PRONE = 0x02, CROUCH = 0x04, STAND = 0x08, FULL = 0x16} tileHeights; //if tile is solid at the flag's height
	unsigned char transparentFlag; //bit flag for transparency at different heights
	unsigned char walkableFlag; //bit flag for walkability at different heights

	std::vector<std::shared_ptr<Tile>> tileList; //list [5] of tiles that holds data at different heights

	bool explored; //if the tile has been explored yet

	Block(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag); //generic constructor that take character, foreground and background color, height, and if it is walkable

	Block(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, Tag tag); //constructor that take character, foreground and background color, height, if it is walkable, and the tile type

	std::shared_ptr<Tile> getTileData(int height) const;

	virtual bool getDestroyed(); //virtual returns if the tile has been destroyed

	void render(Position4 renderPosition, const std::shared_ptr<Pane>& pane) const; //renders tile

	virtual void destroy(int); //does nothing in block
	virtual void interact(); //virtual behaves differently depending on tile type, does nothing in tile
};

struct Destructible : public Block //destructible type of tile that can be destroyed by various items
{
	bool destroyed; //if the tile has been destroyed of not
	int strength; //CHECK  the strenth that the tile has before being destroyed

	Destructible(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int strength); //constructor for destructible that takes character, colors, height, walkability, and strength

	void destroy(int damage);
	bool getDestroyed(); //returns true if the tile has been destroyed
};

struct Stair : public Block //stair type of tile that allows travel between floors
{
	char moveDistance; //number of floors that the stair moves up or down
	
	Stair(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int moveDistance); //constructor for stair that takes character, colors, height, walkability, and move distance

	void interact(); //moves player the move distance
};


//map sections
/*

20 x 20 arrays 



*/