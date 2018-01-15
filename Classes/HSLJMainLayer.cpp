#include "HSLJMainLayer.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "FactionCreateLayer.h"
#include "Const.h"
#include "GameScene.h"
#include "MatchFightLayer.h"
#include "AddFightCountLayer.h"
#include "HSLJRewardDescLayer.h"
#include "HSLJRankLayer.h"
#include "HSLJRewardLayer.h"
#include "GameDataSave.h"

HSLJMainLayer::HSLJMainLayer()
{
	datatype = 0;
	changenexthero = 1;
}


HSLJMainLayer::~HSLJMainLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}


HSLJMainLayer* HSLJMainLayer::create()
{
	HSLJMainLayer *pRet = new HSLJMainLayer();
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

bool HSLJMainLayer::init()
{
	Node* csbnode = CSLoader::createNode("HSLJMainLayer.csb");
	this->addChild(csbnode);

	m_backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(HSLJMainLayer::onBack, this));

	m_matchbtn = (cocos2d::ui::Button*)csbnode->getChildByName("findbtn");
	m_matchbtn->addTouchEventListener(CC_CALLBACK_2(HSLJMainLayer::onMacth, this));
	m_matchbtn->setEnabled(false);

	cocos2d::ui::Widget* addbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("addbtn");
	addbtn->addTouchEventListener(CC_CALLBACK_2(HSLJMainLayer::onAddCount, this));

	cocos2d::ui::Widget* rewarddescbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("awarddescbtn");
	rewarddescbtn->addTouchEventListener(CC_CALLBACK_2(HSLJMainLayer::onRewardDesc, this));

	cocos2d::ui::Widget* ranbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("ranbtn");
	ranbtn->addTouchEventListener(CC_CALLBACK_2(HSLJMainLayer::onRank, this));

	heroNode = (cocos2d::ui::Widget*)csbnode->getChildByName("heronode");
	heroNode->setVisible(false);

	m_hinttext = (cocos2d::ui::Text*)csbnode->getChildByName("hinttext");

	m_time = (cocos2d::ui::Text*)csbnode->getChildByName("time");
	m_time->setString("");

	m_matchno = (cocos2d::ui::Text*)csbnode->getChildByName("cccount");
	m_matchno->setString("");

	m_matchwincount = (cocos2d::ui::Text*)csbnode->getChildByName("matchwincount");
	m_matchwincount->setString("");

	cocos2d::ui::Text* mynicknamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("mynickname");
	mynicknamelbl->setString(GlobalData::getMyNickName());
	
	cocos2d::ui::ImageView* myheroimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("myheroimg");
	std::string headimgstr = StringUtils::format("images/hsljhero%d.png", g_hero->getHeadID());
	myheroimg->loadTexture(headimgstr, cocos2d::ui::Widget::TextureResType::LOCAL);

	m_mydw = (cocos2d::ui::Text*)csbnode->getChildByName("mydw");
	m_mydw->setString("");
	
	m_mywinpercent = (cocos2d::ui::Text*)csbnode->getChildByName("mywin");
	m_mywinpercent->setString("");

	m_progresstext = (cocos2d::ui::Text*)csbnode->getChildByName("progresstext");
	m_progresstext->setString("");

	m_expbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("expbar");
	m_expbar->setPercent(0);

	m_fightcount = (cocos2d::ui::Text*)csbnode->getChildByName("fightcount");
	m_fightcount->setString("5/5");

	m_herodw = (cocos2d::ui::Text*)heroNode->getChildByName("herodw");
	m_herowinpercent = (cocos2d::ui::Text*)heroNode->getChildByName("herowin");
	m_heroname = (cocos2d::ui::Text*)heroNode->getChildByName("heronickname");
	
	m_playerheadimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroimg");

	std::string mplayerid = GameDataSave::getInstance()->getHsljMatchPlayer();
	if (mplayerid.length() > 0)
		datatype = 2;

	getMyMatchInfo();

	GlobalData::g_gameStatus = GAMEPAUSE;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HSLJMainLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void HSLJMainLayer::onAddCount(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AddFightCountLayer* layer = AddFightCountLayer::create(1);
		g_gameLayer->addChild(layer, 5);
	}
}

