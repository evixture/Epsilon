#include "main.hpp"

//position struct
Position::Position(int x, int y)
	:x(x), y(y)
{
}

void Position::setPosition(int nx, int ny)
{
	x = nx;
	y = ny;
}

Position Position::getPosition(std::shared_ptr<Entity> entity)
{
	return entity->position;
}

//window struct
Window::Window(int x, int y)
	:winX(x), winY(y), console(new TCODConsole(x, y))
{
}

Window::~Window()
{
	delete console;
}

void Window::update()
{

}

void Window::render()
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

