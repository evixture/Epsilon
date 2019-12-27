#include "main.hpp"

Font::Font(std::string fontName, std::string xdim, std::string ydim, std::string filePath, int format, int charW, int charH)
	:fontName(fontName), xDim(xdim), yDim(ydim), filePath(filePath), format(format), charW(charW), charH(charH)
{}

//----------------------------------------------------------------------------------------------------

Settings::Settings(int screenCharWidth, int screenCharHeight)
	:screenCharWidth(screenCharWidth), screenCharHeight(screenCharHeight), windowTitle("Epsilon v. Alpha 7"), fullscreen(false), maxFps(0), fovtype(FOV_RESTRICTIVE), renderer(TCOD_RENDERER_SDL2), //USE SDL2 TO RENDER, OTHERS WILL LOCK OR LOWER FPS
	fovRad(0), lightWalls(true), fpsCount(60)
{
	TCODSystem::setFps(0);
	TCODConsole::setCustomFont("data/fonts/Epsilon16x16.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE, 16, 16);

	input = std::make_shared<Input>();
	
	TCODConsole::initRoot(screenCharWidth, screenCharHeight, windowTitle.c_str(), fullscreen, renderer);
	TCODConsole::root->setDefaultBackground(UICOLOR_Root_BG);
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
	TCODConsole::setCustomFont(font->filePath.c_str(), font->format, (font->charW) ? font->charW : NULL, (font->charH) ? font->charH : NULL);
}

void Settings::printLogo() const
{
	TCODConsole::root->printf(1, 0, "Epsilon");
}

void Settings::printDebugStats() const
{
	TCODConsole::root->printf(10, 0, "FPS>%i mx>%i my>%i px>%i py>%i  ph>%i, gs>%i, ammo>%i",
		fpsCount,
		SETTINGS->input->mouse.cx - 1,
		SETTINGS->input->mouse.cy - 3,
		WORLD->player->mapPosition.x,
		WORLD->player->mapPosition.y,
		WORLD->player->mapPosition.level,
		engine->gamestate,
		WORLD->player->selectedMagazine->availableAmmo);
}

void Settings::update(std::shared_ptr<Player> player)
{
	fpsCount = TCODSystem::getFps();
	input->update(player);

	if (INPUT->f11->isSwitched)
	{
		setFullscreen();
	}
}

void Settings::render() const
{
	if (GUI->activeWindow != Gui::ActiveWindow::STARTUPSPLASH)
	{
		printLogo();
	}
	printDebugStats();
}
