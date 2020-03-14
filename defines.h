constexpr auto PI = 3.14159265;

//#pragma warning (disable : C26812)


//CLASS SHORTCUTS
#define ENGINE		engine
#define SETTINGS	engine->settings
#define GUI			engine->gui
#define INPUT		engine->settings->input
#define MAPPANE		engine->gui->worldWindow
#define WORLD		engine->gui->worldWindow->world

//----------------------------------------------------------------------------------------------------

/*
	CHAR MAPPINGS
	~	| 00 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 | 11 | 12 | 13 | 14 | 15 |
	|TILES
	|	| 16 | 17 | 18 | 19 | 20 | 21 | ?? | ?? | 24 | 26 | 26 | 27 | ?? | 29 | 30 | 31 |
	|
	~	| 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 |
	
	~	| 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 | 57 | 58 | 59 | 60 | 61 | 62 | 63 |
	|ALPHA
	|	| 64 | 65 | 66 | 67 | 68 | 69 | 70 | 71 | 72 | 73 | 74 | 75 | 76 | 77 | 78 | 79 |
	|NUMBER
	|	| 80 | 81 | 82 | 83 | 84 | 85 | 86 | 87 | 88 | 89 | 90 | 91 | 92 | 93 | 94 | 95 |
	|SYMBOLS
	|	| 96 | 97 | 98 | 99 | 100| 101| 102| 103| 104| 105| 106| 107| 108| 109| 110| 111|
	|ETC
	~	| 112| 113| 114| 115| 116| 117| 118| 119| 120| 121| 122| 123| 124| 125| 126| 127|
	
	~	| 128| 129| 130| 131| 132| 133| 134| 135| 136| 137| 138| 139| 140| 141| 142| 143|
	|TOOLS
	|	| 144| 145| 146| 147| 148| 149| 150| 151| 152| 153| 154| 155| 156| 157| 158| 159|
	|ITEMS
	~	| 160| 161| 162| 163| 164| 165| 166| 167| 168| 169| 170| 171| 172| 173| 174| 175|
	
	~	| 176| 177| 178| 179| 180| 181| 182| 183| 184| 185| 186| 187| 188| 189| 190| 191|
	|OTHER CHARS
	|	| 192| 193| 194| 195| 196| 197| 198| 199| 200| 201| 202| 203| 204| 205| 206| 207|
	|
	|	| 208| 209| 210| 211| 212| 213| 214| 215| 216| 217| 218| 219| 220| 221| 222| 223|
	|
	|	| 224| 225| 226| 227| 228| 229| 230| 231| 232| 233| 234| 235| 236| 237| 238| 239|
	|
	~	| 240| 241| 242| 243| 244| 245| 246| 247| 248| 249| 250| 251| 252| 253| 254| 255|
*/

#define CHAR_UpStair 16
#define CHAR_DownStair 17
#define CHAR_Door 18
#define CHAR_Flower 19
#define CHAR_Table 20
#define CHAR_Window 21

#define CHAR_BottomSquareBrace 22
#define CHAR_TopSquareBrace 23
#define CHAR_VerticalEquals 24

#define CHAR_Pistol 128
#define CHAR_SMG 129
#define CHAR_Rifle 130
#define CHAR_DMR 131
#define CHAR_Sniper 132
#define CHAR_Backpack 133
#define CHAR_BallisticVest 134

#define CHAR_PistolMagazine 144
#define CHAR_SMGMagazine 145
#define CHAR_RifleMagazine 146
#define CHAR_DMRMagazine 147
#define CHAR_SniperMagazine 148

#define CHAR_Knife 160

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

//----------------------------------------------------------------------------------------------------

//COLORS
#define UICOLOR_Panel_Ribbon_BG		TCODColor(45, 45, 206)
#define UICOLOR_Panel_Ribbon_FG		TCODColor(220, 220, 220)
#define UICOLOR_Panel_Draw_BG		TCODColor(30, 30, 30)
#define UICOLOR_Panel_Draw_FG		TCODColor(220, 220, 220)
#define UICOLOR_Player_Color		TCODColor(0, 127, 255)
#define UICOLOR_Root_BG				TCODColor(0, 0, 0)
#define UICOLOR_Root_FG				TCODColor(220, 220, 220)
#define UICOLOR_OutFOV_BG			TCODColor(0, 0, 0)
#define UICOLOR_OutFOV_FG			TCODColor(30, 30, 30)
#define UICOLOR_Selector			TCODColor(168, 13, 224)

