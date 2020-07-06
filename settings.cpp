#include "main.hpp"

Font::Font(std::string name, std::string xdim, std::string ydim, std::string filePath, int format, int charW, int charH)
	: name(name), xDim(xdim), yDim(ydim), filePath(filePath), format(format), charW(charW), charH(charH)
{
}

//----------------------------------------------------------------------------------------------------

Settings::Settings()
	:showAIMind(false), showWalkable(false)
{
}

void Settings::setfont(std::shared_ptr<Font> font)
{
	TCODConsole::setCustomFont(font->filePath.c_str(), font->format, (font->charW) ? font->charW : NULL, (font->charH) ? font->charH : NULL);
}

void Settings::update()
{
}

void Settings::render() const
{
}
