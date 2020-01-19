constexpr auto PI = 3.14159265;

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

#define CHAR_Pistol 128
#define CHAR_SMG 129
#define CHAR_Rifle 130
#define CHAR_DMR 131
#define CHAR_Sniper 132
#define CHAR_Backpack 133

#define CHAR_PistolMagazine 144
#define CHAR_SMGMagazine 145
#define CHAR_RifleMagazine 146
#define CHAR_DMRMagazine 147
#define CHAR_SniperMagazine 148

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
#define OOOOI Tile::FLOOR
#define OOOII Tile::FLOOR |		Tile::PRONE
#define OOIII Tile::FLOOR |		Tile::PRONE |	Tile::CROUCH
#define IIIII Tile::FLOOR |		Tile::PRONE |	Tile::CROUCH |	Tile::STAND |	Tile::FULL
#define OOIOI Tile::FLOOR |						Tile::CROUCH

//TILES
	//GROUND
		//GRASS PATTERNS
			#define TILE_Grass0				std::make_shared<Tile>			('.',				WORLDCOLOR_GrassFG,		WORLDCOLOR_GrassBG,		OOOOI,	OOOOI)
			#define TILE_Grass1				std::make_shared<Tile>			('"',				WORLDCOLOR_GrassFG,		WORLDCOLOR_GrassBG,		OOOOI,	OOOOI)
			#define TILE_Grass2				std::make_shared<Tile>			('`',				WORLDCOLOR_GrassFG,		WORLDCOLOR_GrassBG,		OOOOI,	OOOOI)
			#define TILE_Grass3				std::make_shared<Tile>			(',',				WORLDCOLOR_GrassFG,		WORLDCOLOR_GrassBG,		OOOOI,	OOOOI)

	#define TILE_BasicFlower				std::make_shared<Tile>			(CHAR_Flower,		WORLDCOLOR_FlowerFG,	WORLDCOLOR_FlowerBG,	OOOOI,	OOIII)
	#define TILE_BasicFloor					std::make_shared<Tile>			(' ',				WORLDCOLOR_FloorFG,		WORLDCOLOR_FloorBG,		OOOOI,	OOOOI)
	#define TILE_BasicConcrete				std::make_shared<Tile>			('`',				WORLDCOLOR_ConcreteFG,	WORLDCOLOR_ConcreteBG,	OOOOI,	OOOOI)
	#define TILE_BasicShingle				std::make_shared<Tile>			(240,				WORLDCOLOR_ShingleFG,	WORLDCOLOR_ShingleBG,	OOOOI,	OOOOI)

	//WALL
	#define TILE_BasicDoor					std::make_shared<Tile>			(CHAR_Door,			WORLDCOLOR_DoorFG,		WORLDCOLOR_WallBG,		IIIII,	OOOOI)
	#define DESTRUCTIBLE_BasicWall			std::make_shared<Destructible>	('#',				WORLDCOLOR_WallFG,		WORLDCOLOR_WallBG,		IIIII,	IIIII,	4)
	#define DESTRUCTIBLE_BasicWindow		std::make_shared<Destructible>	(CHAR_Window,		WORLDCOLOR_WindowFG,	WORLDCOLOR_WindowBG,	OOOOI,	OOIII,	1)

	//PART HEIGHT
	#define TILE_BasicTable					std::make_shared<Destructible>	(CHAR_Table,		WORLDCOLOR_TableFG,		WORLDCOLOR_FloorBG,		OOIOI,	OOIOI,	2)

	//STAIR
	#define STAIR_UpStair					std::make_shared<Stair>			(CHAR_UpStair,		WORLDCOLOR_DoorFG,		WORLDCOLOR_FloorBG,		OOOOI,	OOIII,	1)
	#define STAIR_DownStair					std::make_shared<Stair>			(CHAR_DownStair,	WORLDCOLOR_DoorFG,		WORLDCOLOR_FloorBG,		OOOOI,	OOIII,	-1)

	//SKY
	#define TILE_BasicSky					std::make_shared<Tile>			(' ',				WORLDCOLOR_WindowBG,	WORLDCOLOR_WindowBG,	OOOOO,	OOOOO)

	//ERROR
	#define TILE_error						std::make_shared<Tile>			('%',				TCODColor::black,		TCODColor::pink,		IIIII,	IIIII)

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
			#define DEFAULT_ITEM_TILE								std::make_shared<Tile>			('L',										TCODColor::darkestGrey,														TCODColor::darkGrey,	1,						true)

