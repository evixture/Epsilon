#include "main.hpp"

/*

TABLE

4 - transparent
3 - transparent
2 - solid
1 - transparent
0 - solid

walkability
	get player height
	if at player height, tile is not solid
		if at player height - n, tile is not solid
			if tile at height 0 is set
				tile is walkable
	 
transparency
	if at player height, tile flag is unset, tile is transparent
*/

struct Tile
{
	unsigned char ch;
	TCODColor foregroundColor; //foreground color of the tile
	TCODColor backgroundColor; //background color of the tile
	unsigned short int deceleration;

	Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int deceleration);
};

struct Block //tile class used for the map and items
{
	enum class Tag {STATIC, DESTRUCTIBLE, STAIR} tag; //type and functionality of the tile
	enum TileHeights {FLOOR = 0x01, PRONE = 0x02, CROUCH = 0x04, STAND = 0x08, FULL = 0x16} tileHeights; //if tile is solid at the flag's height
	unsigned char transparentFlag;
	unsigned char walkableFlag;

	std::vector<std::shared_ptr<Tile>> tileList;

	bool explored; //if the tile has been explored yet

	Block(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag); //generic constructor that take character, foreground and background color, height, and if it is walkable

	Block(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, Tag tag); //constructor that take character, foreground and background color, height, if it is walkable, and the tile type

	std::shared_ptr<Tile> getTileData(int height) const;

	virtual bool getDestroyed(); //virtual returns if the tile has been destroyed
	virtual void interact(); //virtual behaves differently depending on tile type

	void render(Position4 renderPosition, const std::shared_ptr<Pane>& pane) const; //renders tile
};

struct Destructible : public Block //destructible type of tile that can be destroyed by various items
{
	bool destroyed; //if the tile has been destroyed of not
	int strength; //CHECK  the strenth that the tile has before being destroyed

	Destructible(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int strength); //constructor for destructible that takes character, colors, height, walkability, and strength

	bool getDestroyed(); //returns true if the tile has been destroyed
	void interact(); //increments strength down until destroyed
};

struct Stair : public Block //stair type of tile that allows travel between floors
{
	char moveDistance; //number of floors that the stair moves up or down
	
	Stair(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int moveDistance); //constructor for stair that takes character, colors, height, walkability, and move distance

	void interact(); //moves player the move distance
};
