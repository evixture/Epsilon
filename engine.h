#include "main.hpp"

class Input
{
public:

	TCOD_key_t key;
	TCOD_mouse_t mouse;

	Input();
	~Input();

	void getKeyInp(TCOD_key_t key);

	void getMouseInp(TCOD_mouse_t);
};

struct Font
{
	const char* filePath;
	int format;
	int charH;
	int charV;

	Font(const char* filePath, int format);
	~Font();

	void setFont(std::shared_ptr<Font> font);
};

class Engine
{
public:

	enum gameState { STARTUP, MAIN, GAMEOVER, EXIT } gameState;

	std::shared_ptr<Input> input;
	std::shared_ptr<Font> terminalFont;
	std::shared_ptr<Gui> gui;

	std::shared_ptr<Entity> player;
	std::vector<std::shared_ptr<Entity>> entityList;

	Engine(int winx, int winy);
	~Engine();

	void update();

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