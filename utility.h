#include "main.hpp"

struct BLine
{
	int x;
	int y;

	BLine(int ix, int iy, int tx, int ty);

	void step();

	bool end();

private:
	int stepx;
	int stepy;
	int e;
	int deltax;
	int deltay;
	int origx;
	int origy;
	int destx;
	int desty;
};

struct Clock
{
	double startTime;

	Clock();
};

double getAngle(int ix, int iy, int tx, int ty);