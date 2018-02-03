#include "WXAddFightCountLayer.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"
#include "SoundManager.h"
#include "MD5.h"
#include "WXHSLJMainLayer.h"
#include "GameScene.h"

WXAddFightCountLayer::WXAddFightCountLayer()
{
	buycount = 0;
	m_matchtype = 0;
}


WXAddFightCountLayer::~WXAddFightCountLayer()
{

}


WXAddFightCountLayer* WXAddFightCountLayer::create(int matchtype)
{
	WXAddFightCountLayer *pRet = new WXAddFightCountLayer();
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

void WXAddFightCountLayer::loadTime() {
	std::string ts = "";
	long m_lastSaveTime = atol(ts.c_str());

	long nowTime = 123456789;
	if (m_lastSaveTime <= 0 || m_lastSaveTime > nowTime) {
		m_lastSaveTime = nowTime;
		saveTime();
	}
	else {
		long dt = nowTime - m_lastSaveTime;
		int add_times = dt / 10;
		if (add_times > 0) {
			const int df = add_times * 2;
			if (5 + df> 20) {

			}
			else {

			}
		}
		long m_countdown = 700 - dt % 50;
		m_lastSaveTime = nowTime - (50 - m_countdown);
		saveTime();
	}
}

bool WXAddFightCountLayer::init(int matchtype)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("addChallengeCountLayer.csb");
	this->addChild(m_csbnode);

	cocos2d::ui::Widget *closebtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(WXAddFightCountLayer::onBack, this));


	cocos2d::ui::Widget *buy1btn = (cocos2d::ui::Button*)m_csbnode->getChildByName("buy1btn");
	buy1btn->addTouchEventListener(CC_CALLBACK_2(WXAddFightCountLayer::onAddCount, this));
	buy1btn->setTag(1);

	cocos2d::ui::Widget *buy5btn = (cocos2d::ui::Button*)m_csbnode->getChildByName("buy5btn");
	buy5btn->addTouchEventListener(CC_CALLBACK_2(WXAddFightCountLayer::onAddCount, this));
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

void WXAddFightCountLayer::saveTime() {

}

void WXAddFightCountLayer::on1sTimer() {
	int m_countdown = 0;
	int m_lastSaveTime = 0;
	if (m_countdown-- <= 0) {
		if (500 < 20) {

		}

		m_countdown = 300;
		m_lastSaveTime = 1230;
		saveTime();
	}
}


void WXAddFightCountLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void WXAddFightCountLayer::onAddCount(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
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

void WXAddFightCountLayer::initA()
{
	const Size size = Director::getInstance()->getVisibleSize();
	auto m_top = CSLoader::createNode("GameTopLayer.csb");
	addChild(m_top, 0);
	m_top->ignoreAnchorPointForPosition(false);
	m_top->setAnchorPoint(Vec2(0.5, 1));
	m_top->setPosition(size.width * 0.5, size.height);

	auto m_bottom = CSLoader::createNode("GameBottomLayer.csb");
	m_bottom->ignoreAnchorPointForPosition(false);
	m_bottom->setAnchorPoint(Vec2(0.5, 0));
	m_bottom->setPosition(size.width * 0.5, 0);
	addChild(m_bottom, 1);
}

void WXAddFightCountLayer::onSuccess()
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
		WXHSLJMainLayer* layer = (WXHSLJMainLayer*)g_gameLayer->getChildByName("hsljmainlayer");
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

void WXAddFightCountLayer::initBg()
{
	const Size size = Director::getInstance()->getVisibleSize();
	auto m_bg = Sprite::create("");
	addChild(m_bg, 0);
	m_bg->setAnchorPoint(Vec2(0.5, 0));
	m_bg->setPosition(size.width * 0.5, 0);

	auto m_bgGround = Sprite::create("");
	addChild(m_bgGround, 1);
	m_bgGround->setPosition(m_bg->getContentSize().width * 0.5, 144 + m_bgGround->getContentSize().height * 0.5);

	auto tree = Sprite::create("");
	addChild(tree);
	tree->setAnchorPoint(Vec2(1, 0.5));
	tree->setPosition(size.width, 120);
}

void WXAddFightCountLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}
