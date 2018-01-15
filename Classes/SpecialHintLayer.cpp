#include "SpecialHintLayer.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "HeroProperNode.h"
#include "GameDataSave.h"

bool SpecialHintLayer::init()
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	Node* csbnode = CSLoader::createNode("sepcialLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(SpecialHintLayer::onOk, this));

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(SpecialHintLayer::onCancel, this));

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

SpecialHintLayer* SpecialHintLayer::create()
{
	SpecialHintLayer *pRet = new SpecialHintLayer();
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

void SpecialHintLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (g_hero->getHeadID() != 4)
		{
			g_hero->setSex(S_NONE);
			GameDataSave::getInstance()->setHeroSex(S_NONE);
		}

		HeroProperNode* heroProperNode = (HeroProperNode*)g_gameLayer->getChildByName("HeroStateUILayer")->getChildByName("csbnode")->getChildByName("HeroProperNode");
		heroProperNode->selectCarryData();
		removSelf();
	}
}

void SpecialHintLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

void SpecialHintLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

