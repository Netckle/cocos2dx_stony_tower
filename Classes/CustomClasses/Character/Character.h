#pragma once

#include "cocos2d.h"

#include "CustomClasses/EnumCollection/EnumCollection.h"

#include "CustomClasses/Stone/Stone.h"

#include "CustomClasses/StatPanel/StatPanel.h"

#include "CustomClasses/Character/DamageValue.h"

#include <AudioEngine.h>

using namespace cocos2d;
using namespace cocos2d::experimental;
using namespace std;

class StatPanel;

class Character
{
public:
    Character(CharType type, Vec2 pos, Size size);
    ~Character();

    void Hide();
    void Show();

    void Action(CharType targetType, Stone* curStone, DamageValue* damage);
    void SufferDamage(DamageValue* damage);
    void RegenEp() {currentEp += regenEp;}

    int GetMaxHp()        {return maxHp;}
    int GetCurrentHp()    {return currentHp;}
    int GetMaxNp()        {return maxNp;}
    int GetCurrentNp()    {return currentNp;}
    int GetMaxEp()        {return maxEp;}
    int GetCurrentEp()    {return currentEp;}

    void UseEp(int epUsed) {currentEp -= epUsed;}

    string GetSpriteName(CharType type, CharAnim anim);
    Sprite* GetSprite() noexcept { return sprite ? sprite : nullptr; }
    CharType GetType() noexcept { return type; }

    void MatchImgSize(CharAnim anim);
private:
    const float actionTime = 0.5f;

    bool isVictory = false;
    bool isLose = false;

    Sprite* sprite;

    CharType type;
    Direction dir;

    Vec2 originPos;

    StatPanel* stat;

    int maxHp;
    int currentHp;
    int maxNp;
    int currentNp;
    int maxEp;
    int currentEp;
    int regenEp;

    string Sound_PA; //Sound of Physical Attack
    string Sound_MA; //Sound of Magic Attack

    string Sound_DG; //Sound of Dodge
    string Sound_AD; //Attack Draw
    string Sound_Hit;
    string Sound_Guard;
};
