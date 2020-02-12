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

namespace ep
{
	namespace symbol
	{
		constexpr char upStair = 16;
		constexpr char downStair = 17;
		constexpr char door = 18;
		constexpr char flower = 19;
		constexpr char table = 20;
		constexpr char window = 21;
		 	
		constexpr char pistol = 128;
		constexpr char SMG = 129;
		constexpr char rifle = 130;
		constexpr char DMR = 131;
		constexpr char sniper = 132;
		constexpr char backpack = 133;
		 	
		constexpr char pistolMagazine = 144;
		constexpr char SMGMagazine = 145;
		constexpr char rifleMagazine = 146;
		constexpr char DMRMagazine = 147;
		constexpr char sniperMagazine = 148;
	}

	namespace color
	{
		namespace gui
		{
			const TCODColor Panel_Ribbon_BG	= 	TCODColor(45, 45, 206)	;
			const TCODColor Panel_Ribbon_FG	= 	TCODColor(220, 220, 220);
			const TCODColor Panel_Draw_BG	=	TCODColor(30, 30, 30)	;
			const TCODColor Panel_Draw_FG	=	TCODColor(220, 220, 220);
			const TCODColor Selector		=	TCODColor(168, 13, 224)	;
			 
			const TCODColor Player_Color	=	TCODColor(0, 127, 255)	;
			 
			const TCODColor Root_BG			= 	TCODColor(0, 0, 0)		;
			const TCODColor Root_FG			= 	TCODColor(220, 220, 220);
			 
			const TCODColor OutFOV_BG		=	TCODColor(0, 0, 0)		;
			const TCODColor OutFOV_FG		=	TCODColor(30, 30, 30)	;
			 
			const TCODColor MessageLow		= 	TCODColor(220, 220, 220);
			const TCODColor MessageMedium	=	TCODColor(178, 179, 236);
			const TCODColor MessageHigh		= 	TCODColor(223, 166, 243);
		}																

		namespace world
		{
			const TCODColor GrassBG		=	TCODColor(32, 70, 19)	;
			const TCODColor GrassFG		=	TCODColor(119, 161, 63)	;
			 						
			const TCODColor FlowerBG		=	TCODColor(45, 39, 39)	;
			const TCODColor FlowerFG		=	TCODColor(254, 212, 16)	;
			  						
			const TCODColor FloorBG		=	TCODColor(98, 72, 49)	;
			const TCODColor FloorFG		=	TCODColor(98, 72, 49)	;
			  						
			const TCODColor ShingleBG		=	TCODColor(33, 35, 32)	;
			const TCODColor ShingleFG		=	TCODColor(105, 111, 109);
			  						
			const TCODColor ConcreteBG	=	TCODColor(114, 115, 119);
			const TCODColor ConcreteFG	=	TCODColor(150, 151, 153);
			  						
			const TCODColor DoorFG		=	TCODColor(63, 50, 31)	;
			  						
			const TCODColor WallBG		=	TCODColor(145, 128, 118);
			const TCODColor WallFG		=	TCODColor(112, 96, 83)	;
			  						
			const TCODColor WindowBG		=	TCODColor(68, 180, 246)	;
			const TCODColor WindowFG		=	TCODColor(237, 237, 237);
			 						
			const TCODColor TableFG		=	TCODColor(150, 123, 104);
		}

		namespace item
		{
			const TCODColor ITEMBG			= TCODColor(254, 77, 0)		;
			 
			const TCODColor HANDFG			= TCODColor(243, 198, 165)	;
			 
			const TCODColor SMALLBACKPACKFG	= TCODColor(182, 34, 46)	;
			 
			const TCODColor RIFLEFG			= TCODColor(79, 83, 84)		;
			 
			const TCODColor PISTOLFG			= TCODColor(94, 93, 91)		;
		}
	}

	namespace blockFlag
	{
						/*	floor	prone	crouch	stand	full	*/
		const char OOOOO  = 0										;
		const char OOOOI  = 0x01									;
		const char OOOII  = 0x01 |	0x02							;
		const char OOIII  = 0x01 |	0x02 |	0x04					;
		const char IIIII  = 0x01 |	0x02 |	0x04 |	0x08 |	0x16	;
		const char OOIOI  = 0x01 |			0x04					;
	}

	namespace blockData
	{
		//struct Tile;

