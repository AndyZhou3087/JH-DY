#include "AddFightCountLayer.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"
#include "SoundManager.h"
#include "MD5.h"
#include "HSLJMainLayer.h"
#include "GameScene.h"

AddFightCountLayer::AddFightCountLayer()
{
	buycount = 0;
	m_matchtype = 0;
}


AddFightCountLayer::~AddFightCountLayer()
{

}


AddFightCountLayer* AddFightCountLayer::create(int matchtype)
{
	AddFightCountLayer *pRet = new AddFightCountLayer();
	if (pRet && pRet->init(matchtype))
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

bool AddFightCountLayer::init(int matchtype)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("addChallengeCountLayer.csb");
	this->addChild(m_csbnode);

	cocos2d::ui::Widget *closebtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(AddFightCountLayer::onBack, this));


	cocos2d::ui::Widget *buy1btn = (cocos2d::ui::Button*)m_csbnode->getChildByName("buy1btn");
	buy1btn->addTouchEventListener(CC_CALLBACK_2(AddFightCountLayer::onAddCount, this));
	buy1btn->setTag(1);

	cocos2d::ui::Widget *buy5btn = (cocos2d::ui::Button*)m_csbnode->getChildByName("buy5btn");
	buy5btn->addTouchEventListener(CC_CALLBACK_2(AddFightCountLayer::onAddCount, this));
	buy5btn->setTag(5);

	m_matchtype = matchtype;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void AddFightCountLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void AddFightCountLayer::onAddCount(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* btnnode = (Node*)pSender;
		buycount = btnnode->getTag();
		int needgold = 20;

		if (buycount == 5)
			needgold = 80;
		int mygold = GlobalData::getMyGoldCount();
		if (mygold >= needgold)
		{
			if (GlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
			{
				GlobalData::dataIsModified = true;
				GlobalData::setMyGoldCount(0);
				HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
				this->addChild(hint);
				return;
			}
		}
		else
		{
			HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("金元宝不足！"));
			this->addChild(hbox);
			return;
		}

		WaitingProgress* waitbox = WaitingProgress::create("处理中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

		ServerDataSwap::init(this)->getFightCount(m_matchtype, buycount);
	}
}

void AddFightCountLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	if (m_matchtype == 0)
	{
		GlobalData::myTotalFihgtCount += buycount;
		GlobalData::myFihgtCount += buycount;
	}
	else if (m_matchtype == 1)
	{
		GlobalData::myMatchInfo.leftcount += buycount;
		HSLJMainLayer* layer = (HSLJMainLayer*)g_gameLayer->getChildByName("hsljmainlayer");
		if (layer != NULL)
			layer->updateMyFightCount();
	}
	int needgold = 20;
	int mygold = GlobalData::getMyGoldCount();
	if (buycount == 5)
	{
		needgold = 80;
	}
	GlobalData::setMyGoldCount(mygold - needgold);
}

void AddFightCountLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}
