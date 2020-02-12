#pragma once

//STD
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
//#include <chrono>
//#include <stdexcept>

//Libraries
#include <libtcod.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <E:/pugixml-1.10/src/pugixml.hpp>

struct Tile;
struct Pane;
struct Creature;
struct Player;

//Class File Imports
#include "defines.h"

#include "utility.h"

	#include "tile.h"
	#include "tool.h"
#include "item.h"

	#include "entity.h"
//	#include "aicreature.h"
#include "map.h"

	#include "input.h"
#include "settings.h"

	#include "window.h"
	#include "panel.h"
#include "gui.h"

#include "engine.h"

//TODO : NEED TO HAVE A VIRTUAL DESTRUCTOR FOR VIRTUAL CLASSES

//TODO : optimize and test access modifiers
//TODO : fov is computed every update, try to optimize later