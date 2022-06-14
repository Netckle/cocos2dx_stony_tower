#ifndef __SELECTWORLD_SCENE_H__
#define __SELECTWORLD_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "CustomClasses/Scene/BattleScene.h"
#include "AudioEngine.h"

using namespace cocos2d;

class SelectScene : public Scene
{
public:
    static Scene* createScene();
private:
    virtual bool init();

    CREATE_FUNC(SelectScene);

    void onEnter();
    void onExit();
    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);

    void SelectStageCallback(Ref* pSender);
    void StartDialog(float f);

    EventListenerTouchOneByOne* listener;

    Sprite* bgImg;
    Sprite* markerImg;
    Sprite* stoneGuideImg;
    Sprite* showGuideBtnImg;
    int bgmId;

    Rect stageSelectRect;
    Vec2 drawMakerPos;
    bool canTouch;
};
#endif