#define UICOLOR_MessageLow			TCODColor(220, 220, 220)
#define UICOLOR_MessageMedium		TCODColor(178, 179, 236)
#define UICOLOR_MessageHigh			TCODColor(223, 166, 243)


#define WORLDCOLOR_GrassBG			TCODColor(32, 70, 19)
#define WORLDCOLOR_GrassFG			TCODColor(119, 161, 63)

#define WORLDCOLOR_FlowerBG			TCODColor(45, 39, 39)
#define WORLDCOLOR_FlowerFG			TCODColor(254, 212, 16)

#define WORLDCOLOR_FloorBG			TCODColor(98, 72, 49)
#define WORLDCOLOR_FloorFG			TCODColor(98, 72, 49)

#define WORLDCOLOR_ShingleBG		TCODColor(33, 35, 32)
#define WORLDCOLOR_ShingleFG		TCODColor(105, 111, 109)

#define WORLDCOLOR_ConcreteBG		TCODColor(114, 115, 119)
#define WORLDCOLOR_ConcreteFG		TCODColor(150, 151, 153)

#define WORLDCOLOR_DoorFG			TCODColor(63, 50, 31)

#define WORLDCOLOR_WallBG			TCODColor(145, 128, 118)
#define WORLDCOLOR_WallFG			TCODColor(112, 96, 83)

#define WORLDCOLOR_WindowBG			TCODColor(68, 180, 246)
#define WORLDCOLOR_WindowFG			TCODColor(237, 237, 237)

#define WORLDCOLOR_TableFG			TCODColor(150, 123, 104)


#define ITEMCOLOR_ITEMBG			TCODColor(254, 77, 0)

#define ITEMCOLOR_HANDFG			TCODColor(243, 198, 165)

#define ITEMCOLOR_SMALLBACKPACKFG	TCODColor(182, 34, 46)

#define ITEMCOLOR_RIFLEFG			TCODColor(79, 83, 84)

#define ITEMCOLOR_PISTOLFG			TCODColor(94, 93, 91)

//----------------------------------------------------------------------------------------------------

//============TILES==================================================CH===FOREGROUND=COLOR==========BACKGROUND=COLOR==========H==WALK===DEST |

//tile height macros
//	heights go from full to floor
#define OOOOO 0
#define OOOOI Block::FLOOR
#define OOOII Block::FLOOR |		Block::PRONE
#define OOIII Block::FLOOR |		Block::PRONE |	Block::CROUCH
#define IIIII Block::FLOOR |		Block::PRONE |	Block::CROUCH |	Block::STAND |	Block::FULL
#define OOIOI Block::FLOOR |						Block::CROUCH

//TILE DATA
//	STARTS WITH BOTTOM LEVEL
//	USE 0 FOR AN EMPTY TILE, SPACE FOR BACKGROUND, AND CHAR FOR CHAR
#define DATA_Grass0																						\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>('.', WORLDCOLOR_GrassFG, WORLDCOLOR_GrassBG	, 999),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0)					\
		}																								\
																										
#define DATA_Grass1																						\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>('"', WORLDCOLOR_GrassFG, WORLDCOLOR_GrassBG	, 999),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0)					\
		}																								\
																										
#define DATA_Grass2																						\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>('`', WORLDCOLOR_GrassFG, WORLDCOLOR_GrassBG	, 999),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0)					\
		}																								\
																										
#define DATA_Grass3																						\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(',', WORLDCOLOR_GrassFG, WORLDCOLOR_GrassBG	, 999),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0)					\
		}																								\
																										
