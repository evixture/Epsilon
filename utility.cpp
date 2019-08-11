#include "utility.h"

//position struct
Position::Position(int x, int y)
	:x(x), y(y)
{
}

//tilemap struct
TileMap::TileMap()
{
	//color, blocks move, blocks light, destructible

	tileMap.insert(std::make_pair
		("grass", Tile(TCODColor::darkestGreen, false, false, false)));
	tileMap.insert(std::make_pair
		("defWall", Tile(TCODColor::TCODColor::lighterSepia, false, false, false)));
}

TileMap::~TileMap()
{
}

//window struct
Window::Window()
{
}

Window::~Window()
{
}

//font struct
Font::Font(const char* filePath, int format)
	:filePath(filePath), format(format)
{
}

Font::Font(const char* filePath, int format, int xsize, int ysize)
	: filePath(filePath), format(format), charH(xsize), charV(ysize)
{
}

Font::~Font()
{
}

void Font::setFont(std::shared_ptr<Font> font)
{
	TCODConsole::setCustomFont(font->filePath, font->format);
}

//input class
Input::Input()
	:key(TCODConsole::checkForKeypress(TCOD_KEY_PRESSED))
{
}

Input::~Input()
{
}

//handles input
void Input::getInp(std::shared_ptr<Entity> entity)
{
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &key, &mouse);

	if (engine.gameState == engine.enMAIN)
	{
		int ix = 0;
		int iy = 0;

		switch (key.vk)
		{
		case TCODK_UP:
			iy = -1;
			break;
		case TCODK_DOWN:
			iy = 1;
			break;
		case TCODK_LEFT:
			ix = -1;
			break;
		case TCODK_RIGHT:
			ix = 1;
			break;
		default:
			break;
		}

		switch (key.c)
		{
		case 'w':
			iy = -1;
			break;
		case 's':
			iy = 1;
			break;
		case 'a':
			ix = -1;
			break;
		case 'd':
			ix = 1;
			break;
		default:
			break;
		}

		if (ix != 0 || iy != 0)
		{
			entity->x += ix;
			entity->y += iy;
		}
		std::cout << entity->x << ':' << entity->y << std::endl;
	}
}