		std::vector<std::shared_ptr<Tile>> Grass0 =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>('.', ep::color::world::GrassFG, ep::color::world::GrassBG	, 999),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0)
		};

		std::vector<std::shared_ptr<Tile>> Grass1 =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>('"', ep::color::world::GrassFG, ep::color::world::GrassBG	, 999),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0)
		};

		std::vector<std::shared_ptr<Tile>> Grass2 =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>('`', ep::color::world::GrassFG, ep::color::world::GrassBG	, 999),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0)
		};

		std::vector<std::shared_ptr<Tile>> Grass3 =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(',', ep::color::world::GrassFG, ep::color::world::GrassBG	, 999),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0)
		};

		std::vector<std::shared_ptr<Tile>> BasicFlower =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(ep::symbol::flower, ep::color::world::FlowerFG,	ep::color::world::FlowerBG	, 10),
				std::make_shared<Tile>(ep::symbol::flower, ep::color::world::FlowerFG,	ep::color::world::FlowerBG	, 10),
				std::make_shared<Tile>(ep::symbol::flower, ep::color::world::FlowerFG,	ep::color::world::FlowerBG	, 10),
				std::make_shared<Tile>(0,			TCODColor::pink,		TCODColor::pink		, 0),
				std::make_shared<Tile>(0,			TCODColor::pink,		TCODColor::pink		, 0)
		};

		std::vector<std::shared_ptr<Tile>> BasicFloor =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(' ', ep::color::world::FloorFG, ep::color::world::FloorBG	, 999),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink		, 0)
		};

		std::vector<std::shared_ptr<Tile>> BasicConcrete =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>('`', ep::color::world::ConcreteFG,	ep::color::world::ConcreteBG	, 999),
				std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink			, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink			, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink			, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink			, 0)
		};

		std::vector<std::shared_ptr<Tile>> BasicShingle =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(240, ep::color::world::ShingleFG,	ep::color::world::ShingleBG, 999),
				std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink		, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,		TCODColor::pink		, 0)
		};

		std::vector<std::shared_ptr<Tile>> BasicDoor =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(ep::symbol::door, ep::color::world::DoorFG, ep::color::world::WallBG, 25),
				std::make_shared<Tile>(ep::symbol::door, ep::color::world::DoorFG, ep::color::world::WallBG, 25),
				std::make_shared<Tile>(ep::symbol::door, ep::color::world::DoorFG, ep::color::world::WallBG, 25),
				std::make_shared<Tile>(ep::symbol::door, ep::color::world::DoorFG, ep::color::world::WallBG, 25),
				std::make_shared<Tile>(ep::symbol::door, ep::color::world::DoorFG, ep::color::world::WallBG, 25)
		};

		std::vector<std::shared_ptr<Tile>> BasicWall =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>('#', ep::color::world::WallFG, ep::color::world::WallBG, 50),
				std::make_shared<Tile>('#', ep::color::world::WallFG, ep::color::world::WallBG, 50),
				std::make_shared<Tile>('#', ep::color::world::WallFG, ep::color::world::WallBG, 50),
				std::make_shared<Tile>('#', ep::color::world::WallFG, ep::color::world::WallBG, 50),
				std::make_shared<Tile>('#',	ep::color::world::WallFG, ep::color::world::WallBG, 50)
		};

		std::vector<std::shared_ptr<Tile>> BasicWindow =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>('#',			ep::color::world::WallFG,		ep::color::world::WallBG	, 50),
				std::make_shared<Tile>('#',			ep::color::world::WallFG,		ep::color::world::WallBG	, 50),
				std::make_shared<Tile>('#',			ep::color::world::WallFG,		ep::color::world::WallBG	, 50),
				std::make_shared<Tile>(ep::symbol::window, ep::color::world::WindowFG,	ep::color::world::WindowBG	, 0),
				std::make_shared<Tile>(ep::symbol::window, ep::color::world::WindowFG,	ep::color::world::WindowBG	, 0)
		};

		std::vector<std::shared_ptr<Tile>> BasicTableLeg =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(' ',			ep::color::world::FloorFG, ep::color::world::FloorBG	,999),
				std::make_shared<Tile>('!',			ep::color::world::TableFG, ep::color::world::FloorBG	,25),
				std::make_shared<Tile>(ep::symbol::table,	ep::color::world::TableFG, ep::color::world::FloorBG	,25),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink		,0),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink		,0)
		};

		std::vector<std::shared_ptr<Tile>> BasicTableTop =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(' ',			ep::color::world::FloorFG, ep::color::world::FloorBG	, 999),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(ep::symbol::table,	ep::color::world::TableFG, ep::color::world::FloorBG	, 25),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink		, 0),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink		, 0)
		};

		std::vector<std::shared_ptr<Tile>> UpStair =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(ep::symbol::upStair, ep::color::world::DoorFG, ep::color::world::FloorBG	, 50),
				std::make_shared<Tile>(ep::symbol::upStair, ep::color::world::DoorFG, ep::color::world::FloorBG	, 50),
				std::make_shared<Tile>(ep::symbol::upStair, ep::color::world::DoorFG, ep::color::world::FloorBG	, 50),
				std::make_shared<Tile>(ep::symbol::upStair, ep::color::world::DoorFG, ep::color::world::FloorBG	, 50),
				std::make_shared<Tile>(ep::symbol::upStair, ep::color::world::DoorFG, ep::color::world::FloorBG	, 50)
		};

		std::vector<std::shared_ptr<Tile>> DownStair =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(ep::symbol::downStair, ep::color::world::DoorFG, ep::color::world::FloorBG, 50),
				std::make_shared<Tile>(ep::symbol::downStair, ep::color::world::DoorFG, ep::color::world::FloorBG, 50),
				std::make_shared<Tile>(ep::symbol::downStair, ep::color::world::DoorFG, ep::color::world::FloorBG, 50),
				std::make_shared<Tile>(ep::symbol::downStair, ep::color::world::DoorFG, ep::color::world::FloorBG, 50),
				std::make_shared<Tile>(ep::symbol::downStair, ep::color::world::DoorFG, ep::color::world::FloorBG, 50)
		};

		std::vector<std::shared_ptr<Tile>> BasicSky =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(' ', ep::color::world::WindowBG, ep::color::world::WindowBG, 0),
				std::make_shared<Tile>(' ', ep::color::world::WindowBG, ep::color::world::WindowBG, 0),
				std::make_shared<Tile>(' ', ep::color::world::WindowBG, ep::color::world::WindowBG, 0),
				std::make_shared<Tile>(' ', ep::color::world::WindowBG, ep::color::world::WindowBG, 0),
				std::make_shared<Tile>(' ', ep::color::world::WindowBG, ep::color::world::WindowBG, 0)
		};

		std::vector<std::shared_ptr<Tile>> Error =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999),
				std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999),
				std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999),
				std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999),
				std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)
		};

		//TOOL TILE DATA																						
		std::vector<std::shared_ptr<Tile>> Pistol =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(ep::symbol::pistol, ep::color::item::PISTOLFG, ep::color::item::ITEMBG, 0),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0)
		};

		std::vector<std::shared_ptr<Tile>> PistolMagazine =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(ep::symbol::pistolMagazine, ep::color::item::PISTOLFG, ep::color::item::ITEMBG, 0),
				std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0)
		};

		std::vector<std::shared_ptr<Tile>> Rifle =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(ep::symbol::rifle,	ep::color::item::RIFLEFG,	ep::color::item::ITEMBG, 0),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,			TCODColor::pink,	TCODColor::pink	, 0)
		};

		std::vector<std::shared_ptr<Tile>> RifleMagazine =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(ep::symbol::rifleMagazine,	ep::color::item::RIFLEFG,	ep::color::item::ITEMBG, 0),
				std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,					TCODColor::pink,	TCODColor::pink	, 0)
		};

		std::vector<std::shared_ptr<Tile>> Backpack =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(ep::symbol::backpack,	ep::color::item::SMALLBACKPACKFG,	ep::color::item::ITEMBG, 0),
				std::make_shared<Tile>(0,				TCODColor::pink,			TCODColor::pink	, 0),
				std::make_shared<Tile>(0,				TCODColor::pink,			TCODColor::pink	, 0),
				std::make_shared<Tile>(0,				TCODColor::pink,			TCODColor::pink	, 0),
				std::make_shared<Tile>(0,				TCODColor::pink,			TCODColor::pink	, 0)
		};

		std::vector<std::shared_ptr<Tile>> testArmor =
			std::vector<std::shared_ptr<Tile>>{
				std::make_shared<Tile>(234,	TCODColor::pink,	TCODColor::black, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink	, 0),
				std::make_shared<Tile>(0,	TCODColor::pink,	TCODColor::pink	, 0)
		};
	}
}

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

