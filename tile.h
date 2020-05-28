#include "main.hpp"

struct Tile
{
	unsigned char ch; //the character to render on the map
	TCODColor foregroundColor; //foreground color of the tile
	TCODColor backgroundColor; //background color of the tile
	unsigned short int deceleration; //the amount that a bullet is slowed down by when it passes through it
	int strength;

	std::string name;

	Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int deceleration, std::string name = "", int strength = -1);
};

struct Block //tile class used for the map and items
{
	enum class Tag {STATIC, STAIR} tag; //type and functionality of the tile
	unsigned char transparentFlag; //bit flag for transparency at different heights
	unsigned char walkableFlag; //bit flag for walkability at different heights
	bool explored; //if the tile has been explored yet
	bool destroyed;

	std::vector<Tile> tileList; //list [5] of tiles that holds data at different heights

	Block(std::vector<Tile> tileList, unsigned char transparentFlag, unsigned char walkableFlag); //generic constructor that take character, foreground and background color, height, and if it is walkable
	Block(std::vector<Tile> tileList, unsigned char transparentFlag, unsigned char walkableFlag, Tag tag); //constructor that take character, foreground and background color, height, if it is walkable, and the tile type

	Tile getTileData(int height) const;

	void render(Position4 renderPosition, const Pane& pane) const; //renders tile

	virtual bool destroy(int damage, int height); //does nothing in block
	virtual void interact(); //virtual behaves differently depending on tile type, does nothing in tile
};

struct Stair : public Block //stair type of tile that allows travel between floors
{
	char moveDistance; //number of floors that the stair moves up or down
	
	Stair(std::vector<Tile> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int moveDistance); //constructor for stair that takes character, colors, height, walkability, and move distance

	void interact(); //moves player the move distance
};

namespace ep
{
	struct tileList
	{
		/*
			FLOOR HEIGHTS
		
			4 :: Top    ^
			3 :: Stand  ^
			2 :: Crouch ^
			1 :: Prone  ^
			0 :: Floor & Window  ^
		
			if player height > floor height , visibility is true, else set to default vis
			stand can see over crouch
		
		
			TILE
		
			|	8 ft (ceiling)
			| o	6 ft (stand)
			| T	4 ft (crouch)
			| ^	2 ft (prone)
			 __ 0 ft (ground)
			/_/ 2 ft
			  2 ft
		*/

		inline static const std::vector<Tile> grass0 =
		{
			Tile('.',	ep::color::grassFG, ep::color::grassBG, 999, "Grass"),
			Tile(0,		TCODColor::pink,	TCODColor::pink,	0),
			Tile(0,		TCODColor::pink,	TCODColor::pink,	0),
			Tile(0,		TCODColor::pink,	TCODColor::pink,	0)
		};
	
		inline static const std::vector<Tile> grass1 =
		{ 
			Tile('"',	ep::color::grassFG, ep::color::grassBG, 999, "Grass"),
			Tile(0,		TCODColor::pink,	TCODColor::pink,	0),
			Tile(0,		TCODColor::pink,	TCODColor::pink,	0),
			Tile(0,		TCODColor::pink,	TCODColor::pink,	0)
		};
	
		inline static const std::vector<Tile> grass2 =
		{
			Tile('`',	ep::color::grassFG, ep::color::grassBG, 999, "Grass"),
			Tile(0,		TCODColor::pink,	TCODColor::pink,	0),
			Tile(0,		TCODColor::pink,	TCODColor::pink,	0),
			Tile(0,		TCODColor::pink,	TCODColor::pink,	0)
		};
	
		inline static const std::vector<Tile> grass3 =
		{
			Tile(',',	ep::color::grassFG, ep::color::grassBG, 999, "Grass"),
			Tile(0,		TCODColor::pink,	TCODColor::pink,	0),
			Tile(0,		TCODColor::pink,	TCODColor::pink,	0),
			Tile(0,		TCODColor::pink,	TCODColor::pink,	0)
		};
	
		inline static const std::vector<Tile> flower =
		{
			Tile(ep::character::flower,		ep::color::flowerFG,	ep::color::flowerBG,	999,	"Flower"),
			Tile(ep::character::flower,		ep::color::flowerFG,	ep::color::flowerBG,	10,		"Flower",	10),
			Tile(ep::character::flower,		ep::color::flowerFG,	ep::color::flowerBG,	10,		"Flower",	10),
			Tile(0,							TCODColor::pink,		TCODColor::pink,		0,		"",			10)
		};
	
