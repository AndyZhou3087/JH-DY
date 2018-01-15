#include "FrozenLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "Const.h"
bool FrozenLayer::init()
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("frozenLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Text* content_0 = (cocos2d::ui::Text*)csbnode->getChildByName("content_0");
	cocos2d::ui::Text* qqtitle = (cocos2d::ui::Text*)csbnode->getChildByName("qqtext");
	content_0->setVisible(false);
	qqtitle->setVisible(false);

	cocos2d::ui::Text* qq1 = (cocos2d::ui::Text*)csbnode->getChildByName("qq");
	qq1->addTouchEventListener(CC_CALLBACK_2(FrozenLayer::onQQ, this));
	qq1->setVisible(false);

	cocos2d::ui::Text* qq2 = (cocos2d::ui::Text*)csbnode->getChildByName("qq_1");
	qq2->addTouchEventListener(CC_CALLBACK_2(FrozenLayer::onQQ, this));
	qq2->setVisible(false);


	int qqsize = GlobalData::vec_qq.size();

	if (qqsize > 0)
	{
		content_0->setVisible(true);
		qqtitle->setVisible(true);
		int rqq = GlobalData::createRandomNum(qqsize);
		qq1->setString(GlobalData::vec_qq[rqq]);
		qq1->setVisible(true);

		if (qqsize > 1)
		{
			qq2->setString(GlobalData::vec_qq[1 - rqq]);
			qq2->setVisible(true);
		}
		else
		{
			qq2->setVisible(false);
		}
	}

	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	//点击任何位置移除掉
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

FrozenLayer* FrozenLayer::create()
{
	FrozenLayer *pRet = new FrozenLayer();
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

void FrozenLayer::onQQ(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		cocos2d::ui::Text* qq = (cocos2d::ui::Text*)pSender;
		GlobalData::copyToClipBoard(qq->getString());
	}

}

