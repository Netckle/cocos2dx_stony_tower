#include "DialogScene.h"

DialogEvent DialogScene::dialogEvent = DialogEvent::NONE;

Scene* DialogScene::createScene()
{
    return DialogScene::create();
}

bool DialogScene::init()
{
    if (!Scene::init())    
        return false;

    dialogManager = new DialogManager(dialogEvent, this);

    return true;
}

void DialogScene::onEnter()
{
    Scene::onEnter();

    listener = EventListenerTouchOneByOne::create();

    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(DialogScene::onTouchBegan, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void DialogScene::onExit()
{
    _eventDispatcher->removeEventListener(listener);

    Scene::onExit();
}

bool DialogScene::onTouchBegan(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();

    AudioEngine::play2d("Sounds/Touch.mp3");

    bool bTouch = dialogManager->dialogPanelImg->getBoundingBox().containsPoint(touchPoint);
    if (bTouch)
    {
        dialogManager->ShowNextDialog();
    }

    return true;
}

