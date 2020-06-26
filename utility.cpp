#include "main.hpp"

Position2::Position2(int x, int y)
	: x(x), y(y)
{
}

Position2::Position2(Position3 position)
	: x(position.x), y(position.y)
{
}

Position2::Position2(Position4 position)
	: x(position.x), y(position.y)
{
}

bool Position2::operator==(const Position2& compPosition)
{
	return (this->x == compPosition.x && this->y == compPosition.y);
}

//----------------------------------------------------------------------------------------------------

Position3::Position3(int x, int y, int z)
	: Position2(x, y), z(z)
{
}

Position3::Position3(Position4 position)
	: Position2(position.x, position.y), z(position.z)
{
}

bool Position3::operator==(const Position3 & compPosition)
{
	return (this->x == compPosition.x && this->y == compPosition.y && this->z == compPosition.z);
}

Position3 Position3::operator-(const Position3& compPosition)
{
	return Position3(this->x - compPosition.x, this->y - compPosition.y, this->z - compPosition.z);
}

//----------------------------------------------------------------------------------------------------

Position4::Position4(int x, int y, int h, int z)
	: Position3(x, y, z), h(h)
{
}

bool Position4::operator==(const Position4& compPosition)
{
	return (this->x == compPosition.x && this->y == compPosition.y && this->h == compPosition.h && this->z == compPosition.z);
}

//----------------------------------------------------------------------------------------------------

