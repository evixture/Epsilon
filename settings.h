#include "main.hpp"

//Font Struct
struct Font
{
	const char* filePath;
	int format;
	int charH;
	int charW;

	const char* fontName;
	const char* xDim;
	const char* yDim;

	Font(const char* filePath, int format);
	Font(const char* filePath, int format, int charW, int charH);
	Font(const char* fontName, const char* xdim, const char* ydim, const char* filePath, int format);

	
};



//Settings Class
class Settings
{
public:
	int windowX;
	int windowY;

	TCOD_fov_algorithm_t fovtype;

	std::shared_ptr<Input> input;

	std::shared_ptr<Font> terminal16x16;
	std::vector<std::shared_ptr<Font>> fontList;

	//main init
	Settings(int w, int h);

	void setFullscreen();

	void update(std::shared_ptr<Player> player);
	void render();

private:

	int maxFps;
	TCOD_renderer_t renderer;
	const char* windowTitle;
	bool fullscreen;

	void setfont(std::shared_ptr<Font> font);
	void printLogo();
	void printFps();

};
