#include "main.hpp"

Position3::Position3()
	: x(0), y(0), floor(0)
{
}

Position3::Position3(int x, int y, int floor)
	: x(x), y(y), floor(floor)
{
}

bool Position3::operator==(const Position3 & compPosition)
{
	return (this->x == compPosition.x && this->y == compPosition.y && this->floor == compPosition.floor);
}

Position4 offsetPosition(Position4 mapPosition, int xOffset, int yOffset)
{
	return Position4(mapPosition.x - xOffset, mapPosition.y - yOffset, mapPosition.height, mapPosition.floor);
}

//----------------------------------------------------------------------------------------------------

Position4::Position4()
	:Position3(0, 0, 0), height(0)
{
}

Position4::Position4(int x, int y, int height, int floor)
	: Position3(x, y, floor), height(height)
{
}

bool Position4::operator==(const Position4& compPosition)
{
	return (this->x == compPosition.x && this->y == compPosition.y && this->height == compPosition.height && this->floor == compPosition.floor);
}

//----------------------------------------------------------------------------------------------------

BLine::BLine(int xStart, int yStart, int xTarget, int yTarget)
	: origx(xStart), origy(yStart), destx(xTarget), desty(yTarget), x(xStart), y(yStart)
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

double getAngle(int xStart, int yStart, int xTarget, int yTarget)
{
	double itan = (double)(yTarget - yStart) / (double)(xTarget - xStart);
	return atan(itan) * 180.0 / PI;
}

double getDistance(int xStart, int yStart, int xTarget, int yTarget)
{
	double xLength = pow(xTarget - xStart, 2);
	double yLength = pow(yTarget - yStart, 2);
	return sqrt(xLength + yLength);
}

float getFallTime(int height)
{
	return sqrt((2.0f * height) / 16);
}

Position4 getWalkableArea(Position4 centerPosition)
{
	/*
	CHECK ORDER:
	1 2 3
	4 0 5
	6 7 8
	*/

	//if center position is valid
	if (WORLD->debugmap->getWalkability(centerPosition, true) == true)
	{
		return centerPosition;
	}

	//search the surrounding area for walkable block
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			if (x != 0 && y != 0) // do not check center position
			{
				if (WORLD->debugmap->getWalkability(Position4(centerPosition.x + x, centerPosition.y + y, centerPosition.height, centerPosition.floor), true) == true)
				{
					return Position4(centerPosition.x + x, centerPosition.y + y, centerPosition.height, centerPosition.floor);
				}
			}
		}
	}
	return Position4(0, 0, 0, 0); //need better return
}

//----------------------------------------------------------------------------------------------------

MagazineData::MagazineData(AmmoType ammoType, int ammoCapacity, int availableAmmo, bool isValid)
	:isValid(isValid), ammoType(ammoType), ammoCapacity(ammoCapacity), availableAmmo(availableAmmo)
{
	switch (ammoType)
	{
	case AmmoType::FIVEPOINTFIVESIX:
		mass = 55;
		velocity = 300;
		break;
	case AmmoType::FOURTYFIVEACP:
		mass = 230;
		velocity = 80;
		break;
	default:
		mass = 0;
		velocity = 0;
		break;
	}
}

bool MagazineData::operator==(const MagazineData& compMag)
{
	if (this->ammoType == compMag.ammoType && ammoCapacity == compMag.ammoCapacity && availableAmmo == compMag.availableAmmo && isValid == compMag.isValid) return true;
	return false;
}

Clock::Clock(float timeBetweenUpdates)
	: numCalls(0), timeBetweenUpdates(timeBetweenUpdates)
{
}

void Clock::tickUp()
{
	numCalls += SETTINGS->lastFrameTime.asSeconds() / timeBetweenUpdates;
}

void Clock::addTime(float seconds)
{
	numCalls -= seconds / timeBetweenUpdates;
}

Menu::Menu(std::vector<std::string> menuList)
	: menuList(menuList), menuIndex(0)
{
	menuSelection = menuList[menuIndex];
}

void Menu::update()
{
	menuSelection = menuList[menuIndex];

	if (INPUT->moveUpKey->isSwitched && menuIndex > 0)
	{
		menuIndex--;
		AUDIO->playSound(Sound(("tick"), 0.0f, 5.0f));
	}

	if (INPUT->moveDownKey->isSwitched && (menuIndex < menuList.size() - 1))
	{	
		++menuIndex;
		AUDIO->playSound(Sound(("tick"), 0.0f, 5.0f));	
	}
}

void Menu::render(const Pane& pane, const int xRender, const int yRender) const
{
	for (int i = 0; i < menuList.size(); ++i)
	{
		if (i == menuIndex)
		{
			pane.console->printf(xRender, yRender + i, "|> %s", menuList[i].c_str());
			pane.console->setCharForeground(xRender + 1, yRender + i, ep::color::selector);
		}
		else
		{
			pane.console->printf(xRender, yRender + i, "|  %s", menuList[i].c_str());
		}
	}
}
