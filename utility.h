#include "main.hpp"

//Position Struct
struct Position
{
	int x;
	int y;
	int level;

	Position(int x, int y, int level);
};

Position offsetPosition(Position& mapPosition, int xOffset, int yOffset);

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

struct Clock
{
	int capacity;
	int step;

	Clock(int capacity);

	void tickDown();
	void tickDownWithReset();
	void reset();
};

int square(int x);

double getAngle(int ix, int iy, int tx, int ty);
double getDistance(int ix, int iy, int tx, int ty);

//template<class T>
//void swapElementOfVector(std::vector<std::shared_ptr<T>>& firstV, std::vector<std::shared_ptr<T>>& secondV, std::shared_ptr<T>& itemToSwap)
//{
//
//}