void HSLJMainLayer::onMacth(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::myMatchInfo.leftcount <= 0)
		{
			AddFightCountLayer* alayer = AddFightCountLayer::create(1);
			Director::getInstance()->getRunningScene()->addChild(alayer, 1);
			return;
		}
		datatype = 1;
		m_matchbtn->setEnabled(false);
		m_backbtn->setEnabled(false);
		m_hinttext->setString(CommonFuncs::gbk2utf("匹配中..."));
		WaitingProgress* waitbox = WaitingProgress::create("加载中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->getMatchFight();
		changeHeroImg(0);
		this->schedule(schedule_selector(HSLJMainLayer::changeHeroImg), 0.3f);
	}
}

void HSLJMainLayer::onRewardDesc(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		HSLJRewardDescLayer* layer = HSLJRewardDescLayer::create();
		this->addChild(layer);
	}
}

void HSLJMainLayer::onRank(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		HSLJRankLayer* layer = HSLJRankLayer::create();
		g_gameLayer->addChild(layer,5, "shljranklayer");
	}
}

void HSLJMainLayer::getMyMatchInfo()
{
	WaitingProgress* waitbox = WaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	if (datatype == 2)
	{
		ServerDataSwap::init(this)->getMatchFightResult(GameDataSave::getInstance()->getHsljMatchPlayer(), -13);
	}
	else if (datatype == 0)
	{
		ServerDataSwap::init(this)->getMyMatchInfo();
	}
}

void HSLJMainLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	if (datatype == 0)
	{
		m_matchbtn->setEnabled(true);
		showMyInfo();
	}
	else if (datatype == 1)
	{
		if (GlobalData::vec_matchPlayerData.size() > 0)
		{
			showMatchInfo();
			int r = GlobalData::createRandomNum(3);
			this->scheduleOnce(schedule_selector(HSLJMainLayer::stopChangeHeroImg), r + 1);
		}
		else
		{
			HintBox * box = HintBox::create(CommonFuncs::gbk2utf("匹配失败！！请重新匹配！"));
			this->addChild(box);
			m_matchbtn->setEnabled(true);
			m_backbtn->setEnabled(true);
			m_hinttext->setString(CommonFuncs::gbk2utf("请匹配你的对手..."));
			this->unschedule(schedule_selector(HSLJMainLayer::changeHeroImg));
			showPlayerHeadImg(0);
		}
	}
	else if (datatype == 2)
	{
		GameDataSave::getInstance()->setHsljMatchPlayer("");
		datatype = 0;
		getMyMatchInfo();
	}
}

void HSLJMainLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	if (datatype != 2)
	{
		if (datatype == 0 && errcode == -3)
		{
			HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据异常！！"));
			this->addChild(box);
			return;
		}

		std::string desc = "数据获取异常，请检查网络连接！！";
		if (datatype == 1 && errcode == -3)
		{
			desc = "匹配失败，暂时无对手数据，请稍后重试！！";
		}
		HintBox * box = HintBox::create(CommonFuncs::gbk2utf(desc.c_str()));
		this->addChild(box);
		if (datatype == 1)
		{
			m_matchbtn->setEnabled(true);
			m_backbtn->setEnabled(true);
			m_hinttext->setString(CommonFuncs::gbk2utf("请匹配你的对手..."));
			this->unschedule(schedule_selector(HSLJMainLayer::changeHeroImg));
			showPlayerHeadImg(0);
		}
		datatype = 0;
	}
	else
	{
		datatype = 0;
		getMyMatchInfo();
	}
}

