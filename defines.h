constexpr auto PI = 3.14159265;

//CLASS SHORTCUTS
#define ENGINE		engine
#define SETTINGS	engine->settings
#define GUI			engine->gui
#define INPUT		engine->settings->input
#define MAPPANE		engine->gui->mapPane
#define WORLD		engine->gui->mapPane->world

/*
	CHAR MAPPINGS
	| 00 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 | 11 | 12 | 13 | 14 | 15 |

	| 16 | 17 | 18 | 19 | 20 | 21 | ?? | ?? | 24 | 26 | 26 | 27 | ?? | 29 | 30 | 31 |
	
	| 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 |
	
	| 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 | 57 | 58 | 59 | 60 | 61 | 62 | 63 |
	
	| 64 | 65 | 66 | 67 | 68 | 69 | 70 | 71 | 72 | 73 | 74 | 75 | 76 | 77 | 78 | 79 |
	
	| 80 | 81 | 82 | 83 | 84 | 85 | 86 | 87 | 88 | 89 | 90 | 91 | 92 | 93 | 94 | 95 |
	
	| 96 | 97 | 98 | 99 | 100| 101| 102| 103| 104| 105| 106| 107| 108| 109| 110| 111|
	
	| 112| 113| 114| 115| 116| 117| 118| 119| 120| 121| 122| 123| 124| 125| 126| 127|
	
	| 128| 129| 130| 131| 132| 133| 134| 135| 136| 137| 138| 139| 140| 141| 142| 143|
	
	| 144| 145| 146| 147| 148| 149| 150| 151| 152| 153| 154| 155| 156| 157| 158| 159|
	
	| 160| 161| 162| 163| 164| 165| 166| 167| 168| 169| 170| 171| 172| 173| 174| 175|
	
	| 176| 177| 178| 179| 180| 181| 182| 183| 184| 185| 186| 187| 188| 189| 190| 191|
	
	| 192| 193| 194| 195| 196| 197| 198| 199| 200| 201| 202| 203| 204| 205| 206| 207|
	
	| 208| 209| 210| 211| 212| 213| 214| 215| 216| 217| 218| 219| 220| 221| 222| 223|
	
	| 224| 225| 226| 227| 228| 229| 230| 231| 232| 233| 234| 235| 236| 237| 238| 239|
	
	|240 | 241| 242| 243| 244| 245| 246| 247| 248| 249| 250| 251| 252| 253| 254| 255|
*/

#define CHAR_UpStair 16
#define CHAR_DownStair 17
#define CHAR_Flower 19
#define CHAR_Table 20

#define CHAR_Pistol 128

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
//COLORS
#define COLOR_Panel_Ribbon_BG TCODColor	(45, 45, 206)
#define COLOR_Panel_Ribbon_FG TCODColor	(220, 220, 220)
#define COLOR_Panel_Draw_BG TCODColor	(30, 30, 30)
#define COLOR_Panel_Draw_FG TCODColor	(220, 220, 220)
#define COLOR_Player_Color TCODColor	(0, 127, 255)
#define COLOR_Root_BG TCODColor			(0, 0, 0)
#define COLOR_Root_FG TCODColor			(220, 220, 220)
#define COLOR_OutFOV_BG TCODColor		(0, 0, 0)
#define COLOR_OutFOV_FG TCODColor		(30, 30, 30)
#define COLOR_Selector TCODColor		(168, 13, 224)

//============TILES===============================CH===FOREGROUND=COLOR==========BACKGROUND=COLOR==========H==WALK===DEST |

