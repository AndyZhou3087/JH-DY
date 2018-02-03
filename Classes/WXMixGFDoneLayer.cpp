#include "WXMixGFDoneLayer.h"
#include "GlobalData.h"
#include "Const.h"
WXMixGFDoneLayer::WXMixGFDoneLayer()
{

}


WXMixGFDoneLayer::~WXMixGFDoneLayer()
{
}

void WXMixGFDoneLayer::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}

bool WXMixGFDoneLayer::init(int ret)
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("mixDoneLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::ImageView* doneimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("doneimg");
	std::string str;
	if (ret == 0){
		//str = StringUtils::format("images/mixdoneok.png");
		doneimg->setVisible(false);
		Node* csbnode = CSLoader::createNode("sgdc.csb");
		csbnode->setPosition(Vec2(360, 600));
		this->addChild(csbnode, 0, "sgdc");
		auto action = CSLoader::createTimeline("sgdc.csb");
		csbnode->runAction(action);
		action->gotoFrameAndPlay(0, false);
	}
	else{
		doneimg->setVisible(true);
		str = StringUtils::format("images/mixdonefail.png");
		doneimg->loadTexture(str, cocos2d::ui::Widget::TextureResType::LOCAL);
	}

	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleOnce(schedule_selector(WXMixGFDoneLayer::removeself), 3.0f);

	return true;
}

time_t WXMixGFDoneLayer::getNowTime()
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

WXMixGFDoneLayer* WXMixGFDoneLayer::create(int ret)
{
	WXMixGFDoneLayer *pRet = new WXMixGFDoneLayer();
	if (pRet && pRet->init(ret))
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

long long WXMixGFDoneLayer::getNowTimeMs() {
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


void WXMixGFDoneLayer::removeself(float dt)
{
	this->removeFromParentAndCleanup(true);
}

bool WXMixGFDoneLayer::isBeforeToday(time_t sec) {
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

long long WXMixGFDoneLayer::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}