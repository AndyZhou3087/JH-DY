#include "GameScene.h"
#include "TopBar.h"
#include "UIScroll.h"
#include "GameDataSave.h"
#include "StorageRoom.h"
#include "GlobalData.h"
#include "MapLayer.h"
#include "CommonFuncs.h"
#include "ReviveLayer.h"
#include "NewerGuideLayer.h"
#include "WXActivitScene.h"
#include "Const.h"
#include "HeroProperNode.h"
#include "MyParticle.h"
#include "WXGetVipRewardLayer.h"
#include "MD5.h"
#include "StartScene.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "WXLoginRewardLayer.h"
#include "WXAchiveDoneAnimLayer.h"
#include "FrozenLayer.h"
#include "CannotTouchLayer.h"

USING_NS_CC;

Nature* g_nature;
Hero* g_hero;

GameScene* g_gameLayer = NULL;

UIScroll* g_uiScroll;
GameScene::GameScene()
{
	issavedata = true;
	isAnewGetData = false;
	homeLayer = NULL;
}

void GameScene::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}

GameScene::~GameScene()
{
	//退出保存数据
	if (issavedata)
		saveAllData();

	GameDataSave::purgeGameSave();
	g_gameLayer = NULL;
	g_nature = NULL;
	g_hero = NULL;
	issavedata = true;
	isAnewGetData = false;
}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	g_gameLayer = GameScene::create();

    // add layer as a child to scene
	scene->addChild(g_gameLayer);

    // return the scene
    return scene;
}

time_t GameScene::getNowTime()
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

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	GlobalData::dataIsModified = false;

	//读取后山资源列表配置文件
	GlobalData::loadHillResJsonData();

	//读取地图配置文件
	GlobalData::loadMapJsonData();

	//读取NPC配置文件
	GlobalData::loadNpcJsonData();

	//读取武功招式配置文件
	GlobalData::loadGftrickData();

	//读取技能配置文件
	GlobalData::loadGfskillData();

	//读取商城数据
	GlobalData::loadShopData();

	//加载武道大会挑战奖励数据
	GlobalData::loadChallengeRewardData();

	GlobalData::loadTempGF_EquipData();
	GlobalData::loadNpcFriendJsonData();
	GlobalData::loadNpcMasterJsonData();
	GlobalData::loadFriendly();

	//读取成就配置文件
	GlobalData::loadAchiveJsonData();

	GlobalData::getAchiveData();
	GlobalData::getAchiveAnimData();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	GlobalData::g_gameStatus = GAMESTART;
	//天气，季节数据
	g_nature = Nature::create();
	this->addChild(g_nature);

	loadSaveNatureData();
	//角色数据
	g_hero = Hero::create();
	this->addChild(g_hero);

	loadSaveHeroData();

	//等级大于1级跳过新手引导
	if (g_hero->getLVValue() >= 1)
	{
		for (int i = 0; i < 68; i++)
		{
			GameDataSave::getInstance()->setIsNewerGuide(i, 0);
		}
	}

	//读取剧情配置文件
	GlobalData::loadPlotMissionJsonData(g_hero->getHeadID());

	//读取支线剧情配置文件
	GlobalData::loadBranchPlotMissionJsonData();


	checkRestBranchMisson();

	//角色目前在哪个地点，第一次进入家
	std::string addrstr = GameDataSave::getInstance()->getHeroAddr();
	if (addrstr.compare("m1-1") == 0)//家
	{
		homeLayer = HomeLayer::create();
		addChild(homeLayer, 1, "homelayer");
	}
	else//其他地图界面
	{
		g_maplayer = MapLayer::create();
		addChild(g_maplayer, 1, "maplayer");
	}

	//任务属性和天气
	topBar = TopBar::create();
	topBar->setPosition(Vec2(visibleSize.width / 2, 1063));
	addChild(topBar, 2, "topbar");

	//滚动文字的背景
	Sprite* bg = Sprite::create("images/topeventwordbox.png");
	bg->setPosition(Vec2(visibleSize.width / 2, 1010));
	this->addChild(bg, 2);

	//滚动文字
	g_uiScroll = UIScroll::create(620.0F, 83.0f);
	g_uiScroll->setPosition(Vec2(visibleSize.width / 2, 1015));
	addChild(g_uiScroll, 2);
	
	this->schedule(schedule_selector(GameScene::updata), 1.0f);
	this->schedule(schedule_selector(GameScene::timerSaveData), 5.0f);
	this->schedule(schedule_selector(GameScene::checkiflive), 0.1f);
	this->schedule(schedule_selector(GameScene::checkAchiveIsDone), 1.0f);
	
	
	//处理制暖
	std::string warmConfigStr = GlobalData::getMakeWarmConfig();
	if (warmConfigStr.length() > 0)
	{
		std::vector<std::string> vec_retstr;
		CommonFuncs::split(warmConfigStr, vec_retstr, "-");
		int startwarmtime = atoi(vec_retstr[0].c_str());
		int warmtime = atoi(vec_retstr[1].c_str());
		int curtimemin = g_nature->getPastDays() * 24 * 60 + g_nature->getTime();
		if (curtimemin - startwarmtime < warmtime)
		{
			g_nature->setIsMaKeWarm(true);
			float wpasttime = curtimemin - startwarmtime;
			g_nature->setWarmtime(warmtime);
			g_nature->setWarmpasttime(wpasttime);
		}
		else
		{
			g_nature->setTemperature(g_nature->getTemperature() - 15);
			GlobalData::setMakeWarmConfig("");
		}
	}

	GlobalData::doAchive(A_3, StorageRoom::getCountById("80"));
	int goldcount = GlobalData::getMyGoldCount();
	GlobalData::doAchive(A_1, goldcount);

	if (GlobalData::isOnline)
	{
		ServerDataSwap::init(this)->vipIsOn(g_hero->getHeadID());
		ServerDataSwap::init(NULL)->getFactionList();
	}
    return true;
}


