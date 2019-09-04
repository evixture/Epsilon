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
	:keyboard(TCODConsole::checkForKeypress(TCOD_KEY_PRESSED)), mouse(), movingup(false), movingdown(false), movingleft(false), movingright(false)
{
}

void Input::getMouseInput()
{
	TCODSystem::checkForEvent(TCOD_EVENT_MOUSE, NULL, &mouse);
	return;
}

void Input::getKeyInput(std::shared_ptr<Player> player)
{
	//check for mortality
		//add momentum etc

	if (engine.gamestate == Engine::MAIN)
	{
		int ix = 0;
		int iy = 0;

		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &keyboard, NULL);

		switch (keyboard.c)
		{
		case 'w':
			iy = -1;
			break;
		case 's':
			iy = 1;
			break;
		case 'a':
			ix = -1;
			break;
		case 'd':
			ix = 1;
			break;
		default:
			break;
		}

		switch (keyboard.vk)
		{
		case TCODK_UP:
			iy--;
			break;
		case TCODK_DOWN:
			iy++;
			break;
		case TCODK_LEFT:
			ix--;
			break;
		case TCODK_RIGHT:
			ix++;
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
		
		if (ix != 0 || iy != 0)
		{
			int jx = ix;
			int jy = iy;
			if (engine.gui->mapWindow->map->getWalkability((ix += player->position.getPosition().x), (iy += player->position.getPosition().y)))
			{
				player->position.x += jx;
				player->position.y += jy;

				//std::cout << entity->position.x << " : " << entity->position.y << std::endl;
			}
		}
		std::cout << player->position.x << " : " << player->position.y << std::endl;
		/*ix = 0;
		iy = 0;*/
	}
}

void Input::getInput(std::shared_ptr<Player> player)
{
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
	TCODConsole::root->printf(20, 0, "%i", TCODSystem::getFps());
}

void Settings::update(std::shared_ptr<Player> player)
{
	input->getInput(player);
}
