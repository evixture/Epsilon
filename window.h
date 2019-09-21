#include "main.hpp"

//Window Struct
struct Pane
{
	int consoleW;
	int consoleH;
	TCODConsole* console;

	TCODColor bgColor;
	TCODColor fgColor;

	Pane(int consoleW, int consoleH, TCODColor bgColor, TCODColor fgColor);
	~Pane();

	void render();
};

//Ribon Struct
struct Ribon
{
	const char* windowName;
	int windowW;

	std::shared_ptr<Pane> ribonWindow;

	Ribon(const char* windowName, int windowW);

	void updateRibonName(const char* ribonName);

	void render();
};

//GuiWindow Struct
struct Window
{
public:

	int w;
	int h;
	const char* panelName;
	Position renderpos;

	std::shared_ptr<Pane> drawWindow;

	Window(int w, int h, const char* panelName, int rx, int ry);

	virtual void update();
	virtual void render();

	void setRibonName(const char* ribonName);
	void clearWindow();
	void pushWindow();

private:
	std::shared_ptr<Ribon> ribon;
	std::shared_ptr<Pane> mainWindow;
};

//GuiMap Struct
