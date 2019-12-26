#include "main.hpp"

struct Position //position struct with x, y, and z dimensions
{
	int x; //x coordinate
	int y; //y coordinate
	int level; //z / height coordinate

	Position(int x, int y, int level); //position constructor that takes x, y, and level ints
};

Position offsetPosition(Position mapPosition, int xOffset, int yOffset); //used to calculate render position from map offsets

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
	int capacity; //number that the clock resets to
	int step; //the steps util time is 0

	Clock(int capacity); //clock constructor that takes a capacity

	void tickDown(); //ticks clock down, does not reset at 0
	void tickDownWithReset(); //ticks clock down, resets at 0
	void reset(); //sets step to capacity
};

int square(int x); //squares a number

double getAngle(int ix, int iy, int tx, int ty); //gets the angle from 2 sets of coordinates
double getDistance(int ix, int iy, int tx, int ty); //gets the distance from 2 sets of coordinates

struct MagazineData //contains all of the important data for magazines
{
	enum class AmmoType { NONE, FOURTYFIVEACP, FIVEPOINTFIVESIX } ammoType; //the type of ammo that the magazine uses

	int ammoCapacity; //the total capacity of the magazine
	int availableAmmo; //how much ammo is availible for use in the magazine

	bool isValid; //if the magazine is valid - actually contains ammo

	MagazineData(AmmoType ammoType, int ammoCapacity, int availableAmmo, bool isValid = true); //magazine data constructor that takes ammo type, capacity, availible ammo, and validity
};
