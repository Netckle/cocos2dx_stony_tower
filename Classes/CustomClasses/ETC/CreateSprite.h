#pragma once

#include "cocos2d.h"
#include <string>

using namespace std;
using namespace cocos2d;

Sprite* CreateSprite(Scene* targetScene, Sprite* sprite, string path,
	Vec2 pos, Size size, bool isVisible);