void HSLJMainLayer::showMyInfo()
{
	std::string str = StringUtils::format("%s至%s", GlobalData::myMatchInfo.starttime.c_str(), GlobalData::myMatchInfo.endtime.c_str());
	m_time->setString(CommonFuncs::gbk2utf(str.c_str()));

	std::string progressstr;
	int barpercent = 0;
	if (GlobalData::myMatchInfo.myexp <= 100)
	{
		progressstr = StringUtils::format("%d/100", GlobalData::myMatchInfo.myexp);
		barpercent = GlobalData::myMatchInfo.myexp * 100 / 100;
	}
	else if (GlobalData::myMatchInfo.myexp <= 300)
	{
		progressstr = StringUtils::format("%d/300", GlobalData::myMatchInfo.myexp);
		barpercent = GlobalData::myMatchInfo.myexp * 100 / 300;
	}
	else if (GlobalData::myMatchInfo.myexp <= 700)
	{
		progressstr = StringUtils::format("%d/700", GlobalData::myMatchInfo.myexp);
		barpercent = GlobalData::myMatchInfo.myexp * 100 / 700;
	}
	else if (GlobalData::myMatchInfo.myexp <= 1000)
	{
		progressstr = StringUtils::format("%d/1000", GlobalData::myMatchInfo.myexp);
		barpercent = GlobalData::myMatchInfo.myexp * 100 / 1000;
	}
	else
	{
		progressstr = StringUtils::format("%d/1000", 1000);
		barpercent = 100;
	}
	str = getDwStr(GlobalData::myMatchInfo.myexp);
	m_mydw->setString(CommonFuncs::gbk2utf(str.c_str()));

	int percent = 0;
	int totalcount = GlobalData::myMatchInfo.mywincount + GlobalData::myMatchInfo.myfailcount;
	if (totalcount > 0)
	{
		percent = GlobalData::myMatchInfo.mywincount * 100 / totalcount;
	}
	str = StringUtils::format("%d%%", percent);
	m_mywinpercent->setString(str);

	str = StringUtils::format("%d", totalcount);
	m_matchno->setString(str);

	str = StringUtils::format("%d", GlobalData::myMatchInfo.mywincount);
	m_matchwincount->setString(str);

	m_progresstext->setString(progressstr);
	m_expbar->setPercent(barpercent);
	
	updateMyFightCount();

	showMyReWard();
}

void HSLJMainLayer::updateMyFightCount()
{
	std::string str = StringUtils::format("%d/5", GlobalData::myMatchInfo.leftcount);
	m_fightcount->setString(str);
}

void HSLJMainLayer::showMatchInfo()
{
	std::string str = getDwStr(GlobalData::matchPlayerInfo.exp);

	m_herodw->setString(CommonFuncs::gbk2utf(str.c_str()));
	
	int percent = 0;
	int totalcount = GlobalData::matchPlayerInfo.wincount + GlobalData::matchPlayerInfo.failcount;
	if (totalcount > 0)
	{
		percent = GlobalData::matchPlayerInfo.wincount * 100 / totalcount;
	}
	str = StringUtils::format("%d%%", percent);

	m_herowinpercent->setString(str);

	m_heroname->setString(GlobalData::matchPlayerInfo.nickname);
}

void HSLJMainLayer::showMyReWard()
{
	if (GlobalData::myMatchInfo.matchaward > 0)
	{
		HSLJRewardLayer* layer = HSLJRewardLayer::create(GlobalData::myMatchInfo.matchaward);
		this->addChild(layer);
	}
}

std::string HSLJMainLayer::getDwStr(int exp)
{
	std::string str;
	if (exp <= 100)
	{
		str = "乡野义士";
	}
	else if (exp <= 300)
	{
		str = "江湖侠士";
	}
	else if (exp <= 700)
	{
		str = "名门侠客";
	}
	else if (exp <= 1000)
	{
		str = "一方大侠";
	}
	else
	{
		str = "墨者";
	}
	return str;
}

void HSLJMainLayer::showVSAnim()
{
	std::string vscsb = "VS.csb";
	Node* csbnode = CSLoader::createNode(vscsb);
	csbnode->setPosition(Vec2(360, 650));
	this->addChild(csbnode);
	auto action = CSLoader::createTimeline(vscsb);
	csbnode->runAction(action);
	action->gotoFrameAndPlay(0, false);
}

void HSLJMainLayer::stopChangeHeroImg(float dt)
{
	this->unschedule(schedule_selector(HSLJMainLayer::changeHeroImg));

	heroNode->setVisible(true);
	m_hinttext->setVisible(false);

	showVSAnim();

	this->scheduleOnce(schedule_selector(HSLJMainLayer::delayEnterFight), 1.5f);
}

void HSLJMainLayer::delayEnterFight(float dt)
{
	this->removeFromParentAndCleanup(true);
	MatchFightLayer* layer = MatchFightLayer::create("m1-11");
	g_gameLayer->addChild(layer, 5);
}

void HSLJMainLayer::changeHeroImg(float dt)
{
	if (changenexthero > 4)
		changenexthero = 1;
	
	showPlayerHeadImg(changenexthero);
	changenexthero++;
}

void HSLJMainLayer::showPlayerHeadImg(int herotype)
{
	std::string str = StringUtils::format("images/hsljhero%d.png", herotype);
	m_playerheadimg->setFlippedX(true);
	m_playerheadimg->setContentSize(Sprite::create(str)->getContentSize());
	m_playerheadimg->loadTexture(str, cocos2d::ui::Widget::TextureResType::LOCAL);
}