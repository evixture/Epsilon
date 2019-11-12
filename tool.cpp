#include "main.hpp"

////Container struct
//Container::Container(const char* name)
//	:name(name), active(false), useTile(false)
//{
//}
//
//void Container::update(int x, int y, int mx, int my, double angle)
//{
//	return;
//}
//
//void Container::render(const std::shared_ptr<Pane>& pane) const
//{
//	return;
//}

//Tool Struct
Tool::Tool(const char* name, TCODColor color)
	:color(color), toolx(0), tooly(0), ch(NULL), dx(0), dy(0)//, Container(name)
{}

void Tool::update(int x, int y, int mx, int my, double angle)
{
	ch = 249;
	dx = mx - x;
	dy = my - y;

	if (dx >= 0 && dy >= 0)
	{
		if (angle <= 22.5)
		{
			toolx = x + 1;
			tooly = y;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			toolx = x + 1;
			tooly = y + 1;
		}
		else if (angle >= 67.5)
		{
			toolx = x;
			tooly = y + 1;
		}
	}
	else if (dx >= 0 && dy <= 0)
	{
		if (angle >= -22.5)
		{
			toolx = x + 1;
			tooly = y;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			toolx = x + 1;
			tooly = y - 1;
		}
		else if (angle <= -67.5)
		{
			toolx = x;
			tooly = y - 1;
		}
	}
	else if (dx <= 0 && dy >= 0)
	{
		if (angle >= -22.5)
		{
			toolx = x - 1;
			tooly = y;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			toolx = x - 1;
			tooly = y + 1;
		}
		else if (angle <= -67.5)
		{
			toolx = x;
			tooly = y + 1;
		}
	}
	else if (dx <= 0 && dy <= 0)
	{
		if (angle <= 22.5)
		{
			toolx = x - 1;
			tooly = y;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			toolx = x - 1;
			tooly = y - 1;
		}
		else if (angle >= 67.5)
		{
			toolx = x;
			tooly = y - 1;
		}
	}
}

void Tool::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(toolx, tooly, ch);
	pane->console->setCharForeground(toolx, tooly, color);
}

