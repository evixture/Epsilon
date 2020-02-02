#include "main.hpp"

Position3::Position3(int x, int y, int level)
	:x(x), y(y), level(level)
{}

bool Position3::operator==(const Position3 & compPosition)
{
	if (this->x == compPosition.x && this->y == compPosition.y && this->level == compPosition.level)
	{
		return true;
	}
	return false;
}

Position4 offsetPosition(Position4 mapPosition, int xOffset, int yOffset)
{
	return Position4(mapPosition.x - xOffset, mapPosition.y - yOffset, mapPosition.height, mapPosition.level);
}

//----------------------------------------------------------------------------------------------------

Position4::Position4(int x, int y, int height, int level)
	:x(x), y(y), height(height), level(level)
{
}

bool Position4::operator==(const Position4& compPosition)
{
	if (this->x == compPosition.x && this->y == compPosition.y && this->height == compPosition.height && this->level == compPosition.level)
	{
		return true;
	}
	return false;
}

Position4::operator Position3() const
{
	return Position3(this->x, this->y, this->level);
}

//----------------------------------------------------------------------------------------------------

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
	}
	else
	{
		if (origy == desty) return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------------------

unsigned char heightToBitFlag(int height)
{
	return 1 << height;
}

double getAngle(int ix, int iy, int tx, int ty)
{
	double itan = (double)(ty - iy) / (double)(tx - ix);
	return atan(itan) * 180.0 / PI;
}

double getDistance(int ix, int iy, int tx, int ty)
{
	int xLength = pow(tx - ix, 2);
	int yLength = pow(ty - iy, 2);
	return sqrt(xLength + yLength);
}

float getFallTime(int height)
{
	return sqrt((2.0f * height) / 16);
}

//----------------------------------------------------------------------------------------------------

//Clock Struct
//Clock::Clock(float capacityInSeconds, float step)
//	:capacity(capacityInSeconds), step(step), score(1)
//{}
//
//void Clock::update(bool tickDown, bool resetAtZero)
//{
//	if (tickDown)
//	{
//		if (step > 0.0f)
//		{
//			step -= SETTINGS->lastFrameTime.asSeconds();
//		}
//	}
//	if (step <= 0.0f) //should loop while step + capacity is less than capacity
//	{
//		if (resetAtZero)
//		{
//			if (capacity != 0.0f)
//			{
//				//while step + capacity < 0, add 1 to score
//				//in class, while clock score is greater than 0, update extra time, --score
//				//score = 0; //need to prevent overflow??
//				//while (step <= 0)
//				//{
//				//	step += capacity;
//				//	++score; //should = 1 on base reset, if greater, used to call extra times
//				//}
//
//				while (step <= 0)
//				{
//					step += capacity;
//					++score; //how many extra times that the function needs to be called
//				}
//
//			}
//		}
//	}
//}
//
////bool Clock::isAtZero()
//{
//	if (step <= 0.0f)
//	{
//		return true;
//	}
//	return false;
//}

MagazineData::MagazineData(AmmoType ammoType, int ammoCapacity, int availableAmmo, bool isValid)
	:isValid(isValid), ammoType(ammoType), ammoCapacity(ammoCapacity), availableAmmo(availableAmmo)
{}

