#include "WXRankLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"
#include "SoundManager.h"
#include "WXPlayerChallengeLayer.h"
#include "WXAddFightCountLayer.h"
#include "GameDataSave.h"

WXRankLayer::WXRankLayer()
{
	isLastFight = false;
}


WXRankLayer::~WXRankLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}


WXRankLayer* WXRankLayer::create()
{
	WXRankLayer *pRet = new WXRankLayer();
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
void WXRankLayer::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}

time_t WXRankLayer::getNowTime()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	return nowTimeval.tv_sec;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	struct tm* tm;
	time_t timep;
	time(&timep);
	return timep;
#endif
}
bool WXRankLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("rankLayer.csb");
	this->addChild(m_csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(WXRankLayer::onBack, this));

	tag0btn = (cocos2d::ui::Button*)m_csbnode->getChildByName("tagbtn0");
	tag0btn->addTouchEventListener(CC_CALLBACK_2(WXRankLayer::onRank, this));
	tag0btn->setTag(0);
	tag0btn->setBright(false);

	tag1btn = (cocos2d::ui::Button*)m_csbnode->getChildByName("tagbtn1");
	tag1btn->addTouchEventListener(CC_CALLBACK_2(WXRankLayer::onRank, this));
	tag1btn->setTag(1);
	tag1btn->setBright(true);

	tag2btn = (cocos2d::ui::Button*)m_csbnode->getChildByName("tagbtn2");
	tag2btn->addTouchEventListener(CC_CALLBACK_2(WXRankLayer::onRank, this));
	tag2btn->setTag(2);
	tag2btn->setBright(true);

	m_particle = (cocos2d::ui::Widget*)m_csbnode->getChildByName("particle");

	srollView = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("scrollview");
	srollView->setScrollBarEnabled(false);
	srollView->setBounceEnabled(true);

	rankcatabox0 = (cocos2d::ui::Widget*)m_csbnode->getChildByName("rankcatabox0");

	rankcatabox1 = (cocos2d::ui::Widget*)m_csbnode->getChildByName("rankcatabox1");

	rankcatabox0->setVisible(true);
	rankcatabox1->setVisible(false);

	text6 = (cocos2d::ui::Text*) rankcatabox0->getChildByName("text6");

	myFihgtNode = (cocos2d::ui::Widget*)m_csbnode->getChildByName("mynode");
	myFihgtNode->setVisible(false);

	hintdown = (cocos2d::ui::Widget*)m_csbnode->getChildByName("hintdown");
	hintdown->setVisible(true);

	GlobalData::g_gameStatus = GAMEPAUSE;

	selectrank = 0;
	myfightingpower = 0;

	getRankData(0);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

long long WXRankLayer::getNowTimeMs() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	return ((long long)(nowTimeval.tv_sec)) * 1000 + nowTimeval.tv_usec / 1000;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);

	return (double)tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

bool WXRankLayer::isBeforeToday(time_t sec) {
	struct tm *tm;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	//win32平台
	time_t timep;
	time(&timep);
	tm = localtime(&timep);
#else  
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	tm = localtime(&nowTimeval.tv_sec);
#endif  

	struct tm * otherDay = gmtime(&sec);

	if (otherDay->tm_year < tm->tm_year) {
		return true;
	}
	else if (otherDay->tm_year > tm->tm_hour) {
		return false;
	}

	if (otherDay->tm_mon < tm->tm_mon) {
		return true;
	}
	else if (otherDay->tm_mon > tm->tm_mon) {
		return false;
	}

	if (otherDay->tm_mday < tm->tm_mday) {
		return true;
	}
	else if (otherDay->tm_mday > tm->tm_mday) {
		return false;
	}

	return false;
}
void WXRankLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