//Bullet Struct
Bullet::Bullet(int ch, int startx, int starty, int dx, int dy, int xbound, int ybound)
	:bulletx(startx), bullety(starty), xbound(xbound), ybound(ybound), hitWall(false), tox(dx), toy(dy), travel(BLine(bulletx, bullety, tox, toy)), moveWait(0), moveCap(0), ch(ch)
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
	moveCap = (int)(engine->settings->fpsCount * .005f);
	if (moveWait < 0)
	{
		moveWait = moveCap;
	}
	else moveWait--;

	if (moveWait == 0)
	{
		if (!hitWall)
		{
			if (bulletx < xbound && bullety < ybound)
			{
				if (engine->gui->mapPane->world->inMapBounds(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, engine->gui->mapPane->world->player->mapPosition.level)
					&& engine->gui->mapPane->world->getWalkability(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, engine->gui->mapPane->world->player->mapPosition.level) != false)
				{
					travel.step();
				}
				else
				{
					if (engine->gui->mapPane->world->inMapBounds(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, engine->gui->mapPane->world->player->mapPosition.level))
					{
						if (engine->gui->mapPane->world->getTile(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, engine->gui->mapPane->world->player->mapPosition.level)->tag == "destructible")
						{
							engine->gui->mapPane->world->getTile(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, engine->gui->mapPane->world->player->mapPosition.level)->interact();
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
}

void Bullet::render(const std::shared_ptr<Pane>& pane) const
{
	if (!hitWall)
	{
		pane->console->setCharForeground(travel.x, travel.y, TCODColor::brass);
		pane->console->setChar(travel.x, travel.y, ch);
	}
}

//Weapon Struct
Weapon::Weapon(const char* name, TCODColor color, int ammoCap, int numberMags, float fireRate, float reloadSpeed)
	: Tool(name, color), baseFireCap(fireRate), fireWait(0), fireCap(60), ammoCap(ammoCap), ammoAmount(ammoCap), numberMags(numberMags), reloadTimer(60), reloadWait(0), baseReloadTimer(reloadSpeed)
{}

void Weapon::update(int x, int y, int mx, int my, double angle)
{
	dx = mx - x;
	dy = my - y;

	fireCap = (int)(baseFireCap * engine->settings->fpsCount);
	reloadTimer = (int)(baseReloadTimer * engine->settings->fpsCount);

	if (dx >= 0 && dy >= 0)
	{
		if (angle <= 22.5)
		{
			toolx = x + 1;
			tooly = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			toolx = x + 1;
			tooly = y + 1;
			ch = '\\';
		}
		else if (angle >= 67.5)
		{
			toolx = x;
			tooly = y + 1;
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx >= 0 && dy <= 0)
	{
		if (angle >= -22.5)
		{
			toolx = x + 1;
			tooly = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			toolx = x + 1;
			tooly = y - 1;
			ch = '/';
		}
		else if (angle <= -67.5)
		{
			toolx = x;
			tooly = y - 1;
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx <= 0 && dy >= 0)
	{
		if (angle >= -22.5)
		{
			toolx = x - 1;
			tooly = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			toolx = x - 1;
			tooly = y + 1;
			ch = '/';
		}
		else if (angle <= -67.5)
		{
			toolx = x;
			tooly = y + 1;
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx <= 0 && dy <= 0)
	{
		if (angle <= 22.5)
		{
			toolx = x - 1;
			tooly = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			toolx = x - 1;
			tooly = y - 1;
			ch = '\\';
		}
		else if (angle >= 67.5)
		{
			toolx = x;
			tooly = y - 1;
			ch = TCOD_CHAR_VLINE;
		}
	}

	//Fire bullet
	if (engine->settings->input->leftMouseClick && fireWait == 0 && ammoAmount != 0 && reloadWait == 0)
	{
		if (!(toolx == dx + toolx && tooly == dy + tooly))
		{
			fireWait = fireCap;
			bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ch, toolx, tooly, dx, dy, engine->gui->mapPane->world->debugmap->mapWidth, engine->gui->mapPane->world->debugmap->mapHeight));
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

	if (reloadWait == 0)
	{
		if (engine->settings->input->r->isSwitched)
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

void Weapon::render(const std::shared_ptr<Pane>& pane) const
{
	if (reloadWait == 0)
	{
		pane->console->setChar(toolx, tooly, ch);
		pane->console->setCharForeground(toolx, tooly, color);
	}

	for (auto& bullet : bulletList)
	{
		bullet->render(pane);
	}
}

//Item struct
//Item::Item(const char* name, std::shared_ptr<Tile> tile, std::shared_ptr<Tool> tool, bool useTile)
//	:tile(tile), tool(tool), Container(name)//, useTile(useTile)
//{
//}
//
//void Item::update(int x, int y, int mx, int my, double angle)
//{
//	//if (active)
//	//{
//	//	renderPosition.x = mapPosition.x - WORLD->xOffset;
//	//	renderPosition.y = mapPosition.y - WORLD->yOffset;
//	//	renderPosition.level = mapPosition.level;
//	//
//	//	if (useTile)
//	//	{
//	//		distToCreature = (float)getDistance(mapPosition.x, mapPosition.y, mapPosition.x, mapPosition.y);
//	//
//	//		if (distToCreature < 5)
//	//		{
//	//			tileBackgoundColor = tile->backgroundColor;
//	//		}
//	//		else
//	//		{
//	//			tileBackgoundColor = TCODColor::pink;
//	//		}
//	//	}
//	//	else
//	//	{
//	//		tool->update(x, y, mx, my, angle);
//	//	}
//	//}
//}
//
//void Item::render(const std::shared_ptr<Pane>& pane) const
//{
//	if (active)
//	{
//		if (useTile)
//		{
//			//tile->render(renderPosition.x, renderPosition.y, pane);
//		}
//		else
//		{
//			tool->render(pane);
//		}
//	}
//}
//