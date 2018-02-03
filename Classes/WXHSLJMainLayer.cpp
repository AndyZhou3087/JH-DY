#include "WXHSLJMainLayer.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "WXFactionCreateLayer.h"
#include "Const.h"
#include "GameScene.h"
#include "WXHSLJFightLayer.h"
#include "WXAddFightCountLayer.h"
#include "WXHSLJRewardDescLayer.h"
#include "WXHSLJRankLayer.h"
#include "WXHSLJRewardLayer.h"
#include "GameDataSave.h"

WXHSLJMainLayer::WXHSLJMainLayer()
{
	datatype = 0;
	changenexthero = 1;
}


WXHSLJMainLayer::~WXHSLJMainLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}
bool WXHSLJMainLayer::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
}


WXHSLJMainLayer* WXHSLJMainLayer::create()
{
	WXHSLJMainLayer *pRet = new WXHSLJMainLayer();
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
void WXHSLJMainLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
	if (nullptr == node) {
		return;
	}

	ActionInterval * action = Sequence::create(
		ScaleTo::create(0.2, 1.1, 0.9, 1),
		Spawn::create(
		EaseExponentialOut::create(ScaleTo::create(0.1, 0.9, 1.1, 1)),
		MoveBy::create(0.2, Vec2(0, dt)),
		NULL),
		Spawn::create(
		EaseExponentialIn::create(ScaleTo::create(0.1, 1.2, 0.9, 1)),
		MoveBy::create(0.2, Vec2(0, -dt)),
		NULL),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	if (repeat) {
		node->runAction(RepeatForever::create(
			Sequence::create(
			action,
			DelayTime::create(intrval),
			NULL)
			));
	}
	else {
		node->runAction(action);
	}

}

void WXHSLJMainLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
	if (nullptr == node) {
		return;
	}

	ActionInterval * action = Sequence::create(
		ScaleTo::create(0.2, 1.1, 0.9, 1),
		Spawn::create(
		EaseExponentialOut::create(ScaleTo::create(0.1, 0.9, 1.1, 1)),
		MoveBy::create(0.2, Vec2(0, dt)),
		NULL),
		Spawn::create(
		EaseExponentialIn::create(ScaleTo::create(0.1, 1.2, 0.9, 1)),
		MoveBy::create(0.2, Vec2(0, -dt)),
		NULL),
		ScaleTo::create(0.1, 0.95, 1.05, 1),
		ScaleTo::create(0.1, 1.05, 0.95, 1),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	if (repeat) {
		if (0 != tag) {
			action->setTag(tag);
		}

		node->runAction(RepeatForever::create(
			Sequence::create(
			action,
			DelayTime::create(intrval),
			NULL)
			));
	}
	else {
		node->runAction(action);
	}
}

bool WXHSLJMainLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("WXHSLJMainLayer.csb");
	this->addChild(csbnode);

	m_backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(WXHSLJMainLayer::onBack, this));

	m_matchbtn = (cocos2d::ui::Button*)csbnode->getChildByName("findbtn");
	m_matchbtn->addTouchEventListener(CC_CALLBACK_2(WXHSLJMainLayer::onMacth, this));
	m_matchbtn->setEnabled(false);

	cocos2d::ui::Widget* addbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("addbtn");
	addbtn->addTouchEventListener(CC_CALLBACK_2(WXHSLJMainLayer::onAddCount, this));

	cocos2d::ui::Widget* rewarddescbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("awarddescbtn");
	rewarddescbtn->addTouchEventListener(CC_CALLBACK_2(WXHSLJMainLayer::onRewardDesc, this));

	cocos2d::ui::Widget* ranbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("ranbtn");
	ranbtn->addTouchEventListener(CC_CALLBACK_2(WXHSLJMainLayer::onRank, this));

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

	/*Label* mynicknamelbl = Label::createWithTTF(GlobalData::getMyNickName(), "fonts/STXINGKA.TTF", 26);
	mynicknamelbl->setAnchorPoint(Vec2(0.5, 0.5));
	mynicknamelbl->setLineBreakWithoutSpace(true);
	mynicknamelbl->setMaxLineWidth(10);
	mynicknamelbl->setString(GlobalData::getMyNickName());
	mynicknamelbl->setColor(Color3B(255, 255, 255));
	mynicknamelbl->setPosition(Vec2(110,948));
	this->addChild(mynicknamelbl);*/

	/*m_heroname = Label::createWithTTF("", "fonts/STXINGKA.TTF", 26);
	m_heroname->setAnchorPoint(Vec2(0.5, 0.5));
	m_heroname->setLineBreakWithoutSpace(true);
	m_heroname->setMaxLineWidth(10);
	m_heroname->setColor(Color3B(255, 255, 255));
	m_heroname->setPosition(Vec2(-14, -42));
	heroNode->addChild(m_heroname);*/
	
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

void WXHSLJMainLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
	if (nullptr == node) {
		return;
	}

	ActionInterval * action = Sequence::create(
		ScaleTo::create(0.2, 1.05, 0.95, 1),
		Spawn::create(
		EaseExponentialOut::create(ScaleTo::create(0.1, 0.95, 1.05, 1)),
		MoveBy::create(0.2, Vec2(0, dt)),
		ac,
		NULL),
		Spawn::create(
		EaseExponentialIn::create(ScaleTo::create(0.1, 1.1, 0.95, 1)),
		MoveBy::create(0.2, Vec2(0, -dt)),
		NULL),
		ScaleTo::create(0.1, 0.98, 1.08, 1),
		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	if (repeat) {
		if (0 != tag) {
			action->setTag(tag);
		}

		node->runAction(RepeatForever::create(
			Sequence::create(
			action,
			DelayTime::create(intrval),
			NULL)
			));
	}
	else {
		node->runAction(action);
	}
}


void WXHSLJMainLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void WXHSLJMainLayer::onAddCount(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		WXAddFightCountLayer* layer = WXAddFightCountLayer::create(1);
		g_gameLayer->addChild(layer, 5);
	}
}

void WXHSLJMainLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
	if (nullptr == node) {
		return;
	}

	ActionInterval * action = Sequence::create(
		EaseSineIn::create(ScaleTo::create(0.08, 0.95, 1.05, 1)),
		EaseSineOut::create(ScaleTo::create(0.2, 1.15, 0.95, 1)),
		ScaleTo::create(0.1, 0.98, 1.08, 1),
		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 0.98, 1.08, 1),
		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	if (repeat) {
		if (0 != tag) {
			action->setTag(tag);
		}
		if (delay) {
			node->runAction(RepeatForever::create(
				Sequence::create(
				DelayTime::create(getRandomNum(1, 10)*0.1),
				action,
				DelayTime::create(intrval),
				NULL)
				));
		}
		else {
			node->runAction(RepeatForever::create(
				Sequence::create(
				action,
				DelayTime::create(intrval),
				NULL)
				));
		}

	}
	else {
		if (delay) {
			node->runAction(Sequence::create(
				DelayTime::create(getRandomNum(1, 10)*0.1),
				action,
				NULL));
		}
		else {
			node->runAction(action);
		}
	}
}


void WXHSLJMainLayer::onMacth(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::myMatchInfo.leftcount <= 0)
		{
			WXAddFightCountLayer* alayer = WXAddFightCountLayer::create(1);
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
		this->schedule(schedule_selector(WXHSLJMainLayer::changeHeroImg), 0.3f);
	}
}

void WXHSLJMainLayer::onRewardDesc(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		WXHSLJRewardDescLayer* layer = WXHSLJRewardDescLayer::create();
		this->addChild(layer);
	}
}

void WXHSLJMainLayer::jumpDown(cocos2d::Node *node, float dt) {
	if (nullptr == node) {
		return;
	}

	const float originY = node->getPositionY();
	node->setPositionY(originY + dt);

	ActionInterval *action = Sequence::create(
		MoveBy::create(0.2, Vec2(0, -dt - 10)),
		MoveBy::create(0.2, Vec2(0, 20)),
		MoveBy::create(0.1, Vec2(0, -18)),
		MoveBy::create(0.1, Vec2(0, 13)),
		MoveBy::create(0.1, Vec2(0, -5)),


		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 0.98, 1, 1),
		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	node->runAction(action);
}

void WXHSLJMainLayer::onRank(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		WXHSLJRankLayer* layer = WXHSLJRankLayer::create();
		g_gameLayer->addChild(layer,5, "shljranklayer");
	}
}
void WXHSLJMainLayer::shake(Node * node) {
	if (NULL == node) {
		return;
	}

	node->runAction(CCSequence::create(
		MoveBy::create(0.02, Vec2(0, 15)),
		MoveBy::create(0.02, Vec2(0, -27)),
		MoveBy::create(0.02, Vec2(0, 22)),
		MoveBy::create(0.02, Vec2(0, -14)),
		MoveBy::create(0.02, Vec2(0, 4)),
		NULL));
}
void WXHSLJMainLayer::getMyMatchInfo()
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

