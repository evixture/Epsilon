#include "main.hpp"

struct Position3 //position struct with x, y, and z dimensions
{
	unsigned int x; //x coordinate
	unsigned int y; //y coordinate
	unsigned int level; //z / height coordinate

	Position3() {}
	Position3(int x, int y, int level); //position constructor that takes x, y, and level ints

	bool operator == (const Position3& compPosition);
};

struct Position4
{
	unsigned int x;
	unsigned int y;
	unsigned char height;
	unsigned int level;

	Position4() {}
	Position4(int x, int y, int height, int level); //position constructor that takes x, y, and level ints

	bool operator == (const Position4& compPosition);
	operator Position3() const;
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

struct Clock //clock used for timings
{
	float capacity; //number that the clock resets to
	float step; //the steps util time is 0

	int score; //rename to better descrip

	//std::function<void()> action;

	Clock(float capacityInSeconds, float step); //clock constructor that takes a capacity

	void update(bool tickDown, bool resetAtZero);

	bool isAtZero();

	//void tickDown(); //ticks clock down, does not reset at 0
	//void tickDownWithReset(); //ticks clock down, resets at 0
	//void reset(); //sets step to capacity
};

unsigned char	heightToBitFlag(int height);
double			getAngle(int ix, int iy, int tx, int ty); //gets the angle from 2 sets of coordinates
double			getDistance(int ix, int iy, int tx, int ty); //gets the distance from 2 sets of coordinates
float			getFallTime(int height);

struct MagazineData //contains all of the important data for magazines
{
	enum class AmmoType { NONE, FOURTYFIVEACP, FIVEPOINTFIVESIX } ammoType; //the type of ammo that the magazine uses

	unsigned short int ammoCapacity; //the total capacity of the magazine
	int availableAmmo; //how much ammo is availible for use in the magazine

	bool isValid; //if the magazine is valid - actually contains ammo

	MagazineData(AmmoType ammoType, int ammoCapacity, int availableAmmo, bool isValid = true); //magazine data constructor that takes ammo type, capacity, availible ammo, and validity
};
