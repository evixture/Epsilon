#include "main.hpp"

class Engine
{
public:

	//main engine state, other states in gui etc.
	enum gameState { enSTARTUP, enMAIN, enGAMEOVER, enEXIT } gameState;

	std::shared_ptr<Input> input;
	std::shared_ptr<Font> terminalFont;
	std::shared_ptr<Window> gui;

	std::shared_ptr<Map> map;

	Engine(int winx, int winy);
	~Engine();

	//head of all other updates
	void update();

	//head of all other render functions
	void render();
};

extern Engine engine;

//TODO : create an input class
//TODO : render map , player, etc
//TODO : Bullet travel
//TODO : better game states
//TODO : make font class
//TODO : rename engine into systems
//TODO : keyboard and mouse input