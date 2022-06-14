#pragma once
#include "cocos2d.h"

#include "CustomClasses/Stone/StonePanel.h"
#include "CustomClasses/Character/Character.h"

using namespace std;
using namespace cocos2d;

class EpPanel 
{
public:
	EpPanel(StonePanel* panel, Character* character);
	void Update(UpdateType type);

	void HideAll();
	void ShowAll();

	Sprite* GetSprite() noexcept { return bgSprite; };
	int GetPredEP() noexcept { return predEp; }

	int predEp = 0;
private:
	Sprite* bgSprite;
	Label* epText;
	StonePanel* stonePanel_target;
	Character* character_target;
};