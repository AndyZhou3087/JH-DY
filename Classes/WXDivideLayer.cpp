#include "WXDivideLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "Const.h"

WXDivideLayer::WXDivideLayer()
{

}


WXDivideLayer::~WXDivideLayer()
{

}

void WXDivideLayer::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}
bool WXDivideLayer::init(BuildActionData* bdata)
{

	m_colorlayer = LayerColor::create(Color4B(11, 32, 22, 200));
	m_colorlayer->setVisible(false);
	this->addChild(m_colorlayer);

	m_csbnode = CSLoader::createNode("divedeLayer.csb");
	this->addChild(m_csbnode);



	m_dividetext = (cocos2d::ui::Widget*)m_csbnode->getChildByName("dividetext");
	m_dividetext->setVisible(false);
	m_desctext = (cocos2d::ui::Text*)m_csbnode->getChildByName("textlbl");
	m_desctext->setVisible(false);
	//m_desctext->setTextColor(Color4B(0, 0, 0, 255));

	m_bdata = bdata;
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

time_t WXDivideLayer::getNowTime()
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

void WXDivideLayer::showContent()
{
	int startx[] = { 360, 300, 240, 180, 120 };
	int spacex[] = { 120, 120, 120, 120, 120 };

	int datasize = m_bdata->res.size();
	for (int i = 0; i < datasize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);
		box->setPosition(Vec2(startx[datasize - 1] + i*spacex[datasize - 1], 300));
		this->addChild(box);

		int rid = m_bdata->res[i] / 1000;
		std::string str = StringUtils::format("ui/%d.png", rid);
		Sprite* res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().width / 2));
		box->addChild(res);
		box->setScale(0.6f);

		std::string strid = StringUtils::format("%d", rid);
		Label * namelbl = Label::createWithTTF(GlobalData::map_allResource[strid].cname, "fonts/STXINGKA.TTF", 25);
		namelbl->setColor(Color3B(255, 255, 255));
		namelbl->setPosition(Vec2(box->getPositionX(), 245));
		this->addChild(namelbl);

		std::string strcount = StringUtils::format("x%d", m_bdata->res[i] % 1000/2);
		Label * coutlbl = Label::createWithTTF(strcount, "fonts/STXINGKA.TTF", 25);//Label::createWithSystemFont(strcount, "", 25);
		coutlbl->enableOutline(Color4B(0, 0, 0, 255), 3);
		coutlbl->setAnchorPoint(Vec2(0, 0.5f));
		coutlbl->setColor(Color3B(255, 255, 255));
		coutlbl->setPosition(Vec2(box->getPositionX() + 40, 270));
		this->addChild(coutlbl);
	}
	m_dividetext->setVisible(true);
	m_desctext->setVisible(true);
	m_colorlayer->setVisible(true);
	m_dividetext->runAction(Spawn::create(MoveTo::create(3.0f, Vec2(360, 700)), FadeOut::create(3.0f), NULL));

	this->scheduleOnce(schedule_selector(WXDivideLayer::removeSelf), 3.0f);
}

long long WXDivideLayer::getNowTimeMs() {
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

void WXDivideLayer::removeSelf(float dt)
{
	this->removeFromParentAndCleanup(true);
}

bool WXDivideLayer::isBeforeToday(time_t sec) {
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


WXDivideLayer* WXDivideLayer::create(BuildActionData* bdata)
{
	WXDivideLayer *pRet = new WXDivideLayer();
	if (pRet && pRet->init(bdata))
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

long long WXDivideLayer::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}