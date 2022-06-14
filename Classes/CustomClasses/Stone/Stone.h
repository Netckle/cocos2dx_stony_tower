#pragma once

#include "cocos2d.h"

#include "CustomClasses/EnumCollection/EnumCollection.h"

#include <random>

using namespace cocos2d;
using namespace std;

class Stone
{
public:
	Stone(StoneType type, StoneTier tier, Vec2 pos, int size);
	~Stone();

	int GetRanValueInRange(int, int);

	void SetPowerAndEpUsage();
	void InitStoneSprite();

	void InitInformSprite();
	void VisibleInform(bool value);

	void Select(bool value);
	bool GetSelect();

	void UpdateState();
	void ChangeOnlyColor();

	void Show();
	void Hide();

	void ShowPower();
	void HidePower();

	Sprite* GetSprite();
	StoneType GetType();
	int	GetPower();
	int GetEpUsage();

private:
	const Color3B normalColor = Color3B::WHITE;
	const Color3B selectedColor = Color3B::GRAY;

	const int fontSize = 24;
	const float actionTime = 0.2f;

	Sprite* sprite;
	Sprite* informSprite;
	Label* powerLabel;

	StoneType type;
	StoneTier tier;

	int epUsage;
	int powerValue;
	bool isSelect;
};

