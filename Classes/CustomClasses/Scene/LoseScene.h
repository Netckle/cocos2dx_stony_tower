#pragma once

#include "cocos2d.h"
#include <AudioEngine.h>
#include <string>

#include "BattleScene.h"

USING_NS_CC;
USING_NS_CC_EP;

class LoseScene : Scene
{
public:
	static Scene* createScene();

private:
	virtual bool init();

	CREATE_FUNC(LoseScene);

	Sprite* bgSprite;
	MenuItemImage* btnGoToPreviousBattle;
	Menu* menu;

	int bgmId;

	EventListenerTouchOneByOne* listener;

	void onEnter();
	void onExit();
	bool onTouchBegan(Touch* touch, Event* event);

	void GoToPreviousBattle(Ref* pSender);

	Size winSize;
};