long long GameScene::getNowTimeMs() {
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


bool GameScene::isBeforeToday(time_t sec) {
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


void GameScene::loadSaveNatureData()
{
	//设置保存的季节
	g_nature->setReason((EReason)GameDataSave::getInstance()->getNatureReason());
	//设置保存的天气
	g_nature->setWeather((EWeather)GameDataSave::getInstance()->getNatureWeather());
	
	int cdata = GameDataSave::getInstance()->getNatureWeatherChangeCount();
	//设置保存的天气改变的次数
	g_nature->changeWeatherCount = cdata / 10000;
	
	//设置天气改变的随机小时数
	g_nature->changeWeatherRandow = cdata % 10000;

	//设置保存的时间
	g_nature->setTime(GameDataSave::getInstance()->getNatureTime());
	//温度
	g_nature->setTemperature(GameDataSave::getInstance()->getNatureTemperature());
	//天数
	int livedays = GameDataSave::getInstance()->getLiveDays();
	g_nature->setPastDays(livedays);
	GlobalData::setMD5LiveDays(md5(livedays));
	
	//设置黑夜或白天

	if (g_nature->getTime() < 60 * 6 - 1 || g_nature->getTime() >= 18 * 60)
	{
		g_nature->setDayOrNight(Night);
	}
	else
	{
		g_nature->setDayOrNight(Day);
	}
}

long long GameScene::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}

bool GameScene::getRandomBoolean(float rate) {

	int rate10 = (int)(rate*10.0);
	int randNum = rand();
	if (randNum % 10 <= rate10) {
		return true;
	}
	else {
		return false;
	}
}

void GameScene::loadSaveHeroData()
{
	//外伤
	g_hero->setOutinjuryValue(GameDataSave::getInstance()->getHeroOutinjury());
	//内伤
	g_hero->setInnerinjuryValue(GameDataSave::getInstance()->getHeroInnerinjury());
	//饱食度
	g_hero->setHungerValue(GameDataSave::getInstance()->getHeroHunger());
	//精神
	g_hero->setSpiritValue(GameDataSave::getInstance()->getHeroSpirit());
	//角色ID
	int heroid = GameDataSave::getInstance()->getHeroId();
	g_hero->setHeadID(heroid);
	//角色名字
	g_hero->setMyName(GlobalData::map_heroAtr[heroid].name);
	//角色等级
	int lv = GameDataSave::getInstance()->getHeroLV();
	g_hero->setLVValue(lv);
	//角色经验值
	int exp = GameDataSave::getInstance()->getHeroExp();
	g_hero->setExpValue(exp);
	//角色是否在家
	g_hero->setIsOut(GameDataSave::getInstance()->getHeroIsOut());

	//角色生命值
	float hlife = GameDataSave::getInstance()->getHeroLife();
	if (hlife > -1)//-100 新的角色第一次开始玩
		g_hero->setLifeValue(hlife);
	else
	{
		g_hero->setLifeValue(g_hero->getMaxLifeValue());
	}

	int sex = GameDataSave::getInstance()->getHeroSex();
	if (sex < 0)
	{
		if (heroid != 4)
			g_hero->setSex(S_MAN);
		else
			g_hero->setSex(S_WOMEN);
	}
	else
		g_hero->setSex((H_SEX)sex);

	//读取保存的仓库数据
	StorageRoom::loadStorageData();
	//读取保存的背包数据
	MyPackage::load();
	//读取保存的资源数据
	GlobalData::loadResData();
	//读取保存的角色属性数据
	loadSavedHeroPropData();

	getNpcRandMap();
}

bool GameScene::getRandomBoolean() {

	if (0 == rand() % 2) {
		return true;
	}
	else {
		return false;
	}
}

int GameScene::getRandomNum(int range) {

	if (range <= 0) {
		return 0;
	}

	return rand() % range;
}

void GameScene::loadSavedHeroPropData()
{
	//角色佩戴装备
	std::string strval = GlobalData::getHeroProperData();
	std::vector<std::string> tmp;
	CommonFuncs::split(strval, tmp, ";");
	//解析
	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		std::vector<std::string> tmp2;
		CommonFuncs::split(tmp[i], tmp2, "-");

		PackageData sdata;
		sdata.strid = tmp2[0];
		sdata.type = atoi(tmp2[1].c_str());
		sdata.count = atoi(tmp2[2].c_str());
		sdata.extype = GlobalData::getResExType(sdata.strid);//atoi(tmp2[3].c_str());
		sdata.lv = atoi(tmp2[4].c_str());
		sdata.exp = atoi(tmp2[5].c_str());
		sdata.goodvalue = atoi(tmp2[6].c_str());//耐久度
		if (tmp2.size() >= 9)
		{
			sdata.slv = atoi(tmp2[7].c_str());//
			sdata.tqu = atoi(tmp2[8].c_str());//
		}

		if (sdata.type == WEAPON || sdata.type == PROTECT_EQU)
		{
			std::vector<PackageData>::iterator it;
			for (it = StorageRoom::map_storageData[sdata.type].begin(); it != StorageRoom::map_storageData[sdata.type].end();)
			{
				if (it->strid.compare(sdata.strid) == 0 && sdata.count > 0)
				{
					sdata.goodvalue += it->goodvalue;
					if (it->slv < sdata.slv)
						it->slv = sdata.slv;
					if (it->lv < sdata.lv)
						it->lv = sdata.lv;
					it = StorageRoom::map_storageData[sdata.type].erase(it);
				}
				else
					++it;
			}
		}
		g_hero->setAtrByType((HeroAtrType)i, sdata);
		//g_hero->set [sdata.type].push_back(sdata);
	}
}