//TILES
	//GROUND
		//GRASS PATTERNS
			#define TILE_BasicGrass0		std::make_shared<Tile>			('.',	TCODColor(119, 161, 63),	TCODColor(32, 70, 19),		0,	true,	Tile::Tag::STATIC)
			#define TILE_BasicGrass1		std::make_shared<Tile>			('"',	TCODColor(119, 161, 63),	TCODColor(32, 70, 19),		0,	true,	Tile::Tag::STATIC)
			#define TILE_BasicGrass2		std::make_shared<Tile>			('`',	TCODColor(119, 161, 63),	TCODColor(32, 70, 19),		0,	true,	Tile::Tag::STATIC)
			#define TILE_BasicGrass3		std::make_shared<Tile>			(',',	TCODColor(119, 161, 63),	TCODColor(32, 70, 19),		0,	true,	Tile::Tag::STATIC)

	#define TILE_BasicFlower				std::make_shared<Tile>			(CHAR_Flower,	TCODColor::darkChartreuse,	TCODColor::darkestSepia,	2,	true,	Tile::Tag::STATIC)
	#define TILE_BasicFloor					std::make_shared<Tile>			(' ',	TCODColor::darkSepia,		TCODColor::darkerSepia,		0,	true,	Tile::Tag::STATIC)
	#define TILE_BasicConcrete				std::make_shared<Tile>			('`',	TCODColor::darkGrey,		TCODColor::grey,			0,	true,	Tile::Tag::STATIC)
	#define TILE_BasicShingle				std::make_shared<Tile>			(240,	TCODColor::grey,			TCODColor::darkGrey,		0,	true,	Tile::Tag::STATIC)

	//WALL
	#define TILE_BasicDoor					std::make_shared<Tile>			(18,	TCODColor::darkSepia,		TCODColor::darkestSepia,	4,	true,	Tile::Tag::STATIC)
	#define DESTRUCTIBLE_BasicWall			std::make_shared<Destructible>	('=',	TCODColor::lightSepia,		TCODColor::lighterSepia,	4,	false,	4)
	#define DESTRUCTIBLE_BasicWindow		std::make_shared<Destructible>	('_',	TCODColor::lighterSepia,	TCODColor::darkerSky,		0,	false,	1)

	//PART HEIGHT
	#define TILE_BasicTable					std::make_shared<Destructible>	(CHAR_Table,	TCODColor::lighterSepia,	TCODColor::darkerSepia,		2,	false,	2)

	//STAIR
	#define STAIR_UpStair					std::make_shared<Stair>			(CHAR_UpStair,	TCODColor::lightSepia,		TCODColor::darkerSepia,		2,	true,	1)
	#define STAIR_DownStair					std::make_shared<Stair>			(CHAR_DownStair,	TCODColor::lightSepia,		TCODColor::darkerSepia,		2,	true,	-1)

	//SKY
	#define TILE_BasicSky					std::make_shared<Tile>			(' ',	TCODColor::lightestCyan,	TCODColor::sky,				0,	false,	Tile::Tag::STATIC)

	//ERROR
	#define TILE_error						std::make_shared<Tile>			('%',	TCODColor::black,			TCODColor::pink,			4,	true ,	Tile::Tag::STATIC)

	//ITEM TILES
		//edit tile set to custom sprites
		//DEFAULT
			#define DEFAULT_ITEM_TILE	std::make_shared<Tile>('L', TCODColor::darkestGrey, TCODColor::darkGrey, 1, true, Tile::Tag::STATIC)
			#define DEFAULT_ITEM_TILE2	std::make_shared<Tile>('!', TCODColor::cyan, TCODColor::pink, 1, true, Tile::Tag::STATIC)
		//CUSTOM
			#define TILE_SmallBackpack std::make_shared<Tile>(233, TCODColor::sepia, TCODColor::darkSepia, 1, true, Tile::Tag::STATIC)

//----------------------------------------------------------------------------------------------------

//TOOLS
	//DEFAULT
		#define TOOL_Default_Container std::make_shared<Tool>("Test Container", TCODColor::pink, TCOD_CHAR_RADIO_UNSET)
	//CUSTOM
		#define TOOL_Hands std::make_shared<Tool>("Hands", TCODColor::lightSepia, TCOD_CHAR_UMLAUT)
		#define TOOL_SmallBackpack std::make_shared<Tool>("Small Backpack", TCODColor::sepia, 233)

	//WEAPONS
		//DEFAULT
			//Weapon(const char* name, TCODColor color, int ammoCap, int numberMags, float fireRate, float reloadSpeed);
			#define WEAPON_DefaultRifle std::make_shared<Weapon>("WEAPON_DefaultRifle", TCODColor::darkestGrey, 30, 10, .09f, 2.0f, MagazineData::AmmoType::FIVEPOINTFIVESIX, Weapon::FireType::FULL)
			#define WEAPON_DefaultSemiRifle std::make_shared<Weapon>("WEAPON_DefaultSemiRifle", TCODColor::white, 30, 10, .09f, 2.0f, MagazineData::AmmoType::FIVEPOINTFIVESIX, Weapon::FireType::SEMI)
		//CUSTOM


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

//----------------------------------------------------------------------------------------------------

