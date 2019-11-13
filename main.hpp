#pragma once

//TODO : move defines to settings
//TILE STRENGTH IS FROM 0

#include "defines.h"

//STD
#include <fstream>
#include <memory>
#include <vector>
//#include <iostream>
#include <string>
#include <math.h>
#include <thread>
#include <mutex>
#include <atomic>
//#include <stdexcept>

//Libraries
#include <libtcod.hpp>
#include <SFML/Window.hpp>

struct Pane;
class Entity;

//Class File Imports
#include "utility.h"
#include "tile.h"
#include "tool.h"
#include "entity.h"
#include "map.h"
#include "input.h"
#include "settings.h"
#include "window.h"
#include "panel.h"
#include "gui.h"
#include "engine.h"


//TODO : NEED TO HAVE A VIRTUAL DESTRUCTOR FOR VIRTUAL CLASSES

//TODO : BULETS STILL GET STUCK ON MAP BOUNDS

//TODO : add more defs for colors in window
//TODO : optimize and test access modifiers
//TODO : fov is computed every update, try to optimize later

//!NOTE : create generic items for tiles and then push them