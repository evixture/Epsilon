#include "main.hpp"

//for main tcod functions and call tree
class Engine
{
public:

	enum gameState {STARTUP, MAIN, GAMEOVER, EXIT} gameState;
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