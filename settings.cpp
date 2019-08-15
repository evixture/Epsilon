#include "main.hpp"

Font::Font(const char* filePath, int format)
	:filePath(filePath), format(format), charW(NULL), charH(NULL)
{
}

Font::Font(const char* filePath, int format, int charW, int charH)
	: filePath(filePath), format(format), charW(charW), charH(charH)
{
}

void Font::setfont(std::shared_ptr<Font> font)
{
	if (font->charH == NULL && font->charW == NULL)
	{
		TCODConsole::setCustomFont(font->filePath, font->format);
	}
	else
	{
		TCODConsole::setCustomFont(font->filePath, font->format, font->charW, font->charH);
	}
}

Settings::Settings(int w, int h)
	: windowX(w), windowY(h), windowTitle("Epsilon v. Alpha"), fullscreen(false), maxFps(60), renderer(TCOD_RENDERER_OPENGL2)
{
	terminalfont = std::make_shared<Font>("data/fonts/terminal16x16_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE);
	terminalfont->setfont(terminalfont);

	TCODConsole::initRoot(windowX, windowY, windowTitle, fullscreen, renderer);
	TCODConsole::root->setDefaultBackground(TCODColor::black);
	TCODSystem::setFps(maxFps);
}