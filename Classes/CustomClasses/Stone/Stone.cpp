#include "Stone.h"

Stone::Stone(StoneType type, StoneTier tier, Vec2 pos, int size)
{
	this->type = type;
	this->tier = tier;

	InitStoneSprite();
	InitInformSprite();

	sprite->setContentSize(Size(size, size));
	sprite->setPosition(pos);

	SetPowerAndEpUsage();

	this->isSelect = false;

	powerLabel = Label::createWithSystemFont(to_string(powerValue), "", fontSize);
	powerLabel->setColor(Color3B::BLACK);
	powerLabel->setPosition(Vec2(size / 2, size / 2));
	sprite->addChild(powerLabel);

	UpdateState();
}

Stone::~Stone()
{

}

Sprite* Stone::GetSprite()
{
	if (sprite != nullptr)
		return sprite;
	else
		return nullptr;
}

//Return Random Value between front and rear
int Stone::GetRanValueInRange(int front, int rear) {
	//return (rand() % (rear - front + 1)) + front;
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dis(front, rear);
	return dis(gen);
}

void Stone::InitStoneSprite()
{
	string filename = "Stn_";
	//----------------------스톤 타입(물공, 마공, 방어, 회피)
	if (type == StoneType::PHYSICAL_ATTACK)		filename += StringUtils::format("PA_");
	else if (type == StoneType::MAGIC_ATTACK)	filename += StringUtils::format("MA_");
	else if (type == StoneType::GUARD)			filename += StringUtils::format("GD_");
	else if (type == StoneType::DODGE)			filename += StringUtils::format("DG_");
	//----------------------스톤 등급(Normal, Rare, Unique, Epic)
	if (tier == StoneTier::NORMAL)				filename += StringUtils::format("N.png");
	else if (tier == StoneTier::RARE)			filename += StringUtils::format("R.png");
	else if (tier == StoneTier::UNIQUE)			filename += StringUtils::format("U.png");
	else if (tier == StoneTier::EPIC)			filename += StringUtils::format("E.png");
	//------------최종 결과물 예시 : "Stn_PA_N.png"

	//스프라이트 생성 / 세팅
	sprite = Sprite::create("Images/Stone/" + filename);
}

void Stone::SetPowerAndEpUsage() {
	if (type == StoneType::GUARD) {
		switch (tier) {
		case StoneTier::NORMAL: {
			powerValue = GetRanValueInRange(1, 4);
			epUsage = 1;
			break;
		}
		case StoneTier::RARE: {
			powerValue = GetRanValueInRange(2, 6);
			epUsage = 2;
			break;
		}
		case StoneTier::UNIQUE: {
			powerValue = GetRanValueInRange(3, 6);
			epUsage = 3;
			break;
		}
		case StoneTier::EPIC: {
			powerValue = GetRanValueInRange(4, 8);
			epUsage = 4;
			break;
		}
		}
	}
	else if (type == StoneType::DODGE) {
		switch (tier) {
		case StoneTier::NORMAL: {
			powerValue = GetRanValueInRange(1, 6);
			epUsage = 1;
			break;
		}
		case StoneTier::RARE: {
			powerValue = GetRanValueInRange(2, 6);
			epUsage = 2;
			break;
		}
		case StoneTier::UNIQUE: {
			powerValue = GetRanValueInRange(2, 8);
			epUsage = 3;
			break;
		}
		case StoneTier::EPIC: {
			powerValue = GetRanValueInRange(4, 8);
			epUsage = 5;
			break;
		}
		}
	}
	else {
		switch (tier) {
		case StoneTier::NORMAL: {
			powerValue = GetRanValueInRange(1, 5);
			epUsage = 1;
			break;
		}
		case StoneTier::RARE: {
			powerValue = GetRanValueInRange(1, 8);
			epUsage = 2;
			break;
		}
		case StoneTier::UNIQUE: {
			powerValue = GetRanValueInRange(3, 8);
			epUsage = 3;
			break;
		}
		case StoneTier::EPIC: {
			powerValue = GetRanValueInRange(5, 9);
			epUsage = 4;
			break;
		}
		}
	}
}

void Stone::UpdateState()
{
	if (isSelect)
		sprite->setColor(selectedColor);	
	else
		sprite->setColor(normalColor);	
}

void Stone::ChangeOnlyColor()
{
	sprite->setColor(normalColor);
}

void Stone::Select(bool value)
{
	isSelect = value;

	UpdateState();
}

bool Stone::GetSelect()
{
	return isSelect;
}

int Stone::GetPower()
{
	return powerValue;
}

int Stone::GetEpUsage() 
{
	return epUsage;
}

StoneType Stone::GetType()
{
	return type;
}

void Stone::Show()
{
	auto fadeIn = FadeIn::create(actionTime);

	sprite->setVisible(true);

	sprite->runAction(fadeIn->clone());
	powerLabel->runAction(fadeIn->clone());
}

void Stone::Hide()
{
	auto fadeOut = FadeOut::create(actionTime);

	sprite->runAction(fadeOut->clone());
	powerLabel->runAction(fadeOut->clone());
}

void Stone::ShowPower()
{
	auto fadeIn = FadeIn::create(actionTime);

	powerLabel->runAction(fadeIn);
}

void Stone::HidePower()
{
	auto fadeOut = FadeOut::create(actionTime);

	powerLabel->runAction(fadeOut);
}

void Stone::InitInformSprite()
{
	string filename = "";

	switch (type)
	{
	case PHYSICAL_ATTACK:
		filename = "PA";
		break;
	case MAGIC_ATTACK:
		filename = "MA";
		break;
	case GUARD:
		filename = "GD";
		break;
	case DODGE:
		filename = "DG";
		break;
	}

	filename += "UI_";

	switch (tier)
	{
	case NORMAL:
		filename += "N";
		break;
	case RARE:
		filename += "R";
		break;
	case UNIQUE:
		filename += "U";
		break;
	case EPIC:
		filename += "E";
		break;
	}

	filename += ".png";

	informSprite = Sprite::create("Images/Value/" + filename);
	informSprite->setVisible(false);




	sprite->addChild(informSprite);
	informSprite->setPosition(25, 100);
}

void Stone::VisibleInform(bool value)
{
	informSprite->setVisible(value);
	informSprite->setGlobalZOrder(1);
}









