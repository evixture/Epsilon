#include "main.hpp"

//Font Struct
Font::Font(const char* filePath, int format)
	:filePath(filePath), format(format), charW(0), charH(0), fontName(NULL), xdim(NULL), ydim(NULL)
{
}

Font::Font(const char* filePath, int format, int charW, int charH)
	: filePath(filePath), format(format), charW(charW), charH(charH), fontName(NULL), xdim(NULL), ydim(NULL)
{
}

Font::Font(const char* fontName, const char* xdim, const char* ydim, const char* filePath, int format)
	: fontName(fontName), xdim(xdim), ydim(ydim), filePath(filePath), format(format), charW(NULL), charH(NULL)
{
}

void Font::setfont(std::shared_ptr<Font> font)
{
	if (font->charH == 0 && font->charW == 0)
	{
		TCODConsole::setCustomFont(font->filePath, font->format);
	}
	else
	{
		TCODConsole::setCustomFont(font->filePath, font->format, font->charW, font->charH);
	}
}

//Input Struct
Input::Input()
	:keyboard(), mouse(), movingUp(false), movingDown(false), movingLeft(false), movingRight(false), moveTimer(0), moveWait(10)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &keyboard, &mouse);
}

void Input::getMouseInput()
{
	return;
}

void Input::getKeyDown()
{
	if (keyEvent == TCOD_KEY_PRESSED)
	{
		/*int ix = 0;
		int iy = 0;*/

		switch (keyboard.c)
		{
		case 'w':
			movingUp = true;
			break;
		case 's':
			movingDown = true;
			break;
		case 'a':
			movingLeft = true;
			break;
		case 'd':
			movingRight = true;
			break;
		default:
			break;
		}

		switch (keyboard.vk)
		{
		case TCODK_UP:
			movingUp = true;
			break;
		case TCODK_DOWN:
			movingDown = true;
			break;
		case TCODK_LEFT:
			movingLeft = true;
			break;
		case TCODK_RIGHT:
			movingRight = true;
			break;
		case TCODK_F11:
			if (!engine.settings->fullscreen)
			{
				TCODConsole::setFullscreen(true);
				engine.settings->fullscreen = true;
			}
			else if (engine.settings->fullscreen)
			{
				TCODConsole::setFullscreen(false);
				engine.settings->fullscreen = false;
			}
			break;
		default:
			break;
		}
	}
}

void Input::getKeyUp()
{
	if (keyEvent == TCOD_KEY_RELEASED)
	{
		switch (keyboard.c)
		{
		case 'w':
			movingUp = false;
			break;
		case 's':
			movingDown = false;
			break;
		case 'a':
			movingLeft = false;
			break;
		case 'd':
			movingRight = false;
			break;
		default:
			break;
		}

		switch (keyboard.vk)
		{
		case TCODK_UP:
			movingUp = false;
			break;
		case TCODK_DOWN:
			movingDown = false;
			break;
		case TCODK_LEFT:
			movingLeft = false;
			break;
		case TCODK_RIGHT:
			movingRight = false;
			break;
		default:
			break;
		}
	}
}


void Input::getKeyInput(std::shared_ptr<Player> player)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &keyboard, &mouse);

	if (engine.gamestate == Engine::MAIN)
	{
		getKeyDown();
		getKeyUp();
	}

	//reset move speed so that it doesnt add up MIGHT NEED TO CHANGE
	moveXSpeed = 0;
	moveYSpeed = 0;

	if (movingUp)
	{
		moveYSpeed--;
	}
	if (movingDown)
	{
		moveYSpeed++;
	}
	if (movingLeft)
	{
		moveXSpeed--;
	}
	if (movingRight)
	{
		moveXSpeed++;
	}
		
	if (moveXSpeed != 0 || moveYSpeed != 0)
	{
		if (moveTimer == 0)
		{
			if (   player->position.x + moveXSpeed >= 0 
				&& player->position.y + moveYSpeed >= 0 
				&& player->position.x + moveXSpeed < engine.gui->mapWindow->map->mapW 
				&& player->position.y + moveYSpeed < engine.gui->mapWindow->map->mapH)
			{
				if (engine.gui->mapWindow->map->getWalkability(player->position.x + moveXSpeed, player->position.y + moveYSpeed))
				{
					player->position.x += moveXSpeed;
					player->position.y += moveYSpeed;

					moveTimer = moveWait;

					std::cout << player->position.x << " : " << player->position.y << std::endl;
				}
			}
		}
		else
		{
			moveTimer--;
		}
	}
}

void Input::getInput(std::shared_ptr<Player> player)
{
	//check mortality in here
	getKeyInput(player);

}

//Settings Class
Settings::Settings(int w, int h)
	: windowX(w), windowY(h), windowTitle("Epsilon v. Alpha"), fullscreen(false), maxFps(60), renderer(TCOD_RENDERER_OPENGL2), fovtype(FOV_PERMISSIVE_8), computeFov(true)
{
	fontList.push_back(terminal16x16 = std::make_shared<Font>("Terminal", "16", "16", "data/fonts/terminal16x16_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE));
	terminal16x16->setfont(terminal16x16);

	input = std::make_shared<Input>();

	TCODConsole::initRoot(windowX, windowY, windowTitle, fullscreen, renderer);
	TCODConsole::root->setDefaultBackground(TCODColor::black);
	TCODSystem::setFps(maxFps);
}

void Settings::printLogo()
{
	TCODConsole::root->printf(1, 0, "Epsilon");
}

void Settings::printFps()
{
	TCODConsole::root->printf(10, 0, "%i  ix %i, iy %i up:%i, down:%i, left:%i, right:%i", TCODSystem::getFps(), input->moveXSpeed, input->moveYSpeed, input->movingUp, input->movingDown, input->movingLeft, input->movingRight);
}

void Settings::update(std::shared_ptr<Player> player)
{
	input->getInput(player);
}
