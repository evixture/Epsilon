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

		inline static const std::vector<std::shared_ptr<Tile>> grass0 = 
		{
			std::make_shared<Tile>('.', ep::color::grassFG, ep::color::grassBG	, 999),
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> grass1 = 
		{ 
			std::make_shared<Tile>('"', ep::color::grassFG, ep::color::grassBG	, 999),
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> grass2 = 
		{
			std::make_shared<Tile>('`', ep::color::grassFG, ep::color::grassBG, 999),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> grass3 = 
		{
			std::make_shared<Tile>(',', ep::color::grassFG, ep::color::grassBG, 999),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> flower = 
		{
			std::make_shared<Tile>(ep::character::flower, ep::color::flowerFG, ep::color::flowerBG, 10),
			std::make_shared<Tile>(ep::character::flower, ep::color::flowerFG, ep::color::flowerBG, 10),
			std::make_shared<Tile>(ep::character::flower, ep::color::flowerFG, ep::color::flowerBG, 10),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> floor = 
		{
			std::make_shared<Tile>(' ', ep::color::floorFG, ep::color::floorBG, 999),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> concrete = 
		{
			std::make_shared<Tile>('`', ep::color::concreteFG, ep::color::concreteBG, 999),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> shingle = 
		{
			std::make_shared<Tile>(240, ep::color::shingleFG, ep::color::shingleBG, 999),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> door = 
		{
			std::make_shared<Tile>(ep::character::door, ep::color::doorFG, ep::color::wallBG, 25),
			std::make_shared<Tile>(ep::character::door, ep::color::doorFG, ep::color::wallBG, 25),
			std::make_shared<Tile>(ep::character::door, ep::color::doorFG, ep::color::wallBG, 25),
			std::make_shared<Tile>(ep::character::door, ep::color::doorFG, ep::color::wallBG, 25)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> wall = 
		{
			std::make_shared<Tile>('#', ep::color::wallFG, ep::color::wallBG, 50),
			std::make_shared<Tile>('#', ep::color::wallFG, ep::color::wallBG, 50),
			std::make_shared<Tile>('#', ep::color::wallFG, ep::color::wallBG, 50),
			std::make_shared<Tile>('#', ep::color::wallFG, ep::color::wallBG, 50)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> window = 
		{
			std::make_shared<Tile>('#', ep::color::wallFG, ep::color::wallBG, 50),
			std::make_shared<Tile>('#', ep::color::wallFG, ep::color::wallBG, 50),
			std::make_shared<Tile>('#', ep::color::wallFG, ep::color::wallBG, 50),
			std::make_shared<Tile>(ep::character::window, ep::color::windowFG, ep::color::windowBG, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> tableLeg = 
		{
			std::make_shared<Tile>(' ', ep::color::floorFG, ep::color::floorBG, 999),
			std::make_shared<Tile>('!', ep::color::tableFG, ep::color::floorBG, 25),
			std::make_shared<Tile>(ep::character::table, ep::color::tableFG, ep::color::floorBG, 25),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> tableTop = 
		{
			std::make_shared<Tile>(' ', ep::color::floorFG, ep::color::floorBG, 999),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(ep::character::table, ep::color::tableFG, ep::color::floorBG, 25),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> upStair = 
		{
			std::make_shared<Tile>(ep::character::upStair, ep::color::doorFG, ep::color::floorBG, 50),
			std::make_shared<Tile>(ep::character::upStair, ep::color::doorFG, ep::color::floorBG, 50),
			std::make_shared<Tile>(ep::character::upStair, ep::color::doorFG, ep::color::floorBG, 50),
			std::make_shared<Tile>(ep::character::upStair, ep::color::doorFG, ep::color::floorBG, 50)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> downStair = 
		{
			std::make_shared<Tile>(ep::character::downStair, ep::color::doorFG, ep::color::floorBG, 50),
			std::make_shared<Tile>(ep::character::downStair, ep::color::doorFG, ep::color::floorBG, 50),
			std::make_shared<Tile>(ep::character::downStair, ep::color::doorFG, ep::color::floorBG, 50),
			std::make_shared<Tile>(ep::character::downStair, ep::color::doorFG, ep::color::floorBG, 50)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> sky = 
		{
			std::make_shared<Tile>(' ', ep::color::windowFG, ep::color::windowBG, 0),
			std::make_shared<Tile>(' ', ep::color::windowFG, ep::color::windowBG, 0),
			std::make_shared<Tile>(' ', ep::color::windowFG, ep::color::windowBG, 0),
			std::make_shared<Tile>(' ', ep::color::windowFG, ep::color::windowBG, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> error = 
		{
			std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999),
			std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999),
			std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999),
			std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)
		};
																					
		inline static const std::vector<std::shared_ptr<Tile>> pistol = 
		{
			std::make_shared<Tile>(ep::character::pistol, ep::color::pistolFG, ep::color::itemBG, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> pistolMagazine = 
		{
			std::make_shared<Tile>(ep::character::pistolMagazine, ep::color::pistolFG, ep::color::itemBG, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> rifle = 
		{
			std::make_shared<Tile>(ep::character::rifle, ep::color::rifleFG, ep::color::itemBG, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> rifleMagazine = 
		{
			std::make_shared<Tile>(ep::character::rifleMagazine, ep::color::rifleFG, ep::color::itemBG, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> smallBackpack = 
		{
			std::make_shared<Tile>(ep::character::backpack, ep::color::smallBackpackFG, ep::color::itemBG, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> L1R3Armor = 
		{
			std::make_shared<Tile>(ep::character::ballisticVest, TCODColor::black, ep::color::itemBG, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	
		inline static const std::vector<std::shared_ptr<Tile>> knife = 
		{
			std::make_shared<Tile>(ep::character::knife, TCODColor::silver, ep::color::itemBG, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};
	};

	struct block
	{
		inline static const Block			grass0 =	Block(tileList::grass0,		ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block			grass1 =	Block(tileList::grass1,		ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block			grass2 =	Block(tileList::grass2,		ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block			grass3 =	Block(tileList::grass3,		ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block			flower =	Block(tileList::flower,		ep::tileFlag::OOIII, ep::tileFlag::OOOOI);
		inline static const Block			floor =		Block(tileList::floor,		ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block			concrete =	Block(tileList::concrete,	ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block			shingle =	Block(tileList::shingle,	ep::tileFlag::OOOOI, ep::tileFlag::OOOOI);
		inline static const Block			door =		Block(tileList::door,		ep::tileFlag::IIIII, ep::tileFlag::OOOOI);
		inline static const Block			sky = 		Block(tileList::sky,		ep::tileFlag::OOOOO, ep::tileFlag::OOOOO);
		inline static const Block			error =		Block(tileList::error,		ep::tileFlag::IIIII, ep::tileFlag::IIIII);

		inline static const Destructible	wall =		Destructible(tileList::wall,		ep::tileFlag::IIIII, ep::tileFlag::IIIII, 1000);
		inline static const Destructible	window =	Destructible(tileList::window,		ep::tileFlag::OOIII, ep::tileFlag::IIIII, 100);
		inline static const Destructible	tableLeg =	Destructible(tileList::tableLeg,	ep::tileFlag::OOIII, ep::tileFlag::OOIII, 500);
		inline static const Destructible	tableTop =	Destructible(tileList::tableTop,	ep::tileFlag::OOIOI, ep::tileFlag::OOIOI, 500);

		inline static const Stair			upStair =	Stair(tileList::upStair,	ep::tileFlag::OOOOI, ep::tileFlag::OOIII, 1);
		inline static const Stair			downStair = Stair(tileList::downStair,	ep::tileFlag::OOOOI, ep::tileFlag::OOIII, -1);

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
