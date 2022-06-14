#pragma once

#include "cocos2d.h"
#include <AudioEngine.h>
#include <string>

#include "StartScene.h"
#include "DialogScene.h"

USING_NS_CC;
USING_NS_CC_EP;

class VictoryScene : Scene
{
public:
	static Scene* createScene();

private:
	virtual bool init();

	CREATE_FUNC(VictoryScene);

	Sprite* bgSprite;
	MenuItemImage* btnGoToMain;
	Menu* menu;

	int bgmId;

	EventListenerTouchOneByOne* listener;

	void onEnter();
	void onExit();
	bool onTouchBegan(Touch* touch, Event* event);

	void GoToMain(Ref* pSender);
	void ShowDialogScene(float f);
};

