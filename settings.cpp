#include "main.hpp"

Font::Font(std::string name, std::string xdim, std::string ydim, std::string filePath, int format, int charW, int charH)
	:name(name), xDim(xdim), yDim(ydim), filePath(filePath), format(format), charW(charW), charH(charH)
{}

//----------------------------------------------------------------------------------------------------

Settings::Settings(int screenCharWidth, int screenCharHeight)
	:screenCharWidth(screenCharWidth), screenCharHeight(screenCharHeight), windowTitle("Epsilon v. Alpha 8"), fullscreen(false), maxFps(0), fovtype(FOV_RESTRICTIVE), renderer(TCOD_RENDERER_OPENGL2), //USE SDL2/ opengl2 TO RENDER, OTHERS WILL LOCK OR LOWER FPS
	fovRad(60), lightWalls(true), fpsCount(60)
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
	TCODConsole::root->printf(10, 0, "FPS>%i | Mouse %i, %i | Player %i, %i %i | winfoc>%i | lft>%f | ii %i | ci %i | angle %f",
		fpsCount,
		SETTINGS->input->mouse.cx,
		SETTINGS->input->mouse.cy,
		WORLD->debugmap->player->mapPosition.x,
		WORLD->debugmap->player->mapPosition.y,
		WORLD->debugmap->player->mapPosition.floor,
		TCODConsole::hasMouseFocus(),
		lastFrameTime.asSeconds(),
		WORLD->debugmap->player->itemIndex,
		WORLD->debugmap->player->containerIndex,
		WORLD->debugmap->player->angle);
}

void Settings::update()
{
	fpsCount = TCODSystem::getFps();
	lastFrameTime = systemClock.restart();
	input->update();

	if (INPUT->fullscreenKey->isSwitched)
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
