#include "main.hpp"

//Position Struct
Position::Position(int x, int y, int level)
	:x(x), y(y), level(level)
{}

//BLine Class
BLine::BLine(int ix, int iy, int tx, int ty)
	:origx(ix), origy(iy), destx(tx), desty(ty), x(ix), y(iy)
{
	deltax = destx - origx;
	deltay = desty - origy;

	if (deltax > 0)
	{
		stepx = 1;
	}

	else if (deltax < 0)
	{
		stepx = -1;
	}

	else stepx = 0;

	if (deltay > 0)
	{
		stepy = 1;
	}

	else if (deltay < 0)
	{
		stepy = -1;
	}

	else stepy = 0;

	if (stepx * deltax > stepy * deltay)
	{
		e = stepx * deltax;
		deltax *= 2;
		deltay *= 2;
	}
	else
	{
		e = stepy * deltay;
		deltax *= 2;
		deltay *= 2;
	}
}

void BLine::step()
{
	if (stepx * deltax > stepy * deltay)
	{
		if (origx == destx) return;
		origx += stepx;
		e -= stepy * deltay;
		if (e < 0)
		{
			origy += stepy;
			e += stepx * deltax;
		}
	}
	else
	{
		if (origy == desty) return;
		origy += stepy;
		e -= stepx * deltax;
		if (e < 0)
		{
			origx += stepx;
			e += stepy * deltay;
		}
	}
	x = origx;
	y = origy;

	return;
}

bool BLine::end()
{
	if (stepx * deltax > stepy * deltay)
	{
		if (origx == destx) return true;
		/*origx += stepx;
		e -= stepy * deltay;
		if (e < 0)
		{
			origy += stepy;
			e += stepx * deltax;
		}*/
	}
	else
	{
		if (origy == desty) return true;
		/*origy += stepy;
		e -= stepx * deltax;
		if (e < 0)
		{
			origx += stepx;
			e += stepy * deltay;
		}*/
	}
	/*x = origx;
	y = origy;
*/
	return false;
}

Position offSetPosition(Position& mapPosition, int xOffset, int yOffset)
{
	return Position(mapPosition.x - xOffset, mapPosition.y - yOffset, mapPosition.level);
}

int square(int x)
{
	return x * x;
}

double getAngle(int ix, int iy, int tx, int ty)
{
	int dx = tx - ix;
	int dy = ty - iy;
	double itan = (double)(dy) / (double)(dx);
	return atan(itan) * 180 / PI;
}

double getDistance(int ix, int iy, int tx, int ty)
{
	int xLength = square(tx - ix);
	int yLength = square(ty - iy);
	return sqrt(xLength + yLength);
}
