#include "main.hpp"

//for main tcod functions and call tree
class Engine
{
public:

	enum gameState { STARTUP, MAIN, GAMEOVER, EXIT } gameState;
	int maxFps = 60;

	int windowx;
	int windowy;

	/*std::vector<std::shared_ptr<Entity>> entityList;
	std::shared_ptr<Entity> player;*/

	TCOD_key_t key;
	TCOD_mouse_t mouse;

	//start of funcs
	Engine(int winx, int winy);

	~Engine();

	//start of update functions
	void update();

	void input();

	//start of render functions
	void render();

	void createCanvas(int x, int y);

private:
};

//struct Font
//{
//	const char* fontPath;
//};

extern Engine engine;

//TODO : create an input class
//TODO : render map , player, etc
//TODO : Bullet travel
//TODO : better game states
//TODO : make font class
//TODO : rename engine into systems
//TODO : keyboard and mouse input