#include "main.hpp"

//Input Struct
struct Input
{
	TCOD_event_t keyEvent;
	TCOD_key_t keyboard;
	TCOD_mouse_t mouse;

	int moveXSpeed = 0;
	int moveYSpeed = 0;

	int moveTimer;
	int moveWait;

	bool moveUp;
	bool moveDown;
	bool moveLeft;
	bool moveRight;

	bool f11Toggle;

	Input();

	//not sure if ent param is needed
	void getInput(std::shared_ptr<Player> player);

	void getKeyDown();
	void getKeyInput(std::shared_ptr<Player> player);

	void getMouseInput();
};
