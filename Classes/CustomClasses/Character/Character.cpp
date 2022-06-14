#include "CustomClasses/Character/Character.h"
#include "CustomClasses/Scene/LoseScene.h"
#include "CustomClasses/Scene/VictoryScene.h"

Character::Character(CharType type, Vec2 pos, Size size)
{
    //Set Character's Sprite
    sprite = Sprite::create("Images/" + GetSpriteName(type, CharAnim::IDLE_ANIM));

    sprite->setPosition(pos);
    sprite->setContentSize(size);

    this->type = type;

    maxHp = 10; currentHp = maxHp;
    maxEp = 20; currentEp = maxEp;
    maxNp = 10; currentNp = maxNp;
    regenEp = 5;

    this->originPos = pos;

    //Set Character's Sound Effects
    switch (this->type)
    {
        case CharType::PLAYER:
        {
            dir = Direction::RIGHT;
            Sound_PA = "Sounds/Craver_PA.mp3";
            Sound_MA = "Sounds/Craver_MA.mp3";
            break;
        }
        case CharType::ENEMY:
        {
            dir = Direction::LEFT;
            Sound_PA = "Sounds/Guardian_PA.mp3";
            Sound_MA = "Sounds/Guardian_MA.mp3";
            break;
        }
    }

    //Set Common Sound Effects
    {
        Sound_DG    = "Sounds/Dodge.mp3";
        Sound_AD    = "Sounds/Attack_Draw.mp3";
        Sound_Hit   = "Sounds/Hit.mp3";
        Sound_Guard = "Sounds/DamageGuard.mp3";
    }

    stat = new StatPanel(type);
    sprite->addChild(stat->GetBackGround());
    switch (type)
    {
    case CharType::PLAYER:
        stat->GetBackGround()->setPosition(Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2 - 90));
        break;
    case CharType::ENEMY:
        stat->GetBackGround()->setPosition(Vec2(sprite->getContentSize().width / 2 + 150, sprite->getContentSize().height / 2 - 90));
        break;
    }
    
    stat->GetBackGround()->addChild(stat->GetHpGauge());
    stat->GetHpGauge()->setPosition(Vec2(stat->GetBackGround()->getContentSize().width / 2 - 50, stat->GetBackGround()->getContentSize().height / 2));
    stat->GetBackGround()->addChild(stat->GetNpGauge());
    stat->GetNpGauge()->setPosition(Vec2(stat->GetBackGround()->getContentSize().width / 2 + 50, stat->GetBackGround()->getContentSize().height / 2));
   
}

Character::~Character()
{

}

void Character::Hide()
{
    auto fadeOut = FadeOut::create(actionTime);

    sprite->runAction(fadeOut);
}

void Character::Show()
{
    auto fadeIn = FadeIn::create(actionTime);

    sprite->runAction(fadeIn);
}

