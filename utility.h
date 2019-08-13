#include "main.hpp"

struct Position
{
	int x;
	int y;

	Position(int x, int y);

	void setPosition(int nx, int ny);

	Position getPosition(std::shared_ptr<Entity> entity);
};

struct Window
{
	int winX;
	int winY;

	TCODConsole* console;

	Window(int x, int y);

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

//TODO : Implement position and other utilities into classes