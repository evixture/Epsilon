#include "main.hpp"

//Font Struct
Font::Font(const char* fontName, const char* xdim, const char* ydim, const char* filePath, int format, int charW, int charH)
	: fontName(fontName), xDim(xdim), yDim(ydim), filePath(filePath), format(format), charW(charW), charH(charH)
{}

//Settings Class
Settings::Settings(int w, int h)
	: windowX(w), windowY(h), windowTitle("Epsilon v. Alpha 2"), fullscreen(false), maxFps(0), fovtype(FOV_RESTRICTIVE), renderer(TCOD_RENDERER_OPENGL2), 
	fovRad(0), lightWalls(true), isActive(true)
{
	//fontList.push_back(terminal16x16 = std::make_shared<Font>("Terminal", "16", "16", "data/fonts/terminal16x16_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE, 16, 16));
	//setfont(terminal16x16);

	TCODConsole::setCustomFont("data/fonts/terminal16x16_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE, 16, 16);

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
		/*
		%i : int
		%f : float and double
		*/
		//TCODConsole::root->printf(10, 0, "FPS %i px: %i py: %i pheight: %i mouseAng:%f",
		//	TCODSystem::getFps(), 
		//	engine->gui->mapPane->world->player->position.x, 
		//	engine->gui->mapPane->world->player->position.y,
		//	engine->gui->mapPane->world->player->height, 
		//	engine->gui->mapPane->world->player->testWeapon->angle ? engine->gui->mapPane->world->player->testWeapon->angle : 0);

		TCODConsole::root->printf(10, 0, "FPS %i mx: %i my: %i mouseAng: %f",
			TCODSystem::getFps(), 
			//(double)((my + 3) - y) / (double)((mx + 1) - x)
			(engine->settings->input->mouse.cx  - 1) - engine->gui->mapPane->world->player->position.x,
			(engine->settings->input->mouse.cy  - 3) - engine->gui->mapPane->world->player->position.y,
			engine->gui->mapPane->world->player->testWeapon->angle ? engine->gui->mapPane->world->player->testWeapon->angle : 0);
	}
}

void Settings::update(std::shared_ptr<Player> player)
{
	input->getInput(player);
}

void Settings::render()
{
	printLogo();
	printDebugStats();
}
