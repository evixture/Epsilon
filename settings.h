#include "main.hpp"

//Font Struct
struct Font
{
	const char* filePath;
	int format;
	int charH;
	int charW;

	Font(const char* filePath, int format);
	Font(const char* filePath, int format, int charW, int charH);

	void setfont(std::shared_ptr<Font> font);
};

//Input Struct
struct Input
{
	TCOD_key_t keyboard;
	TCOD_mouse_t mouse;

	Input();

	void getKeyInput(std::shared_ptr<Entity> entity);
};

//Settings Class
class Settings
{
public:
	int windowX;
	int windowY;
	const char* windowTitle;
	bool fullscreen;
	int maxFps;
	TCOD_renderer_t renderer;

	std::shared_ptr<Input> input;

	//replace terminal font with vector
	std::shared_ptr<Font> terminalfont;

	//main init
	Settings(int w, int h);

	void printLogo();

	void update(std::shared_ptr<Entity> entity);
};