int GameScene::getRandomNum(int rangeStart, int rangeEnd) {

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

void GameScene::saveAllData()
{
	GameDataSave::getInstance()->setNatureReason(g_nature->getReason());
	GameDataSave::getInstance()->setNatureWeather(g_nature->getWeather());
	GameDataSave::getInstance()->setNatureWeatherChangeCount(g_nature->changeWeatherCount * 10000 + g_nature->changeWeatherRandow);
	GameDataSave::getInstance()->setNatureTemperature(g_nature->getTemperature());
	GameDataSave::getInstance()->setNatureTime(g_nature->getTime());
	GameDataSave::getInstance()->setLiveDays(g_nature->getPastDays());
	GameDataSave::getInstance()->setHeroLife(g_hero->getLifeValue());
	GameDataSave::getInstance()->setHeroOutinjury(g_hero->getOutinjuryValue());
	GameDataSave::getInstance()->setHeroInnerinjury(g_hero->getInnerinjuryValue());
	GameDataSave::getInstance()->setHeroHunger(g_hero->getHungerValue());
	GameDataSave::getInstance()->setHeroSpirit(g_hero->getSpiritValue());
	GameDataSave::getInstance()->setHeroLV(g_hero->getLVValue());
	GameDataSave::getInstance()->setHeroId(g_hero->getHeadID());
	GameDataSave::getInstance()->setHeroExp(g_hero->getExpValue());
	GameDataSave::getInstance()->setHeroIsOut(g_hero->getIsOut());
	GameDataSave::getInstance()->setHeroSex(g_hero->getSex());

	//保存资源数据
	GlobalData::saveResData();

	//保存装备数据
	g_hero->saveProperData();

	std::string curmid = GlobalData::getCurBranchPlotMissison();

	if (curmid.length() > 0)
	{
		int subindex = GlobalData::map_BranchPlotMissionItem[curmid].subindex;
		PlotMissionData pd = GlobalData::map_BranchPlotMissionData[curmid][subindex];
		GlobalData::saveBranchPlotMissionStatus(curmid, pd.status);
	}
}


void GameScene::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void GameScene::shake(Node * node) {
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

void GameScene::onExit()
{
	//saveAllData();
	Layer::onExit();
}


bool GameScene::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
}

void GameScene::saveMyData()
{
	if (g_gameLayer != NULL)
	{
		g_gameLayer->saveAllData();
	}
}


void GameScene::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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

void GameScene::changeGameStates(int status)
{
	if (g_gameLayer != NULL)
	{
		GameStatus gs = (GameStatus)status;
		if (gs == GAMEPAUSE)
		{
			if (GlobalData::g_gameStatus == GAMESTART)
			{
				GlobalData::g_gameStatus = gs;
			}
		}
		else if (gs == GAMESTART)
		{
			if (GlobalData::g_gameStatus == GAMEPAUSE)
			{
				GlobalData::g_gameStatus = gs;
			}
		}
	}	
}


void GameScene::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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

void GameScene::updata(float dt)
{
	GlobalData::setTimeGiftLeftTime(GlobalData::getTimeGiftLeftTime() - 1);
	if (GlobalData::g_gameStatus != GAMESTART || GlobalData::isPlayerChallenging)
		return;

	if (g_hero != NULL && g_hero->getIsWDChallenge())
		return;

	//更新资源列表数据
	for (unsigned int i = 0; i < GlobalData::vec_hillResid.size(); i++)
	{
		for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
		{
			ResData* data = &GlobalData::vec_resData[m];
			if (data->strid.compare(GlobalData::vec_hillResid[i]) == 0)
			{
				if (data->count <= 0)//采完，等待
				{
					data->pastmin = 0.0f;
					data->waittime += g_nature->getTimeInterval();
					if (data->waittime >= data->speed[g_nature->getReason()] * data->max)
					{
						data->count = data->max;
						data->waittime = 0.0f;
					}
				}
				else
				{
					data->pastmin += g_nature->getTimeInterval();
					if (data->pastmin >= data->speed[g_nature->getReason()])
					{
						int pcount = data->pastmin / data->speed[g_nature->getReason()];
						data->count += pcount;

						float leftmin = data->pastmin - (data->speed[g_nature->getReason()] * pcount);
						data->pastmin = leftmin;
	
					}

					if (data->count >= data->max)//产出最大
					{
						data->pastmin = 0;
						data->count = data->max;
						if (GameDataSave::getInstance()->getApprenticeDay() > 0)
						{
							PackageData pdata;
							if (data->res.size() > 0)
							{
								int cres = data->res[0];
								if (cres > 0 && atoi(data->strid.c_str()) != 67 && atoi(data->strid.c_str()) != 68)
								{
									std::string strid = StringUtils::format("%d", cres);
									pdata.strid = strid;
									pdata.type = data->type - 1;
									pdata.count = data->max;
									StorageRoom::add(pdata);
									data->count = 0;
									data->waittime = 0.0f;
									std::string str = StringUtils::format("%s%s x%d", CommonFuncs::gbk2utf("你的弟子刚刚帮你收集了").c_str(), GlobalData::map_allResource[strid].cname.c_str(), data->max);
									g_uiScroll->addEventText(str, 25, Color3B(204, 4, 4));
								}
							}
						}
					}
				}
			}
		}
	}

	std::string curmid = GlobalData::getCurBranchPlotMissison();

	if (curmid.length() > 0 && GlobalData::map_BranchPlotMissionItem[curmid].count > 0)
	{
		int subindex = GlobalData::map_BranchPlotMissionItem[curmid].subindex;
		PlotMissionData *pd = &GlobalData::map_BranchPlotMissionData[curmid][subindex];

		if (GlobalData::map_BranchPlotMissionItem[curmid].time > 0)
		{
			GlobalData::map_BranchPlotMissionItem[curmid].time--;
			if (GlobalData::map_BranchPlotMissionItem[curmid].time <= 0)
			{
				GlobalData::map_BranchPlotMissionItem[curmid].count--;
				GlobalData::map_BranchPlotMissionItem[curmid].time = GlobalData::map_BranchPlotMissionItem[curmid].maxtime;
				pd->status = M_NONE;
				GlobalData::saveBranchPlotMissionStatus("", 0);
				GameDataSave::getInstance()->setBranchPlotMissionGiveGoods("");
			}
		}
	}
}


void GameScene::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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

void GameScene::timerSaveData(float dt)
{
	if (issavedata)
		saveAllData();
}

void GameScene::checkiflive(float dt)
{
	//生命为0，死掉，弹出复活界面
	if (g_hero != NULL && g_hero->getLifeValue() <= 0.0f && !GlobalData::isPopingScene)
	{
		this->unschedule(schedule_selector(GameScene::checkiflive));
		topBar->stopLoseAnim();
		GlobalData::g_gameStatus = GAMEPAUSE;
		
		if (g_hero->getIsOut() && g_maplayer != NULL)
		{
			g_maplayer->heroPauseMoving();
		}

		ReviveLayer* layer = ReviveLayer::create();
		g_gameLayer->addChild(layer, 100, "revivelayer");
	}
}

void GameScene::heroRevive()
{
	//复活成功
	g_hero->revive();
	topBar->stopLoseAnim();
	GlobalData::g_gameStatus = GAMESTART;
	if (g_maplayer != NULL && g_hero->getIsOut())
		g_maplayer->heroResumeMoving();
	this->schedule(schedule_selector(GameScene::checkiflive), 0.1f);
}

void GameScene::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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

void GameScene::delayShowOutScence(float dt)
{
	Scene* scene = WXActivitScene::createScene("images/cout.jpg", CommonFuncs::gbk2utf("出门..."));
	if (scene != NULL)
	{
		auto transition = TransitionCrossFade::create(0.5f, scene);
		Director::getInstance()->pushScene(transition);
	}
	this->scheduleOnce(schedule_selector(GameScene::showGOOut), 0.1f);
}

void GameScene::showGOOut(float dt)
{
	g_maplayer = MapLayer::create();
	g_gameLayer->addChild(g_maplayer, 1, "maplayer");
	g_gameLayer->removeChildByName("homelayer");
	g_hero->setIsOut(true);
	if (g_maplayer != NULL)
		g_maplayer->checkNewerGuide();
}

void GameScene::getNpcRandMap()
{
	int pos = GameDataSave::getInstance()->getWxbMapPos();
	GlobalData::setWxbMapPos(pos);
	GlobalData::map_maps[wxbinmap[pos]].npcs.push_back("n012");

	pos = GameDataSave::getInstance()->getDgqbMapPos();
	GlobalData::setDgqbMapPos(pos);
	GlobalData::map_maps[dgqbinmap[pos]].npcs.push_back("n091");
}

void GameScene::showNewerGuide(int step, std::vector<Node*> nodes)
{
	if (NewerGuideLayer::checkifNewerGuide(step))
	{
		if (NewerGuideLayer::isShowing)
			return;

		NewerGuideLayer::isShowing = true;
		m_newerStep = step;
		m_newerNode = nodes;
		CannotTouchLayer* layer = CannotTouchLayer::create();
		g_gameLayer->addChild(layer, 11, "newernotouchlayer");
		this->scheduleOnce(schedule_selector(GameScene::delayShowNewerGuide),0.25f);
	}
}

void GameScene::jumpDown(cocos2d::Node *node, float dt) {
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

void GameScene::delayShowNewerGuide(float dt)
{
	if (g_NewerGuideLayer == NULL)
	{
		g_NewerGuideLayer = NewerGuideLayer::create(m_newerStep, m_newerNode);
		this->addChild(g_NewerGuideLayer, 10, "newerguidelayer");
	}
	g_gameLayer->removeChildByName("newernotouchlayer");
}

void GameScene::onSuccess()
{
	if (GlobalData::isFrozen)
	{
		if (GlobalData::g_gameStatus == GAMESTART)
			GlobalData::g_gameStatus = GAMEPAUSE;
		if (Director::getInstance()->getRunningScene()->getChildByName("frozenlayer") == NULL)
			Director::getInstance()->getRunningScene()->addChild(FrozenLayer::create(), 10000, "frozenlayer");
		return;
	}
	if (isAnewGetData)
	{
		isAnewGetData = false;
		this->scheduleOnce(schedule_selector(GameScene::delayChangeStartScene), 0.5f);
		return;
	}

	if (GlobalData::ispunishment)
	{
		issavedata = false;
		GlobalData::ispunishment = false;
		isAnewGetData = true;
		WaitingProgress* waitbox = WaitingProgress::create("数据异常...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->getAllData();
	}
	else
	{
		if (GlobalData::vec_buyVipIds.size() > 0)
		{
			WXGetVipRewardLayer* layer = WXGetVipRewardLayer::create();
			if (g_gameLayer != NULL)
				g_gameLayer->addChild(layer, 10, "viprewardlayer");
		}
		else
		{
			if (GlobalData::continueLoginDays > 0)
			{
				WXLoginRewardLayer* llayer = WXLoginRewardLayer::create();
				g_gameLayer->addChild(llayer, 100);
			}
			else
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				if (GlobalData::updateDownLoadURL.length() > 0 && !GlobalData::isPopUpdate)
				{
					JniMethodInfo methodInfo;
					char p_str[256] = { 0 };
					sprintf(p_str, "%s", GlobalData::updateDownLoadURL.c_str());
					if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/AppActivity", "updateApk", "(Ljava/lang/String;)V"))
					{
						GlobalData::isPopUpdate = true;
						jstring para1 = methodInfo.env->NewStringUTF(p_str);
						methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
					}
				}
#endif
			}
		}
	}
}

void GameScene::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	if (isAnewGetData)
	{
		isAnewGetData = false;
	}
}

void GameScene::initA()
{
	const Size size = Director::getInstance()->getVisibleSize();
	auto m_top = CSLoader::createNode("GameTopLayer.csb");
	addChild(m_top, 0);
	m_top->ignoreAnchorPointForPosition(false);
	m_top->setAnchorPoint(Vec2(0.5, 1));
	m_top->setPosition(size.width * 0.5, size.height);

	auto m_bottom = CSLoader::createNode("GameBottomLayer.csb");
	m_bottom->ignoreAnchorPointForPosition(false);
	m_bottom->setAnchorPoint(Vec2(0.5, 0));
	m_bottom->setPosition(size.width * 0.5, 0);
	addChild(m_bottom, 1);
}

void GameScene::initBg()
{
	const Size size = Director::getInstance()->getVisibleSize();
	auto m_bg = Sprite::create("");
	addChild(m_bg, 0);
	m_bg->setAnchorPoint(Vec2(0.5, 0));
	m_bg->setPosition(size.width * 0.5, 0);

	auto m_bgGround = Sprite::create("");
	addChild(m_bgGround, 1);
	m_bgGround->setPosition(m_bg->getContentSize().width * 0.5, 144 + m_bgGround->getContentSize().height * 0.5);

	auto tree = Sprite::create("");
	addChild(tree);
	tree->setAnchorPoint(Vec2(1, 0.5));
	tree->setPosition(size.width, 120);
}

void GameScene::delayChangeStartScene(float dt)
{

	Scene* scene = StartScene::createScene();

	Director::getInstance()->replaceScene(scene);
}

void GameScene::checkAchiveIsDone(float dt)
{
	GlobalData::vec_showAchiveNames.clear();
	for (unsigned i = 0; i < GlobalData::vec_achiveData.size(); i++)
	{
		AchiveData* data = &GlobalData::vec_achiveData[i];
		int needcount = GlobalData::getAchiveFinishCount(*data);
		if (data->finish >= needcount && data->isshowanim == 0)
		{
			data->isshowanim = 1;
			GlobalData::vec_showAchiveNames.push_back(data->name);
		}
	}

	if (GlobalData::vec_showAchiveNames.size() > 0)
	{
		this->unschedule(schedule_selector(GameScene::checkAchiveIsDone));
		WXAchiveDoneAnimLayer* aalayer = WXAchiveDoneAnimLayer::create();
		Director::getInstance()->getRunningScene()->addChild(aalayer, 100);
	}
}

void GameScene::checkRestBranchMisson()
{
	int days = GameDataSave::getInstance()->getEnterGameDaysOfYear();

	if (days != GlobalData::getDayOfYear())
	{
		GameDataSave::getInstance()->setBranchPlotMissionGiveGoods("");
		GameDataSave::getInstance()->setBranchPlotMissionStatus("");
		GlobalData::loadBranchPlotMissionJsonData();
		GameDataSave::getInstance()->setEnterGameDaysOfYear(GlobalData::getDayOfYear());
	}
}
