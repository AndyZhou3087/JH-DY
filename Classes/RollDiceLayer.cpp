#include "RollDiceLayer.h"
#include "Const.h"

#include "SoundManager.h"
#include "AnalyticUtil.h"
#include "CommonFuncs.h"
#include "WaitingProgress.h"
#include "HintBox.h"
#include "MD5.h"
#include "ShopLayer.h"

RollDiceLayer::RollDiceLayer()
{
}


RollDiceLayer::~RollDiceLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}

RollDiceLayer* RollDiceLayer::create()
{
	RollDiceLayer *pRet = new RollDiceLayer();
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

bool RollDiceLayer::init()
{
	Node* csbnode = CSLoader::createNode("rollDiceLayer.csb");
	this->addChild(csbnode);

	backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(RollDiceLayer::onBack, this));

	rollbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("getbtn");
	rollbtn->addTouchEventListener(CC_CALLBACK_2(RollDiceLayer::onRoll, this));
	
	mygoldlbl = (cocos2d::ui::Text*)csbnode->getChildByName("mygoldlbl");

	countlbl = (cocos2d::ui::Text*)csbnode->getChildByName("count");

	counttext = (cocos2d::ui::Text*)csbnode->getChildByName("counttext");

	costgoldlbl = (cocos2d::ui::Text*)csbnode->getChildByName("constgoldlbl");
	costgoldlbl->setVisible(false);

	costgold = (cocos2d::ui::Widget*)csbnode->getChildByName("constgold");
	costgold->setVisible(false);

	word = (cocos2d::ui::Text*)csbnode->getChildByName("word");

	animnode = CSLoader::createNode("rollDiceAnim.csb");
	animnode->setPosition(Vec2(360, 800));
	csbnode->addChild(animnode);
	if (GlobalData::g_gameStatus == GAMESTART)
		GlobalData::g_gameStatus = GAMEPAUSE;

	refreshGoldCount(0);
	this->schedule(schedule_selector(RollDiceLayer::refreshGoldCount), 1);
	actiontype = 0;

	WaitingProgress* waitbox = WaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->getlotteryData(actiontype);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void RollDiceLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void RollDiceLayer::refreshGoldCount(float dt)
{
	std::string countstr = StringUtils::format("%d", GlobalData::getMyGoldCount());
	mygoldlbl->setString(countstr);
}

void RollDiceLayer::onRoll(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		word->setString(CommonFuncs::gbk2utf("小宝为了回馈江湖兄弟，特举办保底掷骰子拿元宝，点数既是倍数，点数越高拿的越高！！！"));
		if (GlobalData::myLotteryData.leftcount <= 0)
		{
			HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("每人最多只能参与六轮，不然小宝我就算再有钱也不够赔的啦~"));
			this->addChild(hbox, 10);
			return;
		}

		int mygold = GlobalData::getMyGoldCount();

		if (mygold >= useGold)
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
		this->removeChildByName("dice");
		animnode->getChildByName("node")->getChildByName("cover")->setVisible(true);
		anim_action = CSLoader::createTimeline("rollDiceAnim.csb");
		animnode->runAction(anim_action);
		anim_action->gotoFrameAndPlay(0, true);
		actiontype = 1;
		backbtn->setEnabled(false);
		rollbtn->setEnabled(false);
		ServerDataSwap::init(this)->getlotteryData(actiontype);
	}
}

void RollDiceLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	std::string str = StringUtils::format("%d", GlobalData::myLotteryData.nextcostgold);
	costgoldlbl->setString(str);
	const std::string countcnstr[] = { "一", "二", "三", "四", "五", "六", "六" };
	str = StringUtils::format("%s", CommonFuncs::gbk2utf(countcnstr[6 - GlobalData::myLotteryData.leftcount].c_str()).c_str());
	countlbl->setString(str);
	setUiVisual(true);
	if (actiontype == 1)
	{
		GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - useGold);
		winbs = GlobalData::myLotteryData.wingold / useGold;
		int r = GlobalData::createRandomNum(3);

		this->scheduleOnce(schedule_selector(RollDiceLayer::showResult), r + 1.0f);
	}

	if (GlobalData::myLotteryData.leftcount <= 0)
	{
		costgoldlbl->setVisible(false);
		costgold->setVisible(false);
	}
	useGold = GlobalData::myLotteryData.nextcostgold;
	actiontype = 0;
}

void RollDiceLayer::onErr(int errcode)
{
	animnode->stopAllActions();
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	std::string str = "数据异常，请稍后重试或者联系客服！";
	if (errcode == -3)
	{
		str = "次数已达限制";
		setUiVisual(false);
	}
	backbtn->setEnabled(true);
	rollbtn->setEnabled(true);
	HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf(str.c_str()));
	this->addChild(hbox, 10);
	actiontype = 0;

}

void RollDiceLayer::setUiVisual(bool val)
{
	counttext->setVisible(val);
	costgoldlbl->setVisible(val);
	costgold->setVisible(val);
	if (val)
		backbtn->setPositionX(160);
	else
		backbtn->setPositionX(360);
	rollbtn->setVisible(val);
}

void RollDiceLayer::showResult(float dt)
{
	backbtn->setEnabled(true);
	rollbtn->setEnabled(true);

	anim_action = CSLoader::createTimeline("rollDiceAnim.csb");
	animnode->runAction(anim_action);
	anim_action->gotoFrameAndPlay(0, 0, false);
	animnode->stopAllActions();

	this->scheduleOnce(schedule_selector(RollDiceLayer::openDice), 1.0f);
}

void RollDiceLayer::openDice(float dt)
{
	animnode->getChildByName("node")->getChildByName("cover")->setVisible(false);
	std::string dicestr = StringUtils::format("ui/dice%d.png", winbs);

	Sprite* dicesprite = Sprite::createWithSpriteFrameName(dicestr);
	dicesprite->setPosition(animnode->getPosition());
	this->addChild(dicesprite, 1, "dice");

	std::string str = StringUtils::format("恭喜兄弟掷出%d点，获得%d倍元宝，共%d，再接再厉！！！", winbs, winbs, GlobalData::myLotteryData.wingold);
	word->setString(CommonFuncs::gbk2utf(str.c_str()));
	GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() + GlobalData::myLotteryData.wingold);
}