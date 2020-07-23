#include "main.hpp"

constexpr auto PI = 3.14159265;

#define ENGINE		engine
#define SETTINGS	engine->settings
#define GUI			engine->gui
#define AUDIO		engine->audio
#define INPUT		engine->input
#define MAPPANE		engine->gui->worldWindow
#define WORLD		engine->gui->worldWindow->world

struct Position2;
struct Position3;
struct Position4;

struct Position2
{
	int x;
	int y;

	Position2() = default;
	Position2(int x, int y);

	Position2(Position3 position);
	Position2(Position4 position);
	virtual ~Position2() {};

	bool		operator == (const Position2& compPosition);
};

struct Position3 : public Position2
{
	/*int x;
	int y;*/
	int z; //floor when used as position

	Position3() = default;
	Position3(int x, int y, int z);

	Position3(Position4 position);

	virtual ~Position3() {};

	bool		operator == (const Position3& compPosition);
	Position3	operator -	(const Position3& compPosition);
};

struct Position4 : public Position3
{
	char h; //range 0-3

	Position4() = default;
	Position4(int x, int y, int h, int z);

	bool operator == (const Position4& compPosition);
};

struct BLine
{
	int x;
	int y;

	BLine(int startx, int starty, int targetx, int targety);

	void step();

	bool end();

private:
	int stepx,  stepy,  e,
		deltax, deltay, origx,
		origy,  destx,  desty;
};

struct FLine //function line; not length limited like BLine
{
	int step;
	float slope;
	//float yMultiplier;
	const Position2 startPosition;

	//fline(angle)
	//fline(targetPos)
	FLine(Position2 startPosition, Position2 targetPosition);

	//get pos
	Position2 getPosition() const;
	//get next pos
	Position2 getNextPosition() const;

	bool isValid();
	//get step
	//step
	void stepLine();

private:
	bool vertical;
	bool horizontal;
	bool stepBackwards;
	//bool negative;
	bool yStep;
	bool valid;
};

struct Clock
{
	float numCalls;
	float timeBetweenUpdates;

	Clock(float timeBetweenUpdates);

	void tickUp();
	void addTime(float seconds);
};


struct MagazineData
{
	enum class AmmoType { NONE, FOURTYFIVEACP, FIVEPOINTFIVESIX } ammoType;

	unsigned short int ammoCapacity;
	int availableAmmo;

	int velocity;
	int mass;

	MagazineData(AmmoType ammoType, int ammoCapacity, int availableAmmo);

	bool operator == (const MagazineData& compMag);
};

struct Menu
{
	std::string menuSelection;

	Menu() = default;
	Menu(std::vector<std::string> menuList);

	virtual void update();
	virtual void render(const Pane& pane, const int rx, const int ry) const;

protected:
	unsigned int menuIndex;
	std::vector<std::string> menuList;
};

struct BindMenu : public Menu
{
	std::vector<std::string> keyList;

	bool gotInput;

	BindMenu();

	void update();
	void render(const Pane& pane, const int rx, const int ry) const;
};

struct Bar
{
	std::string label;
	TCODColor color;
	int width;

	int maxValue;
	int currentValue;

	Position3 renderPosition;

	Bar(std::string label, TCODColor color, int width, int maxValue, int currentValue, Position3 renderPosition);

	void setBarValue(int value);
	void render(const Pane& pane) const;

private:
	int pad;
};

/*
drawPane.console->printf(0, 0, "Health : [");
for (int i = 0; i < 20; ++i)
{
	if (displayHealth != -1)
	{
		if (i * 5 < displayHealth)
		{
			drawPane.console->printf(i + 10, 0, "=");
			drawPane.console->setCharForeground(i + 10, 0, TCODColor::red);
		}
	}
	else
	{
		drawPane.console->printf(i + 10, 0, "!");
		drawPane.console->setCharForeground(i + 10, 0, TCODColor::red);
	}
}
drawPane.console->printf(30, 0, "]");
*/

unsigned char	heightToBitFlag(int h);

double			getAngle(int ix, int iy, int tx, int ty);
double			getDistance(int ix, int iy, int tx, int ty);
float			getFallTime(int h);
std::string		getTileName(char id);

Position3		offsetPosition(Position3 mapPosition, int xOffset, int yOffset); //need??
Position4		offsetPosition(Position4 mapPosition, int xOffset, int yOffset);
Position4		getWalkableArea(Position4 mapPosition);
//getRenderPosition(Position2 ...)
Position2		getRenderPosition(Position2 mapPosition);

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

	struct magazineData
	{
		//inline static MagazineData* nullMagazine()
		//{
		//	return new MagazineData(MagazineData::AmmoType::NONE, 0, 0, false); //fix return new later
		//}
	};
}