#include "CustomClasses/ETC/CreateSprite.h"

Sprite* CreateSprite(Scene* targetScene, Sprite* sprite, string path,
	Vec2 pos, Size size, bool isVisible)
{
	sprite = Sprite::create(path);
	sprite->setPosition(pos);
	sprite->setContentSize(size);

	sprite->setVisible(isVisible);

	targetScene->addChild(sprite);

	return sprite;
}
