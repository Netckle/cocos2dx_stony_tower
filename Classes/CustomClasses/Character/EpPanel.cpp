#include "EpPanel.h"

EpPanel::EpPanel(StonePanel* panel, Character* character) 
{
	stonePanel_target = panel;
	character_target = character;

	predEp = 0;

	bgSprite = Sprite::create("Images/Panel/Panel_EP.png");
	epText = Label::createWithSystemFont("", "fonts/Schwarzwald_Regular.ttf", 25);

	bgSprite->addChild(epText);

	epText->setPosition(Vec2(bgSprite->getContentSize().width / 2, bgSprite->getContentSize().height / 2));
	Update(UpdateType::SELECTION);
	bgSprite->setPosition(Vec2(panel->GetSprite()->getPosition().x, panel->GetSprite()->getPosition().y + 50));
}

void EpPanel::Update(UpdateType type) 
{
	if (stonePanel_target->selectedStones.size() == 0)
	{
		epText->setString(StringUtils::format("%d", character_target->GetCurrentEp()));
		predEp = 0;
		return;
	}
	predEp = 0;

	for (Stone* index : stonePanel_target->selectedStones)
	{
		predEp += index->GetEpUsage();
	}

	if(type == UpdateType::SELECTION) 
		epText->setString(StringUtils::format("%d ( -%d)", character_target->GetCurrentEp(), predEp));
	else if(type == UpdateType::RESULT) 
		epText->setString(StringUtils::format("%d", character_target->GetCurrentEp()));
	return;
}

void EpPanel::HideAll() {
	auto fadeOut = FadeOut::create(0.5f);

	bgSprite->runAction(fadeOut->clone());
	epText->runAction(fadeOut->clone());
}

void EpPanel::ShowAll() {
	auto fadeIn = FadeIn::create(0.5f);

	bgSprite->runAction(fadeIn->clone());
	epText->runAction(fadeIn->clone());
}