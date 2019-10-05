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

void Entity::render(std::shared_ptr<Pane> window)
{
	window->console->setChar(position.x, position.y, symbol);
	window->console->setCharForeground(position.x, position.y, color);
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
			if (engine->gui->mapPane->world->getWalkability(travel.x, travel.y, engine->gui->mapPane->world->player->level) != false)
			{
				travel.step();
			}
			else
			{
				hitWall = true;
			}
		}
		else
		{
			hitWall = true;
		}
	}
}

void Bullet::render(std::shared_ptr<Pane> pane)
{
	if (!hitWall)
	{
		pane->console->setCharForeground(travel.x, travel.y, TCODColor::white);
		pane->console->setChar(travel.x, travel.y, TCOD_CHAR_BULLET);
	}
}

//Weapon Struct
Weapon::Weapon(TCODColor color, int ammoCap, int numberMags, int fireRate)
	:weaponColor(color), angle(0), dx(0), dy(0), wx(0), wy(0), fireCap(fireRate), fireWait(0), ammoCap(ammoCap), ammoAmount(ammoCap), numberMags(numberMags)
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
	dx = (mx - 1) - x;
	dy = (my - 3) - y;
	double itan = (double)(dy) / (double)(dx);
	//weird range, check actual angle by checking x and y sign with angle
	angle = atan(itan) * 180 / PI;

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
			//engine->settings->input->reload = false;
		}
	}

	for (auto& bullet : bulletList)
	{
		bullet->update();
	}
}

void Weapon::render(std::shared_ptr<Entity> entity, std::shared_ptr<Pane> window)
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
	
	if (dx >= 0 && dy >= 0)
	{
		if (angle <= 22.5)
		{
			wx = entity->position.x + 1;
			wy = entity->position.y;

			window->console->setChar(wx, wy, TCOD_CHAR_HLINE);
			window->console->setCharForeground(wx, wy, weaponColor);
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			wx = entity->position.x + 1;
			wy = entity->position.y + 1;

			window->console->setChar(wx, wy, '\\');
			window->console->setCharForeground(wx, wy, weaponColor);
		}
		else if (angle >= 67.5)
		{
			wx = entity->position.x;
			wy = entity->position.y + 1;

			window->console->setChar(wx, wy, TCOD_CHAR_VLINE);
			window->console->setCharForeground(wx, wy, weaponColor);
		}
	}
	else if (dx >= 0 && dy <= 0)
	{
		if (angle >= -22.5)
		{
			wx = entity->position.x + 1;
			wy = entity->position.y;

			window->console->setChar(wx, wy, TCOD_CHAR_HLINE);
			window->console->setCharForeground(wx, wy, weaponColor);
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			wx = entity->position.x + 1;
			wy = entity->position.y - 1;

			window->console->setChar(wx, wy, '/');
			window->console->setCharForeground(wx, wy, weaponColor);
		}
		else if (angle <= -67.5)
		{
			wx = entity->position.x;
			wy = entity->position.y - 1;

			window->console->setChar(wx, wy, TCOD_CHAR_VLINE);
			window->console->setCharForeground(wx, wy, weaponColor);
		}
	}
	else if (dx <= 0 && dy >= 0)
	{
		if (angle >= -22.5)
		{
			wx = entity->position.x - 1;
			wy = entity->position.y;

			window->console->setChar(wx, wy, TCOD_CHAR_HLINE);
			window->console->setCharForeground(wx, wy, weaponColor);
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			wx = entity->position.x - 1;
			wy = entity->position.y + 1;

			window->console->setChar(wx, wy, '/');
			window->console->setCharForeground(wx, wy, weaponColor);
		}
		else if (angle <= -67.5)
		{
			wx = entity->position.x;
			wy = entity->position.y + 1;

			window->console->setChar(wx, wy, TCOD_CHAR_VLINE);
			window->console->setCharForeground(wx, wy, weaponColor);
		}
	}
	else if (dx <= 0 && dy <= 0)
	{
		if (angle <= 22.5)
		{
			wx = entity->position.x - 1;
			wy = entity->position.y;

			window->console->setChar(wx, wy, TCOD_CHAR_HLINE);
			window->console->setCharForeground(wx, wy, weaponColor);
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			wx = entity->position.x - 1;
			wy = entity->position.y - 1;

			window->console->setChar(wx, wy, '\\');
			window->console->setCharForeground(wx, wy, weaponColor);
		}
		else if (angle >= 67.5)
		{
			wx = entity->position.x;
			wy = entity->position.y - 1;

			window->console->setChar(wx, wy, TCOD_CHAR_VLINE);
			window->console->setCharForeground(wx, wy, weaponColor);
		}
	}

	for (auto& bullet : bulletList)
	{
		bullet->render(window);
	}
}

Player::Player(Position pos, int symbol, const char* name, TCODColor color)
	:Entity(pos, symbol, name, color, 0), health(100), armor(0), testWeapon(std::make_shared<Weapon>(TCODColor::lighterGrey, 30, 5, 5))
{}

void Player::update()
{
	testWeapon->update(position.x, position.y, engine->settings->input->mouse.cx, engine->settings->input->mouse.cy);
}

void Player::render(std::shared_ptr<Entity> entity, std::shared_ptr<Pane> window)
{
	window->console->setChar(position.x, position.y, symbol);
	window->console->setCharForeground(position.x, position.y, color);
	testWeapon->render(entity, window);
}
