#pragma once

#include <memory>
#include <vector>
#include <libtcod.hpp>

struct Window;
class Entity;

#include "entity.h"
#include "map.h"
#include "settings.h"
#include "window.h"
#include "gui.h"
#include "engine.h"

//TODO : optimize and test access modifiers
//TODO : Remember to clear vectors in destructors
//TODO : to create and push to vector use vector.push(create ent here)
//TODO : tcod consoles start at 0, 0