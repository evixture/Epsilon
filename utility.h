#include "main.hpp"

struct Position
{
	int x;
	int y;

	Position(int x, int y);
};

struct TileMap
{
	std::map<std::string, Tile> tileMap;

	TileMap();

	~TileMap();
};

struct Window
{
	int winX;
	int winY;

	TCODConsole* console;

	Window();

	~Window();

	void update();

	void render();
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




//TODO : Implement position and other utilities into classes