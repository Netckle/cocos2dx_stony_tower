#pragma once

#include "cocos2d.h"
#include <queue>
#include <vector>
#include <string>
#include <sstream>

#include "CustomClasses/ETC/CreateSprite.h"

using namespace std;
using namespace cocos2d;

enum class DialogEvent
{
	NONE, SELECT_STAGE, ENTER_BATTLE_01, ENTER_BATTLE_02, VICTORY_GAME
};

struct Dialog
{
	string name;
	string sentence;
};

typedef vector<Dialog> DialogContainer;

struct DialogManager
{
	// About Sprite, Label
	Sprite* bgImg;
	Sprite* playerImg;
	Sprite* enemyImg;
	Sprite* dialogPanelImg;
	Label* dialogLabel;

	queue<Dialog> dialogQ;

	Dialog currentDialog;

	bool isTalking = false;

	string GetFileNameByEventType(DialogEvent type);

	DialogManager(DialogEvent type, Scene* targetScene);

	void ShowNextDialog();

	void EndDialog();


	void HightlightDialogTarget(string targetName);

	void ShowDialogImage();

	void HideDialogImage();

	// About String Divide - https://chbuljumeok1997.tistory.com/42
	DialogContainer LoadDialogDataFromTextFile(string input);
};

