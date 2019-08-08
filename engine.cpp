#include "main.hpp"

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

Engine::Engine(int winx, int winy)
	:gameState(enSTARTUP)
{
	//input init
	input = std::make_shared<Input>();

	//font init
	terminalFont = std::make_shared<Font>("data/terminal16x16_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE);
	terminalFont->setFont(terminalFont);

	//gui init
	gui = std::make_shared<Gui>(winx, winy);
	gui->createCanvas();

	//player init
	player = std::make_shared<Entity>(1, 1, '@', TCODColor::blue, "player");
	entityList.push_back(player);

	gameState = enMAIN;
}

Engine::~Engine()
{
}

//head of all other updates
void Engine::update()
{
	input->getInp(player);
}

//head of all other render functions
void Engine::render()
{
	player->render();
}