//ITEMS
	//DEFAULT
		#define ITEM_Test_Size2(x, y, level) std::make_shared<Item>(2, DEFAULT_ITEM_TILE, WEAPON_DefaultRifle, Position(x, y, level))
		#define ITEM_Test2_Size2(x, y, level) std::make_shared<Item>(2, DEFAULT_ITEM_TILE, WEAPON_DefaultSemiRifle, Position(x, y, level))
		#define ITEM_Default_Container(x, y, level) std::make_shared<Item>(5, DEFAULT_ITEM_TILE2, TOOL_Default_Container, Position(x, y, level))
	//CUSTOM
		#define ITEM_Hands(x, y, level) std::make_shared<Item>(1, DEFAULT_ITEM_TILE, TOOL_Hands, Position(x, y, level))

		#define ITEM_SmallBackpack(x, y, level) std::make_shared<Item>(2, TILE_SmallBackpack , TOOL_SmallBackpack, Position(x, y, level))

			//cannot use shared ptr because of inheritance

		
//MAGAZINES
	//DEFAULT
	//CUSTOM

//----------------------------------------------------------------------------------------------------

//CONTAINERS
	//DEFAULT
		#define CONTAINER_Default_Container(x, y, level, capacity) std::make_shared<Container>(capacity, ITEM_Default_Container(x, y, level))
	//CUSTOM
		#define CONTAINER_SmallBackpack(x, y, level) std::make_shared<Container>(5, ITEM_SmallBackpack(x, y, level))


//PISTOLS
	//M1911
		//WEAPON
				#define TILE_M1911 std::make_shared<Tile>(CHAR_Pistol, TCODColor::copper, TCODColor::darkerGrey, 1, true, Tile::Tag::STATIC)
				#define WEAPON_M1911 std::make_shared<Weapon>("M1911", TCODColor::grey, 7, 10, .15f, 1.0f, MagazineData::AmmoType::FOURTYFIVEACP, Weapon::FireType::SEMI)
			#define ITEM_M1911(x, y, level) std::make_shared<Item>(2, TILE_M1911 , WEAPON_M1911, Position(x, y, level))
		//MAGAZINE
					#define TILE_45ACPMagazine7 std::make_shared<Tile>('/', TCODColor::darkerGrey, TCODColor::darkGrey, 1, true, Tile::Tag::STATIC)
					#define TOOL_45ACPMagazine7 std::make_shared<Tool>("45 ACP Magazine -7-", TCODColor::darkGrey, '/')
				#define ITEM_45ACPMagazine7(x, y, level) Item(1, TILE_45ACPMagazine7 , TOOL_45ACPMagazine7, Position(x, y, level))
			#define MAGAZINE_45ACPMagazine7(x, y, level) std::make_shared<MagazineItem>(ITEM_45ACPMagazine7(x, y, level), std::make_shared<MagazineData>(MagazineData::AmmoType::FOURTYFIVEACP, 7 , 7))

//RIFLES
	//M4A1
		//WEAPON
				#define TILE_M4A1 std::make_shared<Tile>(214, TCODColor::darkestGrey, TCODColor::darkGrey, 1, true, Tile::Tag::STATIC)
				#define WEAPON_M4A1 std::make_shared<Weapon>("M4A1", TCODColor::darkestGrey, 30, 10, .09f, 2.0f, MagazineData::AmmoType::FIVEPOINTFIVESIX, Weapon::FireType::FULL)
			#define ITEM_M4A1(x, y, level) std::make_shared<Item>(3, TILE_M4A1 , WEAPON_M4A1, Position(x, y, level))
		//MAGAZINE
					#define TILE_556Magazine30 std::make_shared<Tile>('/', TCODColor::darkerGrey, TCODColor::darkGrey, 1, true, Tile::Tag::STATIC)
					#define TOOL_556Magazine30 std::make_shared<Tool>("5.56 Magazine -30-", TCODColor::darkGrey, 'j')
				#define ITEM_556Magazine30(x, y, level) Item(1, TILE_556Magazine30 , TOOL_556Magazine30, Position(x, y, level))
			#define MAGAZINE_556Magazine30(x, y, level) std::make_shared<MagazineItem>(ITEM_556Magazine30(x, y, level), std::make_shared<MagazineData>(MagazineData::AmmoType::FIVEPOINTFIVESIX, 30 , 30))