//CONTAINERS
	//SMALL BACKPACK
				#define TILE_SmallBackpack							std::make_shared<Tile>			(CHAR_Backpack,								ITEMCOLOR_SMALLBACKPACKFG,													ITEMCOLOR_ITEMBG,		1,						true)
				#define TOOL_SmallBackpack							std::make_shared<Tool>			("Small Backpack",							ITEMCOLOR_SMALLBACKPACKFG,													CHAR_Backpack)
			#define ITEM_SmallBackpack(x, y, level, owner)			std::make_shared<Item>			(2,											TILE_SmallBackpack,															TOOL_SmallBackpack,		Position(x, y, level),	owner,										Item::ItemType::NORMAL)
		#define CONTAINER_SmallBackpack(x, y, level, owner)			std::make_shared<Container>		(5,											ITEM_SmallBackpack(x, y, level, owner))

//HANDS
		#define TOOL_Hands											std::make_shared<Tool>			("Hands",									ITEMCOLOR_HANDFG,															TCOD_CHAR_UMLAUT)
	#define ITEM_Hands(x, y, level, owner)							std::make_shared<Item>			(1,											DEFAULT_ITEM_TILE,															TOOL_Hands,				Position(x, y, level),	owner,										Item::ItemType::NODROP)

//PISTOLS
	//SIP45 (standard issie pistol .45 cal)
		//WEAPON
				#define TILE_SIP45									std::make_shared<Tile>			(CHAR_Pistol,								ITEMCOLOR_PISTOLFG,															ITEMCOLOR_ITEMBG,		1,						true)
				#define WEAPON_SIP45								std::make_shared<Firearm>		("SIP45",									ITEMCOLOR_PISTOLFG,															.15f,					1.0f,					MagazineData::AmmoType::FOURTYFIVEACP,		Firearm::FireType::SEMI,	Firearm::FireType::SEMI | Firearm::FireType::SAFE)
			#define ITEM_SIP45(x, y, level, owner)					std::make_shared<Item>			(2,											TILE_SIP45,																	WEAPON_SIP45,			Position(x, y, level),	owner,										Item::ItemType::FIREARM)
		//MAGAZINE
					#define TILE_45Magazine7						std::make_shared<Tile>			(CHAR_PistolMagazine,						ITEMCOLOR_PISTOLFG,															ITEMCOLOR_ITEMBG,		1,						true)
					#define TOOL_45Magazine7						std::make_shared<Tool>			("45 Magazine -7-",						ITEMCOLOR_PISTOLFG,															CHAR_PistolMagazine)
				#define ITEM_45Magazine7(x, y, level, owner)		Item							(1, TILE_45Magazine7,						TOOL_45Magazine7,															Position(x, y, level),	owner,					Item::ItemType::NORMAL)
			#define MAGAZINE_45Magazine7(x, y, level, owner)		std::make_shared<MagazineItem>	(ITEM_45Magazine7(x, y, level, owner),		std::make_shared<MagazineData>(MagazineData::AmmoType::FOURTYFIVEACP,		7,						7))

//RIFLES
	//SIR556 (standard issue rifle 5.56 cal)
		//WEAPON
				#define TILE_SIR556									std::make_shared<Tile>			(CHAR_Rifle,								ITEMCOLOR_RIFLEFG,															ITEMCOLOR_ITEMBG,		1,						true)
				#define WEAPON_SIR556									std::make_shared<Firearm>	("SIR556",									ITEMCOLOR_RIFLEFG,															.09f,					2.0f,					MagazineData::AmmoType::FIVEPOINTFIVESIX,	Firearm::FireType::FULL,	Firearm::FireType::FULL | Firearm::FireType::SEMI | Firearm::FireType::SAFE)
			#define ITEM_SIR556(x, y, level, owner)					std::make_shared<Item>			(3,											TILE_SIR556,																WEAPON_SIR556,			Position(x, y, level),	owner,										Item::ItemType::FIREARM)
		//MAGAZINE
					#define TILE_556Magazine30						std::make_shared<Tile>			(CHAR_RifleMagazine,						ITEMCOLOR_RIFLEFG,															ITEMCOLOR_ITEMBG,		1,						true)
					#define TOOL_556Magazine30						std::make_shared<Tool>			("5.56 Magazine -30-",						ITEMCOLOR_RIFLEFG,															CHAR_RifleMagazine)
				#define ITEM_556Magazine30(x, y, level, owner)		Item							(1,											TILE_556Magazine30,															TOOL_556Magazine30,		Position(x, y, level),	owner,										Item::ItemType::NORMAL)
			#define MAGAZINE_556Magazine30(x, y, level, owner)		std::make_shared<MagazineItem>	(ITEM_556Magazine30(x, y, level, owner),	std::make_shared<MagazineData>(MagazineData::AmmoType::FIVEPOINTFIVESIX,	30,						30))
