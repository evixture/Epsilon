#include "main.hpp"

constexpr auto PI = 3.14159265;

#define ENGINE		engine
#define SETTINGS	engine->settings
#define GUI			engine->gui
#define INPUT		engine->settings->input
#define MAPPANE		engine->gui->worldWindow
#define WORLD		engine->gui->worldWindow->world

struct Position3 //position struct with x, y, and z dimensions
{
	int x; //x coordinate
	int y; //y coordinate
	int floor; //z / height coordinate

	Position3() {}
	Position3(int x, int y, int floor); //position constructor that takes x, y, and floor ints

	bool operator == (const Position3& compPosition);
};

struct Position4 : public Position3
{
	char height; //the height of the creature's view (NOT THE FLOOR)

	Position4() {}
	Position4(int x, int y, int height, int floor); //position constructor that takes x, y, and floor ints

	bool operator == (const Position4& compPosition);
};

Position4 offsetPosition(Position4 mapPosition, int xOffset, int yOffset); //used to calculate render position from map offsets

struct BLine //line algorithm used for bullet paths
{
	int x; //x position on the line
	int y; //y position on the line

	BLine(int ix, int iy, int tx, int ty); //bline constructor that takes start x and y, and target x and y

	void step(); //steps forward on the line

	bool end(); //checks if the line step has reached the end

private:
	int stepx,  stepy,  e, //used for line calculations
		deltax, deltay, origx,
		origy,  destx,  desty;
};

struct Clock
{
	float numCalls; //the number of calls for the function that uses the clock; is greater than 1 if a function needs to be called multiple times for low fps
	float timeBetweenUpdates; //the time that should be between each call

	Clock(float timeBetweenUpdates);

	void tickUp(); //tick up the clock based on the last frame time
	void addTime(float seconds); //adds time by decreasing the numCalls
};

unsigned char	heightToBitFlag(int height); //translates the height of a creature to be used for a block's enum flag
double			getAngle(int ix, int iy, int tx, int ty); //gets the angle from 2 sets of coordinates
double			getDistance(int ix, int iy, int tx, int ty); //gets the distance from 2 sets of coordinates
float			getFallTime(int height); //used to get the time to fall for a bullet

struct MagazineData //contains all of the important data for magazines
{
	enum class AmmoType { NONE, FOURTYFIVEACP, FIVEPOINTFIVESIX } ammoType; //the type of ammo that the magazine uses

	unsigned short int ammoCapacity; //the total capacity of the magazine
	int availableAmmo; //how much ammo is availible for use in the magazine

	bool isValid; //if the magazine is valid - actually contains ammo

	MagazineData(AmmoType ammoType, int ammoCapacity, int availableAmmo, bool isValid = true); //magazine data constructor that takes ammo type, capacity, availible ammo, and validity
};

struct Menu
{
	std::string menuSelection; //the name of the menu item that is selected

	Menu(std::vector<std::string> menuList);

	void update(); //updates the menu selection
	void render(const std::shared_ptr<Pane>& pane, const int rx, const int ry) const; //renders the menu on a pane at render coords

private:
	unsigned int menuIndex; //the index of the menu selection in the menu list
	std::vector<std::string> menuList; //the list of all of the posible menu entries
};

namespace ep
{
	struct tileFlag
	{
		inline static const unsigned char OOOOO = 0x0;
		inline static const unsigned char OOOOI = 0x01;
		inline static const unsigned char OOOII = 0x01 | 0x02;
		inline static const unsigned char OOIII = 0x01 | 0x02 |	0x04;
		inline static const unsigned char IIIII = 0x01 | 0x02 |	0x04 | 0x08 | 0x16;
		inline static const unsigned char OOIOI = 0x01 | 0x04;
	};

