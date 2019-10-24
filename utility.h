#include "main.hpp"

struct BLine
{
	int x;
	int y;

	BLine(int ix, int iy, int tx, int ty);

	void step();

	bool end();

private:
	int stepx,  stepy,  e,
		deltax, deltay, origx,
		origy,  destx,  desty;
};

//Position Struct
struct Position
{
	int x;
	int y;
	int level;

	Position(int x, int y, int level);
};

int square(int x);

double getAngle(int ix, int iy, int tx, int ty);
double getDistance(int ix, int iy, int tx, int ty);