#include "main.hpp"

//Position Struct
Position::Position(int x, int y)
	:x(x), y(y)
{}

//Default Entity Class
Entity::Entity(Position pos, int symbol, const char* name, TCODColor color, int level)
	: mapPosition(pos), renderPosition(pos), symbol(symbol), name(name), color(color), level(level), height(3)
{}

void Entity::update()
{
	return;
}

void Entity::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(mapPosition.x, mapPosition.y, symbol);
	pane->console->setCharForeground(mapPosition.x, mapPosition.y, color);
}

//Bullet Struct
Bullet::Bullet(int startx, int starty, int dx, int dy, int xbound, int ybound)
	:bulletx(startx), bullety(starty), xbound(xbound), ybound(ybound), hitWall(false), tox(dx), toy(dy), travel(BLine(bulletx, bullety, tox, toy))
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
	} while (((tox + bulletx < xbound && tox + bulletx > 0) && tox != 0) || ((toy + bullety < ybound && toy + bullety > 0) && toy != 0));

	tox += bulletx;
	toy += bullety;

	travel = BLine(bulletx, bullety, tox, toy);
}

void Bullet::update() 
{
	if (!hitWall)
	{
		if (bulletx < xbound && bullety < ybound)
		{
			if (engine->gui->mapPane->world->inMapBounds(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, engine->gui->mapPane->world->player->level)
				&& engine->gui->mapPane->world->getWalkability(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, engine->gui->mapPane->world->player->level) != false)
			{
				travel.step();
			}
			else
			{
				if (engine->gui->mapPane->world->inMapBounds(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, engine->gui->mapPane->world->player->level))
				{
					if (engine->gui->mapPane->world->getTile(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, engine->gui->mapPane->world->player->level)->tag == "destructible")
					{
						engine->gui->mapPane->world->getTile(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, engine->gui->mapPane->world->player->level)->interact();
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
Weapon::Weapon(TCODColor color, int ammoCap, int numberMags, int fireRate, int reloadSpeed)
	:weaponColor(color), angle(0), dx(0), dy(0), weaponx(0), weapony(0), fireCap(fireRate), fireWait(0), ammoCap(ammoCap), ammoAmount(ammoCap), numberMags(numberMags), ch(NULL), reloadTimer(reloadSpeed), reloadWait(0)
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
//	//TCODLine::init(weaponx, weapony, endx, endy);
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
//	if (!TCODLine::step(&weaponx, &weapony))
//	{
//		if (engine->gui->mapPane->world->getWalkability(weaponx, weapony, engine->gui->mapPane->world->player->level))
//		{
//		pane->console->setCharBackground(weaponx, weapony, TCODColor::pink);
//		TCODLine::step(&weaponx, &weapony);
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
			weaponx = x + 1;
			weapony = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			weaponx = x + 1;
			weapony = y + 1;
			ch = '\\';
		}
		else if (angle >= 67.5)
		{
			weaponx = x;
			weapony = y + 1;
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx >= 0 && dy <= 0)
	{
		if (angle >= -22.5)
		{
			weaponx = x + 1;
			weapony = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			weaponx = x + 1;
			weapony = y - 1;
			ch = '/';
		}
		else if (angle <= -67.5)
		{
			weaponx = x;
			weapony = y - 1;
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx <= 0 && dy >= 0)
	{
		if (angle >= -22.5)
		{
			weaponx = x - 1;
			weapony = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			weaponx = x - 1;
			weapony = y + 1;
			ch = '/';
		}
		else if (angle <= -67.5)
		{
			weaponx = x;
			weapony = y + 1;
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx <= 0 && dy <= 0)
	{
		if (angle <= 22.5)
		{
			weaponx = x - 1;
			weapony = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			weaponx = x - 1;
			weapony = y - 1;
			ch = '\\';
		}
		else if (angle >= 67.5)
		{
			weaponx = x;
			weapony = y - 1;
			ch = TCOD_CHAR_VLINE;
		}
	}

	//Fire bullet
	if (engine->settings->input->leftMouseClick && fireWait == 0 && ammoAmount != 0)
	{
		if (!(weaponx == dx + weaponx && weapony == dy + weapony))
		{
			fireWait = fireCap;
			bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(weaponx, weapony, dx, dy, engine->gui->mapPane->world->debugmap->mapWidth, engine->gui->mapPane->world->debugmap->mapHeight));
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
	if (reloadWait == 0)
	{
		if (engine->settings->input->reload)
		{
			if (numberMags != 0 && ammoAmount != ammoCap)
			{
					ammoAmount = ammoCap;
					numberMags--;
					reloadWait = reloadTimer;
			}
		}
	}
	else
	{
		reloadWait--;
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
	
	pane->console->setChar(weaponx, weapony, ch);
	pane->console->setCharForeground(weaponx, weapony, weaponColor);

	for (auto& bullet : bulletList)
	{
		bullet->render(pane);
	}
}

Player::Player(Position pos, int symbol, const char* name, TCODColor color)
	:Entity(pos, symbol, name, color, 0), health(100), armor(0), testWeapon(std::make_shared<Weapon>(TCODColor::darkestGrey, 30, 10, 5, 120))
{}

void Player::update()
{
	renderPosition.x = mapPosition.x - WORLD->xOffset;
	renderPosition.y = mapPosition.y - WORLD->yOffset;

	testWeapon->update(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx, engine->settings->input->mouse.cy);

	if (engine->settings->input->changeFloor == true)
	{
		if (WORLD->getTile(mapPosition.x, mapPosition.y, level)->tag == "stair")
		{
			WORLD->getTile(mapPosition.x, mapPosition.y, level)->interact();
		}
	}

	//if (WORLD->getTile(mapPosition.x, mapPosition.y, height)->tag == "stair" && engine->settings->input->changeFloor)
	//{
	//	WORLD->getTile(mapPosition.x, mapPosition.y, height)->interact();
	//}
}

void Player::render(std::shared_ptr<Entity> entity, const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(renderPosition.x, renderPosition.y, symbol);
	pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);
	testWeapon->render(entity, pane);
}
