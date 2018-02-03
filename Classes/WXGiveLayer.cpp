#include "WXGiveLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "MyPackage.h"
#include "Const.h"
#include "GameScene.h"
#include "StorageRoom.h"
#include "SoundManager.h"
#include "WXNpcLayer.h"
#include "MyMenu.h"
#include "GameDataSave.h"
#include "Winlayer.h"

WXGiveLayer::WXGiveLayer()
{
	lastMyGoodsSrollViewHeight = -1;
	lastGiveGoodsSrollViewHeight = -1;
}


WXGiveLayer::~WXGiveLayer()
{

}

void WXGiveLayer::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}
WXGiveLayer* WXGiveLayer::create(std::string npcid, std::string addrid)
{
	WXGiveLayer *pRet = new WXGiveLayer();
	if (pRet && pRet->init(npcid, addrid))
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

time_t WXGiveLayer::getNowTime()
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

bool WXGiveLayer::init(std::string npcid, std::string addrid)
{
	Node* csbnode = CSLoader::createNode("giveLayer.csb");
	this->addChild(csbnode);

	m_npcid = npcid;
	m_addrid = addrid;

	m_backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(WXGiveLayer::onBack, this));

	m_givebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("givebtn");
	m_givebtn->addTouchEventListener(CC_CALLBACK_2(WXGiveLayer::onGive, this));

	cocos2d::ui::ImageView* npchead = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcicon");
	std::string npcheadstr = StringUtils::format("ui/%s.png", m_npcid.c_str());
	npchead->loadTexture(npcheadstr, cocos2d::ui::TextureResType::PLIST);
	npchead->setScale(0.6f);

	cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)csbnode->getChildByName("npcname");
	npcname->setString(GlobalData::map_npcs[npcid].name);

	m_giveGoodsSrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("givescroll");
	m_giveGoodsSrollView->setScrollBarEnabled(false);
	m_myGoodsSrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("mygoodsscroll");
	m_myGoodsSrollView->setScrollBarEnabled(false);

	giveval = 0;
	friendly = GlobalData::map_myfriendly[m_npcid].friendly;
	friendlylbl = (cocos2d::ui::Text*)csbnode->getChildByName("friendly");

	if (friendly < -100000 || friendly > 100000)
		friendly = 0;

	std::string friendstr = StringUtils::format("%d", friendly);
	friendlylbl->setString(friendstr);


	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		myGoodsData.push_back(MyPackage::vec_packages[i]);
	}
	updataMyGoodsUI();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

long long WXGiveLayer::getNowTimeMs() {
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

void WXGiveLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}


bool WXGiveLayer::isBeforeToday(time_t sec) {
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
void WXGiveLayer::onGiveGoodsItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();

	int datatag = node->getTag();

	PackageData udata = *data;
	udata.count = 1;

	if (--data->count <= 0)
	{
		myGiveData.erase(myGiveData.begin() + datatag);
	}
	updateMyGoods(udata);

	updata();
}

void WXGiveLayer::updateMyGoods(PackageData data)
{

	unsigned int i = 0;
	for (i = 0; i < myGoodsData.size(); i++)
	{
		if (data.strid.compare(myGoodsData[i].strid) == 0 && (myGoodsData[i].type == FOOD || myGoodsData[i].type == MEDICINAL || myGoodsData[i].type == RES_1 || myGoodsData[i].type == RES_2))
		{
			if (myGoodsData[i].count < 10)
			{
				myGoodsData[i].count++;
				break;
			}
			else
				continue;
		}
	}
	if (i == myGoodsData.size())
	{
		data.count = 1;
		myGoodsData.push_back(data);
	}

}

long long WXGiveLayer::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}

bool WXGiveLayer::getRandomBoolean(float rate) {

	int rate10 = (int)(rate*10.0);
	int randNum = rand();
	if (randNum % 10 <= rate10) {
		return true;
	}
	else {
		return false;
	}
}

bool WXGiveLayer::getRandomBoolean() {

	if (0 == rand() % 2) {
		return true;
	}
	else {
		return false;
	}
}

int WXGiveLayer::getRandomNum(int range) {

	if (range <= 0) {
		return 0;
	}

	return rand() % range;
}
void WXGiveLayer::updateGiveGoods(PackageData data)
{

	unsigned int i = 0;
	for (i = 0; i < myGiveData.size(); i++)
	{
		if (data.strid.compare(myGiveData[i].strid) == 0 && (myGiveData[i].type == FOOD || myGiveData[i].type == MEDICINAL || myGiveData[i].type == RES_1 || myGiveData[i].type == RES_2))
		{
			myGiveData[i].count++;
			break;
		}
	}
	if (i == myGiveData.size())
	{
		data.count = 1;
		myGiveData.push_back(data);
	}

}

