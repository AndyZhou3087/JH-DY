#include "HuafeiAwardListLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"
#include "SoundManager.h"

HuafeiAwardListLayer::HuafeiAwardListLayer()
{
}


HuafeiAwardListLayer::~HuafeiAwardListLayer()
{
}


HuafeiAwardListLayer* HuafeiAwardListLayer::create()
{
	HuafeiAwardListLayer *pRet = new HuafeiAwardListLayer();
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

bool HuafeiAwardListLayer::init()
{
	Node* csbnode = CSLoader::createNode("huafeiAwardListLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(HuafeiAwardListLayer::onBack, this));

	resultlbl = (cocos2d::ui::Text*)csbnode->getChildByName("resultlbl");
	resultlbl->setString("");

	cocos2d::ui::Text* desclbl = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desclbl->setString(GlobalData::couponinfo);

	getData();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HuafeiAwardListLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
void HuafeiAwardListLayer::getData()
{
	WaitingProgress* waitbox = WaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

	ServerDataSwap::init(this)->getKajuanAwardList();

}

void HuafeiAwardListLayer::delayShowData(float dt)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	std::string str;
	if (GlobalData::myLastHuafeiRank > 0)
	{
		str = StringUtils::format("恭喜你！打败扫地僧排名第%d名！", GlobalData::myLastHuafeiRank);
	}
	else
	{
		str = "很遗憾！未排名上榜，再接再励！";
		resultlbl->setTextColor(Color4B(27, 141, 0, 255));
	}

	resultlbl->setString(CommonFuncs::gbk2utf(str.c_str()));
}

void HuafeiAwardListLayer::onSuccess()
{
	this->scheduleOnce(schedule_selector(HuafeiAwardListLayer::delayShowData), 0.1f);
}

void HuafeiAwardListLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}