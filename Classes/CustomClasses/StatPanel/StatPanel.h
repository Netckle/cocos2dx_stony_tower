#pragma once

#include "CustomClasses/EnumCollection/EnumCollection.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace std;
using namespace cocos2d;
using namespace ui;

class StatPanel {
private:
	Sprite* statBackG;
	LoadingBar* hpGauge;
	LoadingBar* npGauge;

public:
	StatPanel(CharType type);
	void UpdateStat();
	void SetHpGauge(int maxHp, int currentHp);
	void SetNpGauge(int maxNp, int currentNp);
	Sprite* GetBackGround() { return statBackG; }
	LoadingBar* GetHpGauge() { return hpGauge; }
	LoadingBar* GetNpGauge() { return npGauge; }
};