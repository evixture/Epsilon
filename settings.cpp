#include "main.hpp"

//Font Struct
Font::Font(const char* filePath, int format)
	:filePath(filePath), format(format), charW(0), charH(0), fontName(NULL), xDim(NULL), yDim(NULL)
{}

Font::Font(const char* filePath, int format, int charW, int charH)
	: filePath(filePath), format(format), charW(charW), charH(charH), fontName(NULL), xDim(NULL), yDim(NULL)
{}

Font::Font(const char* fontName, const char* xdim, const char* ydim, const char* filePath, int format)
	: fontName(fontName), xDim(xdim), yDim(ydim), filePath(filePath), format(format), charW(NULL), charH(NULL)
{}

//Settings Class
Settings::Settings(int w, int h)
	: windowX(w), windowY(h), windowTitle("Epsilon v. Alpha"), fullscreen(false), maxFps(60),fovtype(FOV_RESTRICTIVE), renderer(TCOD_RENDERER_OPENGL2), fovRad(0), lightWalls(true), isActive(true)
{
	fontList.push_back(terminal16x16 = std::make_shared<Font>("Terminal", "16", "16", "data/fonts/terminal16x16_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE));
	setfont(terminal16x16);

	input = std::make_shared<Input>();

	TCODConsole::initRoot(windowX, windowY, windowTitle, fullscreen, renderer);
	TCODConsole::root->setDefaultBackground(TCODColor::black);
	TCODSystem::setFps(maxFps);
}

void Settings::setFullscreen()
{
	if (!fullscreen)
	{
		TCODConsole::setFullscreen(true);
		fullscreen = true;
	}
	else if (fullscreen)
	{
		TCODConsole::setFullscreen(false);
		fullscreen = false;
	}
}

void Settings::setfont(std::shared_ptr<Font> font)
{
	if (font->charH == 0 && font->charW == 0)
	{
		TCODConsole::setCustomFont(font->filePath, font->format);
	}
	else
	{
		TCODConsole::setCustomFont(font->filePath, font->format, font->charW, font->charH);
	}
}

void Settings::printLogo()
{
	TCODConsole::root->printf(1, 0, "Epsilon");
}

void Settings::printFps()
{
	TCODConsole::root->printf(10, 0, "FPS %i px: %i py: %i",
		TCODSystem::getFps(), engine.gui->mapWindow->map->player->position.x, engine.gui->mapWindow->map->player->position.y);

	/*
	TCODConsole::root->printf(10, 0, "%i up:%i, down:%i, left:%i, right:%i, movewait:%i, shift:%i, ctrl:%i",
		TCODSystem::getFps(), input->moveUp, input->moveDown, input->moveLeft, input->moveRight, input->moveWait, input->keyboard.shift, input->keyboard.lctrl);
	*/
}

void Settings::update(std::shared_ptr<Player> player)
{
	input->getInput(player);
}

void Settings::render()
{
	printLogo();
	printFps();
}