		inline static const std::vector<Tile> floor =
		{
			Tile(' ', ep::color::floorFG, ep::color::floorBG, 999, "Floor"),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<Tile> concrete =
		{
			Tile('`', ep::color::concreteFG, ep::color::concreteBG, 999, "Concrete"),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<Tile> shingle =
		{
			Tile(240, ep::color::shingleFG, ep::color::shingleBG, 999, "Shingles"),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<Tile> door =
		{
			Tile(ep::character::door, ep::color::doorFG, ep::color::wallBG, 25, "Door"),
			Tile(ep::character::door, ep::color::doorFG, ep::color::wallBG, 25, "Door"),
			Tile(ep::character::door, ep::color::doorFG, ep::color::wallBG, 25, "Door"),
			Tile(ep::character::door, ep::color::doorFG, ep::color::wallBG, 25, "Door")
		};
	
		inline static const std::vector<Tile> wall =
		{
			Tile('#', ep::color::wallFG, ep::color::wallBG, 50, "Wall", -1),
			Tile('#', ep::color::wallFG, ep::color::wallBG, 50, "Wall", 1000),
			Tile('#', ep::color::wallFG, ep::color::wallBG, 50, "Wall", 1000),
			Tile('#', ep::color::wallFG, ep::color::wallBG, 50, "Wall", 1000)
		};
	
		inline static const std::vector<Tile> window =
		{
			Tile('#', ep::color::wallFG, ep::color::wallBG, 50, "Wall", -1),
			Tile('#', ep::color::wallFG, ep::color::wallBG, 50, "Wall", 1000),
			Tile('#', ep::color::wallFG, ep::color::wallBG, 50, "Wall", 1000),
			Tile(ep::character::window, ep::color::windowFG, ep::color::windowBG, 0, "Window", 100)
		};
	
		inline static const std::vector<Tile> tableLeg =
		{
			Tile(' ', ep::color::floorFG, ep::color::floorBG, 999, "Floor"),
			Tile('!', ep::color::tableFG, ep::color::floorBG, 25, "Table Leg", 200),
			Tile(ep::character::table, ep::color::tableFG, ep::color::floorBG, 25, "Table Top", 500),
			Tile(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<Tile> tableTop =
		{
			Tile(' ', ep::color::floorFG, ep::color::floorBG, 999, "Floor"),
			Tile(0, TCODColor::pink, TCODColor::pink, 0, "Floor", 200),
			Tile(ep::character::table, ep::color::tableFG, ep::color::floorBG, 25, "Table Top", 500),
			Tile(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<Tile> upStair =
		{
			Tile(ep::character::upStair, ep::color::doorFG, ep::color::floorBG, 50, "Upwards Stair"),
			Tile(ep::character::upStair, ep::color::doorFG, ep::color::floorBG, 50, "Upwards Stair"),
			Tile(ep::character::upStair, ep::color::doorFG, ep::color::floorBG, 50, "Upwards Stair"),
			Tile(ep::character::upStair, ep::color::doorFG, ep::color::floorBG, 50, "Upwards Stair")
		};
	
		inline static const std::vector<Tile> downStair =
		{
			Tile(ep::character::downStair, ep::color::doorFG, ep::color::floorBG, 50, "Downwards Stair"),
			Tile(ep::character::downStair, ep::color::doorFG, ep::color::floorBG, 50, "Downwards Stair"),
			Tile(ep::character::downStair, ep::color::doorFG, ep::color::floorBG, 50, "Downwards Stair"),
			Tile(ep::character::downStair, ep::color::doorFG, ep::color::floorBG, 50, "Downwards Stair")
		};
	
		inline static const std::vector<Tile> sky =
		{
			Tile(' ', ep::color::windowFG, ep::color::windowBG, 0, "Sky"),
			Tile(' ', ep::color::windowFG, ep::color::windowBG, 0, "Sky"),
			Tile(' ', ep::color::windowFG, ep::color::windowBG, 0, "Sky"),
			Tile(' ', ep::color::windowFG, ep::color::windowBG, 0, "Sky")
		};
	
		inline static const std::vector<Tile> error =
		{
			Tile('%', TCODColor::pink, TCODColor::pink, 999, "ERROR"),
			Tile('%', TCODColor::pink, TCODColor::pink, 999, "ERROR"),
			Tile('%', TCODColor::pink, TCODColor::pink, 999, "ERROR"),
			Tile('%', TCODColor::pink, TCODColor::pink, 999, "ERROR")
		};
																					
		inline static const std::vector<Tile> pistol =
		{
			Tile(ep::character::pistol, ep::color::pistolFG, ep::color::itemBG, 0, "Pistol"),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<Tile> pistolMagazine =
		{
			Tile(ep::character::pistolMagazine, ep::color::pistolFG, ep::color::itemBG, 0, "Pistol Magazine"),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<Tile> rifle =
		{
			Tile(ep::character::rifle, ep::color::rifleFG, ep::color::itemBG, 0, "Rifle"),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<Tile> rifleMagazine =
		{
			Tile(ep::character::rifleMagazine, ep::color::rifleFG, ep::color::itemBG, 0, "Rifle Magazine"),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<Tile> smallBackpack =
		{
			Tile(ep::character::backpack, ep::color::smallBackpackFG, ep::color::itemBG, 0, "Small Backpack"),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<Tile> L1R3Armor =
		{
			Tile(ep::character::ballisticVest, TCODColor::black, ep::color::itemBG, 0, "Level 1, R3 Armor"),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<Tile> knife =
		{
			Tile(ep::character::knife, TCODColor::silver, ep::color::itemBG, 0, "Knife"),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0),
			Tile(0, TCODColor::pink, TCODColor::pink, 0)
		};
	};

	struct block
	{
		inline static const Block	grass0 =	Block(tileList::grass0,		ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block	grass1 =	Block(tileList::grass1,		ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block	grass2 =	Block(tileList::grass2,		ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block	grass3 =	Block(tileList::grass3,		ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block	floor =		Block(tileList::floor,		ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block	concrete =	Block(tileList::concrete,	ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block	shingle =	Block(tileList::shingle,	ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block	door =		Block(tileList::door,		ep::tileFlag::IIIII, ep::tileFlag::OOOOI);
		inline static const Block	sky = 		Block(tileList::sky,		ep::tileFlag::OOOOO, ep::tileFlag::OOOOO);
		inline static const Block	error =		Block(tileList::error,		ep::tileFlag::IIIII, ep::tileFlag::IIIII);

		//destructible
		inline static const Block	flower =	Block(tileList::flower,		ep::tileFlag::OOIII, ep::tileFlag::OOOOI);
		inline static const Block	wall =		Block(tileList::wall,		ep::tileFlag::IIIII, ep::tileFlag::IIIII);
		inline static const Block	window =	Block(tileList::window,		ep::tileFlag::OOIII, ep::tileFlag::IIIII);
		inline static const Block	tableLeg =	Block(tileList::tableLeg,	ep::tileFlag::OOIII, ep::tileFlag::OOIII);
		inline static const Block	tableTop =	Block(tileList::tableTop,	ep::tileFlag::OOIOI, ep::tileFlag::OOIOI);

		inline static const Stair	upStair =	Stair(tileList::upStair,	ep::tileFlag::OOOOI, ep::tileFlag::OOIII, 1);
		inline static const Stair	downStair = Stair(tileList::downStair,	ep::tileFlag::OOOOI, ep::tileFlag::OOIII, -1);

		struct item
		{
			inline static const Block defBlock =			Block(ep::tileList::door,			ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
			inline static const Block smallBackpack =		Block(ep::tileList::smallBackpack,	ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
			inline static const Block L1R3Armor =			Block(ep::tileList::L1R3Armor,		ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
			inline static const Block sip45 =				Block(ep::tileList::pistol,			ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
			inline static const Block cal45Magazine7 =		Block(ep::tileList::pistolMagazine, ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
			inline static const Block sir556 =				Block(ep::tileList::rifle,			ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
			inline static const Block cal556Magazine30 =	Block(ep::tileList::rifleMagazine,	ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
			inline static const Block knife =				Block(ep::tileList::knife,			ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		};
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