//============TILES==================================================CH===FOREGROUND=COLOR==========BACKGROUND=COLOR==========H==WALK===DEST |

//TILE DATA
//	STARTS WITH BOTTOM LEVEL
//	USE 0 FOR AN EMPTY TILE, SPACE FOR BACKGROUND, AND CHAR FOR CHAR


//TILES
	//GROUND
		//GRASS PATTERNS
			#define TILE_Grass0				std::make_shared<Block>			(ep::blockData::Grass0,			ep::blockFlag::OOOOI,	ep::blockFlag::OOOOI) 
			#define TILE_Grass1				std::make_shared<Block>			(ep::blockData::Grass1,			ep::blockFlag::OOOOI,	ep::blockFlag::OOOOI)
			#define TILE_Grass2				std::make_shared<Block>			(ep::blockData::Grass2,			ep::blockFlag::OOOOI,	ep::blockFlag::OOOOI)
			#define TILE_Grass3				std::make_shared<Block>			(ep::blockData::Grass3,			ep::blockFlag::OOOOI,	ep::blockFlag::OOOOI)
																			 								
	#define TILE_BasicFlower				std::make_shared<Block>			(ep::blockData::BasicFlower,	ep::blockFlag::OOIII,	ep::blockFlag::OOOOI)
	#define TILE_BasicFloor					std::make_shared<Block>			(ep::blockData::BasicFloor,		ep::blockFlag::OOOOI,	ep::blockFlag::OOOOI)
	#define TILE_BasicConcrete				std::make_shared<Block>			(ep::blockData::BasicConcrete,	ep::blockFlag::OOOOI,	ep::blockFlag::OOOOI)
	#define TILE_BasicShingle				std::make_shared<Block>			(ep::blockData::BasicShingle,	ep::blockFlag::OOOOI,	ep::blockFlag::OOOOI)
																											
	//WALL																									
	#define TILE_BasicDoor					std::make_shared<Block>			(ep::blockData::BasicDoor,		ep::blockFlag::IIIII,	ep::blockFlag::OOOOI)
	#define DESTRUCTIBLE_BasicWall			std::make_shared<Destructible>	(ep::blockData::BasicWall,		ep::blockFlag::IIIII,	ep::blockFlag::IIIII,	1000)
	#define DESTRUCTIBLE_BasicWindow		std::make_shared<Destructible>	(ep::blockData::BasicWindow,	ep::blockFlag::OOIII,	ep::blockFlag::IIIII,	100)
																			 								
	//PART HEIGHT															 								
	#define TILE_BasicTableLeg				std::make_shared<Destructible>	(ep::blockData::BasicTableLeg,	ep::blockFlag::OOIII,	ep::blockFlag::OOIII,	500)
	#define TILE_BasicTableTop				std::make_shared<Destructible>	(ep::blockData::BasicTableTop,	ep::blockFlag::OOIOI,	ep::blockFlag::OOIOI,	500)
																			 								
	//STAIR																	 								
	#define STAIR_UpStair					std::make_shared<Stair>			(ep::blockData::UpStair,		ep::blockFlag::OOOOI,	ep::blockFlag::OOIII,	1)
	#define STAIR_DownStair					std::make_shared<Stair>			(ep::blockData::DownStair,		ep::blockFlag::OOOOI,	ep::blockFlag::OOIII,	-1)
																			 								
	//SKY																	 								
	#define TILE_BasicSky					std::make_shared<Block>			(ep::blockData::BasicSky,		ep::blockFlag::OOOOO,	ep::blockFlag::OOOOO)
																			 								
	//ERROR																	 								
	#define TILE_error						std::make_shared<Block>			(ep::blockData::Error,			ep::blockFlag::IIIII,	ep::blockFlag::IIIII)

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
			#define DEFAULT_ITEM_TILE								std::make_shared<Block>			(ep::blockData::BasicDoor,	ep::blockFlag::OOOOI, ep::blockFlag::OOOOI)

