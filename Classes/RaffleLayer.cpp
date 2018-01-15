#include "RaffleLayer.h"
#include "Const.h"
#include "GameDataSave.h"
#include "SoundManager.h"
#include "AnalyticUtil.h"
#include "CommonFuncs.h"
#include "WaitingProgress.h"
#include "HintBox.h"
#include "MD5.h"
#include "RaffleResultLayer.h"

RaffleLayer::RaffleLayer()
{
}


RaffleLayer::~RaffleLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}

RaffleLayer* RaffleLayer::create()
{
	RaffleLayer *pRet = new RaffleLayer();
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

bool RaffleLayer::init()
{
	Node* csbnode = CSLoader::createNode("raffleLayer.csb");
	this->addChild(csbnode);

	backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(RaffleLayer::onBack, this));

	GlobalData::myRaffleData.iscanplay = false;
	GlobalData::myRaffleData.mywingold = -1;
	GlobalData::myRaffleData.mywinrank = 0;
	GlobalData::myRaffleData.curstage = "";

	joinbtn = (cocos2d::ui::Button*)csbnode->getChildByName("joinbtn");
	joinbtn->addTouchEventListener(CC_CALLBACK_2(RaffleLayer::onJoin, this));
	joinbtn->setEnabled(false);

	mygoldlbl = (cocos2d::ui::Text*)csbnode->getChildByName("mygoldlbl");

	poolnum = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("poolnum");

	lefttimelbl = (cocos2d::ui::Text*)csbnode->getChildByName("time");
	prizebox = (cocos2d::ui::Widget*)csbnode->getChildByName("prizebox");
	for (int i = 0; i < 3; i++)
	{
		std::string namestr = StringUtils::format("name%d", i + 1);
		winnames[i] = (cocos2d::ui::Text*)prizebox->getChildByName(namestr);
		namestr = StringUtils::format("winnum%d", i + 1);
		wingolds[i] = (cocos2d::ui::Text*)prizebox->getChildByName(namestr);
	}
	prizebox->setVisible(false);

	if (GlobalData::g_gameStatus == GAMESTART)
		GlobalData::g_gameStatus = GAMEPAUSE;

	refreshGoldCount(0);
	this->schedule(schedule_selector(RaffleLayer::refreshGoldCount), 1);
	updatePool(0);
	this->schedule(schedule_selector(RaffleLayer::updatePool), 60);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void RaffleLayer::updatePool(float dt)
{
	actiontype = 0;

	WaitingProgress* waitbox = WaitingProgress::create("刷新中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->getCoinpoolData();
}

void RaffleLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void RaffleLayer::refreshGoldCount(float dt)
{
	std::string countstr = StringUtils::format("%d", GlobalData::getMyGoldCount());
	mygoldlbl->setString(countstr);
}

void RaffleLayer::onJoin(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		int mygold = GlobalData::getMyGoldCount();
		if (mygold >= 20)
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
			this->addChild(hbox, 0, "hintbox");
			return;
		}
		actiontype = 1;
		WaitingProgress* waitbox = WaitingProgress::create("投注中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->playCoinpoolData();
	}
}

void RaffleLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	if (actiontype == 0)
	{
		int leftmin = GlobalData::myRaffleData.leftime % 60 == 0 ? GlobalData::myRaffleData.leftime / 60 : (GlobalData::myRaffleData.leftime / 60 + 1);

		if (leftmin < 10)
			lefttimelbl->setTextColor(Color4B(204, 4, 4, 255));

		std::string str = StringUtils::format("%d", leftmin);
		lefttimelbl->setString(str);

		str = StringUtils::format("%d", GlobalData::myRaffleData.poolgold);
		poolnum->setString(str);

		if (GlobalData::myRaffleData.iscanplay)
		{
			joinbtn->setEnabled(true);
			joinbtn->setTitleText(CommonFuncs::gbk2utf("参与"));
			joinbtn->setTitleFontSize(40);
		}
		else
		{
			joinbtn->setEnabled(false);
			joinbtn->setTitleText(CommonFuncs::gbk2utf("本期已参与"));
			joinbtn->setTitleFontSize(28);
		}

		for (unsigned int i = 0; i < GlobalData::myRaffleData.vec_nicknames.size(); i++)
		{
			winnames[i]->setString(GlobalData::myRaffleData.vec_nicknames[i]);
			prizebox->setVisible(true);
		}
		for (unsigned int i = 0; i < GlobalData::myRaffleData.vec_wingold.size(); i++)
		{
			std::string resstr = StringUtils::format("%d", GlobalData::myRaffleData.vec_wingold[i]);
			wingolds[i]->setString(resstr);
		}

		if (GlobalData::myRaffleData.mywingold > 0)
		{
			RaffleResultLayer* rrlayer = RaffleResultLayer::create();
			this->addChild(rrlayer, 100);
			GameDataSave::getInstance()->setRaffleStage("");
		}
		else
		{
			std::string myrafflestage = GameDataSave::getInstance()->getRaffleStage();

			if (myrafflestage.length() > 0 && myrafflestage.compare(GlobalData::myRaffleData.curstage) != 0)
			{
				GameDataSave::getInstance()->setRaffleStage("");
				std::string str = StringUtils::format("您参与的第%s期未中奖，祝下次好运！", GlobalData::myRaffleData.mywinstage.c_str());
				HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf(str.c_str()));
				this->addChild(hbox, 10);
			}
		}
		GlobalData::myRaffleData.mywingold = -1;
	}
	else
	{
		std::string str = StringUtils::format("%d", GlobalData::myRaffleData.poolgold);
		poolnum->setString(str);

		GameDataSave::getInstance()->setRaffleStage(GlobalData::myRaffleData.curstage);

		GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - 20);
		GlobalData::myRaffleData.iscanplay = false;
		joinbtn->setEnabled(false);
		joinbtn->setTitleText(CommonFuncs::gbk2utf("本期已参与"));
		joinbtn->setTitleFontSize(28);
	}
	actiontype = 0;
}

void RaffleLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	std::string str = "数据异常，请稍后重试或者联系客服！";
	HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf(str.c_str()));
	this->addChild(hbox, 10);

	actiontype = 0;
}