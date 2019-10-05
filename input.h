#include "main.hpp"

//Input Struct
struct Input
{
	TCOD_event_t keyEvent;
	TCOD_key_t keyboard;
	TCOD_mouse_t mouse;

	int moveXSpeed = 0;
	int moveYSpeed = 0;

	int baseMoveWait;
	int moveTimer;
	int moveWait;

	bool moveUp;
	bool moveDown;
	bool moveLeft;
	bool moveRight;

	bool reload;
	bool reloadToggle;

	bool leftMouseClick;

	bool f11Toggle;

	Input();

	//TODO : Move key checks to funcs with buttton params for config support

	//not sure if ent param is needed
	void update(std::shared_ptr<Player> player);

	void getKeyDown();
	void getKeyInput(std::shared_ptr<Player> player);

	void getMouseInput();
};