//CONTAINERS
	//SMALL BACKPACK
				#define TILE_SmallBackpack							std::make_shared<Block>			(ep::blockData::Backpack,	ep::blockFlag::OOOOI, ep::blockFlag::OOOOI)
				#define TOOL_SmallBackpack							std::make_shared<Tool>			("Small Backpack",							ep::color::item::SMALLBACKPACKFG,													ep::symbol::backpack)
			#define ITEM_SmallBackpack(x, y, level, owner)			std::make_shared<Item>			(2,											TILE_SmallBackpack,															TOOL_SmallBackpack,				Position4(x, y, 0, level),		owner,										Item::ItemType::NORMAL)
		#define CONTAINER_SmallBackpack(x, y, level, owner)			std::make_shared<Container>		(5,											ITEM_SmallBackpack(x, y, level, owner))

//HANDS
		#define TOOL_Hands											std::make_shared<Tool>			("Hands",									ep::color::item::HANDFG,															TCOD_CHAR_UMLAUT)
	#define ITEM_Hands(x, y, level, owner)							std::make_shared<Item>			(1,											DEFAULT_ITEM_TILE,															TOOL_Hands,						Position4(x, y, 0, level),		owner,										Item::ItemType::NODROP)

//testarmor
		#define TILE_testArmor										std::make_shared<Block>			(ep::blockData::testArmor, ep::blockFlag::OOOOI, ep::blockFlag::OOOOI)
		#define ARMOR_testArmor										std::make_shared<Armor>			("test armor",								TCODColor::pink,															100,							300)
	#define ITEM_testArmor(x, y, level, owner)						std::make_shared<Item>			(2,											TILE_testArmor,																ARMOR_testArmor,				Position4(x, y, 0, level),		owner,										Item::ItemType::ARMOR)