#define DATA_BasicFlower																				\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(CHAR_Flower, WORLDCOLOR_FlowerFG,	WORLDCOLOR_FlowerBG	, 10),		\
			std::make_shared<Tile>(CHAR_Flower, WORLDCOLOR_FlowerFG,	WORLDCOLOR_FlowerBG	, 10),		\
			std::make_shared<Tile>(CHAR_Flower, WORLDCOLOR_FlowerFG,	WORLDCOLOR_FlowerBG	, 10),		\
			std::make_shared<Tile>(0,			TCODColor::pink,		TCODColor::pink		, 0)		\
		}																								\
																										
#define DATA_BasicFloor																					\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(' ', WORLDCOLOR_FloorFG, WORLDCOLOR_FloorBG	, 999),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),					\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0)					\
		}																								\
																										
#define DATA_BasicConcrete																				\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>('`', WORLDCOLOR_ConcreteFG,	WORLDCOLOR_ConcreteBG	, 999),			\
			std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink			, 0),			\
			std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink			, 0),			\
			std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink			, 0)			\
		}																								\
																										
#define DATA_BasicShingle																				\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(240, WORLDCOLOR_ShingleFG,	WORLDCOLOR_ShingleBG, 999),				\
			std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink		, 0),				\
			std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink		, 0),				\
			std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink		, 0)				\
		}																								\
																										
#define DATA_BasicDoor																					\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(CHAR_Door, WORLDCOLOR_DoorFG, WORLDCOLOR_WallBG, 25),				\
			std::make_shared<Tile>(CHAR_Door, WORLDCOLOR_DoorFG, WORLDCOLOR_WallBG, 25),				\
			std::make_shared<Tile>(CHAR_Door, WORLDCOLOR_DoorFG, WORLDCOLOR_WallBG, 25),				\
			std::make_shared<Tile>(CHAR_Door, WORLDCOLOR_DoorFG, WORLDCOLOR_WallBG, 25)				\
		}																								\
																										
#define DATA_BasicWall																					\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>('#', WORLDCOLOR_WallFG, WORLDCOLOR_WallBG, 50),						\
			std::make_shared<Tile>('#', WORLDCOLOR_WallFG, WORLDCOLOR_WallBG, 50),						\
			std::make_shared<Tile>('#', WORLDCOLOR_WallFG, WORLDCOLOR_WallBG, 50),						\
			std::make_shared<Tile>('#', WORLDCOLOR_WallFG, WORLDCOLOR_WallBG, 50)						\
		}																								\
																										
#define DATA_BasicWindow																				\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>('#',			WORLDCOLOR_WallFG,		WORLDCOLOR_WallBG	, 50),		\
			std::make_shared<Tile>('#',			WORLDCOLOR_WallFG,		WORLDCOLOR_WallBG	, 50),		\
			std::make_shared<Tile>('#',			WORLDCOLOR_WallFG,		WORLDCOLOR_WallBG	, 50),		\
			std::make_shared<Tile>(CHAR_Window, WORLDCOLOR_WindowFG,	WORLDCOLOR_WindowBG	, 0)		\
		}																								\
																										
#define DATA_BasicTableLeg																				\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(' ',			WORLDCOLOR_FloorFG, WORLDCOLOR_FloorBG	,999),			\
			std::make_shared<Tile>('!',			WORLDCOLOR_TableFG, WORLDCOLOR_FloorBG	,25),			\
			std::make_shared<Tile>(CHAR_Table,	WORLDCOLOR_TableFG, WORLDCOLOR_FloorBG	,25),			\
			std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink		,0)			\
		}																								\
																										
#define DATA_BasicTableTop																				\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(' ',			WORLDCOLOR_FloorFG, WORLDCOLOR_FloorBG	, 999),			\
			std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink		, 0),			\
			std::make_shared<Tile>(CHAR_Table,	WORLDCOLOR_TableFG, WORLDCOLOR_FloorBG	, 25),			\
			std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink		, 0)			\
		}																								\
																										
#define DATA_UpStair																					\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(CHAR_UpStair, WORLDCOLOR_DoorFG, WORLDCOLOR_FloorBG	, 50),			\
			std::make_shared<Tile>(CHAR_UpStair, WORLDCOLOR_DoorFG, WORLDCOLOR_FloorBG	, 50),			\
			std::make_shared<Tile>(CHAR_UpStair, WORLDCOLOR_DoorFG, WORLDCOLOR_FloorBG	, 50),			\
			std::make_shared<Tile>(CHAR_UpStair, WORLDCOLOR_DoorFG, WORLDCOLOR_FloorBG	, 50)			\
		}																								\
																										
