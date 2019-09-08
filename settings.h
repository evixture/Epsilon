#include "main.hpp"

//Font Struct
struct Font
{
	const char* filePath;
	int format;
	int charH;
	int charW;

	const char* fontName;
	const char* xdim;
	const char* ydim;

	Font(const char* filePath, int format);
	Font(const char* filePath, int format, int charW, int charH);
	Font(const char* fontName, const char* xdim, const char* ydim, const char* filePath, int format);

	void setfont(std::shared_ptr<Font> font);
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
	TCOD_fov_algorithm_t fovtype;
	bool computeFov;

	std::shared_ptr<Input> input;

	std::vector<std::shared_ptr<Font>> fontList;
	std::shared_ptr<Font> terminal16x16;

	//main init
	Settings(int w, int h);

	void printLogo();

	void printFps();

	void update(std::shared_ptr<Player> player);
};
