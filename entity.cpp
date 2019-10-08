#include "main.hpp"

//Position Struct
Position::Position(int x, int y)
	:x(x), y(y)
{}

Position Position::getPosition()
{
	return Position(x, y);
}

void Position::setPosition(int nx, int ny)
{
	this->x = nx;
	this->y = ny;
}

//Default Entity Class
Entity::Entity(Position pos, int symbol, const char* name, TCODColor color, int level)
	: position(pos), symbol(symbol), name(name), color(color), level(level), height(3)
{}

void Entity::update()
{
	return;
}

void Entity::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(position.x, position.y, symbol);
	pane->console->setCharForeground(position.x, position.y, color);
}

//Bullet Struct
Bullet::Bullet(int startx, int starty, int dx, int dy, int xbound, int ybound)
	:bx(startx), by(starty), xbound(xbound), ybound(ybound), hitWall(false), tox(dx), toy(dy), travel(BLine(bx, by, tox, toy))
{
	do
	{
		if (tox != 0)
		{
			tox *= 2;
		}
		if (toy != 0)
		{
			toy *= 2;
		}
	} while (((tox + bx < xbound && tox + bx > 0) && tox != 0) || ((toy + by < ybound && toy + by > 0) && toy != 0));

	tox += bx;
	toy += by;

	travel = BLine(bx, by, tox, toy);
}

void Bullet::update() 
{
	if (!hitWall)
	{
		if (bx < xbound && by < ybound)
		{
			if (engine->gui->mapPane->world->inMapBounds(travel.x, travel.y, engine->gui->mapPane->world->player->level) 
				&& engine->gui->mapPane->world->getWalkability(travel.x, travel.y, engine->gui->mapPane->world->player->level) != false)
			{
				travel.step();
			}
			else
			{
				if (engine->gui->mapPane->world->inMapBounds(travel.x, travel.y, engine->gui->mapPane->world->player->level))
				{
					if (engine->gui->mapPane->world->getTile(travel.x, travel.y, engine->gui->mapPane->world->player->level)->tag == "destructible")
					{
						engine->gui->mapPane->world->getTile(travel.x, travel.y, engine->gui->mapPane->world->player->level)->interact();
					}
					hitWall = true;
				}
				else
				{
					hitWall = true;
				}
			}
		}
		else
		{
			hitWall = true;
		}
	}
}

void Bullet::render(const std::shared_ptr<Pane>& pane) const
{
	if (!hitWall)
	{
		pane->console->setCharForeground(travel.x, travel.y, TCODColor::copper);
		pane->console->setChar(travel.x, travel.y, 248);
	}
}
//TCOD_CHAR_BULLET
//Weapon Struct
Weapon::Weapon(TCODColor color, int ammoCap, int numberMags, int fireRate)
	:weaponColor(color), angle(0), dx(0), dy(0), wx(0), wy(0), fireCap(fireRate), fireWait(0), ammoCap(ammoCap), ammoAmount(ammoCap), numberMags(numberMags), ch(NULL)
{}

/*

if left click
	start fire of gun

	if bullet not done with travel and not hit wall
		step through line
	else 
		stop bullet rendering

*/

//
//void Weapon::weaponFire(int endx, int endy, std::shared_ptr<Pane> pane, int xbound, int ybound)
//{
//
//	//TCODLine::init(wx, wy, endx, endy);
//	int x = endx;
//	int y = endy;
//
//	while (x < xbound)
//	{
//		x = x * 2;
//	}
//
//	while (y < ybound)
//	{
//		x = x * 2;
//	}
//	
//	if (!TCODLine::step(&wx, &wy))
//	{
//		if (engine->gui->mapPane->world->getWalkability(wx, wy, engine->gui->mapPane->world->player->level))
//		{
//		pane->console->setCharBackground(wx, wy, TCODColor::pink);
//		TCODLine::step(&wx, &wy);
//		}
//		else
//		{
//			return;
//		}
//	}
//}

