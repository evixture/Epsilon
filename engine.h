#include "main.hpp"

class Input
{
public:

	//events
	TCOD_key_t key;
	TCOD_mouse_t mouse;
	TCOD_event_t event;

	Input();
	~Input();

	//handles input
	void getInp(std::shared_ptr<Entity> entity);
};

struct Font
{
	//font properties
	const char* filePath;
	int format;
	int charH;
	int charV;

	Font(const char* filePath, int format);
	Font(const char* filePath, int format, int xsize, int ysize);
	~Font();

	//sets the used font
	void setFont(std::shared_ptr<Font> font);
};

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