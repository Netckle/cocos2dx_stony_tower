#ifndef __STARTWORLD_SCENE_H__
#define __STARTWORLD_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "AudioEngine.h"

using namespace cocos2d;

class StartScene : public Scene
{
public:
    static Scene* createScene();

    virtual bool init();

    CREATE_FUNC(StartScene);

    void startGameCallback(Ref* pSender);
    void exitGameCallback(Ref* pSender);

    void update(float delta);

    void onEnter();

    void onExit();

    bool onTouchBegan(Touch* touch, Event* event);

private:
    Sprite* bgImg;

    int bgmId;
    const float maxVolume = 1.0f;
    float currentVolume;
    float soundFadeTime;
    bool soundFadeIsEnd;

    EventListenerTouchOneByOne* listener;
};
#endif