void Weapon::update(int x, int y, int mx, int my)
{
	//get angle
	dx = (mx - 1) - x;
	dy = (my - 3) - y;
	double itan = (double)(dy) / (double)(dx);
	angle = atan(itan) * 180 / PI;

	if (dx >= 0 && dy >= 0)
	{
		if (angle <= 22.5)
		{
			wx = x + 1;
			wy = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			wx = x + 1;
			wy = y + 1;
			ch = '\\';
		}
		else if (angle >= 67.5)
		{
			wx = x;
			wy = y + 1;
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx >= 0 && dy <= 0)
	{
		if (angle >= -22.5)
		{
			wx = x + 1;
			wy = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			wx = x + 1;
			wy = y - 1;
			ch = '/';
		}
		else if (angle <= -67.5)
		{
			wx = x;
			wy = y - 1;
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx <= 0 && dy >= 0)
	{
		if (angle >= -22.5)
		{
			wx = x - 1;
			wy = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			wx = x - 1;
			wy = y + 1;
			ch = '/';
		}
		else if (angle <= -67.5)
		{
			wx = x;
			wy = y + 1;
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx <= 0 && dy <= 0)
	{
		if (angle <= 22.5)
		{
			wx = x - 1;
			wy = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			wx = x - 1;
			wy = y - 1;
			ch = '\\';
		}
		else if (angle >= 67.5)
		{
			wx = x;
			wy = y - 1;
			ch = TCOD_CHAR_VLINE;
		}
	}

	//Fire bullet
	if (engine->settings->input->leftMouseClick && fireWait == 0 && ammoAmount != 0)
	{
		if (!(wx == dx + wx && wy == dy + wy))
		{
			fireWait = fireCap;
			bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(wx, wy, dx, dy, engine->gui->mapPane->world->debugmap->mapW, engine->gui->mapPane->world->debugmap->mapH));
			ammoAmount--;
		}
	}

	else if (fireWait != 0)
	{
		fireWait--;
	}

	//CLEAR BULLETS
	if (bulletList.size() > ammoCap)
	{
		bulletList.pop_back();
	}

	//TODO : FIND A WAY TO SOMEHOW LIMIT RELOAD SPAMMING
	//ON RELOAD
	if (engine->settings->input->reload)
	{
		if (numberMags != 0)
		{
			ammoAmount = ammoCap;
			numberMags--;
		}
	}

	for (auto& bullet : bulletList)
	{
		bullet->update();
	}
}

void Weapon::render(std::shared_ptr<Entity> entity, const std::shared_ptr<Pane>& pane) const
{
	/*
	dx and dy values
	....|....
	.-..|..+.
	.-..|..-.
	....|....
	----@----
	....|....
	.-..|..+.
	.+..|..+.
	....|....
	*/
	
	pane->console->setChar(wx, wy, ch);
	pane->console->setCharForeground(wx, wy, weaponColor);

	for (auto& bullet : bulletList)
	{
		bullet->render(pane);
	}
}

Player::Player(Position pos, int symbol, const char* name, TCODColor color)
	:Entity(pos, symbol, name, color, 0), health(100), armor(0), testWeapon(std::make_shared<Weapon>(TCODColor::darkestGrey, 30, 10, 5))
{}

void Player::update()
{
	testWeapon->update(position.x, position.y, engine->settings->input->mouse.cx, engine->settings->input->mouse.cy);

	if (engine->settings->input->changeFloor == true)
	{
		if (WORLD->getTile(position.x, position.y, level)->tag == "stair")
		{
			WORLD->getTile(position.x, position.y, level)->interact();
		}
	}

	//if (WORLD->getTile(position.x, position.y, height)->tag == "stair" && engine->settings->input->changeFloor)
	//{
	//	WORLD->getTile(position.x, position.y, height)->interact();
	//}
}

void Player::render(std::shared_ptr<Entity> entity, const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(position.x, position.y, symbol);
	pane->console->setCharForeground(position.x, position.y, color);
	testWeapon->render(entity, pane);
}
