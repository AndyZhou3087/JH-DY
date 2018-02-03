#include "WXHSLJFightResultLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"
#include "SoundManager.h"
#include "GameDataSave.h"
#include "WXRankLayer.h"
#include "WXHSLJMainLayer.h"

WXHSLJFightResultLayer::WXHSLJFightResultLayer()
{
	GameDataSave::getInstance()->setHsljMatchPlayer("");
}


WXHSLJFightResultLayer::~WXHSLJFightResultLayer()
{
	GlobalData::isPlayerChallenging = false;
}
void WXHSLJFightResultLayer::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}

WXHSLJFightResultLayer* WXHSLJFightResultLayer::create(int myfinalhero, int win)
{
	WXHSLJFightResultLayer *pRet = new WXHSLJFightResultLayer();
	if (pRet && pRet->init(myfinalhero, win))
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

time_t WXHSLJFightResultLayer::getNowTime()
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
bool WXHSLJFightResultLayer::init(int myfinalhero, int win)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("playerChallengeResultLayer.csb");
	m_csbnode->setPositionY(100);
	this->addChild(m_csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(WXHSLJFightResultLayer::onBack, this));

	cocos2d::ui::Text *title = (cocos2d::ui::Text*)m_csbnode->getChildByName("title");
	title->setString(CommonFuncs::gbk2utf("华山论剑"));

	std::string str = StringUtils::format("ui/tophero%d.png", myfinalhero);
	cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("heroimg");
	headimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	headimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	str = StringUtils::format("ui/wintitle%d.png", win);
	cocos2d::ui::ImageView* wintitle = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("wintitle");
	wintitle->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	winicon = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("winicon");
	winicon->setVisible(false);

	str = StringUtils::format("ui/winbox%d.png", win);
	cocos2d::ui::ImageView* winbox = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("winbox");
	winbox->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	wintext = (cocos2d::ui::Text*)m_csbnode->getChildByName("wintext");
	wintext->setVisible(false);

	cocos2d::ui::Text* explbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("explbl");

	ranknum = (cocos2d::ui::TextBMFont*)m_csbnode->getChildByName("ranknum");
	ranknum->setVisible(false);

	int myherocount = 0;
	std::vector<std::string> vec_ids = GlobalData::getSaveListId();

	std::vector<std::string> vec_userid;
	for (unsigned int i = 0; i < vec_ids.size(); i++)
	{
		std::string saveuid = vec_ids[i];
		if (saveuid.length() > 0)
			myherocount++;
	}

	int curexp = 0;

	rankup = (cocos2d::ui::Text*)m_csbnode->getChildByName("rankup");
	rankup->setVisible(false);
	if (win == 1)
	{
		curexp = 10 + GlobalData::vec_matchPlayerData.size();
		str = StringUtils::format("+%d", curexp);
	}
	else
	{
		curexp = -(10 + myherocount);
		str = StringUtils::format("%d", curexp);
	}
	explbl->setString(str);
	
	
	WaitingProgress* waitbox = WaitingProgress::create("数据处理中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

	ServerDataSwap::init(this)->getMatchFightResult(GlobalData::matchPlayerInfo.playerid, curexp);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
long long WXHSLJFightResultLayer::getNowTimeMs() {
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


void WXHSLJFightResultLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		WXHSLJMainLayer* alayer = WXHSLJMainLayer::create();
		g_gameLayer->addChild(alayer, 5, "hsljmainlayer");
		this->removeFromParentAndCleanup(true);
	}
}

void WXHSLJFightResultLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	showResult();
}

bool WXHSLJFightResultLayer::isBeforeToday(time_t sec) {
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

void WXHSLJFightResultLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}

long long WXHSLJFightResultLayer::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}

void WXHSLJFightResultLayer::showResult()
{
	std::string str1;
	std::string str2;
	std::string str3;

	int up = GlobalData::myMatchInfo.afterrank - GlobalData::myMatchInfo.beforerank;
	if (up <= 0)
	{
		str1 = CommonFuncs::gbk2utf("排名上升");
		str2 = "ui/winicon1.png";
		str3 = StringUtils::format("%d", -up);
	}
	else
	{
		str1 = CommonFuncs::gbk2utf("排名下降");
		str2 = "ui/winicon0.png";
		str3 = StringUtils::format("%d", up);
	}
	wintext->setVisible(true);
	wintext->setString(str1);
	winicon->setVisible(true);
	winicon->loadTexture(str2, cocos2d::ui::TextureResType::PLIST);
	rankup->setVisible(true);
	rankup->setString(str3);

	ranknum->setVisible(true);
	_myrank = GlobalData::myMatchInfo.afterrank;
	_visualmyrank = GlobalData::myMatchInfo.beforerank;

	showRank(0);
}


bool WXHSLJFightResultLayer::getRandomBoolean(float rate) {

	int rate10 = (int)(rate*10.0);
	int randNum = rand();
	if (randNum % 10 <= rate10) {
		return true;
	}
	else {
		return false;
	}
}

void WXHSLJFightResultLayer::showRank(float dt)
{

	float step = (_myrank - _visualmyrank) * 0.2f;
	if (fabs(step) < 0.21)
	{
		step = 0.21 * ((step > 0) ? 1 : -1);
	}
	_visualmyrank += step;

	if (fabs(_myrank - _visualmyrank) < 1.0f) {
		_visualmyrank = _myrank;
		this->unschedule(schedule_selector(WXHSLJFightResultLayer::showRank));
	}

	std::string rankstr = StringUtils::format("%d", (int)_visualmyrank);
	ranknum->setString(rankstr);

}

bool WXHSLJFightResultLayer::getRandomBoolean() {

	if (0 == rand() % 2) {
		return true;
	}
	else {
		return false;
	}
}

int WXHSLJFightResultLayer::getRandomNum(int range) {

	if (range <= 0) {
		return 0;
	}

	return rand() % range;
}


int WXHSLJFightResultLayer::getRandomNum(int rangeStart, int rangeEnd) {

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

void WXHSLJFightResultLayer::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}
void WXHSLJFightResultLayer::shake(Node * node) {
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
