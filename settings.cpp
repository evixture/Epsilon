#include "main.hpp"

//Font Struct
Font::Font(const char* fontName, const char* xdim, const char* ydim, const char* filePath, int format, int charW, int charH)
	: fontName(fontName), xDim(xdim), yDim(ydim), filePath(filePath), format(format), charW(charW), charH(charH)
{}

//Settings Class
Settings::Settings(int screenCharWidth, int screenCharHeight)
	: screenCharWidth(screenCharWidth), screenCharHeight(screenCharHeight), windowTitle("Epsilon v. Alpha 5"), fullscreen(false), maxFps(0), fovtype(FOV_RESTRICTIVE), renderer(TCOD_RENDERER_OPENGL2), 
	fovRad(0), lightWalls(true)
{
	//fontList.push_back(terminal16x16 = std::make_shared<Font>("Terminal", "16", "16", "data/fonts/terminal16x16_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE, 16, 16));
	//setfont(terminal16x16);

	TCODConsole::setCustomFont("data/fonts/terminal16x16_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE, 16, 16);

	input = std::make_shared<Input>();

	TCODConsole::initRoot(screenCharWidth, screenCharHeight, windowTitle, fullscreen, renderer);
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
	TCODConsole::setCustomFont(font->filePath, font->format, (font->charW) ? font->charW : NULL, (font->charH) ? font->charH : NULL);
}

void Settings::printLogo()
{
	TCODConsole::root->printf(1, 0, "Epsilon");
}

void Settings::printDebugStats()
{
	if (engine->gamestate == engine->MAIN)
	{
		TCODConsole::root->printf(10, 0, "FPS>%i mx>%i my>%i px>%i py>%i ammo>%i, mags>%i ph>%i, space>%i xoff> %i yoff%i",
			TCODSystem::getFps(),
			engine->settings->input->mouse.cx - 1,
			engine->settings->input->mouse.cy - 3,
			engine->gui->mapPane->world->player->mapPosition.x,
			engine->gui->mapPane->world->player->mapPosition.y,
			engine->gui->mapPane->world->player->testWeapon->ammoAmount,
			engine->gui->mapPane->world->player->testWeapon->numberMags,
			engine->gui->mapPane->world->player->mapPosition.level,
			engine->settings->input->changeFloor,
			engine->gui->mapPane->world->xOffset,
			engine->gui->mapPane->world->yOffset);
	}
}

void Settings::update(std::shared_ptr<Player> player)
{
	input->update(player);
}

void Settings::render()
{
	printLogo();
	printDebugStats();
}