#define DATA_DownStair																					\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(CHAR_DownStair, WORLDCOLOR_DoorFG, WORLDCOLOR_FloorBG, 50),			\
			std::make_shared<Tile>(CHAR_DownStair, WORLDCOLOR_DoorFG, WORLDCOLOR_FloorBG, 50),			\
			std::make_shared<Tile>(CHAR_DownStair, WORLDCOLOR_DoorFG, WORLDCOLOR_FloorBG, 50),			\
			std::make_shared<Tile>(CHAR_DownStair, WORLDCOLOR_DoorFG, WORLDCOLOR_FloorBG, 50)			\
		}																								\
																										
#define DATA_BasicSky																					\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(' ', WORLDCOLOR_WindowBG, WORLDCOLOR_WindowBG, 0),					\
			std::make_shared<Tile>(' ', WORLDCOLOR_WindowBG, WORLDCOLOR_WindowBG, 0),					\
			std::make_shared<Tile>(' ', WORLDCOLOR_WindowBG, WORLDCOLOR_WindowBG, 0),					\
			std::make_shared<Tile>(' ', WORLDCOLOR_WindowBG, WORLDCOLOR_WindowBG, 0)					\
		}																								\
																										
#define DATA_Error																						\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999),							\
			std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999),							\
			std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999),							\
			std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)							\
		}																								\
																										
//TOOL TILE DATA																						
#define DATA_Pistol																						\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(CHAR_Pistol, ITEMCOLOR_PISTOLFG, ITEMCOLOR_ITEMBG, 0),				\
			std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0),				\
			std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0),				\
			std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0)				\
		}																								\
																										
#define DATA_PistolMagazine																				\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(CHAR_PistolMagazine, ITEMCOLOR_PISTOLFG, ITEMCOLOR_ITEMBG, 0),		\
			std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0),		\
			std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0),		\
			std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0)		\
		}																								\
																										
#define DATA_Rifle																						\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(CHAR_Rifle,	ITEMCOLOR_RIFLEFG,	ITEMCOLOR_ITEMBG, 0),				\
			std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0),				\
			std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0),				\
			std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0)				\
		}																								\
																										
#define DATA_RifleMagazine																				\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(CHAR_RifleMagazine,	ITEMCOLOR_RIFLEFG,	ITEMCOLOR_ITEMBG, 0),		\
			std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0),		\
			std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0),		\
			std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0)		\
		}																								\
																										
#define DATA_Backpack																					\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(CHAR_Backpack,	ITEMCOLOR_SMALLBACKPACKFG,	ITEMCOLOR_ITEMBG, 0),	\
			std::make_shared<Tile>(0,				TCODColor::pink,			TCODColor::pink	, 0),	\
			std::make_shared<Tile>(0,				TCODColor::pink,			TCODColor::pink	, 0),	\
			std::make_shared<Tile>(0,				TCODColor::pink,			TCODColor::pink	, 0)	\
		}																								\

#define DATA_L1R3Armor																					\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(CHAR_BallisticVest,	TCODColor::black,	ITEMCOLOR_ITEMBG, 0),		\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink	, 0),						\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink	, 0),						\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink	, 0)						\
		}																								\

#define DATA_Knife																						\
		std::vector<std::shared_ptr<Tile>>{																\
			std::make_shared<Tile>(CHAR_Knife,	TCODColor::silver,		ITEMCOLOR_ITEMBG, 0),			\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink	, 0),						\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink	, 0),						\
			std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink	, 0)						\
		}																								\


//TILES
	//GROUND
		//GRASS PATTERNS
//			#define TILE_Grass0				std::make_shared<Block>			(DATA_Grass0,			OOOOI,	OOOOI)
//			#define TILE_Grass1				std::make_shared<Block>			(DATA_Grass1,			OOOOI,	OOOOI)
//			#define TILE_Grass2				std::make_shared<Block>			(DATA_Grass2,			OOOOI,	OOOOI)
//			#define TILE_Grass3				std::make_shared<Block>			(DATA_Grass3,			OOOOI,	OOOOI)