void WXHSLJMainLayer::onSuccess()
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
			this->scheduleOnce(schedule_selector(WXHSLJMainLayer::stopChangeHeroImg), r + 1);
		}
		else
		{
			HintBox * box = HintBox::create(CommonFuncs::gbk2utf("匹配失败！！请重新匹配！"));
			this->addChild(box);
			m_matchbtn->setEnabled(true);
			m_backbtn->setEnabled(true);
			m_hinttext->setString(CommonFuncs::gbk2utf("请匹配你的对手..."));
			this->unschedule(schedule_selector(WXHSLJMainLayer::changeHeroImg));
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


void WXHSLJMainLayer::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void WXHSLJMainLayer::onErr(int errcode)
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
			this->unschedule(schedule_selector(WXHSLJMainLayer::changeHeroImg));
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

int WXHSLJMainLayer::getRandomNum(int rangeStart, int rangeEnd) {

	if (rangeEnd < rangeStart) {
		CCASSERT(false, "get random fail");
		return 0;
	}

	if (rangeStart == rangeEnd) {
		return rangeStart;
	}

	int delta = rand() % (rangeEnd - rangeStart);
	return rangeStart + delta;
}
void WXHSLJMainLayer::showMyInfo()
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

void WXHSLJMainLayer::updateMyFightCount()
{
	std::string str = StringUtils::format("%d/5", GlobalData::myMatchInfo.leftcount);
	m_fightcount->setString(str);
}

bool WXHSLJMainLayer::getRandomBoolean(float rate) {

	int rate10 = (int)(rate*10.0);
	int randNum = rand();
	if (randNum % 10 <= rate10) {
		return true;
	}
	else {
		return false;
	}
}

bool WXHSLJMainLayer::getRandomBoolean() {

	if (0 == rand() % 2) {
		return true;
	}
	else {
		return false;
	}
}

int WXHSLJMainLayer::getRandomNum(int range) {

	if (range <= 0) {
		return 0;
	}

	return rand() % range;
}


void WXHSLJMainLayer::showMatchInfo()
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

void WXHSLJMainLayer::showMyReWard()
{
	if (GlobalData::myMatchInfo.matchaward > 0)
	{
		WXHSLJRewardLayer* layer = WXHSLJRewardLayer::create(GlobalData::myMatchInfo.matchaward);
		this->addChild(layer);
	}
}

std::string WXHSLJMainLayer::getDwStr(int exp)
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

void WXHSLJMainLayer::showVSAnim()
{
	std::string vscsb = "VS.csb";
	Node* csbnode = CSLoader::createNode(vscsb);
	csbnode->setPosition(Vec2(360, 650));
	this->addChild(csbnode);
	auto action = CSLoader::createTimeline(vscsb);
	csbnode->runAction(action);
	action->gotoFrameAndPlay(0, false);
}

void WXHSLJMainLayer::stopChangeHeroImg(float dt)
{
	this->unschedule(schedule_selector(WXHSLJMainLayer::changeHeroImg));

	heroNode->setVisible(true);
	m_hinttext->setVisible(false);

	showVSAnim();

	this->scheduleOnce(schedule_selector(WXHSLJMainLayer::delayEnterFight), 1.5f);
}

void WXHSLJMainLayer::delayEnterFight(float dt)
{
	this->removeFromParentAndCleanup(true);
	WXHSLJFightLayer* layer = WXHSLJFightLayer::create("m1-11");
	g_gameLayer->addChild(layer, 5);
}

void WXHSLJMainLayer::changeHeroImg(float dt)
{
	if (changenexthero > 4)
		changenexthero = 1;
	
	showPlayerHeadImg(changenexthero);
	changenexthero++;
}

void WXHSLJMainLayer::showPlayerHeadImg(int herotype)
{
	std::string str = StringUtils::format("images/hsljhero%d.png", herotype);
	m_playerheadimg->setFlippedX(true);
	m_playerheadimg->setContentSize(Sprite::create(str)->getContentSize());
	m_playerheadimg->loadTexture(str, cocos2d::ui::Widget::TextureResType::LOCAL);
}