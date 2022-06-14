#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "AudioEngine.h"

#include "CustomClasses/EnumCollection/EnumCollection.h"

#include "CustomClasses/Character/Character.h"

#include "CustomClasses/StatPanel/StatPanel.h"
#include "CustomClasses/Character/EpPanel.h"
#include "CustomClasses/Character/DamageValue.h"

#include "CustomClasses/Stone/Stone.h"
#include "CustomClasses/Stone/StonePanel.h"

#include "CustomClasses/Scene/DialogScene.h"

#include <random>

USING_NS_CC;
USING_NS_CC_EP;

using namespace std;

class BattleScene : public Scene
{
public:
    void shake();
    static Scene* createScene();

    virtual bool init();    

    CREATE_FUNC(BattleScene);

    StonePanel* playerStone;
    StonePanel* enemyStone;

    Character* player;
    Character* enemy;

    StatPanel* playerStat;
    StatPanel* enemyStat;

    EpPanel* epDisplay;

    Size winSize;

    Sprite* bgImg;
    int bgmId;

    MenuItemImage* btnBattle;
    Menu* menu;
    EventListenerTouchOneByOne* listener;

    void onEnter();
    void onExit();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);

    void update(float f);

    void StartDialog(float f);

    bool canTouch;
    bool canSelect;

    bool isBattle;

    // About Battle

    void        StartBattle(Ref* pSender);
    void        ActivateCurStone(Stone* playerStone, Stone* enemyStone);
    CharType    SetTarget(Stone* playerStone, Stone* enemyStone);
    void        SetDamageValue(Stone* attacker, Stone* receiver, DamageValue* damage);
    Character*  GetCharWithEnum(CharType type);
};
#endif
