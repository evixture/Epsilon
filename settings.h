#include "main.hpp"

//Font Struct
struct Font
{
	std::string filePath;
	int format;
	int charH;
	int charW;

	std::string fontName;
	std::string xDim;
	std::string yDim;

	Font(std::string fontName, std::string xdim, std::string ydim, std::string filePath, int format, int charW, int charH);
};

//Settings Class
class Settings
{
public:
	int screenCharWidth;
	int screenCharHeight;

	int fpsCount;

	TCOD_fov_algorithm_t fovtype;
	int fovRad;
	bool lightWalls;

	//std::shared_ptr<TCODRandom> RandNumGen;

	std::shared_ptr<Input> input;

	std::shared_ptr<Font> terminal16x16;
	std::vector<std::shared_ptr<Font>> fontList;

	Settings(int screenCharWidth, int screenCharHeight);

	void setFullscreen();

	void update(std::shared_ptr<Player> player);
	void render() const;

private:
	//DEBUG VALUES
	int maxFps;

	//TCOD PROPERTIES
	TCOD_renderer_t renderer;
	std::string windowTitle;
	bool fullscreen;

	void setfont(std::shared_ptr<Font> font);
	void printLogo() const;
	void printDebugStats() const;
};
