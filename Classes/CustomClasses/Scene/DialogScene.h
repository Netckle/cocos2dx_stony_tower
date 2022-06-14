#ifndef __DialogScene_SCENE_H__
#define __DialogScene_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "AudioEngine.h"
#include "CustomClasses/Manager/DialogManager.h"

using namespace cocos2d;
using namespace cocos2d::experimental;

class DialogScene : public Scene
{
public:
    static DialogEvent dialogEvent;
	static Scene* createScene();

private:
    virtual bool init(); 

    CREATE_FUNC(DialogScene);

    DialogManager* dialogManager;

    EventListenerTouchOneByOne* listener;
    void onEnter();
    void onExit();
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif

