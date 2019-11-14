constexpr auto PI = 3.14159265;

//CLASS SHORTCUTS
#define ENGINE		engine
#define SETTINGS	engine->settings
#define GUI			engine->gui
#define INPUT		engine->settings->input
#define MAPPANE		engine->gui->mapPane
#define WORLD		engine->gui->mapPane->world

/*
	FLOOR HEIGHTS

	4 :: Top    ^
	3 :: Stand  ^
	2 :: Crouch ^
	1 :: Prone  ^
	0 :: Floor & Window  ^

	if player height > floor height , visibility is true, else set to default vis
	stand can see over crouch
*/

//============TILES===============================CH===FOREGROUND=COLOR==========BACKGROUND=COLOR==========H==WALK===DEST |

//TILES
	//GROUND
	#define TILE_BasicGrass		std::make_shared<Tile>			('.',	TCODColor(119, 161, 63),	TCODColor(32, 70, 19),		0,	true,	"static")
	#define TILE_BasicFlower	std::make_shared<Tile>			('*',	TCODColor::darkChartreuse,	TCODColor::darkestSepia,	2,	true,	"static")
	#define TILE_BasicFloor		std::make_shared<Tile>			(' ',	TCODColor::darkSepia,		TCODColor::darkerSepia,		0,	true,	"static")
	#define TILE_BasicConcrete	std::make_shared<Tile>			('`',	TCODColor::darkGrey,		TCODColor::grey,			0,	true,	"static")
	#define TILE_BasicShingle	std::make_shared<Tile>			(240,	TCODColor::grey,			TCODColor::darkGrey,		0,	true,	"static")

	//WALL
	#define TILE_BasicDoor		std::make_shared<Tile>			('#',	TCODColor::darkSepia,		TCODColor::darkestSepia,	4,	true,	"static")
	#define TILE_BasicWall		std::make_shared<Destructible>	('=',	TCODColor::lightSepia,		TCODColor::lighterSepia,	4,	false,	4)
	#define TILE_BasicWindow	std::make_shared<Destructible>	('_',	TCODColor::lighterSepia,	TCODColor::darkerSky,		0,	false,	1)

	//PART HEIGHT
	#define TILE_BasicTable		std::make_shared<Destructible>	('n',	TCODColor::lighterSepia,	TCODColor::darkerSepia,		2,	false,	2)

	//STAIR
	#define TILE_UpStair		std::make_shared<Stair>			('/',	TCODColor::lightSepia,		TCODColor::darkerSepia,		2,	true,	1)
	#define TILE_DownStair		std::make_shared<Stair>			('\\',	TCODColor::lightSepia,		TCODColor::darkerSepia,		2,	true,	-1)

	//SKY
	#define TILE_BasicSky		std::make_shared<Tile>			(' ',	TCODColor::lightestCyan,	TCODColor::sky,				0,	false,	"static")

	//ERROR
	#define TILE_error			std::make_shared<Tile>			('%',	TCODColor::black,			TCODColor::pink,			4,	true ,	"static")

//TOOLS
	//Weapon(const char* name, TCODColor color, int ammoCap, int numberMags, float fireRate, float reloadSpeed);
	#define WEAPON_Test		std::shared_ptr<Weapon>	(TCODColor::darkestGrey,	30,		10,		5,	120)

//GUI
	#define RIBONBGCOLOR TCODColor::darkBlue
	#define RIBONFGCOLOR TCODColor::lightestGrey


//TEST ITEM
	#define TESTITEM std::make_shared<Item>(2, std::shared_ptr<Tile>('!', TCODColor::cyan, TCODColor::pink, 1, true, "static"), WEAPON_Test, 5, 5, 0)

//work on map container and updating inventory update render