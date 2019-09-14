#include "main.hpp"

//GuiMap Struct
MapPane::MapPane(int w, int h, int rx, int ry)
	:Window(w, h, "World", rx, ry)
{
	mapSidePanel = std::make_shared<Pane>(1, 61, RIBONBGCOLOR, TCODColor::white);

	map = std::make_shared<World>();
	//map->createMap(map->debugmap);
	map->updateProperties();
}

void MapPane::update()
{
	map->update(drawWindow);
}

//GuiMap Render
void MapPane::render()
{
	clearWindow();
	map->render(drawWindow);
	pushWindow();

	mapSidePanel->render();
	mapSidePanel->console->blit(mapSidePanel->console, 0, 0, 1, 61, TCODConsole::root, 62, 2, 1, 1);
}