//PISTOLS
	//SIP45 (standard issue pistol .45 cal)
		//WEAPON
				#define TILE_SIP45									std::make_shared<Block>			(ep::blockData::Pistol, ep::blockFlag::OOOOI, ep::blockFlag::OOOOI)
				#define WEAPON_SIP45								std::make_shared<Firearm>		("SIP45",									ep::color::item::PISTOLFG,															2,							1.0f,							MagazineData::AmmoType::FOURTYFIVEACP,		Firearm::FireType::SEMI,	Firearm::FireType::SEMI | Firearm::FireType::SAFE)
			#define ITEM_SIP45(x, y, level, owner)					std::make_shared<Item>			(2,											TILE_SIP45,																	WEAPON_SIP45,					Position4(x, y, 0, level),		owner,										Item::ItemType::FIREARM)
		//MAGAZINE
					#define TILE_45Magazine7						std::make_shared<Block>			(ep::blockData::PistolMagazine, ep::blockFlag::OOOOI, ep::blockFlag::OOOOI)
					#define TOOL_45Magazine7						std::make_shared<Tool>			("45 Magazine -7-",							ep::color::item::PISTOLFG,															ep::symbol::pistolMagazine)
				#define ITEM_45Magazine7(x, y, level, owner)		Item							(1, TILE_45Magazine7,						TOOL_45Magazine7,															Position4(x, y, 0, level),		owner,							Item::ItemType::NORMAL)
			#define MAGAZINE_45Magazine7(x, y, level, owner)		std::make_shared<MagazineItem>	(ITEM_45Magazine7(x, y, level, owner),		std::make_shared<MagazineData>(MagazineData::AmmoType::FOURTYFIVEACP,		7,								7))

//RIFLES
	//SIR556 (standard issue rifle 5.56 cal)
		//WEAPON
				#define TILE_SIR556									std::make_shared<Block>			(ep::blockData::Rifle,	ep::blockFlag::OOOOI,	ep::blockFlag::OOOOI)
				#define WEAPON_SIR556								std::make_shared<Firearm>		("SIR556",										ep::color::item::RIFLEFG,															10,							2.0f,							MagazineData::AmmoType::FIVEPOINTFIVESIX,	Firearm::FireType::FULL,	Firearm::FireType::FULL | Firearm::FireType::SEMI | Firearm::FireType::SAFE)
			#define ITEM_SIR556(x, y, level, owner)					std::make_shared<Item>			(3,												TILE_SIR556,																WEAPON_SIR556,					Position4(x, y, 0, level),		owner,										Item::ItemType::FIREARM)
		//MAGAZINE
					#define TILE_556Magazine30						std::make_shared<Block>			(ep::blockData::RifleMagazine, ep::blockFlag::OOOOI, ep::blockFlag::OOOOI)
					#define TOOL_556Magazine30						std::make_shared<Tool>			("5.56 Magazine -30-",						ep::color::item::RIFLEFG,															ep::symbol::rifleMagazine)
				#define ITEM_556Magazine30(x, y, level, owner)		Item							(1,											TILE_556Magazine30,															TOOL_556Magazine30,				Position4(x, y, 0, level),		owner,										Item::ItemType::NORMAL)
			#define MAGAZINE_556Magazine30(x, y, level, owner)		std::make_shared<MagazineItem>	(ITEM_556Magazine30(x, y, level, owner),	std::make_shared<MagazineData>(MagazineData::AmmoType::FIVEPOINTFIVESIX,	30,								30))
