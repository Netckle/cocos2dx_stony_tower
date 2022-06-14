#include "BattleScene.h"

float noise(int x, int y) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

void BattleScene::shake()
{
    float interval = 0.f;
    float duration = 0.5f;
    float speed = 2.0f;
    float magnitude = 1.0f;

    static float elapsed = 0.f;

    this->schedule([=](float dt) {
        float randomStart = random(-1000.0f, 1000.0f);
            elapsed += dt;

        float percentComplete = elapsed / duration;

        // We want to reduce the shake from full power to 0 starting half way through
        float damper = 1.0f - clampf(2.0f * percentComplete - 1.0f, 0.0f, 1.0f);

        // Calculate the noise parameter starting randomly and going as fast as speed allows
        float alpha = randomStart + speed * percentComplete;

        // map noise to [-1, 1]
        float x = noise(alpha, 0.0f) * 2.0f - 1.0f;
        float y = noise(0.0f, alpha) * 2.0f - 1.0f;

        x *= magnitude * damper;
        y *= magnitude * damper;
        this->setPosition(x, y);

        if (elapsed >= duration)
        {
            elapsed = 0;
            this->unschedule("Shake");
            this->setPosition(Vec2::ZERO);
        }

        }, interval, CC_REPEAT_FOREVER, 0.f, "Shake");
}

Scene* BattleScene::createScene()
{
    return BattleScene::create();
}

bool BattleScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    srand((unsigned int)time(NULL));

    canTouch  = false;
    canSelect = false;
    isBattle  = false;

    winSize = Director::getInstance()->getVisibleSize();

    bgImg = Sprite::create("Images/Background/BG_Battle.png");
    bgImg->setPosition(winSize.width / 2, winSize.height / 2);
    bgImg->setContentSize(winSize);
    this->addChild(bgImg);

    AudioEngine::stopAll();
    bgmId = AudioEngine::play2d("Sounds/BGM_Battle.mp3", true);

    playerStone = new StonePanel(
        CharType::PLAYER,
        Vec2(winSize.width / 2, winSize.height / 8),
        Size(winSize.width, winSize.height / 4)
    );
    this->addChild(playerStone->GetSprite());

    enemyStone = new StonePanel(
        CharType::ENEMY,
        Vec2(winSize.width / 2, winSize.height - winSize.height / 8),
        Size(winSize.width, winSize.height / 4)
    );
    this->addChild(enemyStone->GetSprite());

    player = new Character(
        CharType::PLAYER,
        Vec2(winSize.width / 2 - winSize.width / 4, winSize.height / 2),
        Size(300, 300)
    );
    this->addChild(player->GetSprite());

    enemy = new Character(
        CharType::ENEMY,
        Vec2(winSize.width / 2 + winSize.width / 4, winSize.height / 2),
        Size(600, 300)
    );
    enemy->GetSprite()->setAnchorPoint(Vec2(0.75f, 0.5f));
    this->addChild(enemy->GetSprite());


    epDisplay = new EpPanel(playerStone, player);
    this->addChild(epDisplay->GetSprite());

    // ep 창 설정
    btnBattle = MenuItemImage::create(
        "Images/Button/Btn_Battle_Normal.png",
        "Images/Button/Btn_Battle_Push.png",
        "Images/Button/Btn_Battle_Normal.png",
        CC_CALLBACK_1(BattleScene::StartBattle, this));

    menu = Menu::create(btnBattle, nullptr);
    menu->alignItemsHorizontally();
    menu->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(menu);

    scheduleUpdate();
    this->scheduleOnce(schedule_selector(BattleScene::StartDialog), 1.5f);

    auto up = MoveBy::create(1.0f, Vec2(0, 5));
    auto down = up->reverse();

    auto seq = Sequence::create(up, down, nullptr);
    auto repeat = RepeatForever::create(seq);
    player->GetSprite()->runAction(repeat->clone());
    enemy->GetSprite()->runAction(repeat->clone());

    return true;
}

