#include "DialogManager.h"

string DialogManager::GetFileNameByEventType(DialogEvent type)
{
	string path = "";

	switch (type)
	{
	case DialogEvent::SELECT_STAGE:
		enemyImg->setVisible(false);
		path = "enter_select.txt";
		break;
	case DialogEvent::ENTER_BATTLE_01:
		enemyImg->setVisible(true);
		bgImg->setTexture("Images/Background/BG_Battle.png");
		bgImg->setContentSize(Size(720, 480));
		path = "enter_battle_01.txt";
		break;
	case DialogEvent::ENTER_BATTLE_02:
		enemyImg->setVisible(true);
		path = "enter_battle_02.txt";
		break;
	case DialogEvent::VICTORY_GAME:
		enemyImg->setVisible(false);
		bgImg->setTexture("Images/Background/BG_Victory.png");
		bgImg->setContentSize(Size(720, 480));
		path = "victory_game.txt";
		break;
	}

	return path;
}

DialogManager::DialogManager(DialogEvent type, Scene* targetScene)
{
	bgImg = CreateSprite(targetScene, bgImg, "Images/Background/BG_Select.png",
		Vec2(360, 240), Size(720, 480), true);	

	playerImg = CreateSprite(targetScene, playerImg, "Images/Craver/Craver_Standing.png",
		Vec2(-300, 240), Size(600, 600), true);

	enemyImg = CreateSprite(targetScene, enemyImg, "Images/Guardian/Guardian_Standing.png",
		Vec2(720 + 300, 240), Size(1100, 600), true);
	enemyImg->setAnchorPoint(Vec2(0.75f, 0.5f));

	dialogPanelImg = CreateSprite(targetScene, dialogPanelImg, "Images/Panel/StonePanel_Craver.png",
		Vec2(360, -480 / 8), Size(720, 480 / 4), true);

	dialogLabel = Label::createWithTTF("", "fonts/NEXONLv1GothicBold.ttf", 20);
	dialogLabel->setColor(Color3B::BLACK);
	dialogLabel->setPosition(dialogPanelImg->getContentSize().width / 2, dialogPanelImg->getContentSize().height / 2);
	dialogPanelImg->addChild(dialogLabel);

	isTalking = true;

	string fileName = GetFileNameByEventType(type);

	string data = FileUtils::getInstance()->getStringFromFile("Dialog/" + fileName);

	DialogContainer dialogContainer = LoadDialogDataFromTextFile(data);

	for (int i = 0; i < dialogContainer.size(); i++)
	{
		dialogQ.push(dialogContainer[i]);
	}

	ShowDialogImage();

	ShowNextDialog();
}

void DialogManager::ShowNextDialog()
{
	{
		if (dialogQ.empty())
		{
			EndDialog();
			return;
		}

		currentDialog = dialogQ.front();
		dialogQ.pop();

		HightlightDialogTarget(currentDialog.name);

		dialogLabel->setString(currentDialog.sentence);
	}
}

void DialogManager::EndDialog()
{
	{
		dialogLabel->setString("");

		HideDialogImage();
	}
}

void DialogManager::HightlightDialogTarget(string targetName)
{
	if (targetName == "Player")
	{
		playerImg->setColor(Color3B::WHITE);
		enemyImg->setColor(Color3B::GRAY);
	}
	else if (targetName == "Enemy")
	{
		playerImg->setColor(Color3B::GRAY);
		enemyImg->setColor(Color3B::WHITE);
	}
}

void DialogManager::ShowDialogImage()
{
	auto panelUp = MoveBy::create(1.0f, Vec2(0, 480 / 4));
	auto easePanelUp = EaseBackInOut::create(panelUp);

	auto playerRight = MoveBy::create(1.0f, Vec2(500, 0));
	auto easePlayerRight = EaseBackInOut::create(playerRight);

	auto enemyLeft = MoveBy::create(1.0f, Vec2(-500, 0));
	auto easeEnemyLeft = EaseBackInOut::create(enemyLeft);

	dialogPanelImg->runAction(easePanelUp);
	playerImg->runAction(easePlayerRight);
	enemyImg->runAction(easeEnemyLeft);
}

void DialogManager::HideDialogImage()
{
	auto panelDown = MoveBy::create(1.0f, Vec2(0, -480 / 4));
	auto easePanelDown = EaseBackInOut::create(panelDown);

	auto dialogEnd = CallFunc::create([=]()->void {
		isTalking = false;
		Director::getInstance()->popScene();

		});

	auto seq = Sequence::create(easePanelDown, dialogEnd, nullptr);

	auto playerLeft = MoveBy::create(1.0f, Vec2(-500, 0));
	auto easePlayerLeft = EaseBackInOut::create(playerLeft);

	auto enemyRight = MoveBy::create(1.0f, Vec2(500, 0));
	auto easeEnemyRight = EaseBackInOut::create(enemyRight);

	dialogPanelImg->runAction(seq);
	playerImg->runAction(easePlayerLeft);
	enemyImg->runAction(easeEnemyRight);
}

DialogContainer DialogManager::LoadDialogDataFromTextFile(string input)
{
	string str = input;
	istringstream ss(str);
	string strBuffer;

	vector<string> dividedText;
	dividedText.clear();

	while (getline(ss, strBuffer, ','))
	{
		dividedText.push_back(strBuffer);
	}

	DialogContainer output;

	for (int i = 0; i < dividedText.size(); i++)
	{
		string str = dividedText[i];
		istringstream ss(str);
		string strBuffer;

		vector<string> dividedDialogData;
		dividedDialogData.clear();

		while (getline(ss, strBuffer, ':'))
		{
			dividedDialogData.push_back(strBuffer);
		}

		Dialog dialog;
		dialog.name = dividedDialogData[0];
		dialog.sentence = dividedDialogData[1];

		output.push_back(dialog);
	}

	return output;
}