long long WXRankLayer::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}
void WXRankLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
void WXRankLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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
void WXRankLayer::onRank(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		cocos2d::ui::Button* btnnode = (cocos2d::ui::Button*)pSender;
		isLastFight = false;

		btnnode->setBright(false);
		int tag = btnnode->getTag();

		if (selectrank == tag)
			return;

		if (tag == 0)
		{
			m_particle->setPositionX(160);
			rankcatabox0->setVisible(true);
			rankcatabox1->setVisible(false);
			tag1btn->setBright(true);
			tag2btn->setBright(true);
			text6->setString(CommonFuncs::gbk2utf("天数"));
		}
		else if (tag == 1)
		{
			m_particle->setPositionX(356);
			rankcatabox0->setVisible(true);
			rankcatabox1->setVisible(false);
			tag0btn->setBright(true);
			tag2btn->setBright(true);
			text6->setString(CommonFuncs::gbk2utf("战斗力"));
		}
		else if (tag == 2)
		{
			m_particle->setPositionX(553);
			rankcatabox0->setVisible(false);
			rankcatabox1->setVisible(true);
			tag0btn->setBright(true);
			tag1btn->setBright(true);
		}

		selectrank = btnnode->getTag();
		getRankData(selectrank);
	}
}
void WXRankLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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
void WXRankLayer::getRankData(int type)
{
	WaitingProgress* waitbox = WaitingProgress::create("排名中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

	myfightingpower = 0;

	std::string orderstr = StringUtils::format("days=%d", g_nature->getPastDays());
	if (type == 1)
	{
		if (g_hero != NULL)
		{
			myfightingpower = g_hero->getMaxLifeValue() + g_hero->getTotalDf() * 20 + g_hero->getTotalAtck() * 10 + g_hero->getCritRate() * 100 + g_hero->getdodgeRate() * 100;
		}
		orderstr = StringUtils::format("fightingpower=%d", myfightingpower);
	}

	if (myfightingpower >= 1000000)
		myfightingpower = 30000;
	std::string addtypestr = StringUtils::format("&ranktype=%d", type);
	orderstr.append(addtypestr);

	if (type == 2)
	{
		ServerDataSwap::init(this)->getChallengeranklist();
	}
	else
	{
		ServerDataSwap::init(this)->getRankData(orderstr);
	}
}
void WXRankLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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
void WXRankLayer::delayShowData(float dt)
{
	int rankitemtype = 0;

	int contentheight = 850;
	int scrolly = 170;

	if (selectrank == 2)
	{
		rankitemtype = 1;
		contentheight = 800;
		scrolly = 220;
	}

	this->unschedule(schedule_selector(WXRankLayer::updateFightCount));

	srollView->removeAllChildrenWithCleanup(true);
	int size = GlobalData::vec_rankData.size();

	int itemheight = 78;
	int innerheight = itemheight * size;

	srollView->setContentSize(Size(srollView->getContentSize().width, contentheight));
	if (innerheight < contentheight)
		innerheight = contentheight;
	srollView->setInnerContainerSize(Size(srollView->getContentSize().width, innerheight));
	srollView->setPositionY(scrolly);

	for (unsigned int i = 0; i < GlobalData::vec_rankData.size(); i++)
	{
		RankItem* node = RankItem::create(&GlobalData::vec_rankData[i], rankitemtype);
		node->setPosition(Vec2(srollView->getContentSize().width/2, innerheight - itemheight / 2 - i * itemheight));
		srollView->addChild(node);
	}

	RankData myrankdata;
	myrankdata.rank = GlobalData::myrank;
	myrankdata.nickname = GlobalData::getMyNickName();
	myrankdata.herotype = g_hero->getHeadID();
	myrankdata.herolv = g_hero->getLVValue();
	myrankdata.herosex = g_hero->getSex();

	if (myfightingpower > 0)
		myrankdata.heroval = myfightingpower;
	else
		myrankdata.heroval = g_nature->getPastDays();

	this->removeChildByName("mynode");
	if (rankitemtype == 0)
	{
		Node* mynode = RankItem::create(&myrankdata);
		mynode->setPosition(Vec2(360, 130));
		this->addChild(mynode, 0, "mynode");
		myFihgtNode->setVisible(false);
		hintdown->setVisible(true);
	}
	else
	{
		hintdown->setVisible(false);
		myFihgtNode->setVisible(true);
		cocos2d::ui::Button* addbtn = (cocos2d::ui::Button*)myFihgtNode->getChildByName("addbtn");
		addbtn->addTouchEventListener(CC_CALLBACK_2(WXRankLayer::onAddCount, this));

		ranktext = (cocos2d::ui::Text*)myFihgtNode->getChildByName("myrank");
		std::string str = StringUtils::format("%d", GlobalData::myrank);
		ranktext->setString(str);

		str = StringUtils::format("%d", GlobalData::myFihgtexp);
		exptext = (cocos2d::ui::Text*)myFihgtNode->getChildByName("myfightexp");
		exptext->setString(str);

		int fightingpower = g_hero->getMaxLifeValue() + g_hero->getTotalDf() * 20 + g_hero->getTotalAtck() * 10 + g_hero->getCritRate() * 100 + g_hero->getdodgeRate() * 100;
		str = StringUtils::format("%d", fightingpower);

		cocos2d::ui::Text* fightpowertext = (cocos2d::ui::Text*)myFihgtNode->getChildByName("myfight");
		fightpowertext->setString(str);

		str = StringUtils::format("%d/%d", GlobalData::myFihgtCount, 5);
		countext = (cocos2d::ui::Text*)myFihgtNode->getChildByName("countext");
		countext->setString(str);

		this->schedule(schedule_selector(WXRankLayer::updateFightCount), 1.0f);
	}

	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void WXRankLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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
				DelayTime::create(20*0.1),
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
				DelayTime::create(30*0.1),
				action,
				NULL));
		}
		else {
			node->runAction(action);
		}
	}
}
void WXRankLayer::onAddCount(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		WXAddFightCountLayer* layer = WXAddFightCountLayer::create(0);
		g_gameLayer->addChild(layer, 5);
	}
}

