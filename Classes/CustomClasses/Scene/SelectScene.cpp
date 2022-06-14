#include "SelectScene.h"
#include "DialogScene.h"

Scene* SelectScene::createScene()
{
	return SelectScene::create();
}

bool SelectScene::init()
{
	if (!Scene::init())
		return false;

    stageSelectRect = Rect(515, 325, 100, 100);

    drawMakerPos = Vec2(565, 375);

    canTouch = true;

    bgmId = AudioEngine::play2d("Sounds/BGM_Middle.mp3", true, 1.0f);

    bgImg = Sprite::create("Images/Background/BG_Select.png");
    bgImg->setPosition(
        Director::getInstance()->getWinSize().width / 2,
        Director::getInstance()->getWinSize().height / 2);
    bgImg->setContentSize(Size(
        Director::getInstance()->getWinSize().width,
        Director::getInstance()->getWinSize().height));
    this->addChild(bgImg);

    markerImg = Sprite::create("Images/ETC/marker.png");
    markerImg->setPosition(565 + 50, 375 + 50);
    markerImg->setContentSize(Size(100, 100));
    markerImg->setVisible(false);
    this->addChild(markerImg);

    showGuideBtnImg = Sprite::create("Images/Button/Btn_ShowStone.png");
    showGuideBtnImg->setPosition(595, 50);
    showGuideBtnImg->setContentSize(Size(200, 50));
    this->addChild(showGuideBtnImg);

    stoneGuideImg = Sprite::create("Images/ETC/StoneGuide.png");
    stoneGuideImg->setPosition(Director::getInstance()->getWinSize().width / 2,
        Director::getInstance()->getWinSize().height / 2);
    stoneGuideImg->setContentSize(Size(500, 300));
    stoneGuideImg->setVisible(false);
    this->addChild(stoneGuideImg);


    this->scheduleOnce(schedule_selector(SelectScene::StartDialog), 1.5f);

	return true;
}

void SelectScene::onEnter()
{
    Scene::onEnter();

    listener = EventListenerTouchOneByOne::create();

    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(SelectScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(SelectScene::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void SelectScene::onExit()
{
    _eventDispatcher->removeEventListener(listener);

    Scene::onExit();
}

bool SelectScene::onTouchBegan(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();

    if (!canTouch)
        return true;

    AudioEngine::play2d("Sounds/Touch.mp3");

    cocos2d::log("%f, %f", touchPoint.x, touchPoint.y);

    bool isSelected = stageSelectRect.containsPoint(touchPoint);
    if (isSelected)
    {
        canTouch = false;

        markerImg->setPosition(drawMakerPos);
        markerImg->setVisible(true);

        SelectStageCallback(this);
    }
    Rect showButtonRect = Rect(595 - 100, 50 - 25, 200, 50);
    bool showStoneGuide = showButtonRect.containsPoint(touchPoint);
    if (showStoneGuide)
    {
        stoneGuideImg->setVisible(true);
    }

    return true;
}

void SelectScene::onTouchEnded(Touch* touch, Event* event)
{
    stoneGuideImg->setVisible(false);
}

void SelectScene::SelectStageCallback(Ref* pSender)
{
    AudioEngine::play2d("Sounds/OK_Touch.mp3");

    auto scaleUp = ScaleTo::create(0.5f, 1.5f);
    auto scaleOrigin = ScaleTo::create(0.5f, 1.0f);

    auto sceneChange = CallFunc::create([=]()->void {
        AudioEngine::play2d("Sounds/SceneChange.mp3"); 
        Director::getInstance()->replaceScene(TransitionFade::create(1, BattleScene::createScene())); });

    auto scaleSeq = Sequence::create(scaleUp, scaleOrigin, sceneChange,NULL);

    markerImg->runAction(scaleSeq);
}

void SelectScene::StartDialog(float f)
{
    DialogScene::dialogEvent = DialogEvent::SELECT_STAGE;
    Director::getInstance()->pushScene(DialogScene::createScene());
}
