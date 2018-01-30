#include "FactionKickComfirmLayer.h"
#include "SoundManager.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "MD5.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "GameScene.h"
#include "FactionMemberLayer.h"


void FactionKickComfirmLayer::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}

bool FactionKickComfirmLayer::init(Node* target, FactionMemberData *fmdata)
{
    if (!Layer::init()) 
	{
		return false;
    }

	m_data = fmdata;
	m_target = target;

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("factionComfirmLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(FactionKickComfirmLayer::onOk, this));

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(FactionKickComfirmLayer::onCancel, this));

	cocos2d::ui::Text* desctext = (cocos2d::ui::Text*)csbnode->getChildByName("content");
	std::string str = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("是否确认逐出").c_str(), fmdata->nickname.c_str(), CommonFuncs::gbk2utf("？").c_str());

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

FactionKickComfirmLayer* FactionKickComfirmLayer::create(Node* target, FactionMemberData *fmdata)
{
	FactionKickComfirmLayer *pRet = new FactionKickComfirmLayer();
	if (pRet && pRet->init(target, fmdata))
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

time_t FactionKickComfirmLayer::getNowTime()
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


void FactionKickComfirmLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		WaitingProgress* waitbox = WaitingProgress::create("处理中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->kickFaction(m_data->factionid, m_data->userid, m_data->herotype);
	}
}

void FactionKickComfirmLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}


long long FactionKickComfirmLayer::getNowTimeMs() {
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

void FactionKickComfirmLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}


bool FactionKickComfirmLayer::isBeforeToday(time_t sec) {
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

void FactionKickComfirmLayer::onSuccess()
{
	for (unsigned int i = 0; i < GlobalData::vec_factionListData.size(); i++)
	{
		if (GlobalData::vec_factionListData[i].id == m_data->factionid)
		{
			GlobalData::vec_factionListData[i].membercount--;
			break;
		}
	}
	FactionMemberItem* fmitem = (FactionMemberItem*)m_target;
	fmitem->removeItem();
	removSelf();
}

long long FactionKickComfirmLayer::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}


void FactionKickComfirmLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据处理异常，请检查网络连接！！"));
	Director::getInstance()->getRunningScene()->addChild(box,1);
	removSelf();
}

