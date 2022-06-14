#pragma once

#include "cocos2d.h"

#include "CustomClasses/EnumCollection/EnumCollection.h"
#include "CustomClasses/Stone/Stone.h"

#include <list>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace cocos2d;
using namespace std;

typedef vector<Stone*> StoneContainer;

struct StonePanel
{
public:
	const int maxCount = 24;
	const int maxHandCount = 6;

	StoneContainer::iterator it;

	StoneContainer allStones;
	StoneContainer handStones;
	StoneContainer selectedStones;

	StonePanel(CharType target, Vec2 pos, Size size);
	~StonePanel();

	void InitStones();

	StoneType GetRandomType();
	StoneTier GetRandomTier();
	int GetRanValueInRange(int front, int rear);

	Stone* GetCurrentStone();
	Stone* PopStone();

	void SelectStone(int index);
	void UnSelectedStone(int index);

	void HideAll();
	void ShowAll();

	void ShowCurrentStone();
	void HideCurrentStone();

	void PushRandomStones(int size); // Only for Enemy

	void ClearSelectedStone() noexcept { selectedStones.clear(); }
	void EndBattle();

	Sprite* GetSprite();

private:
	const float actionTime = 0.5f;

	Sprite* panelSprite;
	Size panelSize;

	int hOffset;
	int vOffset;

	Stone* currentStone;

	CharType targetCharType;
	bool alreadyInit;
};