void WXGiveLayer::onMyGoodsItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();
	int datatag = node->getTag();

	PackageData udata = *data;
	udata.count = 1;

	if (--data->count <= 0)
	{
		myGoodsData.erase(myGoodsData.begin() + datatag);
	}
	updateGiveGoods(udata);
	updata();
}

int WXGiveLayer::getRandomNum(int rangeStart, int rangeEnd) {

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

void WXGiveLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
void WXGiveLayer::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void WXGiveLayer::shake(Node * node) {
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
void WXGiveLayer::removeSelf(float dt)
{
	this->removeFromParentAndCleanup(true);
}

void WXGiveLayer::onGive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_backbtn->setEnabled(false);
		m_givebtn->setEnabled(false);
		MyPackage::vec_packages.clear();

		for (unsigned int i = 0; i < myGoodsData.size(); i++)
		{
			MyPackage::vec_packages.push_back(myGoodsData[i]);
		}
		GlobalData::map_myfriendly[m_npcid].friendly = friendly + giveval;
		GlobalData::saveFriendly();
		MyPackage::save();
		WXNpcLayer* npclayer = (WXNpcLayer*)g_gameLayer->getChildByName("npclayer");
		npclayer->reFreshFriendlyUI();
		doGiveMission();
	}
}


void WXGiveLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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

void WXGiveLayer::checkValue()
{
	giveval = 0;

	for (unsigned int i = 0; i < myGiveData.size(); i++)
	{
		std::string resid = myGiveData[i].strid;
		giveval += GlobalData::map_allResource[resid].fval * myGiveData[i].count;
	}
	std::string friendstr = StringUtils::format("%d", friendly + giveval);
	friendlylbl->setString(friendstr);
}
void WXGiveLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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
void WXGiveLayer::updata()
{
	updataMyGoodsUI();
	updataGiveGoodsUI();
	checkValue();
}
void WXGiveLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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


void WXGiveLayer::updataMyGoodsUI()
{
	int size = myGoodsData.size();

	m_myGoodsSrollView->removeAllChildrenWithCleanup(true);

	int row = size % 5 == 0 ? size / 5 : (size / 5 + 1);

	int innerheight = row * 140;

	if (lastMyGoodsSrollViewHeight != innerheight)
	{
		lastMyGoodsSrollViewHeight = innerheight;
		int contentheight = m_myGoodsSrollView->getContentSize().height;
		if (innerheight < contentheight)
			innerheight = contentheight;
		m_myGoodsSrollView->setInnerContainerSize(Size(m_myGoodsSrollView->getContentSize().width, innerheight));
	}

	std::vector<PackageData*> allMydata;
	for (unsigned int i = 0; i < myGoodsData.size(); i++)
	{
		allMydata.push_back(&myGoodsData[i]);
	}

	int allsize = allMydata.size();
	for (int i = 0; i < allsize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData* tmpdata = allMydata[i];
		if (tmpdata->type == WEAPON || tmpdata->type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata->strid].qu);
		}
		else if (tmpdata->type == N_GONG || tmpdata->type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata->strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(WXGiveLayer::onMyGoodsItem, this));
		boxItem->setUserData(allMydata[i]);
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 10 + i % 5 * 125, innerheight - boxItem->getContentSize().height / 2 - i / 5 * 140));
		MyMenu* menu = MyMenu::create();
		menu->addChild(boxItem);
		menu->setTouchlimit(m_myGoodsSrollView);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		m_myGoodsSrollView->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", tmpdata->strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		str = StringUtils::format("%d", tmpdata->count);
		Label * reslbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 18);//Label::createWithSystemFont(str, "", 18);
		reslbl->enableOutline(Color4B(0, 0, 0, 255), 3);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}

void WXGiveLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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


void WXGiveLayer::updataGiveGoodsUI()
{
	int size = myGiveData.size();

	m_giveGoodsSrollView->removeAllChildrenWithCleanup(true);

	int row = size % 5 == 0 ? size / 5 : (size / 5 + 1);

	int innerheight = m_giveGoodsSrollView->getInnerContainerSize().height;
	if (lastGiveGoodsSrollViewHeight < 0)
	{
		innerheight = row * 140;
		int contentheight = m_giveGoodsSrollView->getContentSize().height;
		if (innerheight < contentheight)
			innerheight = contentheight;
		lastGiveGoodsSrollViewHeight = innerheight;
		m_giveGoodsSrollView->setInnerContainerSize(Size(m_giveGoodsSrollView->getContentSize().width, innerheight));
	}

	std::vector<PackageData*> allNpcdata;
	for (unsigned int i = 0; i < myGiveData.size(); i++)
	{
		allNpcdata.push_back(&myGiveData[i]);
	}

	int allsize = allNpcdata.size();
	for (int i = 0; i < allsize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData *tmpdata = allNpcdata[i];
		if (tmpdata->type == WEAPON || tmpdata->type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata->strid].qu);
		}
		else if (tmpdata->type == N_GONG || tmpdata->type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata->strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(WXGiveLayer::onGiveGoodsItem, this));
		boxItem->setUserData(allNpcdata[i]);
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 10 + i % 5 * 125, innerheight - boxItem->getContentSize().height / 2 - i / 5 * 140));
		MyMenu* menu = MyMenu::create();
		menu->addChild(boxItem);
		menu->setTouchlimit(m_giveGoodsSrollView);

		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		m_giveGoodsSrollView->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", tmpdata->strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", tmpdata->count);
		Label * reslbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 18);//Label::createWithSystemFont(str, "", 18);
		reslbl->enableOutline(Color4B(0, 0, 0, 255), 3);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}

