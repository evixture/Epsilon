#include "main.hpp"

//Window Struct
struct Window
{
	int consoleW;
	int consoleH;
	TCODConsole* console;

	TCODColor bgColor;
	TCODColor fgColor;

	Window(int consoleW, int consoleH, TCODColor bgColor, TCODColor fgColor);
	~Window();

	void render();
};

//Ribon Struct
struct Ribon
{
	const char* windowName;
	int windowW;

	std::shared_ptr<Window> ribonWindow;

	Ribon(const char* windowName, int windowW);

	void render();
};

//GuiWindow Struct
struct Panel
{
public:

	int w;
	int h;
	const char* panelName;
	Position renderpos;

	std::shared_ptr<Window> drawWindow;

	Panel(int w, int h, const char* panelName, int rx, int ry);
	
	virtual void update();
	virtual void render();

	void clearWindow();
	void pushWindow();

private:

	std::shared_ptr<Ribon> ribon;
	std::shared_ptr<Window> mainWindow;

};

//GuiMap Struct
struct GuiMap : public Panel
{
	std::shared_ptr<Window> mapSidePanel;

	std::shared_ptr<Map> map;
	std::shared_ptr<TCODMap> tcodMap;

	GuiMap(int w, int h, int rx, int ry);

	void update();

	void render();
};