void Character::Action(CharType targetType, Stone* curStone, DamageValue* damage)
{
	if (type == targetType)  {
        CallFunc* doAction;

		if (damage->NpDamage() > 0)   {
            if (currentHp - damage->HpDamage() > 0)  {
                doAction = CallFunc::create([=]()->void {MatchImgSize(CharAnim::DAMAGE_ANIM); });
            }
            else if (currentHp - damage->HpDamage() <= 0) {
                doAction = CallFunc::create([=]()->void {MatchImgSize(CharAnim::DEAD_ANIM);  });

                switch (type)  {
                case CharType::PLAYER:
                    sprite->setContentSize(Size(300, 300));
                    isLose = true;
                    
                    break;
                case CharType::ENEMY:
                    sprite->setContentSize(Size(600, 300));
                    isVictory = true;
                   
                    break;
                }
            }                

			Vec2 moveDistance = Vec2((int)dir * 100, 0);

			auto moveBack   = MoveTo::create(actionTime, originPos - moveDistance);
			auto moveOrigin = MoveTo::create(actionTime, originPos);

			auto doIdle = CallFunc::create([=]()->void 
                {
                    MatchImgSize(CharAnim::IDLE_ANIM);

                    if (isLose)
                        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, LoseScene::createScene()));
                    else if (isVictory)
                        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, VictoryScene::createScene()));

                });

            auto moveSeq = Sequence::create(
                DelayTime::create(actionTime),
                doAction,
                EaseBackInOut::create(moveBack),
                DelayTime::create(0.2f),
                EaseBackInOut::create(moveOrigin),
                doIdle, nullptr);

			AudioEngine::play2d(Sound_Hit);

			sprite->runAction(moveSeq);
            SufferDamage(damage);
            return;
		}
        SufferDamage(damage);
	}

    Sequence* moveSeq;
    
    auto doAction   = CallFunc::create([=]()->void { MatchImgSize((CharAnim)curStone->GetType()); });
    auto doMove     = CallFunc::create([=]()->void { MatchImgSize(CharAnim::MOVE_ANIM); });
    auto doIdle     = CallFunc::create([=]()->void { MatchImgSize(CharAnim::IDLE_ANIM); });

	Vec2 moveDistance = Vec2((int)dir * 100, 0);

	auto moveFront  = MoveTo::create(actionTime, originPos + moveDistance);
	auto moveBack   = MoveTo::create(actionTime, originPos - moveDistance);
	auto moveOrigin = MoveTo::create(actionTime, originPos);

	switch (curStone->GetType()) 
    {
	case StoneType::PHYSICAL_ATTACK: 
    {
		AudioEngine::play2d(Sound_PA);
		moveSeq = Sequence::create(
            doMove,
            EaseBackInOut::create(moveFront),
            doAction,
            DelayTime::create(0.2f),
            EaseBackInOut::create(moveOrigin),
            doIdle, nullptr);
	}break;
	case StoneType::MAGIC_ATTACK: 
    {
		AudioEngine::play2d(Sound_MA);
		moveSeq = Sequence::create(
            doAction,
            DelayTime::create(actionTime),
            DelayTime::create(0.2f),
            doIdle,
            DelayTime::create(actionTime), nullptr);
	}break;
	case StoneType::GUARD: 
    {
        AudioEngine::play2d(Sound_Guard);
		moveSeq = Sequence::create(
            doAction,
            DelayTime::create(actionTime),
			DelayTime::create(0.2f),
            doIdle,
            DelayTime::create(actionTime), nullptr);
	}break;
	case StoneType::DODGE: 
    {
		AudioEngine::play2d(Sound_DG);
		moveSeq = Sequence::create(
            doAction,
            EaseBackInOut::create(moveBack),
			DelayTime::create(0.2f),
            doMove,
            EaseBackInOut::create(moveOrigin),
            doIdle, nullptr);
	}break;
	}
    sprite->runAction(moveSeq);
}

void Character::SufferDamage(DamageValue* damage){
    damage->NpDamage(ceil(damage->NpDamage()));
    currentHp -= damage->HpDamage(0);
    currentNp -= damage->NpDamage(0);

	stat->SetHpGauge(maxHp, currentHp);
	stat->SetNpGauge(maxNp, currentNp);
}

string Character::GetSpriteName(CharType type, CharAnim anim)
{
    string targetName = "";
    switch (type)
    {
    case CharType::PLAYER:
        targetName = "Craver/Craver_";
        break;
    case CharType::ENEMY:
        targetName = "Guardian/Guardian_";
        break;
    }

    string targetAnim = "";
    switch (anim)
    {
    case PHYSICAL_ATTACK_ANIM: targetAnim = "PA"; break;
    case MAGIC_ATTACK_ANIM: targetAnim = "MA";  break;
    case GUARD_ANIM: targetAnim = "GD"; break;
    case DODGE_ANIM: targetAnim = "DG"; break;
    case DAMAGE_ANIM: targetAnim = "Hit"; break;
    case IDLE_ANIM: targetAnim = "Standing"; break;
    case DEAD_ANIM: targetAnim = "Die"; break;
    case MOVE_ANIM: targetAnim = "Move"; break;
    } 

    return targetName + targetAnim + ".png";
}

void Character::MatchImgSize(CharAnim anim)
{
    sprite->setTexture("Images/" + GetSpriteName(type, anim));
    switch (type)
    {
    case CharType::PLAYER:
        sprite->setContentSize(Size(300, 300));
        break;
    case CharType::ENEMY:
        sprite->setContentSize(Size(600, 300));
        break;
    }
}
