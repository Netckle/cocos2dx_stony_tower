#include "StartScene.h"
#include "SelectScene.h"
#include "SimpleAudioEngine.h"

Scene* StartScene::createScene()
{
	return StartScene::create();
}

bool StartScene::init()
{
    if (!Scene::init())    
        return false;

    soundFadeTime = 5.0f;

    AudioEngine::stopAll();

    bgmId = AudioEngine::play2d("Sounds/BGM_Start.mp3", true, 0.0f);

    bgImg = Sprite::create("Images/Background/BG_Start.png");
    bgImg->setPosition(
        Director::getInstance()->getWinSize().width / 2,
        Director::getInstance()->getWinSize().height / 2);
    bgImg->setContentSize(Size(
        Director::getInstance()->getWinSize().width,
        Director::getInstance()->getWinSize().height));
    this->addChild(bgImg);

    Director::getInstance()->setContentScaleFactor(bgImg->getContentSize().height / Director::getInstance()->getWinSize().height);

    auto item_1 = MenuItemImage::create(
        "Images/Button/Btn_Start.png",
        "Images/Button/Btn_PushStart.png",
        "Images/Button/Btn_Start.png",
        CC_CALLBACK_1(StartScene::startGameCallback, this));
    
    
    auto item_2 = MenuItemImage::create(
        "Images/Button/Btn_Exit.png",
        "Images/Button/Btn_PushExit.png",
        "Images/Button/Btn_Exit.png",
        CC_CALLBACK_1(StartScene::exitGameCallback, this));
    
    
    auto menu = Menu::create(item_1, item_2, NULL);

    menu->alignItemsVertically();
   
    menu->setPositionY(Director::getInstance()->getWinSize().height / 4);
    this->addChild(menu);


    scheduleUpdate();
    
    return true;
}

void StartScene::startGameCallback(Ref* pSender)
{
    AudioEngine::stop(bgmId);
    AudioEngine::play2d("Sounds/SceneChange.mp3");

    Director::getInstance()->replaceScene(TransitionFade::create(1, SelectScene::createScene()));
}

void StartScene::exitGameCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void StartScene::update(float delta)
{
    if (!soundFadeIsEnd)
    {
        if (currentVolume > 1.0f)
        {
            currentVolume = maxVolume;
            soundFadeIsEnd = true;
        }

        currentVolume += delta / soundFadeTime;

        AudioEngine::setVolume(bgmId, currentVolume);
    }
}

// Touch

void StartScene::onEnter()
{
    Scene::onEnter();

    listener = EventListenerTouchOneByOne::create();

    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(StartScene::onTouchBegan, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void StartScene::onExit()
{
    _eventDispatcher->removeEventListener(listener);

    Scene::onExit();
}

bool StartScene::onTouchBegan(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();

    AudioEngine::play2d("Sounds/Touch.mp3");
    
    return true;
}
