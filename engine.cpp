#include "main.hpp"

Engine::Engine(int winx, int winy)
	:gameState(enSTARTUP)
{
	//input init
	input = std::make_shared<Input>();

	//font init
	terminalFont = std::make_shared<Font>("data/fonts/terminal16x16_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE);
	terminalFont->setFont(terminalFont);

	//gui init
	gui = std::make_shared<Window>(winx, winy);
	gui->createCanvas();

	//map init
	map = std::make_shared<Map>(62, 62, 62, 62);

	gameState = enMAIN;
}

Engine::~Engine()
{
}

//head of all other updates
void Engine::update()
{
	input->getInp(map->player);
}

//head of all other render functions
void Engine::render()
{
	map->render();
}

//TODO : find a way to check player bounds