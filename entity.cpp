#include "main.hpp"

//Position Struct
Position::Position(int x, int y, int level)
	:x(x), y(y), level(level)
{}

//Default Entity Class
Entity::Entity(Position pos, int symbol, const char* name, TCODColor color)
	: mapPosition(pos), renderPosition(pos), symbol(symbol), name(name), color(color), height(3)
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

Creature::Creature(Position pos, int symbol, const char* name, TCODColor color, int health, int armor)
	:Entity(Position(pos), symbol, name, color), health(health), armor(armor), angle(0)
{
}

void Creature::update()
{
	angle = getAngle(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx, engine->settings->input->mouse.cy);
}

void Creature::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(mapPosition.x, mapPosition.y, symbol);
	pane->console->setCharForeground(mapPosition.x, mapPosition.y, color);
}

Player::Player(Position pos)
	:Creature(Position(pos), '@', "player", TCODColor::azure, 100, 0), weaponSelection(0)
{
	//inventory->push_back(hands = std::make_shared<Tool>("tempName", TCODColor::peach));
	//inventory->push_back(std::make_shared<Tool>(TCODColor::peach));

	toolList.push_back(hands = std::make_shared<Tool>("hands", TCODColor::peach));
	toolList.push_back(testWeapon = std::make_shared<Weapon>("tempName", TCODColor::darkestGrey, 30, 10, .09f, 2.0f));
	currentTool = toolList[0];
}

void Player::update()
{
	renderPosition.x = mapPosition.x - WORLD->xOffset;
	renderPosition.y = mapPosition.y - WORLD->yOffset;
	renderPosition.level = mapPosition.level;

	if (engine->settings->input->mouse.wheel_up || engine->settings->input->mouse.wheel_down)
	{
		if (engine->settings->input->mouse.wheel_down)
		{
			if (weaponSelection < toolList.size() - 1)
			{
				weaponSelection++;
			}
		}
		if (engine->settings->input->mouse.wheel_up)
		{
			if (weaponSelection > 0)
			{
				weaponSelection--;
			}
		}
		currentTool = toolList[weaponSelection];
	}

	angle = getAngle(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3);

	currentTool->update(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3, angle);

	if (engine->settings->input->changeFloor == true)
	{
		if (WORLD->getTile(mapPosition.x, mapPosition.y, mapPosition.level)->tag == "stair")
		{
			WORLD->getTile(mapPosition.x, mapPosition.y, mapPosition.level)->interact();
		}
	}
}

void Player::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(renderPosition.x, renderPosition.y, symbol);
	pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);
	currentTool->render(pane);
}
