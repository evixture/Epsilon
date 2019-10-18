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

int square(int x);

float getAngle(int ix, int iy, int tx, int ty);

float getDistance(int ix, int iy, int tx, int ty);