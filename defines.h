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
//NEED TO EVENTUALLY MOVE TO A CLASS
#define COLOR_Panel_Ribbon_BG TCODColor(45, 45, 206)
#define COLOR_Panel_Ribbon_FG TCODColor(220, 220, 220)
#define COLOR_Panel_Draw_BG TCODColor(30, 30, 30)
#define COLOR_Panel_Draw_FG TCODColor(220, 220, 220)
#define COLOR_Player_Color TCODColor(0, 127, 255)
#define COLOR_Root_BG TCODColor(0, 0, 0)
#define COLOR_Root_FG TCODColor(220, 220, 220)
#define COLOR_OutFOV_BG TCODColor(0, 0, 0)
#define COLOR_OutFOV_FG TCODColor(30, 30, 30)
#define COLOR_Selector TCODColor(168, 13, 224)

//============TILES===============================CH===FOREGROUND=COLOR==========BACKGROUND=COLOR==========H==WALK===DEST |

//TILES
	//GROUND
		//GRASS PATTERNS
			#define TILE_BasicGrass0		std::make_shared<Tile>			('.',	TCODColor(119, 161, 63),	TCODColor(32, 70, 19),		0,	true,	"static")
			#define TILE_BasicGrass1		std::make_shared<Tile>			('"',	TCODColor(119, 161, 63),	TCODColor(32, 70, 19),		0,	true,	"static")
			#define TILE_BasicGrass2		std::make_shared<Tile>			('`',	TCODColor(119, 161, 63),	TCODColor(32, 70, 19),		0,	true,	"static")
			#define TILE_BasicGrass3		std::make_shared<Tile>			(',',	TCODColor(119, 161, 63),	TCODColor(32, 70, 19),		0,	true,	"static")

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

	//ITEM TILES
		//edit tile set to custom sprites
	#define DEFAULT_ITEM_TILE	std::make_shared<Tile>('L', TCODColor::darkestGrey, TCODColor::darkGrey, 1, true, "static")
	#define DEFAULT_ITEM_TILE2	std::make_shared<Tile>('!', TCODColor::cyan, TCODColor::pink, 1, true, "static")

//TOOLS
	#define TOOL_Hands std::make_shared<Tool>("Hands", TCODColor::lightSepia)
	#define TOOL_Default_Container std::make_shared<Tool>("Test Container", TCODColor::pink)

	//Weapon(const char* name, TCODColor color, int ammoCap, int numberMags, float fireRate, float reloadSpeed);
	#define WEAPON_DefaultRifle std::make_shared<Weapon>("WEAPON_DefaultRifle", TCODColor::darkestGrey, 30, 10, .09f, 2.0f)
	#define WEAPON_DefaultRifle2 std::make_shared<Weapon>("WEAPON_DefaultRifle2", TCODColor::white, 30, 10, .09f, 2.0f)

//GUI
	#define RIBONBGCOLOR TCODColor::darkBlue
	#define RIBONFGCOLOR TCODColor::lightestGrey

//TEST ITEM
	#define ITEM_Hands(x, y, level) std::make_shared<Item>(1, DEFAULT_ITEM_TILE, TOOL_Hands, Position(x, y, level))
	#define ITEM_Test_Size2(x, y, level) std::make_shared<Item>(2, DEFAULT_ITEM_TILE, WEAPON_DefaultRifle, Position(x, y, level))
	#define ITEM_Test2_Size2(x, y, level) std::make_shared<Item>(2, DEFAULT_ITEM_TILE, WEAPON_DefaultRifle2, Position(x, y, level))
	#define ITEM_Default_Container(x, y, level) std::make_shared<Item>(5, DEFAULT_ITEM_TILE2, TOOL_Default_Container, Position(x, y, level))

//CONTAINERS
	#define CONTAINER_Default_Container(x, y, level, capacity) std::make_shared<Container>(capacity, ITEM_Default_Container(x, y, level))


//work on map container and updating inventory update render