void WXRankLayer::onSuccess()
{
	if (selectrank == 0)
	{
		std::string cdata = GameDataSave::getInstance()->getPlayerChallengeData();
		if (cdata.length() > 0)
		{
			std::vector<std::string> tmp;
			CommonFuncs::split(cdata, tmp, ",");
			if (tmp.size() >= 2)
			{
				isLastFight = true;
				GameDataSave::getInstance()->setPlayerChallengeData("");

				ServerDataSwap::init(this)->getChallengeResult(10000, tmp[0], atoi(tmp[1].c_str()), 1, 0);
			}
		}
		else
		{
			this->scheduleOnce(schedule_selector(WXRankLayer::delayShowData), 0.1f);
		}
	}
	else
	{
		this->scheduleOnce(schedule_selector(WXRankLayer::delayShowData), 0.1f);
	}
}

void WXRankLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	std::string descstr = "数据获取异常，请检查网络连接！！";
	if (selectrank == 2 && errcode == -3)
		descstr = "检测到你有作弊行为，无法使用该功能！！";
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf(descstr.c_str()));
	this->addChild(box);
}

void WXRankLayer::updateFightCount(float dt)
{
	std::string str = StringUtils::format("%d/%d", GlobalData::myFihgtCount, 5);
	countext->setString(str);

	str = StringUtils::format("%d", GlobalData::myrank);
	ranktext->setString(str);

	str = StringUtils::format("%d", GlobalData::myFihgtexp);
	exptext->setString(str);
}

RankItem::RankItem()
{

}
RankItem::~RankItem()
{

}

