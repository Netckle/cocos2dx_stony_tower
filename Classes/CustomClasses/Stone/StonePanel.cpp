#include "StonePanel.h"

StonePanel::StonePanel(CharType target, Vec2 pos, Size size)
{
	currentStone = nullptr;

	if (target == CharType::PLAYER)	
		panelSprite = Sprite::create("Images/Panel/StonePanel_Craver.png");
	
	else if (target == CharType::ENEMY)	
		panelSprite = Sprite::create("Images/Panel/StonePanel_Guardian.png");
	
	panelSprite->setPosition(pos);
	panelSprite->setContentSize(size);

	this->targetCharType = target;
	this->alreadyInit = false;

	InitStones();
}

StonePanel::~StonePanel()
{

}

void StonePanel::InitStones()
{
	panelSize = panelSprite->getContentSize();

	hOffset = panelSize.width / (maxHandCount + 1);
	vOffset = panelSize.height / 2;

	// Init Stone's deck
	for (int i = 0; i < maxCount; i++)
	{
		allStones.push_back(new Stone
		(
			GetRandomType(), GetRandomTier(), Vec2::ZERO, 50
		));
		allStones[i]->GetSprite()->setVisible(false);
		panelSprite->addChild(allStones[i]->GetSprite());
	}

	selectedStones.clear();
	handStones.clear();

	// Init Stone's on Hand
	for (int i = 0; i < maxHandCount; i++)
	{
		handStones.push_back(allStones[i]);
		handStones[i]->GetSprite()->setVisible(true);
		handStones[i]->GetSprite()->setPosition(Vec2(hOffset * (i + 1), vOffset));
	}
}

void StonePanel::SelectStone(int index)
{
	handStones[index]->Select(true);

	selectedStones.push_back(handStones[index]);
}

void StonePanel::UnSelectedStone(int index)
{
	Stone* target = handStones[index];

	it = find(selectedStones.begin(), selectedStones.end(), target);
	if (it != selectedStones.end())
	{
		target->Select(false);

		selectedStones.erase(it);
	}
}

void StonePanel::HideAll()
{
	auto fadeOut = FadeOut::create(actionTime);

	panelSprite->runAction(fadeOut->clone());

	for (Stone* stone : handStones)	
		stone->Hide();	
}

void StonePanel::ShowAll()
{
	auto fadeIn = FadeIn::create(actionTime);

	panelSprite->runAction(fadeIn->clone());

	selectedStones.clear();
	handStones.clear();

	// Show Stone's on Hand
	for (int i = 0; i < maxHandCount; i++)
	{
		allStones[i]->Show();
		handStones.push_back(allStones[i]);

		handStones[i]->GetSprite()->setPosition(Vec2(hOffset * (i + 1), vOffset)); // hOffset * 1 부터 시작해야 함
	}

	for (Stone* stone : allStones)	
		stone->Select(false);
	

	for (Stone* stone : handStones)	
		stone->GetSprite()->setVisible(true);	
}

Stone* StonePanel::GetCurrentStone()
{
	if (currentStone != nullptr)	
		return currentStone;	
}

Stone* StonePanel::PopStone()
{
	if (selectedStones.empty())	
		return nullptr;	
	else
	{
		it = selectedStones.begin();
		Stone* stone = &(*(selectedStones.front()));

		selectedStones.erase(it);

		return stone;
	}
}

void StonePanel::ShowCurrentStone()
{
	// Just Change Color
	for (Stone* stone : handStones)
	{
		stone->ChangeOnlyColor();
	}

	// Get CurrentStone on Selected Stone Vector
	currentStone = PopStone();

	if (currentStone == nullptr)
	{
		EndBattle();
		return;
	}

	Vec2 targetPos = Vec2(panelSize.width / 2, panelSize.height / 2);

	// Move To Middle before Show Stone
	currentStone->GetSprite()->setPosition(targetPos);
	currentStone->Show();
}

void StonePanel::HideCurrentStone()
{
	if (currentStone != nullptr)
	{
		auto hide = CallFunc::create([=]()->void
			{
				currentStone->Hide();
			});

		auto moveToLast = CallFunc::create([=]()->void
			{
				it = find(allStones.begin(), allStones.end(), currentStone);
				if (it != allStones.end())
				{
					allStones.erase(it);

					allStones.push_back(currentStone);
				}
			});

		auto hideSeq = Sequence::create(hide, DelayTime::create(0.2f), moveToLast, nullptr);

		currentStone->GetSprite()->runAction(hideSeq);
	}
}

void StonePanel::PushRandomStones(int size)
{
	while (selectedStones.size() != size)
	{
		const int index = rand() % maxCount;

		Stone* target = allStones[index];

		it = find(selectedStones.begin(), selectedStones.end(), target);
		if (it != selectedStones.end())
		{
			continue;
		}
		else
		{
			selectedStones.push_back(target);
		}
	}
}

void StonePanel::EndBattle()
{
	alreadyInit = false;
}

Sprite* StonePanel::GetSprite()
{
	return panelSprite ? panelSprite : nullptr;
}

StoneType StonePanel::GetRandomType()
{
	switch (GetRanValueInRange(0, 3))
	{
	case 0: return StoneType::PHYSICAL_ATTACK;
	case 1: return StoneType::MAGIC_ATTACK;
	case 2: return StoneType::GUARD;
	case 3: return StoneType::DODGE;
	}
}

StoneTier StonePanel::GetRandomTier()
{
	switch (GetRanValueInRange(0, 3))
	{
	case 0: return StoneTier::NORMAL;
	case 1: return StoneTier::RARE;
	case 2: return StoneTier::UNIQUE;
	case 3: return StoneTier::EPIC;
	}
}

int StonePanel::GetRanValueInRange(int front, int rear)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dis(front, rear);
	return dis(gen);
}