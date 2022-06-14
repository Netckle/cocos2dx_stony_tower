#include "VictoryScene.h"

Scene* VictoryScene::createScene()
{
    return VictoryScene::create();
}

bool VictoryScene::init()
{
    if (!cocos2d::Scene::init())
        return false;

    AudioEngine::stopAll();
    bgmId = AudioEngine::play2d("Sounds/BGM_Result.mp3");

    bgSprite = Sprite::create("Images/Background/BG_Victory.png");
    bgSprite->setPosition(360, 240);
    bgSprite->setContentSize(Size(720, 480));
    this->addChild(bgSprite);

    btnGoToMain = MenuItemImage::create("Images/Button/Btn_Main.png",
        "Images/Button/Btn_PushMain.png",
        "Images/Button/Btn_Main.png",
        CC_CALLBACK_1(VictoryScene::GoToMain, this));
    btnGoToMain->setScale(Director::getInstance()->getWinSize().width / 720);

    menu = Menu::create(btnGoToMain, NULL);
    int yPos = Director::getInstance()->getWinSize().height / 4;
    menu->setPositionY(yPos);
    menu->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(menu);

    this->scheduleOnce(schedule_selector(VictoryScene::ShowDialogScene), 1.5f);
    
    return true;
}

void VictoryScene::onEnter()
{
    Scene::onEnter();

    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(VictoryScene::onTouchBegan, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void VictoryScene::onExit()
{
    _eventDispatcher->removeEventListener(listener);

    Scene::onExit();
}

bool VictoryScene::onTouchBegan(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();

    cocos2d::experimental::AudioEngine::play2d("Sounds/Touch.mp3");

    return true;
}

void VictoryScene::GoToMain(Ref* pSender)
{
    AudioEngine::play2d("Sounds/SceneChange.mp3");
    Director::getInstance()->replaceScene(TransitionFade::create(1, StartScene::createScene()));
}

void VictoryScene::ShowDialogScene(float f)
{
    DialogScene::dialogEvent = DialogEvent::VICTORY_GAME;
    Director::getInstance()->pushScene(DialogScene::createScene());
}
