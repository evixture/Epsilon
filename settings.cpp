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
	:keyboard(TCODConsole::checkForKeypress(TCOD_KEY_PRESSED)), mouse()
{
}

void Input::getKeyInput(std::shared_ptr<Entity> entity)
{
	//check for mortality
	//add momentum etc

	if (engine.gamestate == Engine::MAIN)
	{
		int ix = 0;
		int iy = 0;

		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &keyboard, NULL);

		switch (keyboard.c)
		{
		case 'w':
			iy--;
			break;
		case 's':
			iy++;
			break;
		case 'a':
			ix--;
			break;
		case 'd':
			ix++;
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
			entity->position.x += ix;
			entity->position.y += iy;
			std::cout << entity->position.x << " : " << entity->position.y << std::endl;
		}
	}
}

//Settings Class
Settings::Settings(int w, int h)
	: windowX(w), windowY(h), windowTitle("Epsilon v. Alpha"), fullscreen(false), maxFps(60), renderer(TCOD_RENDERER_OPENGL2)
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

void Settings::update(std::shared_ptr<Entity> entity)
{
	input->getKeyInput(entity);
}
