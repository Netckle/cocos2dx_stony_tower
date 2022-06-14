#include "CustomClasses/StatPanel/StatPanel.h"

StatPanel::StatPanel(CharType type) {
	//Set StatusPanel Image by type
	switch (type) {
	case CharType::PLAYER: {
		statBackG = Sprite::create("Images/StatPanel/Crv_StatBar.png");
		statBackG->setScale(Director::getInstance()->getWinSize().width / 720);
		hpGauge = LoadingBar::create("Images/StatPanel/hpBar.png");
		npGauge = LoadingBar::create("Images/StatPanel/npBar.png");
		hpGauge->setDirection(LoadingBar::Direction::RIGHT);
		npGauge->setDirection(LoadingBar::Direction::LEFT);

	}break;
	case CharType::ENEMY: {
		statBackG = Sprite::create("Images/StatPanel/Enemy_StatBar.png");
		statBackG->setScale(Director::getInstance()->getWinSize().width / 720);
		hpGauge = LoadingBar::create("Images/StatPanel/hpBar.png");
		npGauge = LoadingBar::create("Images/StatPanel/npBar.png");
		hpGauge->setDirection(LoadingBar::Direction::RIGHT);
		npGauge->setDirection(LoadingBar::Direction::LEFT);


	}break;
	default:	break;
	}
	//target = newTarget;
	UpdateStat();
}
void StatPanel::UpdateStat() {
	hpGauge->setPercent(100);
	npGauge->setPercent(100);
}
void StatPanel::SetHpGauge(int maxHp, int currentHp) {
	hpGauge->setPercent(((float)currentHp / (float)maxHp) * 100);
}
void StatPanel::SetNpGauge(int maxNp, int currentNp) {
	npGauge->setPercent(((float)currentNp / (float)maxNp) * 100);
}