bool RankItem::init(RankData *data, int type)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_data = data;

	Node* csbnode = CSLoader::createNode("rankNode.csb");//物品节点
	if (type == 1)
	{
		csbnode = CSLoader::createNode("fightlistNode.csb");
	}
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);


	cocos2d::ui::Text* ranknumlbl = (cocos2d::ui::Text*)csbnode->getChildByName("ranknum");
	cocos2d::ui::Text* nicknamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("nickname");
	cocos2d::ui::Text* herolvlbl = (cocos2d::ui::Text*)csbnode->getChildByName("herolv");
	cocos2d::ui::Text* herosexlbl = (cocos2d::ui::Text*)csbnode->getChildByName("herosex");
	cocos2d::ui::Text* herovallbl = (cocos2d::ui::Text*)csbnode->getChildByName("heroval");
	cocos2d::ui::ImageView* commonline_1 = (cocos2d::ui::ImageView*)csbnode->getChildByName("commonline_1");
	/*ranknumlbl->setColor(Color3B(0, 0, 0));
	nicknamelbl->setColor(Color3B(0, 0, 0));
	herolvlbl->setColor(Color3B(0, 0, 0));
	herosexlbl->setColor(Color3B(0, 0, 0));
	herovallbl->setColor(Color3B(0, 0, 0));*/

	int rank = data->rank;

	std::string itemstr;

	if (type == 1)
	{
		if (rank % 2 == 0)
		{
			//itemstr = "ui/fightitem0.png";
		}
	}
	else
	{
		if (data->nickname.compare(GlobalData::getMyNickName()) == 0)
		{
			commonline_1->loadTexture("ui/commonline1.png", cocos2d::ui::TextureResType::PLIST);
			cocos2d::ui::Text* heroname = (cocos2d::ui::Text*)csbnode->getChildByName("heroname");
			heroname->setTextColor(Color4B(192, 88, 32, 255));
			ranknumlbl->setTextColor(Color4B(192, 88, 32,255));
			nicknamelbl->setTextColor(Color4B(192, 88, 32, 255));
			herolvlbl->setTextColor(Color4B(192, 88, 32, 255));
			herosexlbl->setTextColor(Color4B(192, 88, 32, 255));
			herovallbl->setTextColor(Color4B(192, 88, 32, 255));
		}
		else if (rank % 2 == 0)
		{
			itemstr = "ui/commonframe_1.png";
		}
	}
	if (itemstr.length() > 0)
	{
		/*cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)csbnode->getChildByName("item");
		item->loadTexture(itemstr, cocos2d::ui::TextureResType::PLIST);*/
	}

	std::string str = StringUtils::format("%d", rank);
	ranknumlbl->setString(str);

	if (rank >= 1000)
		ranknumlbl->setScale(0.7f);

	if (rank <= 3)
	{
		ranknumlbl->setVisible(false);
		std::string rankspritename = StringUtils::format("ui/ranknum%d.png", rank);
		Sprite* ranknum = Sprite::createWithSpriteFrameName(rankspritename);
		ranknum->setPosition(ranknumlbl->getPosition());
		csbnode->addChild(ranknum);
	}

	nicknamelbl->setString(data->nickname);

	if (type == 0)
	{
		cocos2d::ui::Text* heronamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("heroname");
		heronamelbl->setString(CommonFuncs::gbk2utf(heroname[(data->herotype - 1)].c_str()));
	}
	else
	{
		cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("headimg");
		std::string headimgstr = StringUtils::format("ui/fhero%d.png", data->herotype);
		headimg->loadTexture(headimgstr, cocos2d::ui::TextureResType::PLIST);
		cocos2d::ui::Button* fightbtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
		fightbtn->addTouchEventListener(CC_CALLBACK_2(RankItem::onFight, this));

		if (data->playerid.compare(GlobalData::UUID()) == 0 && data->herotype == g_hero->getHeadID())
			fightbtn->setEnabled(false);
	}

	str = StringUtils::format("%d", data->herolv + 1);
	herolvlbl->setString(str);

	const std::string sexstr[] = { "不详", "男", "女" };
	herosexlbl->setString(CommonFuncs::gbk2utf(sexstr[data->herosex].c_str()));
	if (data->herosex == 0)
	{
		herosexlbl->setTextColor(Color4B(204, 4, 4,255));
	}

	str = StringUtils::format("%d", data->heroval);
	herovallbl->setString(CommonFuncs::gbk2utf(str.c_str()));

	return true;
}
void RankItem::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void RankItem::shake(Node * node) {
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
RankItem* RankItem::create(RankData *data ,int type)
{
	RankItem *pRet = new RankItem();
	if (pRet && pRet->init(data, type))
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

void RankItem::onFight(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::myFihgtCount <= 0)
		{
			WXAddFightCountLayer* alayer = WXAddFightCountLayer::create(0);
			Director::getInstance()->getRunningScene()->addChild(alayer, 1);
			return;
		}
		WaitingProgress* waitbox = WaitingProgress::create("加载中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->getMyFihgterData(m_data->playerid, m_data->herotype);
	}
}

void RankItem::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	WXPlayerChallengeLayer* layer = WXPlayerChallengeLayer::create("m1-7", m_data);
	g_gameLayer->addChild(layer, 5, "playerchallengelayer");

}

void RankItem::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	Director::getInstance()->getRunningScene()->addChild(box, 2);
}
