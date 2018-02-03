#include "WXFactionComfirmLayer.h"
#include "SoundManager.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "MD5.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "GameScene.h"
#include "WXFactionMainLayer.h"
#include "WXFactionMemberLayer.h"

bool WXFactionComfirmLayer::init(FactionListData *fldata)
{
    if (!Layer::init()) 
	{
		return false;
    }
	f_action = F_NONE;
	m_fldata = fldata;

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("factionComfirmLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(WXFactionComfirmLayer::onOk, this));

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(WXFactionComfirmLayer::onCancel, this));

	cocos2d::ui::Text* desctext = (cocos2d::ui::Text*)csbnode->getChildByName("content");
	std::string str;
	if (GlobalData::mytitle == 1)
	{
		str = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("是否确认花费50金元宝解散").c_str(), m_fldata->factionname.c_str(), CommonFuncs::gbk2utf("？").c_str());
	}
	else
	{
		str = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("是否确认退出").c_str(), m_fldata->factionname.c_str(), CommonFuncs::gbk2utf("？").c_str());

	}
	desctext->setString(str);
	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	//点击任何位置移除掉
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		return;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void WXFactionComfirmLayer::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}

WXFactionComfirmLayer* WXFactionComfirmLayer::create(FactionListData *fldata)
{
	WXFactionComfirmLayer *pRet = new WXFactionComfirmLayer();
	if (pRet && pRet->init(fldata))
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

time_t WXFactionComfirmLayer::getNowTime()
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

void WXFactionComfirmLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::mytitle == 1)
		{
			int mygold = GlobalData::getMyGoldCount();

			if (mygold >= 50)
			{
				if (GlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
				{
					GlobalData::dataIsModified = true;
					GlobalData::setMyGoldCount(0);
					HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
					this->addChild(hint);
					return;
				}

				f_action = F_RELEASE;
				WaitingProgress* waitbox = WaitingProgress::create("处理中...");
				Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
				ServerDataSwap::init(this)->leaveFaction(1, m_fldata->id, g_hero->getHeadID());
			}
			else
			{
				HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("金元宝不足！！"));
				Director::getInstance()->getRunningScene()->addChild(hint, 1);
				removSelf();
			}
		}
		else
		{
			f_action = F_LEAVE;

			WaitingProgress* waitbox = WaitingProgress::create("加载中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
			ServerDataSwap::init(this)->leaveFaction(0, m_fldata->id, g_hero->getHeadID());
		}
	}
}

long long WXFactionComfirmLayer::getNowTimeMs() {
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

void WXFactionComfirmLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void WXFactionComfirmLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

void WXFactionComfirmLayer::onSuccess()
{
	if (f_action == F_RELEASE)
	{
		GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - 50);
		int usegold = GlobalData::getUseGold() + 50;
		GlobalData::setUseGold(usegold);
	}
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	GlobalData::myFaction = 0;
	GlobalData::mytitle = 0;
	f_action = F_NONE;
	removSelf();

	WXFactionMemberLayer* flayer = (WXFactionMemberLayer*)g_gameLayer->getChildByName("factionmemberlayer");
	if (flayer != NULL)
	{
		flayer->removeFromParentAndCleanup(true);
	}

	WXFactionMainLayer* fmlayer = (WXFactionMainLayer*)g_gameLayer->getChildByName("factionmainlayer");
	if (fmlayer != NULL)
	{
		fmlayer->getFactionListData();
	}
}

bool WXFactionComfirmLayer::isBeforeToday(time_t sec) {
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
void WXFactionComfirmLayer::onErr(int errcode)
{
	f_action = F_NONE;
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据处理异常，请检查网络连接！！"));
	Director::getInstance()->getRunningScene()->addChild(box,1);
	removSelf();
}

long long WXFactionComfirmLayer::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}
