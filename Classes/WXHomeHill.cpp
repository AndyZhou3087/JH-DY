#include "WXHomeHill.h"
#include "json.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "WXActionGetLayer.h"
#include "Const.h"
#include "GameScene.h"
#include "CommonFuncs.h"
#include "WXFightLayer.h"
#include "SoundManager.h"
#include "MapLayer.h"
#include "NewerGuideLayer.h"
WXHomeHill::WXHomeHill()
{
}


WXHomeHill::~WXHomeHill()
{
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_MAP);
}

bool WXHomeHill::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("homeHillLayer.csb");
	this->addChild(csbnode);

	//返回按钮
	m_backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(WXHomeHill::onBack, this));
	
	int ressize = GlobalData::vec_hillResid.size();
	scrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");
	scrollView->setScrollBarEnabled(false);
	scrollView->setBounceEnabled(true);

	int itemheight = 160;
	int innerheight = itemheight * ressize;
	int contentheight = scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollView->setInnerContainerSize(Size(650, innerheight));

	for (int i = 0; i < ressize; i++)
	{
		//产出资源node
		Node* resnode = CSLoader::createNode("resNode.csb");
		std::string namestr = StringUtils::format("node%d", i);
		scrollView->addChild(resnode,0, namestr);
		resnode->setPosition(Vec2(scrollView->getContentSize().width/2-10, innerheight - i * itemheight - itemheight / 2));

		//资源id
		cocos2d::ui::ImageView* iconimg = (cocos2d::ui::ImageView*)resnode->getChildByName("icon");
		//资源名称
		cocos2d::ui::Text* name = (cocos2d::ui::Text*)resnode->getChildByName("name");
		//产出个数
		cocos2d::ui::Text* count = (cocos2d::ui::Text*)resnode->getChildByName("count");
		//产出速度
		cocos2d::ui::Text* speed = (cocos2d::ui::Text*)resnode->getChildByName("speed");
		//等待时间，采完之后显示，恢复满
		cocos2d::ui::Text* waittime = (cocos2d::ui::Text*)resnode->getChildByName("waittime");
		//资源描述
		cocos2d::ui::Text* desc = (cocos2d::ui::Text*)resnode->getChildByName("desc");
		cocos2d::ui::Text* waittext = (cocos2d::ui::Text*)resnode->getChildByName("waittext");
		
		//操作按钮
		cocos2d::ui::Button* actionbtn = (cocos2d::ui::Button*)resnode->getChildByName("actionbtn");
		actionbtn->addTouchEventListener(CC_CALLBACK_2(WXHomeHill::onclick, this));
		cocos2d::ui::Text* antionbtnlabel = (cocos2d::ui::Text*)actionbtn->getChildByName("text");

		for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
		{
			ResData data = GlobalData::vec_resData[m];
			if (GlobalData::vec_hillResid[i].compare(data.strid) == 0)
			{
				actionbtn->setUserData((void*)GlobalData::vec_resData[m].strid.c_str());
				std::string str = StringUtils::format("ui/%s.png", data.strid.c_str());
				iconimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
				iconimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

				name->setString(GlobalData::map_allResource[data.strid].cname);
				desc->setString(GlobalData::map_allResource[data.strid].desc);

				str = StringUtils::format("%d", data.count);
				count->setString(str);

				float fs = (data.speed[g_nature->getReason()]) / 60.0f;
				str = StringUtils::format("%.1fh", fs);
				speed->setString(str);

				float wfs = fs * data.max;
				str = StringUtils::format("%.1fh", wfs);
				waittime->setString(str);

				if (data.count <= 0)
				{
					waittext->setVisible(true);
					waittime->setVisible(true);
					actionbtn->setEnabled(false);
				}
				else
				{
					waittext->setVisible(false);
					waittime->setVisible(false);
					actionbtn->setEnabled(true);
				}

				//actionbtn->setTitleText(CommonFuncs::gbk2utf(acname[data.actype].c_str()));
				antionbtnlabel->setString(CommonFuncs::gbk2utf(acname[data.actype].c_str()));
				
			}
		}

	}
	this->schedule(schedule_selector(WXHomeHill::updateUI),0.2f);


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_ENTER_MAPADDR);

	return true;
}
void WXHomeHill::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}
void WXHomeHill::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	showNewerGuide(18);
}
time_t WXHomeHill::getNowTime()
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