BLine::BLine(int xStart, int yStart, int xTarget, int yTarget)
	: origx(xStart), origy(yStart), destx(xTarget), desty(yTarget), x(xStart), y(yStart)
{
	deltax = destx - origx;
	deltay = desty - origy;

	if (deltax > 0)			stepx = 1;
	else if (deltax < 0)	stepx = -1;
	else					stepx = 0;

	if (deltay > 0)			stepy = 1;
	else if (deltay < 0)	stepy = -1;
	else					stepy = 0;

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


FLine::FLine(Position2 startPosition, Position2 targetPosition) //cant shoot straight up
	: step(0), startPosition(startPosition)
{
	if (!(startPosition == targetPosition))
	{
		if (targetPosition.x <= startPosition.x && targetPosition.y <= startPosition.y) negative = true;
		else negative = false;

		valid = true;
		if ((float)(abs(targetPosition.y - startPosition.y)) / (float)(abs(targetPosition.x - startPosition.x)) > 1.0f)
		{
			yStep = true; //if slope too great, step along y axis in order to not skip blocks
			multiplier = (float)(targetPosition.x - startPosition.x) / (float)(targetPosition.y - startPosition.y);
		}
		else
		{
			yStep = false;
			multiplier = (float)(targetPosition.y - startPosition.y) / (float)(targetPosition.x - startPosition.x);
		}
	}
	else
	{
		valid = false;
		yStep = false;
		multiplier = 0.0f;
	}

	//if (targetPosition.x - startPosition.x != 0) //needs to be y step if angle is greater than 45 deg
	//{
	//	yStep = false; //step along x
	//	multiplier = (float)(targetPosition.y - startPosition.y) / (float)(targetPosition.x - startPosition.x);
	//}
	//else
	//{
	//	yStep = true; //step along y
	//	if (targetPosition.y - startPosition.y > 0) multiplier = 1;
	//	else if (targetPosition.y - startPosition.y < 0) multiplier = -1;
	//	else multiplier = 0; //no line
	//}
}

Position2 FLine::getPosition() const
{
	//if (!yStep)
	//{
	//	return Position2(startPosition.x + step, startPosition.y + step * multiplier);
	//}
	//else
	//{
	//	if (multiplier == 1) return Position2(startPosition.x, startPosition.y + step); //vert line up
	//	else if (multiplier == -1) return Position2(startPosition.x, startPosition.y - step); //vert line down
	//}
	//return Position2(0, 0); //no line

	if (valid)
	{
		if (yStep == true)
		{
			return Position2(startPosition.x + (step * multiplier), startPosition.y + step);
		}
		else
		{
			return Position2(startPosition.x + step, startPosition.y + (step * multiplier));
		}
	}
}

Position2 FLine::getNextPosition() const
{
	//if (!yStep)
	//{
	//	return Position2(startPosition.x + (step + 1), startPosition.y + (step + 1) * multiplier);
	//}
	//else
	//{
	//	if (multiplier == 1) return Position2(startPosition.x, startPosition.y + (step + 1)); //vert line up
	//	else if (multiplier == -1) return Position2(startPosition.x, startPosition.y - (step - 1)); //vert line down
	//}
	//return Position2(0, 0); //no line

	if (valid)
	{
		if (yStep == true)
		{
			if (negative) return Position2(startPosition.x + ((step - 1) * multiplier), startPosition.y + (step - 1));
			else return Position2(startPosition.x + ((step + 1) * multiplier), startPosition.y + (step + 1));
		}
		else
		{
			if (negative) return Position2(startPosition.x + (step - 1), startPosition.y + ((step - 1) * multiplier));
			return Position2(startPosition.x + (step + 1), startPosition.y + ((step + 1) * multiplier));
		}
	}
}

bool FLine::isValid()
{
	return valid;
}

void FLine::stepLine()
{
	if (valid)
	{
		if (negative) step--;
		else step++;
	}
}

//----------------------------------------------------------------------------------------------------

unsigned char heightToBitFlag(int h)
{
	return 1 << h;
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

float getFallTime(int h)
{
	return sqrt((2.0f * h) / 16);
}

std::string getTileName(char id)
{
	switch (id)
	{
	case -1:
		return "ERROR";
		break;
	case 0:
		return "---";
		break;

	case 1:
		return "Grass";
		break;

	case 2:
		return "Flower";
		break;

	case 3:
		return "Floor";
		break;

	case 4:
		return "Concrete";
		break;

	case 5:
		return "Shingle";
		break;

	case 6:
		return "Door";
		break;

	case 7:
		return "Wall";
		break;

	case 8:
		return "Window";
		break;

	case 9:
		return "Table Leg";
		break;

	case 10:
		return "Table Top";
		break;

	case 11:
		return "Upward Stair";
		break;

	case 12:
		return "Downward Stair";
		break;

	case 13:
		return "Sky";
		break;

	case 14:
		return "Pistol";
		break;

	case 15:
		return "Pistol Magazine";
		break;

	case 16:
		return "Rifle";
		break;

	case 17:
		return "Rifle Magazine";
		break;

	case 18:
		return "Small Backpack";
		break;

	case 19:
		return "Level 1, R3 Armor";
		break;

	case 20:
		return "Knife";
		break;

	case 21:
		return "Rubble";
		break;

	default:
		return "ERROR";
		break;
	}
}

/*
	CHECK ORDER:
	1 2 3
	4 0 5
	6 7 8
*/

Position4 getWalkableArea(Position4 centerPosition)
{
	if (WORLD->debugmap->getWalkability(centerPosition, true) == true) return centerPosition; //if center position is valid

	for (int y = -1; y <= 1; y++) //search the surrounding area for walkable block
	{
		for (int x = -1; x <= 1; x++)
		{
			if (!(x == 0 && y == 0)) // do not check center position
			{
				if (WORLD->debugmap->getWalkability(Position4(centerPosition.x + x, centerPosition.y + y, centerPosition.h, centerPosition.z), true) == true) //can use single var for this and return
				{
					return Position4(centerPosition.x + x, centerPosition.y + y, centerPosition.h, centerPosition.z);
				}
			}
		}
	}
	return Position4(0, 0, 0, 0); //need better return
}

//----------------------------------------------------------------------------------------------------

MagazineData::MagazineData(AmmoType ammoType, int ammoCapacity, int availableAmmo)
	: ammoType(ammoType), ammoCapacity(ammoCapacity), availableAmmo(availableAmmo)
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
	return (this->ammoType == compMag.ammoType && ammoCapacity == compMag.ammoCapacity && availableAmmo == compMag.availableAmmo);
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

Bar::Bar(std::string label, TCODColor color, int width, int maxValue, int currentValue, Position3 renderPosition)
	: label(label), color(color), width(width), maxValue(maxValue), currentValue(currentValue), renderPosition(renderPosition)
{
	pad = (int)label.length() + 4; //position of the start of the bar
}

void Bar::setBarValue(int value)
{
	currentValue = value;
}

void Bar::render(const Pane& pane) const
{
	pane.console->printf(renderPosition.x, renderPosition.y, "%s : [", label.c_str());
	for (int i = 0; i < width; ++i)
	{
		if (currentValue != -1)
		{
			if (i * (maxValue / width) < currentValue)
			{
				pane.console->printf(i + pad + renderPosition.x, renderPosition.y, "=");
				pane.console->setCharForeground(i + pad + renderPosition.x, renderPosition.y, color);
			}
		}
		else
		{
			pane.console->printf(i + pad + renderPosition.x, renderPosition.y, "!");
			pane.console->setCharForeground(i + pad + renderPosition.x, renderPosition.y, color);
		}
	}
	pane.console->printf(pad + width, renderPosition.y, "]");
}

Position3 offsetPosition(Position3 mapPosition, int xOffset, int yOffset)
{
	return Position3(mapPosition.x - xOffset, mapPosition.y - yOffset, mapPosition.z);
}

Position4 offsetPosition(Position4 mapPosition, int xOffset, int yOffset)
{
	return Position4(mapPosition.x - xOffset, mapPosition.y - yOffset, mapPosition.h, mapPosition.z);
}

Position2 getRenderPosition(Position2 mapPosition)
{
	return Position2(mapPosition.x - WORLD->xOffset, mapPosition.y - WORLD->yOffset);
}