	struct character
	{
/*
	CHAR MAPPINGS
	=				| 00 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 | 11 | 12 | 13 | 14 | 15 |
	|TILES
	|				| 16 | 17 | 18 | 19 | 20 | 21 | ?? | ?? | 24 | 26 | 26 | 27 | ?? | 29 | 30 | 31 |
	|
	=				| 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 |

	=				| 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 | 57 | 58 | 59 | 60 | 61 | 62 | 63 |
	|ALPHA
	|				| 64 | 65 | 66 | 67 | 68 | 69 | 70 | 71 | 72 | 73 | 74 | 75 | 76 | 77 | 78 | 79 |
	|NUMBER
	|				| 80 | 81 | 82 | 83 | 84 | 85 | 86 | 87 | 88 | 89 | 90 | 91 | 92 | 93 | 94 | 95 |
	|SYMBOLS
	|				| 96 | 97 | 98 | 99 | 100| 101| 102| 103| 104| 105| 106| 107| 108| 109| 110| 111|
	|ETC
	=				| 112| 113| 114| 115| 116| 117| 118| 119| 120| 121| 122| 123| 124| 125| 126| 127|

	=				| 128| 129| 130| 131| 132| 133| 134| 135| 136| 137| 138| 139| 140| 141| 142| 143|
	|TOOLS
	|				| 144| 145| 146| 147| 148| 149| 150| 151| 152| 153| 154| 155| 156| 157| 158| 159|
	|ITEMS
	=				| 160| 161| 162| 163| 164| 165| 166| 167| 168| 169| 170| 171| 172| 173| 174| 175|

	=				| 176| 177| 178| 179| 180| 181| 182| 183| 184| 185| 186| 187| 188| 189| 190| 191|
	|OTHER CHARS
	|				| 192| 193| 194| 195| 196| 197| 198| 199| 200| 201| 202| 203| 204| 205| 206| 207|
	|
	|				| 208| 209| 210| 211| 212| 213| 214| 215| 216| 217| 218| 219| 220| 221| 222| 223|
	|
	|				| 224| 225| 226| 227| 228| 229| 230| 231| 232| 233| 234| 235| 236| 237| 238| 239|
	|
	=				| 240| 241| 242| 243| 244| 245| 246| 247| 248| 249| 250| 251| 252| 253| 254| 255|
*/

		inline static const int upStair =			16;
		inline static const int downStair =			17;
		inline static const int door =				18;
		inline static const int flower =			19;
		inline static const int table =				20;
		inline static const int window =			21;
							
		inline static const int bottomSquareBrace = 22;
		inline static const int topSquareBrace =	23;
		inline static const int verticalEquals =	24;
							
		inline static const int pistol =			128;
		inline static const int smg =				129;
		inline static const int rifle =				130;
		inline static const int dmr =				131;
		inline static const int sniper =			132;
		inline static const int backpack =			133;
		inline static const int ballisticVest =		134;
							
		inline static const int pistolMagazine =	144;
		inline static const int smgMagazine =		145;
		inline static const int rifleMagazine =		146;
		inline static const int dmrMagazine =		147;
		inline static const int sniperMagazine =	148;
							
		inline static const int knife =				160;
	};

	struct color
	{
		inline static const TCODColor ribbonBG =		TCODColor(45, 45, 206);
		inline static const TCODColor ribbonFG =		TCODColor(220, 220, 220);

		inline static const TCODColor drawBG =			TCODColor(30, 30, 30);
		inline static const TCODColor drawFG =			TCODColor(220, 220, 220);

		inline static const TCODColor player =			TCODColor(0, 127, 255);

		inline static const TCODColor rootBG =			TCODColor(0, 0, 0);
		inline static const TCODColor rootFG =			TCODColor(220, 220, 220);

		inline static const TCODColor outFOVBG =		TCODColor(0, 0, 0);
		inline static const TCODColor outFOVFG =		TCODColor(30, 30, 30);

		inline static const TCODColor selector =		TCODColor(168, 13, 224);

		inline static const TCODColor messageLow =		TCODColor(220, 220, 220);
		inline static const TCODColor messageMed =		TCODColor(178, 179, 236);
		inline static const TCODColor messageHigh =		TCODColor(223, 166, 243);

		inline static const TCODColor grassBG =			TCODColor(32, 70, 19);
		inline static const TCODColor grassFG =			TCODColor(119, 161, 63);

		inline static const TCODColor flowerBG =		TCODColor(45, 39, 39);
		inline static const TCODColor flowerFG =		TCODColor(254, 212, 16);

		inline static const TCODColor floorBG =			TCODColor(98, 72, 49);
		inline static const TCODColor floorFG =			TCODColor(98, 72, 49);

		inline static const TCODColor shingleBG =		TCODColor(33, 35, 32);
		inline static const TCODColor shingleFG =		TCODColor(105, 111, 109);

		inline static const TCODColor concreteBG =		TCODColor(114, 115, 119);
		inline static const TCODColor concreteFG =		TCODColor(150, 151, 153);

		inline static const TCODColor doorFG =			TCODColor(63, 50, 31);

		inline static const TCODColor wallBG =			TCODColor(145, 128, 118);
		inline static const TCODColor wallFG =			TCODColor(112, 96, 83);

		inline static const TCODColor windowBG =		TCODColor(68, 180, 246);
		inline static const TCODColor windowFG =		TCODColor(237, 237, 237);

		inline static const TCODColor tableFG =			TCODColor(150, 123, 104);

		inline static const TCODColor itemBG =			TCODColor(254, 77, 0);

		inline static const TCODColor handFG =			TCODColor(243, 198, 165);

		inline static const TCODColor smallBackpackFG = TCODColor(182, 34, 46);

		inline static const TCODColor rifleFG =			TCODColor(79, 83, 84);

		inline static const TCODColor pistolFG =		TCODColor(94, 93, 91);
	};
}