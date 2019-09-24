#include "main.hpp"

//GuiMap Struct
MapPane::MapPane(int w, int h, int rx, int ry)
	:Window(w, h, "World", rx, ry)
{
	mapSidePanel = std::make_shared<Pane>(1, 61, RIBONBGCOLOR, RIBONFGCOLOR);

	world = std::make_shared<World>();
	//map->createMap(map->debugmap);
}

void MapPane::update()
{
	world->update(drawWindow);
	//setRibonName(world->debugmap->mapName);
}

//GuiMap Render
void MapPane::render()
{
	clearWindow();
	world->render(drawWindow);

	mapSidePanel->render();
	mapSidePanel->console->blit(mapSidePanel->console, 0, 0, 1, 61, TCODConsole::root, 62, 2, 1, 1);

	pushWindow();
}

//StatusPane Struct
StatusPane::StatusPane(int w, int h, int rx, int ry)
	:Window(w, h, "Status", rx, ry), displayHealth(0), displayArmor(0)
{

}

void StatusPane::drawBar(std::shared_ptr<Pane> window, int x, int y, const char* barTitle, int barValue, int barMax)
{
	window->console->printf(x, y, "%s : [", barTitle);

}

void StatusPane::update()
{
	displayHealth = engine->gui->mapPane->world->player->health;
	displayArmor = engine->gui->mapPane->world->player->armor;
}

void StatusPane::render()
{
	clearWindow();

	drawWindow->console->printf(0, 0, "Health : [");
	drawWindow->console->printf(0, 1, "Armor  : [");

	for (int healthPrint = 0; healthPrint < displayHealth; healthPrint++)
	{
		if (healthPrint < (engine->gui->mapPane->world->player->health * 5))
		{
		drawWindow->console->printf(healthPrint, 0, "=");
		}
	}


	for (int armorPrint = 0; armorPrint < displayArmor; armorPrint++)
	{
		if (armorPrint < (engine->gui->mapPane->world->player->armor * 5))
		{
			drawWindow->console->printf(armorPrint, 1, "=");
		}
	}

	pushWindow();
}