void BattleScene::onEnter()
{
    Scene::onEnter();

    listener = EventListenerTouchOneByOne::create();

    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(BattleScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(BattleScene::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void BattleScene::onExit()
{
    _eventDispatcher->removeEventListener(listener);

    Scene::onExit();
}

bool BattleScene::onTouchBegan(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();

    if (!canTouch)
        return true;

    AudioEngine::play2d("Sounds/Touch.mp3");

    if (!canSelect)
        return true;

    for (int i = 0; i < playerStone->maxHandCount; i++)
    {
        bool bTouch = playerStone->handStones[i]->GetSprite()->getBoundingBox().containsPoint(touchPoint);
        if (bTouch)
        {
            if (playerStone->handStones[i]->GetSelect()){
                playerStone->UnSelectedStone(i);
            }
            else if (playerStone->handStones[i]->GetSelect() == false && canTouch){
                if (epDisplay->GetPredEP() + playerStone->handStones[i]->GetEpUsage() > player->GetCurrentEp()) return true;
                playerStone->SelectStone(i);
            }
            playerStone->handStones[i]->VisibleInform(true);
            epDisplay->Update(UpdateType::SELECTION);
        }
    }
    return true;
}

void BattleScene::onTouchEnded(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();

    for (int i = 0; i < playerStone->maxHandCount; i++) 
    {
        playerStone->handStones[i]->VisibleInform(false); 
    }
}

void BattleScene::update(float f)
{
    if (isBattle)
    {
        menu->setVisible(false);
        return;
    }

    playerStone->selectedStones.size() >= 4 ? 
        canSelect = false : canSelect = true;    

    playerStone->selectedStones.size() > 0 ?
        menu->setVisible(true) : menu->setVisible(false);    
}

void BattleScene::StartDialog(float f)
{
    canTouch  = true;
    canSelect = true;

    DialogScene::dialogEvent = DialogEvent::ENTER_BATTLE_01;
    Director::getInstance()->pushScene(DialogScene::createScene());
}

void BattleScene::StartBattle(Ref* pSender)
{
    AudioEngine::play2d("Sounds/OK_Touch.mp3");

    auto hideAll            = CallFunc::create([=]()->void
        {
            canSelect   = false;
            isBattle    = true;

            playerStone ->HideAll();
            enemyStone  ->HideAll();
            epDisplay   ->HideAll();
        });
    auto showCurStone       = CallFunc::create([=]()->void
        {
            // Push Random Stone for Enemy
            enemyStone  ->PushRandomStones(playerStone->selectedStones.size());

            playerStone ->ShowCurrentStone();
            enemyStone  ->ShowCurrentStone();


        });
    auto compareCurStone    = CallFunc::create([=]()->void
        {
            ActivateCurStone(playerStone->GetCurrentStone(), enemyStone->GetCurrentStone());
            shake();

        });
    auto hideCurStone       = CallFunc::create([=]()->void
        {
            playerStone ->HideCurrentStone();
            enemyStone  ->HideCurrentStone();
        });
    auto showAll            = CallFunc::create([=]()->void
        {
            playerStone ->ShowAll();
            enemyStone  ->ShowAll();
            epDisplay   ->ShowAll();
        });
    auto endBattle          = CallFunc::create([=]()->void
        {
            playerStone ->EndBattle();
            enemyStone  ->EndBattle();

            canSelect = true;
            isBattle = false;
        });
    auto clearSelected      = CallFunc::create([=]()->void
        {
            playerStone ->ClearSelectedStone();
            enemyStone  ->ClearSelectedStone();
        });
    auto updateEpPanel      = CallFunc::create([=]()->void
        {
            epDisplay   ->Update(UpdateType::SELECTION);
        });
    auto regenEp            = CallFunc::create([=]()->void 
        {
            player      ->RegenEp();
            enemy       ->RegenEp();
        });

#pragma region Final Sequence

    Sequence* battleSeq;
    switch (playerStone->selectedStones.size())
    {
    case 1:
        battleSeq = Sequence::create(
            updateEpPanel,
            hideAll,            DelayTime::create(0.5f),
            showCurStone,       DelayTime::create(1.0f),
            compareCurStone,    DelayTime::create(2.0f),
            hideCurStone,       DelayTime::create(1.0f),
            showAll,            DelayTime::create(0.5f),
            endBattle,          DelayTime::create(0.5f),
            regenEp, 
            updateEpPanel,
            nullptr);
        break;
    case 2:
        battleSeq = Sequence::create(
            updateEpPanel,
            hideAll,            DelayTime::create(0.5f),

            showCurStone,       DelayTime::create(1.0f),
            compareCurStone,    DelayTime::create(2.0f),
            hideCurStone,       DelayTime::create(1.0f),

            showCurStone,       DelayTime::create(1.0f),
            compareCurStone,    DelayTime::create(2.0f),
            hideCurStone,       DelayTime::create(1.0f),

            showAll,            DelayTime::create(0.5f),
            endBattle,          DelayTime::create(0.5f),
            regenEp,      
            updateEpPanel,
            nullptr);
        break;
    case 3:
        battleSeq = Sequence::create(
            updateEpPanel,
            hideAll,            DelayTime::create(0.5f),

            showCurStone,       DelayTime::create(1.0f),
            compareCurStone,    DelayTime::create(2.0f),
            hideCurStone,       DelayTime::create(1.0f),

            showCurStone,       DelayTime::create(1.0f),
            compareCurStone,    DelayTime::create(2.0f),
            hideCurStone,       DelayTime::create(1.0f),

            showCurStone,       DelayTime::create(1.0f),
            compareCurStone,    DelayTime::create(2.0f),
            hideCurStone,       DelayTime::create(1.0f),

            showAll,            DelayTime::create(0.5f),
            endBattle,          DelayTime::create(0.5f),
            regenEp, 
            updateEpPanel,
            nullptr);
        break;
    case 4:
        battleSeq = Sequence::create(
            updateEpPanel,
            hideAll,            DelayTime::create(0.5f),

            showCurStone,       DelayTime::create(1.0f),
            compareCurStone,    DelayTime::create(2.0f),
            hideCurStone,       DelayTime::create(1.0f),

            showCurStone,       DelayTime::create(1.0f),
            compareCurStone,    DelayTime::create(2.0f),
            hideCurStone,       DelayTime::create(1.0f),

            showCurStone,       DelayTime::create(1.0f),
            compareCurStone,    DelayTime::create(2.0f),
            hideCurStone,       DelayTime::create(1.0f),

            showCurStone,       DelayTime::create(1.0f),
            compareCurStone,    DelayTime::create(2.0f),
            hideCurStone,       DelayTime::create(1.0f),

            showAll,            DelayTime::create(0.5f),
            endBattle,          DelayTime::create(0.5f),
            regenEp, 
            updateEpPanel,
            nullptr);
        break;
    }
#pragma endregion

    player      ->UseEp(epDisplay->GetPredEP());
    this        ->runAction(battleSeq);
    epDisplay   ->Update(UpdateType::RESULT);
}

void BattleScene::ActivateCurStone(Stone* playerStone, Stone* enemyStone) 
{
    CharType target = SetTarget(playerStone, enemyStone);
    DamageValue* damage = new DamageValue();
    if (enemyStone->GetPower() > playerStone->GetPower()) SetDamageValue(enemyStone, playerStone, damage);
    else if (enemyStone->GetPower() < playerStone->GetPower()) SetDamageValue(playerStone, enemyStone, damage);

    enemy->Action(target, enemyStone, damage); 
    player->Action(target, playerStone, damage);
}

void BattleScene::SetDamageValue(Stone* attacker, Stone* receiver, DamageValue* damage)
{
	if (attacker->GetType() == StoneType::PHYSICAL_ATTACK || attacker->GetType() == StoneType::MAGIC_ATTACK) {
		if (receiver->GetType() == StoneType::PHYSICAL_ATTACK || receiver->GetType() == StoneType::MAGIC_ATTACK) {
			damage->HpDamage(attacker->GetPower());
			damage->NpDamage(damage->HpDamage() / 2);
		}
		else if (receiver->GetType() == StoneType::GUARD) {
			damage->HpDamage(attacker->GetPower() - receiver->GetPower());
			damage->NpDamage(damage->HpDamage() / 2.0);
		}
		else if (receiver->GetType() == StoneType::DODGE) {
			damage->HpDamage(attacker->GetPower());
			damage->NpDamage(damage->HpDamage() / 2.0);
		}
	}
	else if (attacker->GetType() == StoneType::GUARD) {
		if (receiver->GetType() == StoneType::PHYSICAL_ATTACK || receiver->GetType() == StoneType::MAGIC_ATTACK) {
			damage->NpDamage(receiver->GetPower() / 2.0);
		}
		else if (receiver->GetType() == StoneType::GUARD) {
			damage->NpDamage((attacker->GetPower() - receiver->GetPower()) / 2.0);
		}
		else if (receiver->GetType() == StoneType::DODGE) {
			damage->NpDamage(attacker->GetPower());
		}
	}
	else if (attacker->GetType() == StoneType::DODGE) {
		if (receiver->GetType() == StoneType::PHYSICAL_ATTACK || receiver->GetType() == StoneType::MAGIC_ATTACK) {
			damage->NpDamage((attacker->GetPower() * -1));
		}
		else if (receiver->GetType() == StoneType::GUARD) {
			damage->NpDamage((attacker->GetPower() * -1));
		}
		else {}
	}
}

CharType BattleScene::SetTarget(Stone* playerStone, Stone* enemyStone) 
{
    if (playerStone->GetPower() - enemyStone->GetPower() > 0) {
        if (playerStone->GetType() == StoneType::DODGE) return CharType::PLAYER;
        else return CharType::ENEMY;
    }
    else if (playerStone->GetPower() - enemyStone->GetPower() < 0) {
        if (enemyStone->GetType() == StoneType::DODGE) return CharType::ENEMY;
        else    return CharType::PLAYER;
    }
    else return CharType::NONE;
}

Character* BattleScene::GetCharWithEnum(CharType type) 
{
    if (type == CharType::PLAYER) return player;
    else if (type == CharType::ENEMY) return enemy;
    else return nullptr;
}