void WXHomeHill::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (g_maplayer != NULL)
			g_maplayer->checkNewerGuide();
		this->removeFromParentAndCleanup(true);
	}
}
long long WXHomeHill::getNowTimeMs() {
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
void WXHomeHill::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		unsigned int i = 0;
		ResData* data = NULL;
		for (i = 0; i<GlobalData::vec_resData.size(); i++)
		{
			data = &GlobalData::vec_resData[i];
			std::string strid = (char*)node->getUserData();
			if (data->strid.compare(strid) == 0)
				break;
		}

		if (GlobalData::vec_resData[i].count > 0)
		{
			GlobalData::vec_resData[i].count--;

			if (data->strid.compare("67") == 0 || data->strid.compare("68") == 0)//兔子，狼战斗界面
			{
				std::string npcid = "n002";
				if (data->strid.compare("68") == 0)
					npcid = "n003";
				WXFightLayer* layer = WXFightLayer::create("m1-2", npcid);
				if (g_gameLayer != NULL)
					g_gameLayer->addChild(layer, 4, "fightlayer");
			}
			else
			{
				WXActionGetLayer* layer = WXActionGetLayer::create(i, data->res, data->type, data->actype);
				if (g_gameLayer != NULL)
					g_gameLayer->addChild(layer, 2, "WXActionGetLayer");
			}


		}
	}
}

bool WXHomeHill::isBeforeToday(time_t sec) {
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
void WXHomeHill::updateUI(float dt)
{
	for (unsigned int i = 0; i < GlobalData::vec_hillResid.size(); i++)
	{
		std::string namestr = StringUtils::format("node%d", i);
		Node* resnode = scrollView->getChildByName(namestr);

		cocos2d::ui::Text* count = (cocos2d::ui::Text*)resnode->getChildByName("count");
		cocos2d::ui::Text* speed = (cocos2d::ui::Text*)resnode->getChildByName("speed");
		cocos2d::ui::Text* waittime = (cocos2d::ui::Text*)resnode->getChildByName("waittime");
		cocos2d::ui::Text* waittext = (cocos2d::ui::Text*)resnode->getChildByName("waittext");
		cocos2d::ui::Button* actionbtn = (cocos2d::ui::Button*)resnode->getChildByName("actionbtn");
		for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
		{
			ResData* data = &GlobalData::vec_resData[m];
			if (GlobalData::vec_hillResid[i].compare(data->strid) == 0)
			{
				std::string str = StringUtils::format("%d", data->count);
				count->setString(str);
				str = StringUtils::format("%.1fh", data->speed[g_nature->getReason()] / 60.f);
				speed->setString(str);

				str = StringUtils::format("%.1fh", (data->speed[g_nature->getReason()] * data->max - data->waittime) / 60.f);
				waittime->setString(str);
	
				if (data->count <= 0)
				{
					waittext->setVisible(true);
					waittime->setVisible(true);
					count->setColor(Color3B::RED);
					actionbtn->setEnabled(false);
				}
				else
				{
					waittext->setVisible(false);
					waittime->setVisible(false);
					count->setColor(Color3B::BLACK);
					actionbtn->setEnabled(true);
				}
			}
		}
	}
}

long long WXHomeHill::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}
void WXHomeHill::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void WXHomeHill::shake(Node * node) {
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

void WXHomeHill::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 18)
	{
		Node* resnode = scrollView->getChildByName("node0");
		NewerGuideLayer::pushUserData("normalbtn");
		nodes.push_back(resnode->getChildByName("actionbtn"));
	}
	else if (step == 28)
	{
		scrollView->jumpToPercentVertical(20);
		Node* resnode = scrollView->getChildByName("node4");
		NewerGuideLayer::pushUserData("normalbtn");
		nodes.push_back(resnode->getChildByName("actionbtn"));
	}
	else if (step == 37)
	{
		nodes.push_back(m_backbtn);
		NewerGuideLayer::pushUserData("backbtn");
	}
	if (step == 18 || step == 28 || step == 37)
		g_gameLayer->showNewerGuide(step, nodes);
}
bool WXHomeHill::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
}

void WXHomeHill::jumpDown(cocos2d::Node *node, float dt) {
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