void WXGiveLayer::doGiveMission()
{
	bool isAnim = false;
	std::string curmid = GlobalData::getCurBranchPlotMissison();
	std::map<std::string, int> map_needGoods;
	std::map<std::string, int>::iterator it;
	
	if (curmid.length() > 0)
	{
		int subindex = GlobalData::map_BranchPlotMissionItem[curmid].subindex;
		PlotMissionData pd = GlobalData::map_BranchPlotMissionData[curmid][subindex];
		std::vector<std::string> vec_rwdres = pd.rewords;
		std::string savedgstr = GameDataSave::getInstance()->getBranchPlotMissionGiveGoods();
		std::vector<std::string> needgoods;
		if (savedgstr.length() > 0)
		{
			std::vector<std::string> vec_retstr;
			CommonFuncs::split(savedgstr, needgoods, ";");
		}
		else
		{
			needgoods = pd.needgoods;
		}

		if (pd.status == M_DOING && pd.type == 2 && GlobalData::map_BranchPlotMissionItem[curmid].count > 0)
		{
			for (unsigned int i = 0; i < needgoods.size(); i++)
			{
				std::string resid = needgoods[i];
				int intresid = atoi(resid.c_str());
				int count = 1;
				if (intresid > 0)
				{
					resid = StringUtils::format("%d", intresid / 1000);
					count = intresid % 1000;
				}
				map_needGoods[resid] = count;
			}
			for (unsigned int i = 0; i < myGiveData.size(); i++)
			{
				for (it = map_needGoods.begin(); it != map_needGoods.end(); it++)
				{
					if (myGiveData[i].strid.compare(it->first) == 0)
					{
						if (myGiveData[i].count >= map_needGoods[it->first])
						{
							map_needGoods.erase(it);
							break;
						}
						else
						{
							map_needGoods[it->first] -= myGiveData[i].count;
						}
					}
				}
			}
			if (needgoods.size() > 0)
			{
				if (map_needGoods.size() <= 0)
				{
					//完成
					int subindex = GlobalData::map_BranchPlotMissionItem[curmid].subindex;
					GlobalData::map_BranchPlotMissionData[curmid][subindex].status = M_NONE;

					giveRes(GlobalData::map_BranchPlotMissionData[curmid][subindex].rewords);

					if (subindex + 1 >= GlobalData::map_BranchPlotMissionData[curmid].size())
					{
						GlobalData::map_BranchPlotMissionItem[curmid].subindex = 0;
						GlobalData::map_BranchPlotMissionItem[curmid].count--;
						GlobalData::map_BranchPlotMissionItem[curmid].time = GlobalData::map_BranchPlotMissionItem[curmid].maxtime;
						GlobalData::saveBranchPlotMissionStatus("", 0);
					}
					else
					{
						GlobalData::map_BranchPlotMissionItem[curmid].subindex++;
						GlobalData::saveBranchPlotMissionStatus(curmid, M_NONE);
					}

					GameDataSave::getInstance()->setBranchPlotMissionGiveGoods("");
					Winlayer::showMissionAnim(this, "任务完成", vec_rwdres);
					isAnim = true;
				}
				else
				{
					std::string str;
					//未完成
					for (it = map_needGoods.begin(); it != map_needGoods.end(); it++)
					{
						std::string tempstr;
						int intresid = atoi(it->first.c_str());
						if (intresid > 0)
							tempstr = StringUtils::format("%d;", intresid * 1000 + map_needGoods[it->first]);
						else
							tempstr = StringUtils::format("%s;", it->first.c_str());

						str.append(tempstr);
					}
					if (str.length() > 0)
					{
						GameDataSave::getInstance()->setBranchPlotMissionGiveGoods(str.substr(0, str.length() - 1));
					}
				}
			}
		}
	}

	if (isAnim)
	{
		this->scheduleOnce(schedule_selector(WXGiveLayer::removeSelf), 2.5f);
	}
	else
	{
		removeSelf(0);
	}
}

void WXGiveLayer::giveRes(std::vector<std::string> vec_res)
{
	WXNpcLayer::getWinRes(vec_res, m_addrid);
}
