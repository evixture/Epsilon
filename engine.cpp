#include "main.hpp"

Engine::Engine(int screenCharWidth, int screenCharHeight)
	: screenCharWidth(screenCharWidth), screenCharHeight(screenCharHeight), windowTitle("Epsilon v. Alpha 10"), fullscreen(false), fovtype(FOV_RESTRICTIVE), renderer(TCOD_RENDERER_OPENGL2), //USE SDL2/ opengl2 TO RENDER, OTHERS WILL LOCK OR LOWER FPS
	fovRad(60), lightWalls(true), fpsCount(60)
{
	gamestate = Gamestate::MAIN;

	input = std::make_shared<Input>();
	settings = std::make_shared<Settings>();
	audio = std::make_shared<Audio>();
	gui = std::make_shared<Gui>(screenCharWidth, screenCharHeight); //input is undefined when entering


	TCODSystem::setFps(0);
	TCODConsole::setCustomFont("data/fonts/Epsilon16x16.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE, 16, 16);

	TCODConsole::initRoot(screenCharWidth, screenCharHeight, windowTitle.c_str(), fullscreen, renderer);
	TCODConsole::root->setDefaultBackground(ep::color::rootBG);
}

Engine::~Engine()
{
}

void Engine::setFullscreen()
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

void Engine::update()
{
	fpsCount = TCODSystem::getFps();
	lastFrameTime = systemClock.restart();
	input->update();

	if (input->fullscreen->isSwitched) setFullscreen();

	settings->update();
	gui->update();
	audio->update();
}

void Engine::render() const
{
	TCODConsole::root->clear();

	settings->render();
	gui->render();
	
	if (GUI->activeWindow != Gui::ActiveWindow::STARTUPSPLASH) printLogo();

	if (SETTINGS->showDebug) printDebugStats();

	TCODConsole::flush();	
}

void Engine::printLogo() const
{
	TCODConsole::root->printf(1, 0, "Epsilon");
}

void Engine::printDebugStats() const
{
	TCODConsole::root->printf(10, 0, "FPS>%i | Mouse %i, %i (%i, %i) | Player %i, %i, %i | a %i | s %i | c %i, i %i",
		fpsCount,
		input->mouse->screenPosition.x,
		input->mouse->screenPosition.y,
		input->mouse->mapPosition.x,
		input->mouse->mapPosition.y,
		WORLD->debugmap->player->mapPosition.x,
		WORLD->debugmap->player->mapPosition.y,
		WORLD->debugmap->player->mapPosition.z,
		AUDIO->soundList.size(),
		AUDIO->soLoud.getActiveVoiceCount(),
		WORLD->debugmap->player->containerIndex,
		WORLD->debugmap->player->itemIndex);
}
