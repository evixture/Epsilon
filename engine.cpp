#include "main.hpp"

Input::Input()
{
}

Input::~Input()
{
}

void Input::getKeyInp(TCOD_key_t key)
{
}

void Input::getMouseInp(TCOD_mouse_t)
{
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

	player = std::make_shared<Entity>(1, 1, '@', TCODColor::blue, "player")
}

Engine::~Engine()
{
}

void Engine::update()
{
}

void Engine::render()
{
}