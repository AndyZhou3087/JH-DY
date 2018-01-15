#include "HSLJRewardLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "SoundManager.h"
#include "GameScene.h"
#include "WaitingProgress.h"
#include "HintBox.h"

HSLJRewardLayer::HSLJRewardLayer()
{
}


HSLJRewardLayer::~HSLJRewardLayer()
{
}


HSLJRewardLayer* HSLJRewardLayer::create(int goldcount)
{
	HSLJRewardLayer *pRet = new HSLJRewardLayer();
	if (pRet && pRet->init(goldcount))
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

bool HSLJRewardLayer::init(int goldcount)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("hsljRewardLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *okbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(HSLJRewardLayer::onGet, this));

	cocos2d::ui::Text* namelbl = (cocos2d::ui::Text*)csbnode->getChildByName("namelbl");
	std::string str = StringUtils::format("金元宝x%d", goldcount);
	namelbl->setString(CommonFuncs::gbk2utf(str.c_str()));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	m_goldcount = goldcount;
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HSLJRewardLayer::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		WaitingProgress* waitbox = WaitingProgress::create("加载中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->getHSLJRewardData();
	}
}

void HSLJRewardLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() + m_goldcount);
	this->removeFromParentAndCleanup(true);
}

void HSLJRewardLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	this->addChild(HintBox::create(CommonFuncs::gbk2utf("领取失败，请检查网络设置或者稍后重试！")));
}