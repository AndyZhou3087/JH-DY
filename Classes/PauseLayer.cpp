#include "PauseLayer.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "StartScene.h"
#include "CommonFuncs.h"
#include "MapLayer.h"
#include "GameScene.h"
bool PauseLayer::init()
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("pauseLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* continuebtn = (cocos2d::ui::Button*)csbnode->getChildByName("continuebtn");
	continuebtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::onContinue, this));

	cocos2d::ui::Button* gohomebtn = (cocos2d::ui::Button*)csbnode->getChildByName("gohomebtn");
	gohomebtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::onGoHome, this));


	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	//点击任何位置移除掉
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		removSelf();
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

PauseLayer* PauseLayer::create()
{
	PauseLayer *pRet = new PauseLayer();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

void PauseLayer::onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void PauseLayer::onGoHome(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ServerDataSwap::init()->postOneData(GlobalData::getUId());
		removSelf();
		Director::getInstance()->replaceScene(StartScene::createScene());
	}
}

void PauseLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
	GlobalData::g_gameStatus = GAMESTART;
	if (g_maplayer != NULL && g_hero->getIsOut())
		g_maplayer->heroResumeMoving();
}