//	#define TILE_BasicFlower				std::make_shared<Block>			(DATA_BasicFlower,		OOIII,	OOOOI)
//	#define TILE_BasicFloor					std::make_shared<Block>			(DATA_BasicFloor,		OOOOI,	OOOOI)
//	#define TILE_BasicConcrete				std::make_shared<Block>			(DATA_BasicConcrete,	OOOOI,	OOOOI)
//	#define TILE_BasicShingle				std::make_shared<Block>			(DATA_BasicShingle,		OOOOI,	OOOOI)

	//WALL
//	#define TILE_BasicDoor					std::make_shared<Block>			(DATA_BasicDoor,		IIIII,	OOOOI)
//	#define DESTRUCTIBLE_BasicWall			std::make_shared<Destructible>	(DATA_BasicWall,		IIIII,	IIIII,	1000)
//	#define DESTRUCTIBLE_BasicWindow		std::make_shared<Destructible>	(DATA_BasicWindow,		OOIII,	IIIII,	100)

	//PART HEIGHT
//	#define TILE_BasicTableLeg				std::make_shared<Destructible>	(DATA_BasicTableLeg,	OOIII,	OOIII,	500)
//	#define TILE_BasicTableTop				std::make_shared<Destructible>	(DATA_BasicTableTop,	OOIOI,	OOIOI,	500)

	//STAIR
//	#define STAIR_UpStair					std::make_shared<Stair>			(DATA_UpStair,			OOOOI,	OOIII,	1)
//	#define STAIR_DownStair					std::make_shared<Stair>			(DATA_DownStair,		OOOOI,	OOIII, -1)

	//SKY
//	#define TILE_BasicSky					std::make_shared<Block>			(DATA_BasicSky,			OOOOO,	OOOOO)

	//ERROR
//	#define TILE_error						std::make_shared<Block>			(DATA_Error,			IIIII,	IIIII)

	//----------------------------------------------------------------------------------------------------

/*
ITEM SIZES
	1 UNIT
		COMPACT PISTOLS
	2 UNITS
		COMPACT SMGS
		PISTOLS
	3 UNITS
		STANDARD RIFLES
		LARGE SMGS
	4 UNITS
		MACHINE GUNS
		LARGE RIFLES
	5 UNITS
		SNIPER RIFLES
*/

//DEFAULTS
			#define DEFAULT_ITEM_TILE									std::make_shared<Block>			(DATA_BasicDoor, OOOOI, OOOOI)

//CONTAINERS
	//SMALL BACKPACK
				#define TILE_SmallBackpack								std::make_shared<Block>			(DATA_Backpack, OOOOI, OOOOI)
				#define TOOL_SmallBackpack								std::make_shared<Tool>			("Small Backpack",							ITEMCOLOR_SMALLBACKPACKFG,													CHAR_Backpack)
			#define ITEM_SmallBackpack(x, y, level, owner)				std::make_shared<Item>			(2,											TILE_SmallBackpack,															TOOL_SmallBackpack,				Position4(x, y, 0, level),		owner,										Item::ItemType::NORMAL)
		#define CONTAINER_SmallBackpack(x, y, level, owner)				std::make_shared<Container>		(5,											ITEM_SmallBackpack(x, y, level, owner))

