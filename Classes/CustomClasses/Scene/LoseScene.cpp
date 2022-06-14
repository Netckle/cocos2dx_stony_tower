#include "LoseScene.h"

Scene* LoseScene::createScene()
{
    return LoseScene::create();
}

bool LoseScene::init()
{
    if (!Scene::init())
        return false;

    AudioEngine::stopAll();
    bgmId = AudioEngine::play2d("Sounds/BGM_Start.mp3");

    winSize = Director::getInstance()->getVisibleSize();

    bgSprite = Sprite::create("Images/Background/BG_Lose.png");
    bgSprite->setPosition(winSize.width / 2, winSize.height / 2);
    bgSprite->setContentSize(winSize);
    this->addChild(bgSprite);

    btnGoToPreviousBattle = MenuItemImage::create("Images/Button/Btn_Next.png",
        "Images/Button/Btn_PushNext.png",
        "Images/Button/Btn_Next.png",
        CC_CALLBACK_1(LoseScene::GoToPreviousBattle, this));
    btnGoToPreviousBattle->setScale(Director::getInstance()->getWinSize().width / 720);

    menu = Menu::create(btnGoToPreviousBattle, NULL);
    int yPos = winSize.height / 4;
    menu->setPositionY(yPos);
    menu->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(menu);
    
    return true;
}

void LoseScene::onEnter()
{
    Scene::onEnter();

    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(LoseScene::onTouchBegan, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void LoseScene::onExit()
{
    _eventDispatcher->removeEventListener(listener);

    Scene::onExit();
}

bool LoseScene::onTouchBegan(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();

    cocos2d::experimental::AudioEngine::play2d("Sounds/Touch.mp3");

    return true;
}

void LoseScene::GoToPreviousBattle(Ref* pSender)
{
    AudioEngine::play2d("Sounds/SceneChange.mp3");
    Director::getInstance()->replaceScene(TransitionFade::create(1, BattleScene::createScene()));
}
