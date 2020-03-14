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
	bool explored; //if the tile has been explored yet

	std::vector<std::shared_ptr<Tile>> tileList; //list [5] of tiles that holds data at different heights

	Block(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag); //generic constructor that take character, foreground and background color, height, and if it is walkable
	Block(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, Tag tag); //constructor that take character, foreground and background color, height, if it is walkable, and the tile type

	std::shared_ptr<Tile> getTileData(int height) const;

	virtual bool getDestroyed(); //virtual returns if the tile has been destroyed

	void render(Position4 renderPosition, const std::shared_ptr<Pane>& pane) const; //renders tile

	virtual void destroy(int); //does nothing in block
	virtual void interact(); //virtual behaves differently depending on tile type, does nothing in tile

	//static Block	grass0, grass1, grass2, grass3, flower, 
	//				floor, concrete, shingle, door, sky, error;

};

struct Destructible : public Block //destructible type of tile that can be destroyed by various items
{
	bool destroyed; //if the tile has been destroyed of not
	int strength; //CHECK  the strenth that the tile has before being destroyed

	Destructible(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int strength); //constructor for destructible that takes character, colors, height, walkability, and strength

	void destroy(int damage);
	bool getDestroyed(); //returns true if the tile has been destroyed

	//static Destructible wall, window, tableLeg, tableTop;
};

struct Stair : public Block //stair type of tile that allows travel between floors
{
	char moveDistance; //number of floors that the stair moves up or down
	
	Stair(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int moveDistance); //constructor for stair that takes character, colors, height, walkability, and move distance

	void interact(); //moves player the move distance

	//static Stair upStair, downStair;
};

namespace ep
{
	struct block
	{
		inline static const Block			grass0 = Block(DATA_Grass0, OOOOI, OOOOI);
		inline static const Block			grass1 = Block(DATA_Grass1, OOOOI, OOOOI);
		inline static const Block			grass2 = Block(DATA_Grass2, OOOOI, OOOOI);
		inline static const Block			grass3 = Block(DATA_Grass3, OOOOI, OOOOI);
		inline static const Block			flower = Block(DATA_BasicFlower, OOIII, OOOOI);
		inline static const Block			floor = Block(DATA_BasicFloor, OOOOI, OOOOI);
		inline static const Block			concrete = Block(DATA_BasicConcrete, OOOOI, OOOOI);
		inline static const Block			shingle = Block(DATA_BasicShingle, OOOOI, OOOOI);
		inline static const Block			door = Block(DATA_BasicDoor, IIIII, OOOOI);
		inline static const Block			sky = Block(DATA_BasicSky, OOOOO, OOOOO);
		inline static const Block			error = Block(DATA_Error, IIIII, IIIII);

		inline static const Destructible	wall = Destructible(DATA_BasicWall, IIIII, IIIII, 1000);
		inline static const Destructible	window = Destructible(DATA_BasicWindow, OOIII, IIIII, 100);
		inline static const Destructible	tableLeg = Destructible(DATA_BasicTableLeg, OOIII, OOIII, 500);
		inline static const Destructible	tableTop = Destructible(DATA_BasicTableTop, OOIOI, OOIOI, 500);

		inline static const Stair			upStair = Stair(DATA_UpStair, OOOOI, OOIII, 1);
		inline static const Stair			downStair = Stair(DATA_DownStair, OOOOI, OOIII, -1);
	};
}


//map sections
/*
CURRENT MAP
	INIT
		block list is directly created from the block list of the xml map

	UPDATE
		all tiles in the tile list are updated every frame

	RENDER

SECTION BASED MAP
	INIT
		details below

	UPDATE
		map should update all of the blocks in the section list as well as all of the sections that bullets interact with

		fovMap??  OPTIMIZATION:::
			HAS TO BE A SINGLE CONTINUOUS FOV MAP, COORDS CAN NOT BE OUT OF MAP
			UPDATE FOV MAP BASED ON COORDS FROM EACH SECTION

			IF FOV MAP IS NOT ON THE SAME LEVEL AS THE PLAYER HEIGHT, ONLY UPDATE EVERY SO OFTEN

	render
		render every block in the section list


RENDER VIEW PORT IS 60 x 60
	THE SECTION LIST SHOULD ALWAYS BE 5 X 5 ARRAY OF 15 X 15 SECTIONS

	THE START OF THE SECTION LIST IS THE OFFSET DIVIDED BY SECTION WIDTH : OFFSET 10 / SECTION WIDTH 15 = 0, START OF SECTION IS 0

	XMLMAP IS 1 DIMENSIONAL ARRAY; SHOULD BE MULTIPLE OF SECTION DIMENSION



	  0|    2|    3|
	|  |  |  |  |  |
-	0  1  2  3  4  5
-	6  7  8  9 10 11
-  12 13 14 15 16 17
-  18 19 20 21 22 23
-  24 25 26 27 28 29
-  30 31 32 33 34 35

SECTIONS 2 X 2

SECTION 0
	0  1
	6  7

SECTION 1
	2  3
	8  9

SECTION 2
	4  5
	10 11

	SHOULD THE MAP BE SAVED AS A SECTION ARRAY AND THEN THE MAP CLASS TAKES CERTAIN SECTIONS?
		OR SHOULD EACH SECTION BE TAKEN FROM THE MAP FILE EACH FRAME?

	SECTION LIST = VECTOR LEVELS ( VECTOR SECTIONS ( VECTOR BLOCKS ( VECTOR TILES )))


	FOR (INT FLOOR = 0; FLOOR <= TOTAL FLOORS; FLOOR ++)
	{
		FOR (INT SECT = 0; SECT <= (MAPWIDTH / SECTION WIDTH) * (MAP HEIGHT / SECTION HEIGHT); SECT ++)
		{
			FOR (INT Y = 0; Y <= MAPWIDTH * (SECTION HEIGHT - 1); Y += MAPWIDTH )
			{
				FOR (INT X = SECTION INDEX * SECTION WIDTH; I < SECTIONINDEX * SECTION WIDTH + SECTION WIDTH; X++)
				{
					SECTION LIST [ FLOOR ] [ SECT ] PUSHBACK (GET TILE FROM CODE ( MAPSTRING [X + Y] ))

					//SECTION LIST [ FLOORS ] PUSHBACK ( BLOCKLIST [GET TILE FROM CODE (MAPSTRING[X + Y]] )
				}
			}
		}
	}

*/

