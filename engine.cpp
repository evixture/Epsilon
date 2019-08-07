#include "main.hpp"

Input::Input()
	:key(TCODConsole::checkForKeypress(TCOD_KEY_PRESSED))
{
}

Input::~Input()
{
}

void Input::getInp(std::shared_ptr<Entity> entity)
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
		entity->x = entity->x + ix;
		entity->y = entity->y + iy;
	}
}

Font::Font(const char* filePath, int format)
	:filePath(filePath), format(format)
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
	:gameState(STARTUP)
{
	input = std::make_shared<Input>();

	terminalFont = std::make_shared<Font>("data/terminal16x16_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE);
	terminalFont->setFont(terminalFont);

	gui = std::make_shared<Gui>(winx, winy);
	gui->createCanvas();

	player = std::make_shared<Entity>(1, 1, '@', TCODColor::blue, "player");
}

Engine::~Engine()
{
}

void Engine::update()
{
	input->getInp(player);
}

void Engine::render()
{
}