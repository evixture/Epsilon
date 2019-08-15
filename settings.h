#include "main.hpp"

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

class Settings
{
public:
	int windowX;
	int windowY;
	const char* windowTitle;
	bool fullscreen;
	int maxFps;
	TCOD_renderer_t renderer;

	std::shared_ptr<Font> terminalfont;

	//main init
	Settings(int w, int h);
};