//ITEMS
	//HANDS
			#define MELEE_Hands											std::make_shared<Melee>			(Tool("Hands",								ITEMCOLOR_HANDFG,															TCOD_CHAR_UMLAUT),								30,																			20)
		#define ITEM_Hands(x, y, level, owner)							std::make_shared<Item>			(1,											DEFAULT_ITEM_TILE,															MELEE_Hands,						Position4(x, y, 0, level),		owner,										Item::ItemType::HAND)

	//Level 1, Rank 3 Armor (level 1 is 100 strength, rank 3 is 300 durability)
			#define TILE_L1R3Armor										std::make_shared<Block>			(DATA_L1R3Armor, OOOOI, OOOOI)
			#define ARMOR_L1R3Armor										std::make_shared<Armor>			("test armor",								TCODColor::black,															100,							300)
		#define ITEM_L1R3Armor(x, y, level, owner)						std::make_shared<Item>			(2,											TILE_L1R3Armor,																ARMOR_L1R3Armor,				Position4(x, y, 0, level),		owner,										Item::ItemType::ARMOR)

	//PISTOLS
		//SIP45 (standard issue pistol .45 cal)
			//WEAPON
					#define TILE_SIP45									std::make_shared<Block>			(DATA_Pistol, OOOOI, OOOOI)
					#define WEAPON_SIP45								std::make_shared<Firearm>		("SIP45",									ITEMCOLOR_PISTOLFG,															2,							1.0f,							MagazineData::AmmoType::FOURTYFIVEACP,		Firearm::FireType::SEMI,	Firearm::FireType::SEMI | Firearm::FireType::SAFE)
				#define ITEM_SIP45(x, y, level, owner)					std::make_shared<Item>			(2,											TILE_SIP45,																	WEAPON_SIP45,					Position4(x, y, 0, level),		owner,										Item::ItemType::FIREARM)
			//MAGAZINE
						#define TILE_45Magazine7						std::make_shared<Block>			(DATA_PistolMagazine, OOOOI, OOOOI)
						#define TOOL_45Magazine7						std::make_shared<Tool>			("45 Magazine -7-",							ITEMCOLOR_PISTOLFG,															CHAR_PistolMagazine)
					#define ITEM_45Magazine7(x, y, level, owner)		Item							(1, TILE_45Magazine7,						TOOL_45Magazine7,															Position4(x, y, 0, level),		owner,							Item::ItemType::NORMAL)
				#define MAGAZINE_45Magazine7(x, y, level, owner)		std::make_shared<MagazineItem>	(ITEM_45Magazine7(x, y, level, owner),		std::make_shared<MagazineData>(MagazineData::AmmoType::FOURTYFIVEACP,		7,								7))

	//RIFLES
		//SIR556 (standard issue rifle 5.56 cal)
			//WEAPON
					#define TILE_SIR556									std::make_shared<Block>			(DATA_Rifle, OOOOI, OOOOI)
					#define WEAPON_SIR556								std::make_shared<Firearm>		("SIR556",									ITEMCOLOR_RIFLEFG,															10,								2.0f,							MagazineData::AmmoType::FIVEPOINTFIVESIX,	Firearm::FireType::FULL,	Firearm::FireType::FULL | Firearm::FireType::SEMI | Firearm::FireType::SAFE)
				#define ITEM_SIR556(x, y, level, owner)					std::make_shared<Item>			(3,											TILE_SIR556,																WEAPON_SIR556,					Position4(x, y, 0, level),		owner,										Item::ItemType::FIREARM)
			//MAGAZINE
						#define TILE_556Magazine30						std::make_shared<Block>			(DATA_RifleMagazine, OOOOI, OOOOI)
						#define TOOL_556Magazine30						std::make_shared<Tool>			("5.56 Magazine -30-",						ITEMCOLOR_RIFLEFG,															CHAR_RifleMagazine)
					#define ITEM_556Magazine30(x, y, level, owner)		Item							(1,											TILE_556Magazine30,															TOOL_556Magazine30,				Position4(x, y, 0, level),		owner,										Item::ItemType::NORMAL)
				#define MAGAZINE_556Magazine30(x, y, level, owner)		std::make_shared<MagazineItem>	(ITEM_556Magazine30(x, y, level, owner),	std::make_shared<MagazineData>(MagazineData::AmmoType::FIVEPOINTFIVESIX,	30,								30))

	//MELEE
		//Knife
			#define TILE_Knife											std::make_shared<Block>			(DATA_Knife, OOOOI, OOOOI)
			#define MELEE_Knife											std::make_shared<Melee>			(Tool("-Test Knife 34 damage-",				TCODColor::silver,															CHAR_Knife),							0,								34)
		#define ITEM_Knife(x, y, level, owner)							std::make_shared<Item>			(1,											TILE_Knife,																	MELEE_Knife,				Position4(x, y, 0, level),		owner,										Item::ItemType::MELEE)

