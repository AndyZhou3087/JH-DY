﻿#include "WXPlayerChallengeLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "GameScene.h"
#include "Winlayer.h"
#include "SoundManager.h"
#include "AnalyticUtil.h"
#include "MapLayer.h"
#include "Shake.h"
#include "MyActionProgressTimer.h"
#include "WXPlayerChallengeResultLayer.h"
#include "GameDataSave.h"

WXPlayerChallengeLayer::WXPlayerChallengeLayer()
{
	isUseWg = false;
	win = 0;
	GlobalData::isPlayerChallenging = true;
}

void WXPlayerChallengeLayer::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}

WXPlayerChallengeLayer::~WXPlayerChallengeLayer()
{
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_ENTER_MAPADDR);
}

WXPlayerChallengeLayer* WXPlayerChallengeLayer::create(std::string addrid, RankData* rankData)
{
	WXPlayerChallengeLayer *pRet = new WXPlayerChallengeLayer();
	if (pRet && pRet->init(addrid, rankData))
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

time_t WXPlayerChallengeLayer::getNowTime()
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

bool WXPlayerChallengeLayer::init(std::string addrid, RankData* rankData)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("fightLayer.csb");
	csbnode->setPositionY(100);
	this->addChild(csbnode);

	m_playerData = rankData;
	//地点名称
	cocos2d::ui::Text* addrnametxt = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	addrnametxt->setString(GlobalData::map_maps[addrid].cname);

	heroheadbox = (cocos2d::ui::Widget*)csbnode->getChildByName("heroheadbox");
	npcheadbox = (cocos2d::ui::Widget*)csbnode->getChildByName("npcheadbox");
	fightherokuang = (cocos2d::ui::Widget*)heroheadbox->getChildByName("fightherokuang");
	fightnpckuang = (cocos2d::ui::Widget*)npcheadbox->getChildByName("fightnpckuang");
	fightherokuang->setVisible(false);
	fightnpckuang->setVisible(false);

	cocos2d::ui::ImageView* heroicon = (cocos2d::ui::ImageView*)heroheadbox->getChildByName("heroicon");
	std::string heroiconstr = StringUtils::format("ui/fhero%d.png", g_hero->getHeadID());
	heroicon->loadTexture(heroiconstr, cocos2d::ui::TextureResType::PLIST);
	heroicon->setScale(0.6f);

	// player 图标
	playerhead = (cocos2d::ui::ImageView*)npcheadbox->getChildByName("npcicon");
	std::string playerheadstr = StringUtils::format("ui/fhero%d.png", rankData->herotype);
	playerhead->loadTexture(playerheadstr, cocos2d::ui::TextureResType::PLIST);
	playerhead->setScale(0.6f);
	playerhead->setFlippedX(true);

	//player名称
	playernametxt = (cocos2d::ui::Text*)npcheadbox->getChildByName("npcname");
	playernametxt->setString(rankData->nickname);
	if (rankData->nickname.length() >= 12)
		playernametxt->setFontSize(25);
	//角色名
	cocos2d::ui::Text* heronametxt = (cocos2d::ui::Text*)heroheadbox->getChildByName("heroname");
	heronametxt->setString(GlobalData::getMyNickName());

	int maxlife = g_hero->getMaxLifeValue();

	herolife = maxlife;
	//角色血量显示
	herohpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("herohpvaluetext");
	std::string hpstr = StringUtils::format("%d/%d", herolife, maxlife);
	herohpvaluetext->setString(hpstr);


	//角色血量进度
	float herohppercent = 100 * herolife / maxlife;

	herohpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("herohpbar");
	herohpbar->setPercent(herohppercent);

	/*herohpbar2 = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("herohpbar2");
	herohpbar2->setPercent(herohppercent);*/

	playermaxhp = getPlayerMaxLife();
	playerlife = playermaxhp;

	playeratk = getPlayerAtk();
	playerdf = getPlayerDf();

	//palyer血量显示
	playerhpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("npchpvaluetext");
	hpstr = StringUtils::format("%d/%d", playerlife, playermaxhp);
	playerhpvaluetext->setString(hpstr);

	//palyer血量进度
	int playerhppercent = 100 * playerlife / playermaxhp;
	playerhpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("npchpbar");
	playerhpbar->setPercent(playerhppercent);

	/*playerhpbar2 = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("npchpbar2");
	playerhpbar2->setPercent(playerhppercent);*/

	// 滚动文字
	m_fihgtScorll = UIScroll::create(600.0f, 435.0f);
	m_fihgtScorll->setPosition(Vec2(360, 370));
	csbnode->addChild(m_fihgtScorll);

	heroactimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroactimg");
	playeractimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcactimg");

	herocritfnt = (cocos2d::ui::TextBMFont*)heroactimg->getChildByName("herocritfnt");
	playercritfnt = (cocos2d::ui::TextBMFont*)playeractimg->getChildByName("npccritfnt");

	cocos2d::ui::Button* escapebtn = (cocos2d::ui::Button*)csbnode->getChildByName("escapebtn");
	escapebtn->setVisible(false);

	resetSkills();

	this->scheduleOnce(schedule_selector(WXPlayerChallengeLayer::delayHeroFight), 1.0f);//1.0s，hero->player
	std::string pdatastr = StringUtils::format("%s,%d", m_playerData->playerid.c_str(), m_playerData->herotype);
	GameDataSave::getInstance()->setPlayerChallengeData(pdatastr);
	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	int r = GlobalData::createRandomNum(4);
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_FIGHT_0 + r);
	return true;
}

long long WXPlayerChallengeLayer::getNowTimeMs() {
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

int WXPlayerChallengeLayer::getPlayerHurt()
{
	int heroCurAck = g_hero->getTotalAtck();

	int playerhurt = heroCurAck - playerdf;
	float fminack = 0.1f * heroCurAck;
	int intminack = int(fminack + 0.5f);
	if (playerhurt < intminack)
		playerhurt = intminack;

	return playerhurt;
}

bool WXPlayerChallengeLayer::isBeforeToday(time_t sec) {
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

void WXPlayerChallengeLayer::heroSkillComboAtk(float dt)
{
	int count = GlobalData::map_gfskills[S_SKILL_3].leftval;
	int c = getPlayerHurt();
	playerlife -= c * count / 10;
	GlobalData::map_gfskills[S_SKILL_3].leftval--;

	if (playerlife <= 0)
	{
		playerlife = 0;
		this->unschedule(schedule_selector(WXPlayerChallengeLayer::heroSkillComboAtk));
	}
	updatePlayerLife();
}


long long WXPlayerChallengeLayer::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}

void WXPlayerChallengeLayer::playerSkillComboAtk(float dt)
{
	int count = GlobalData::map_gfskills[S_SKILL_3].fightPlayerleftval;
	int c = getHeroHurt();
	herolife -= c * count / 10;
	GlobalData::map_gfskills[S_SKILL_3].fightPlayerleftval--;

	if (herolife <= 0)
	{
		herolife = 0;
		this->unschedule(schedule_selector(WXPlayerChallengeLayer::playerSkillComboAtk));
	}
	updateHeroLife();
}

void WXPlayerChallengeLayer::delayHeroFight(float dt)
{
	int playerhurt = getPlayerHurt();

	int skilltype = checkPlayerSkill(H_WG);

	heroheadbox->setScale(1.15);
	npcheadbox->setScale(1);
	/*fightherokuang->setVisible(true);
	fightnpckuang->setVisible(false);*/

	if (skilltype == S_SKILL_1 || skilltype == S_SKILL_2 || skilltype == S_SKILL_5)
	{
		GlobalData::map_gfskills[skilltype].fightPlayerleftval--;
	}

	if (isHeroAct != 1)
	{
		if (herolife <= 0)
		{
			showResultLayer(0);
			return;
		}

		if (skilltype == S_SKILL_1 || skilltype == S_SKILL_5)
		{
			showPlayerSkill(skilltype);
			herolife -= getHeroHurt() * 3 / 10;
			if (herolife < 0)
				herolife = 0;
			updateHeroLife();
			if (herolife <= 0)
			{
				showResultLayer(0);
				return;
			}
		}
		else if (skilltype == S_SKILL_2)
		{
			showPlayerSkill(skilltype);
			this->scheduleOnce(schedule_selector(WXPlayerChallengeLayer::delayPlayerFight), 1.5f);
			return;
		}
		skilltype = checkPlayerSkill(H_NG);
		if (skilltype == S_SKILL_6)
		{
			showPlayerSkill(skilltype);

			std::map<std::string, int>::iterator fite;
			for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
			{
				std::string nid = fite->first;
				int value = GlobalData::map_fighterPlayerData[nid];

				if (nid.compare(0, 1, "x") == 0)
				{
					playerhurt = playerhurt * (100 - GlobalData::map_wgngs[nid].skilleffect) / 100;
				}
			}
		}
		else if (skilltype == S_SKILL_7)
		{
			showPlayerSkill(skilltype);
			int herolosthp = 0;

			std::map<std::string, int>::iterator fite;
			for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
			{
				std::string nid = fite->first;
				int value = GlobalData::map_fighterPlayerData[nid];

				if (nid.compare(0, 1, "x") == 0)
				{
					herolosthp = herolife * GlobalData::map_wgngs[nid].skilleffect / 100;
				}
			}

			herolife -= herolosthp;
			if (herolife < 0)
				herolife = 0;
			updateHeroLife();
			playerlife =+ herolosthp;
			updatePlayerLife();
			if (herolife <= 0)
			{
				showResultLayer(0);
				return;
			}
		}
	}
	isHeroAct = -1;
	isplayerAct = - 1;


	int critrnd = g_hero->getCritRate() * 100;

	if (checkHeroSkill(H_WG) == S_SKILL_4)
	{
		showHeroSkill(S_SKILL_4);
		if (g_hero->getAtrByType(H_WG)->count > 0)
		{
			critrnd += GlobalData::map_wgngs[g_hero->getAtrByType(H_WG)->strid].skilleffect * 100;
		}
	}

	int npcdodgernd = getPlayerDodde() * 100;
	int r = GlobalData::createRandomNum(10000);

	if (checkPlayerSkill(H_NG) == S_SKILL_8)
	{
		showPlayerSkill(S_SKILL_8);

		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::map_fighterPlayerData[nid];
			if (nid.compare(0, 1, "x") == 0)
			{
				npcdodgernd += GlobalData::map_wgngs[nid].skilleffect * 100;
			}
		}
	}

	if (r < critrnd)
	{
		isHeroAct = 0;
		playerhurt *= 2;
		std::string tmpstr = "ui/crit.png";
		playeractimg->loadTexture(tmpstr, cocos2d::ui::TextureResType::PLIST);
		playeractimg->setContentSize(Sprite::createWithSpriteFrameName(tmpstr)->getContentSize());
		playeractimg->setVisible(true);
		playeractimg->setOpacity(200);
		playeractimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		playeractimg->runAction(Sequence::create(ac1, DelayTime::create(1.0f), Hide::create(), NULL));
		tmpstr = StringUtils::format("%d", playerhurt);
		playercritfnt->setString(tmpstr);
		playercritfnt->setVisible(true);
	}
	else if (r < npcdodgernd)
	{
		isplayerAct = 1;
		std::string imgstr = "ui/dodge.png";
		playeractimg->loadTexture(imgstr, cocos2d::ui::TextureResType::PLIST);
		playeractimg->setContentSize(Sprite::createWithSpriteFrameName(imgstr)->getContentSize());
		playeractimg->setVisible(true);
		playercritfnt->setVisible(false);
		playeractimg->setOpacity(200);
		playeractimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		playeractimg->runAction(Sequence::create(ac1, Shake::create(0.2f, 20, 1), DelayTime::create(0.8f), Hide::create(), NULL));

	}

	if (isplayerAct != 1)
	{
		playerlife -= playerhurt;
		if (playerlife < 0)
			playerlife = 0;
	}

	updatePlayerLife();

	showFightWord(0, playerhurt);

	if (playerhurt <= 0)//胜利
	{
		showResultLayer(1);
	}
	else
	{
		if (checkHeroSkill(H_WG) == S_SKILL_3)
		{
			int count = GlobalData::map_gfskills[S_SKILL_3].leftval;
			if (count > 0)
			{
				showHeroSkill(S_SKILL_3);
				this->schedule(schedule_selector(WXPlayerChallengeLayer::heroSkillComboAtk), 0.3f, count - 1, 0.2f);
			}
		}
		this->scheduleOnce(schedule_selector(WXPlayerChallengeLayer::delayPlayerFight), 1.5f);//延迟显示NPC 攻击，主要文字显示，需要看一下，所以延迟下
	}
}

bool WXPlayerChallengeLayer::getRandomBoolean(float rate) {

	int rate10 = (int)(rate*10.0);
	int randNum = rand();
	if (randNum % 10 <= rate10) {
		return true;
	}
	else {
		return false;
	}
}

void WXPlayerChallengeLayer::delayPlayerFight(float dt)
{

	float curheroHp = herolife;

	int herohurt = getHeroHurt();

    int skilltype = checkHeroSkill(H_WG);

	heroheadbox->setScale(1);
	npcheadbox->setScale(1.15);
	/*fightherokuang->setVisible(false);
	fightnpckuang->setVisible(true);*/
    
    if (skilltype == S_SKILL_1 || skilltype == S_SKILL_2 || skilltype == S_SKILL_5)
    {
       GlobalData::map_gfskills[skilltype].leftval--;
    }
    
	if (isplayerAct != 1)
	{
		if (playerlife <= 0)
		{
			showResultLayer(1);
			return;
		}
		
		if (skilltype == S_SKILL_1 || skilltype == S_SKILL_5)
		{
			showHeroSkill(skilltype);
			playerlife -= getPlayerHurt() * 3 / 10;
			if (playerlife < 0)
				playerlife = 0;
			updatePlayerLife();
			if (playerlife <= 0)
			{
				showResultLayer(1);
				return;
			}
		}
		else if (skilltype == S_SKILL_2)
		{
			showHeroSkill(skilltype);
			this->scheduleOnce(schedule_selector(WXPlayerChallengeLayer::delayHeroFight), 1.5f);
			return;
		}
		skilltype = checkHeroSkill(H_NG);
		if (skilltype == S_SKILL_6)
		{
			showHeroSkill(skilltype);
			if (g_hero->getAtrByType(H_NG)->count > 0)
			{
				herohurt = herohurt * (100 - GlobalData::map_wgngs[g_hero->getAtrByType(H_NG)->strid].skilleffect) / 100;
			}
		}
		else if (skilltype == S_SKILL_7)
		{
			showHeroSkill(skilltype);
			int playerlosthp = 0;
			if (g_hero->getAtrByType(H_NG)->count > 0)
			{
				playerlosthp = playerlife * GlobalData::map_wgngs[g_hero->getAtrByType(H_NG)->strid].skilleffect / 100;
			}
			playerlife -= playerlosthp;
			if (playerlife < 0)
				playerlife = 0;
			updatePlayerLife();
			herolife = curheroHp + playerlosthp;
			updateHeroLife();
			if (playerlife <= 0)
			{
				showResultLayer(1);
				return;
			}
		}
		
	}


	isHeroAct = -1;
	isplayerAct = -1;

	int dodgernd = g_hero->getdodgeRate() * 100;
	if (checkHeroSkill(H_NG) == S_SKILL_8)
	{
		showHeroSkill(S_SKILL_8);
		if (g_hero->getAtrByType(H_NG)->count > 0)
		{
			dodgernd += GlobalData::map_wgngs[g_hero->getAtrByType(H_NG)->strid].skilleffect * 100;
		}
		
	}

	int playercritrnd = getPlayerCrit() * 100;
	int r = GlobalData::createRandomNum(10000);

	if (checkPlayerSkill(H_WG) == S_SKILL_4)
	{
		showPlayerSkill(S_SKILL_4);
		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::map_fighterPlayerData[nid];
			if (nid.compare(0, 1, "w") == 0)
			{
				playercritrnd += GlobalData::map_wgngs[nid].skilleffect * 100;
			}
		}
		
	}

	if (r < playercritrnd)
	{
		isplayerAct = 0;
		herohurt *= 2;
		std::string tmpstr = "ui/crit.png";
		heroactimg->loadTexture(tmpstr, cocos2d::ui::TextureResType::PLIST);
		heroactimg->setContentSize(Sprite::createWithSpriteFrameName(tmpstr)->getContentSize());
		heroactimg->setVisible(true);
		herocritfnt->setVisible(true);
		heroactimg->setOpacity(200);
		heroactimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		heroactimg->runAction(Sequence::create(ac1, DelayTime::create(1.0f), Hide::create(), NULL));
		tmpstr = StringUtils::format("%d", herohurt);
		herocritfnt->setString(tmpstr);
	}
	else if (r < dodgernd)
	{
		isHeroAct = 1;
		std::string imgstr = "ui/dodge.png";
		heroactimg->loadTexture(imgstr, cocos2d::ui::TextureResType::PLIST);
		heroactimg->setContentSize(Sprite::createWithSpriteFrameName(imgstr)->getContentSize());
		heroactimg->setVisible(true);
		herocritfnt->setVisible(false);
		heroactimg->setOpacity(200);
		heroactimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		heroactimg->runAction(Sequence::create(ac1, Shake::create(0.2f, 20, 1), DelayTime::create(0.8f), Hide::create(), NULL));

	}

	if (isHeroAct != 1)
	{
		curheroHp -= herohurt;
	}

	showFightWord(1, herohurt);

	if (curheroHp < 0.0f)
		curheroHp = 0.0f;

	herolife = curheroHp;
	updateHeroLife();

	if (herolife > 0.0f)
	{
		this->scheduleOnce(schedule_selector(WXPlayerChallengeLayer::delayHeroFight), 1.5f);

		if (checkPlayerSkill(H_WG) == S_SKILL_3)
		{
			int count = GlobalData::map_gfskills[S_SKILL_3].fightPlayerleftval;
			if (count > 0)
			{
				showPlayerSkill(S_SKILL_3);
				this->schedule(schedule_selector(WXPlayerChallengeLayer::playerSkillComboAtk), 0.3f, count - 1, 0.2f);
			}
		}
	}
	else
	{
		showResultLayer(0);
	}
	
}

bool WXPlayerChallengeLayer::getRandomBoolean() {

	if (0 == rand() % 2) {
		return true;
	}
	else {
		return false;
	}
}
int WXPlayerChallengeLayer::getRandomNum(int range) {

	if (range <= 0) {
		return 0;
	}

	return rand() % range;
}


int WXPlayerChallengeLayer::getHeroHurt()
{
	int curheroDf = g_hero->getTotalDf();
	int herohurt = playeratk - curheroDf;

	float fminack = 0.1f * playeratk;
	int intminack = int(fminack + 0.5f);

	if (herohurt < intminack)
		herohurt = intminack;
	return herohurt;
}

void WXPlayerChallengeLayer::resetSkills()
{
	for (int i = S_SKILL_1; i <= S_SKILL_8; i++)
	{
		GlobalData::map_gfskills[i].leftval = 0;
		GlobalData::map_gfskills[i].fightPlayerleftval = 0;
	}
}

int WXPlayerChallengeLayer::getRandomNum(int rangeStart, int rangeEnd) {

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
void WXPlayerChallengeLayer::delayShowResultLayer(float dt)
{
	WXPlayerChallengeResultLayer* layer = WXPlayerChallengeResultLayer::create(m_playerData, win);
	g_gameLayer->addChild(layer, 5);
	this->removeFromParentAndCleanup(true);
}

void WXPlayerChallengeLayer::showFightWord(int type, int value)
{
	std::string wordstr;
	int size = 0;
	int r = 0;
	isUseWg = false;
	if (type == 0)//
	{
		std::string herowordstr;
		if (g_hero->getAtrByType(H_WEAPON)->count > 0 && g_hero->getAtrByType(H_WEAPON)->goodvalue > 0)//是否有武器
		{
			int showr = GlobalData::createRandomNum(2);

			std::string wstrid = g_hero->getAtrByType(H_WEAPON)->strid;
			if (g_hero->getAtrByType(H_WG)->count > 0)
			{
				if (showr == 0)
					isUseWg = true;
			}
			if (isUseWg)
			{
				herowordstr = getHeroGfFightStr(g_hero->getAtrByType(H_WG)->strid);
			}
			else
			{
				int extype = g_hero->getAtrByType(H_WEAPON)->extype;
				size = sizeof(herofightdesc1[extype - 1]) / sizeof(herofightdesc1[extype - 1][0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc1[extype - 1][r];
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), GlobalData::map_allResource[wstrid].cname.c_str(), m_playerData->nickname.c_str());
			}

			
			int r = GlobalData::createRandomNum(100);
			if (r < 40)
			{
				g_hero->getAtrByType(H_WEAPON)->goodvalue--;
				if (g_hero->getAtrByType(H_WEAPON)->goodvalue <= 0)
				{
					std::string desc = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("你的").c_str(), GlobalData::map_allResource[wstrid].cname.c_str(), CommonFuncs::gbk2utf("已毁坏！！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
				}
				else if (g_hero->getAtrByType(H_WEAPON)->goodvalue == 20 || g_hero->getAtrByType(H_WEAPON)->goodvalue == 10)
				{
					std::string descstr = StringUtils::format("%s%s%d", GlobalData::map_allResource[wstrid].cname.c_str(), CommonFuncs::gbk2utf("耐久度仅剩").c_str(), g_hero->getAtrByType(H_WEAPON)->goodvalue);

					g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
				}
			}
		}
		else//没有武器
		{

			if (g_hero->getAtrByType(H_WG)->count > 0)
			{
				isUseWg = true;
			}
			if (isUseWg)
			{
				herowordstr = getHeroGfFightStr(g_hero->getAtrByType(H_WG)->strid);
			}
			else
			{
				size = sizeof(herofightdesc) / sizeof(herofightdesc[0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc[r];
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), m_playerData->nickname.c_str());
			}
		}
		
		checkWordLblColor(herowordstr);

		if (isUseWg)
		{
			int r1 = GlobalData::createRandomNum(4);
			wordstr = bossgetherowg[r1];
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), m_playerData->nickname.c_str(), value);
		}
		else
		{
			if (g_hero->getAtrByType(H_WEAPON)->count > 0)
			{
				int extype = g_hero->getAtrByType(H_WEAPON)->extype;
				size = sizeof(herofightdesc2[extype - 1]) / sizeof(herofightdesc2[extype - 1][0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc2[extype - 1][r];
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), m_playerData->nickname.c_str(), value);
			}
			else
			{
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightdesc3[0].c_str()).c_str(), m_playerData->nickname.c_str(), value);
			}
		}

		std::string playerarmorstr;
		std::map<std::string, int>::iterator eite;
		for (eite = GlobalData::map_fighterPlayerData.begin(); eite != GlobalData::map_fighterPlayerData.end(); ++eite)
		{
			std::string nid = eite->first;
			int value = GlobalData::map_fighterPlayerData[nid];

			int goodvalue = value % 1000 - 1;
			if (nid.compare(0, 1, "e") == 0 && goodvalue > 0)
			{
				playerarmorstr = nid;
			}
		}

		if (playerarmorstr.length() > 0)//player是有有防具
		{
			int r = GlobalData::createRandomNum(100);
			std::string astrid = playerarmorstr;
			if (r < 40)
			{
				int gv = GlobalData::map_fighterPlayerData[playerarmorstr]--;

				if (gv <= 0)
				{
					std::string desc = StringUtils::format("%s%s%s", m_playerData->nickname.c_str(), GlobalData::map_allResource[astrid].cname.c_str(), CommonFuncs::gbk2utf("已毁坏！！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
				}
			}

			size = sizeof(bossfight1) / sizeof(bossfight1[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = bossfight1[r];
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), m_playerData->nickname.c_str(), GlobalData::map_allResource[astrid].cname.c_str(), value);
		}
		else//没有防具
		{
			size = sizeof(bossfight) / sizeof(bossfight[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = bossfight[r];
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str() , m_playerData->nickname.c_str(), value);
		}

		if (isplayerAct == 1)//npc闪避
		{
			int dcount = sizeof(npcdodgedesc) / sizeof(npcdodgedesc[0]);
			int dodgernd = GlobalData::createRandomNum(dcount);
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(npcdodgedesc[dodgernd].c_str()).c_str(), m_playerData->nickname.c_str());
		}
		checkWordLblColor(herowordstr);

		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_ATTACK);
	}
	else//
	{
		std::string bosswordstr;

		std::string playerweaponstr;
		std::string wstrid;
		std::map<std::string, int>::iterator wite;
		for (wite = GlobalData::map_fighterPlayerData.begin(); wite != GlobalData::map_fighterPlayerData.end(); ++wite)
		{
			std::string nid = wite->first;
			int value = GlobalData::map_fighterPlayerData[nid];

			int goodvalue = value % 1000 - 1;
			if (nid.compare(0, 1, "a") == 0 && goodvalue > 0)
			{
				playerweaponstr = nid;
			}
			else if (nid.compare(0, 1, "w") == 0)
			{
				wstrid = nid;
			}
		}
		if (playerweaponstr.length() > 0)//是否有武器
		{
			int showr = GlobalData::createRandomNum(2);

			if (wstrid.length() > 0)
			{
				if (showr == 0)
					isUseWg = true;
			}
			if (isUseWg)
			{
				bosswordstr = getPlayerGfFightStr(wstrid);
			}
			else
			{
				int extype = GlobalData::getResExType(playerweaponstr);
				size = sizeof(herofightdesc1[extype - 1]) / sizeof(herofightdesc1[extype - 1][0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc1[extype - 1][r];
				bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), m_playerData->nickname.c_str(), GlobalData::map_allResource[playerweaponstr].cname.c_str(), CommonFuncs::gbk2utf("你").c_str());
			}


			int r = GlobalData::createRandomNum(100);
			if (r < 40)
			{
				int gv = GlobalData::map_fighterPlayerData[playerweaponstr]--;
				if (gv <= 0)
				{
					std::string desc = StringUtils::format("%s%s%s", m_playerData->nickname.c_str(), GlobalData::map_allResource[wstrid].cname.c_str(), CommonFuncs::gbk2utf("已毁坏！！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
				}
			}
		}
		else//没有武器
		{

			if (wstrid.length() > 0)
			{
				isUseWg = true;
			}
			if (isUseWg)
			{
				bosswordstr = getPlayerGfFightStr(wstrid);
			}
			else
			{
				size = sizeof(herofightdesc) / sizeof(herofightdesc[0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc[r];
				bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), m_playerData->nickname.c_str(), CommonFuncs::gbk2utf("你").c_str());
			}
		}

		checkWordLblColor(bosswordstr);

		if (isUseWg)
		{
			int r1 = GlobalData::createRandomNum(4);
			wordstr = bossgetherowg[r1];
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), value);
		}
		else
		{
			if (playerweaponstr.length() > 0)
			{
				int extype = GlobalData::getResExType(playerweaponstr);
				size = sizeof(herofightdesc2[extype - 1]) / sizeof(herofightdesc2[extype - 1][0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc2[extype - 1][r];
				bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), value);
			}
			else
			{
				bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightdesc3[0].c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), value);
			}
		}

		if (g_hero->getAtrByType(H_ARMOR)->count > 0 && g_hero->getAtrByType(H_ARMOR)->goodvalue > 0)//是有有防具
		{
			int r = GlobalData::createRandomNum(100);
			std::string astrid = g_hero->getAtrByType(H_ARMOR)->strid;
			if (r < 40)
			{
				g_hero->getAtrByType(H_ARMOR)->goodvalue--;

				if (g_hero->getAtrByType(H_ARMOR)->goodvalue <= 0)
				{
					std::string desc = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("你的").c_str(), GlobalData::map_allResource[astrid].cname.c_str(), CommonFuncs::gbk2utf("已毁坏！！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
				}
				else if (g_hero->getAtrByType(H_ARMOR)->goodvalue == 20 || g_hero->getAtrByType(H_ARMOR)->goodvalue == 10)
				{
					std::string descstr = StringUtils::format("%s%s%d", GlobalData::map_allResource[astrid].cname.c_str(), CommonFuncs::gbk2utf("耐久度仅剩").c_str(), g_hero->getAtrByType(H_ARMOR)->goodvalue);

					g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
				}
			}

			size = sizeof(bossfight1) / sizeof(bossfight1[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = bossfight1[r];
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), m_playerData->nickname.c_str(), CommonFuncs::gbk2utf("你").c_str(), GlobalData::map_allResource[astrid].cname.c_str(), value);
		}
		else//没有防具
		{
			size = sizeof(bossfight) / sizeof(bossfight[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = bossfight[r];
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), m_playerData->nickname.c_str(), CommonFuncs::gbk2utf("你").c_str(), value);
		}

		if (isHeroAct == 1)
		{
			int dcount = sizeof(herododgedesc) / sizeof(herododgedesc[0]);
			int dodgernd = GlobalData::createRandomNum(dcount);
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(herododgedesc[dodgernd].c_str()).c_str(), m_playerData->nickname.c_str(), CommonFuncs::gbk2utf("你").c_str());
		}
		checkWordLblColor(bosswordstr);

		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_HURT);
	}
}

void WXPlayerChallengeLayer::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void WXPlayerChallengeLayer::checkWordLblColor(std::string wordstr)
{
	Label* wordlbl = Label::createWithTTF(wordstr, "fonts/STXINGKA.TTF", 28);
	wordlbl->setLineBreakWithoutSpace(true);
	wordlbl->setMaxLineWidth(610);
	int index = 0;
	//getletter --是每个字，中文，英文都算一个字
	while (wordlbl->getLetter(index) != NULL)
	{
		wordlbl->getLetter(index)->setColor(Color3B::BLACK);
		index++;
	}

	std::u16string utf16word;
	StringUtils::UTF8ToUTF16(wordstr, utf16word);

	std::u16string wordtmp = utf16word;
	std::u16string utf16playername;
	StringUtils::UTF8ToUTF16(m_playerData->nickname, utf16playername);
	std::size_t findpos;
	//player名称颜色 红色
	while (1){
		findpos = wordtmp.find(utf16playername);
		if (findpos != std::string::npos){
			wordtmp = wordtmp.substr(findpos + utf16playername.length(), wordtmp.length() - utf16playername.length());
			int len = utf16playername.length();
			int sindex = utf16word.length() - wordtmp.length() - utf16playername.length();
			for (int i = sindex; i < sindex + len; i++)
			{
				wordlbl->getLetter(i)->setColor(Color3B(230, 35, 35));
			}
		}
		else
			break;
	}
	//角色名称颜色 绿色
	wordtmp = utf16word;
	std::u16string utf16heroname;
	StringUtils::UTF8ToUTF16(CommonFuncs::gbk2utf("你"), utf16heroname);
	while (1){
		findpos = wordtmp.find(utf16heroname);
		if (findpos != std::string::npos){
			wordtmp = wordtmp.substr(findpos + utf16heroname.length(), wordtmp.length() - utf16heroname.length());
			int len = utf16heroname.length();
			int sindex = utf16word.length() - wordtmp.length() - utf16heroname.length();
			for (int i = sindex; i < sindex + len; i++)
			{
				wordlbl->getLetter(i)->setColor(Color3B(27, 141, 0));
			}
		}
		else
			break;
	}
	wordtmp = utf16word;
	//武器，功法名称颜色 紫色
	std::map<std::string, EquipData>::iterator ite;
	for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
	{
		std::string ename = GlobalData::map_allResource[ite->first].cname;
		std::u16string utf16ename;
		StringUtils::UTF8ToUTF16(ename, utf16ename);
		std::size_t findpos;

		while (1){
			findpos = wordtmp.find(utf16ename);
			if (findpos != std::string::npos){
				wordtmp = wordtmp.substr(findpos + utf16ename.length(), wordtmp.length() - utf16ename.length());
				int len = utf16ename.length();
				int sindex = utf16word.length() - wordtmp.length() - utf16ename.length();
				for (int i = sindex; i < sindex + len; i++)
				{
					wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
				}
			}
			else
				break;
		}
	}
	wordtmp = utf16word;
	if (g_hero->getAtrByType(H_WG)->count > 0)
	{
		GFTrickData gftrickdata = GlobalData::map_gftricks[g_hero->getAtrByType(H_WG)->strid];
		int mygftricksize = gftrickdata.snames.size();
		
		for (int n = 0; n < mygftricksize; n++)
		{
			std::string gfname = gftrickdata.snames[n];

			std::u16string utf16gfname;
			StringUtils::UTF8ToUTF16(gfname, utf16gfname);
			std::size_t findpos;

			while (1){
				findpos = wordtmp.find(utf16gfname);
				if (findpos != std::string::npos){
					wordtmp = wordtmp.substr(findpos + utf16gfname.length(), wordtmp.length() - utf16gfname.length());
					int len = utf16gfname.length();
					int sindex = utf16word.length() - wordtmp.length() - utf16gfname.length();
					for (int i = sindex; i < sindex + len; i++)
					{
						wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
					}
				}
				else
					break;
			}
		}
	}

	wordtmp = utf16word;

	////数字颜色 红色

	for (unsigned int i = wordtmp.length() - 8; i < wordtmp.length(); i++)
	{
		if (wordtmp[i] >= '0' && wordtmp[i] <= '9')
		{
			wordlbl->getLetter(i)->setColor(Color3B(230, 35, 35));
		}
	}

	m_fihgtScorll->addEventLabel(wordlbl);
}


void WXPlayerChallengeLayer::shake(Node * node) {
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


std::string WXPlayerChallengeLayer::getHeroGfFightStr(std::string wgstr)
{
	std::string retstr;

	int r1 = GlobalData::createRandomNum(2);
	int r2 = GlobalData::createRandomNum(4);

	std::string gfstr = wgstr;
	GFTrickData ssdata = GlobalData::map_gftricks[gfstr];
	int r3 = GlobalData::createRandomNum(ssdata.snames.size());
	std::string gfsname = ssdata.snames[r3];
	std::string	wordstr = CommonFuncs::gbk2utf(herousewgfight[r1][r2].c_str());
	if (r1 == 0)
		retstr = StringUtils::format(wordstr.c_str(), CommonFuncs::gbk2utf("你").c_str(), gfsname.c_str(), m_playerData->nickname.c_str());
	else
		retstr = StringUtils::format(wordstr.c_str(), CommonFuncs::gbk2utf("你").c_str(), m_playerData->nickname.c_str(), gfsname.c_str(), gfsname.c_str(), m_playerData->nickname.c_str());

	return retstr;
}

bool WXPlayerChallengeLayer::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
}

std::string WXPlayerChallengeLayer::getPlayerGfFightStr(std::string wgstr)
{
	std::string retstr;

	int r1 = GlobalData::createRandomNum(2);
	int r2 = GlobalData::createRandomNum(4);

	std::string gfstr = wgstr;
	GFTrickData ssdata = GlobalData::map_gftricks[gfstr];
	int r3 = GlobalData::createRandomNum(ssdata.snames.size());
	std::string gfsname = ssdata.snames[r3];
	std::string	wordstr = CommonFuncs::gbk2utf(herousewgfight[r1][r2].c_str());
	if (r1 == 0)
		retstr = StringUtils::format(wordstr.c_str(), m_playerData->nickname.c_str(), gfsname.c_str(), CommonFuncs::gbk2utf("你").c_str());
	else
		retstr = StringUtils::format(wordstr.c_str(), m_playerData->nickname.c_str(), CommonFuncs::gbk2utf("你").c_str(), gfsname.c_str(), gfsname.c_str(), CommonFuncs::gbk2utf("你").c_str());

	return retstr;
}


void WXPlayerChallengeLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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

void WXPlayerChallengeLayer::updateHeroLife()
{
	std::string hpstr = StringUtils::format("%d/%d", herolife, (int)g_hero->getMaxLifeValue());
	herohpvaluetext->setString(hpstr);
	float herohppercent = 100 * herolife / g_hero->getMaxLifeValue();
	herohpbar->setPercent(herohppercent);

	/*MyProgressTo * fromto = MyProgressTo::create(0.5f, herohppercent);
	herohpbar2->runAction(fromto);*/
}

void WXPlayerChallengeLayer::updatePlayerLife()
{
	//NPC血量显示
	std::string hpstr = StringUtils::format("%d/%d", playerlife, playermaxhp);
	playerhpvaluetext->setString(hpstr);

	//NCP血量进度
	int playerhppercent = 100 * playerlife / playermaxhp;
	playerhpbar->setPercent(playerhppercent);
	/*MyProgressTo * to = MyProgressTo::create(0.5f, playerhppercent);
	playerhpbar2->runAction(to);*/
}
void WXPlayerChallengeLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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


int WXPlayerChallengeLayer::checkHeroSkill(HeroAtrType gftype)
{
	int ret = S_SNONE;
	if (g_hero->getAtrByType(gftype)->count > 0)
	{
		int lv = g_hero->getAtrByType(gftype)->lv;
		std::string gfstr = g_hero->getAtrByType(gftype)->strid;
		int stype = GlobalData::map_wgngs[gfstr].skilltype;
		if (GlobalData::map_gfskills[stype].leftval > 0)
		{
			ret =  stype;
		}
		else
		{
			if (stype > S_SNONE)
			{
				int rand = GlobalData::map_wgngs[gfstr].vec_skrnd[lv] * 100;
				int r = GlobalData::createRandomNum(10000);
				if (r < rand)
				{
					if (stype != S_SKILL_4 && stype != S_SKILL_6 && stype != S_SKILL_7 && stype != S_SKILL_8)
						GlobalData::map_gfskills[stype].leftval = GlobalData::map_wgngs[gfstr].skilleffect;
					ret = stype;
				}
			}
		}
	}

	return ret;

}

void WXPlayerChallengeLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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

int WXPlayerChallengeLayer::checkPlayerSkill(HeroAtrType gftype)
{
	int ret = S_SNONE;
	std::string gfstr = "";
	int lv = 0;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
	{
		std::string nid = fite->first;
		int value = GlobalData::map_fighterPlayerData[nid];

		if (nid.compare(0, 1, "w") == 0 && gftype == H_WG)
		{
			gfstr = nid;
			lv = value - 1;
		}
		else if (nid.compare(0, 1, "x") == 0 && gftype == H_NG)
		{
			gfstr = nid;
			lv = value - 1;
		}
	}

	if (gfstr.length() > 0)
	{
		int stype = GlobalData::map_wgngs[gfstr].skilltype;
		if (GlobalData::map_gfskills[stype].fightPlayerleftval > 0)
		{
			ret = stype;
		}
		else
		{
			if (stype > S_SNONE)
			{
				int rand = GlobalData::map_wgngs[gfstr].vec_skrnd[lv] * 100;
				int r = GlobalData::createRandomNum(10000);
				if (r < rand)
				{
					if (stype != S_SKILL_4 && stype != S_SKILL_6 && stype != S_SKILL_7 && stype != S_SKILL_8)
						GlobalData::map_gfskills[stype].fightPlayerleftval = GlobalData::map_wgngs[gfstr].skilleffect;
					ret = stype;
				}
			}
		}
	}

	return ret;
}

void WXPlayerChallengeLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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


void WXPlayerChallengeLayer::showHeroSkill(int skilltype)
{
	if (skilltype >= S_SKILL_1 && skilltype <= S_SKILL_5)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showPlayerTextAmin(str);
	}
	else if (skilltype >= S_SKILL_6 && skilltype <= S_SKILL_8)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showHeroTextAmin(str);
	}
}


void WXPlayerChallengeLayer::jumpDown(cocos2d::Node *node, float dt) {
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
void WXPlayerChallengeLayer::showPlayerSkill(int skilltype)
{
	if (skilltype >= S_SKILL_1 && skilltype <= S_SKILL_5)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showHeroTextAmin(str);
	}
	else if (skilltype >= S_SKILL_6 && skilltype <= S_SKILL_8)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showPlayerTextAmin(str);
	}
}

void WXPlayerChallengeLayer::showHeroTextAmin(std::string filename)
{
	heroactimg->loadTexture(filename, cocos2d::ui::TextureResType::PLIST);
	heroactimg->setContentSize(Sprite::createWithSpriteFrameName(filename)->getContentSize());
	heroactimg->setVisible(true);
	herocritfnt->setVisible(false);
	heroactimg->setOpacity(200);
	heroactimg->setScale(3);
	ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
	heroactimg->runAction(Sequence::create(ac1, Shake::create(0.2f, 20, 1), DelayTime::create(0.6f), Hide::create(), NULL));
	herocritfnt->setVisible(false);
}

void WXPlayerChallengeLayer::initData()
{
	m_starnum = 0;
	m_stageIcon = (cocos2d::ui::ImageView*)m_node->getChildByName("image");
	std::string str;
	for (int i = 0; i < 3; i++)
	{
		str = StringUtils::format("star%d", i);
		m_star[i] = (cocos2d::ui::Widget*)m_node->getChildByName(str);
		str = StringUtils::format("star%dbg", i);
		m_starbg[i] = (cocos2d::ui::Widget*)m_node->getChildByName(str);
	}
	m_stagenumlbl = (cocos2d::ui::TextBMFont*)m_node->getChildByName("stagenum");
}

void WXPlayerChallengeLayer::setStar(int num, bool isboss)
{
	m_starnum = num;
	m_isboss = isboss;
	std::string stagestr;
	if (num > 0)
	{
		stagestr = "stage1.png";
		for (int i = 0; i < 3; i++)
		{
			m_starbg[i]->setVisible(true);
		}
		for (int i = 0; i < num; i++)
		{
			m_star[i]->setVisible(true);
		}
	}
	else
	{
		stagestr = "stage0.png";
		for (int i = 0; i < 3; i++)
		{
			m_star[i]->setVisible(false);
			m_starbg[i]->setVisible(false);
		}
	}
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
	if (isboss)
	{
		int index = num > 0 ? 1 : 0;
		std::string bossstr = StringUtils::format("sboss%d.png", index);
		m_stageIcon->loadTexture(bossstr, cocos2d::ui::TextureResType::PLIST);
		m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(bossstr)->getContentSize());
	}
}

void WXPlayerChallengeLayer::showPlayerTextAmin(std::string filename)
{
	playeractimg->loadTexture(filename, cocos2d::ui::TextureResType::PLIST);
	playeractimg->setContentSize(Sprite::createWithSpriteFrameName(filename)->getContentSize());
	playeractimg->setVisible(true);
	playeractimg->setOpacity(200);
	playeractimg->setScale(3);
	ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
	playeractimg->runAction(Sequence::create(ac1, DelayTime::create(0.8f), Hide::create(), NULL));
	playercritfnt->setVisible(false);
}
void WXPlayerChallengeLayer::setStageNum(int stage)
{
	std::string numstr = StringUtils::format("%d", stage);
	m_stagenumlbl->setString(numstr);
	if (m_isboss)
		m_stagenumlbl->setVisible(false);
}

int WXPlayerChallengeLayer::getPlayerMaxLife()
{
	float friendhppercent = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::map_fightPlayerfriendly.begin(); it != GlobalData::map_fightPlayerfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::map_fightPlayerfriendly[nid].relation == F_FRIEND)
		{
			friendhppercent += GlobalData::map_NPCFriendData[nid].hppercent / 100;
		}
		else if (GlobalData::map_fightPlayerfriendly[nid].relation == F_MASTER)
		{
			friendhppercent += GlobalData::map_NPCMasterData[nid].hppercent / 100;
		}
	}

	float mixfglifepercent = 0.0f;

	std::string mixgf = GlobalData::fightPlayerMixgf;
	if (mixgf.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mixgf];

		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::map_fighterPlayerData[nid];

			if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
				mixfglifepercent += mdata.hppercent / 100;
		}

	}

	float flife = GlobalData::map_heroAtr[m_playerData->herotype].vec_maxhp[m_playerData->herolv] * 1.0f;
	flife = flife + flife*friendhppercent + flife * mixfglifepercent;

	if (GlobalData::fightPlayerFactionLv == 3)
		flife += flife*0.02f;
	return flife;
}

void WXPlayerChallengeLayer::hilight()
{
	m_stageIcon->stopAllActions();
	m_node->setScale(1);
	std::string stagestr;
	if (m_isboss)
	{
		stagestr = "sboss1.png";
	}
	else
	{
		stagestr = "stage2.png";
	}
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
}
int WXPlayerChallengeLayer::getPlayerDf()
{
	int adf = 0;
	int ngdf = 0;
	float slvdf = 0.0f;//强化等级加防

	float frienddfpercent = 0.0f;
	std::string masternpc;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::map_fightPlayerfriendly.begin(); it != GlobalData::map_fightPlayerfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::map_fightPlayerfriendly[nid].relation == F_FRIEND)
		{
			frienddfpercent += GlobalData::map_NPCFriendData[nid].dfpercent / 100;
		}
		else if (GlobalData::map_fightPlayerfriendly[nid].relation == F_MASTER)
		{
			frienddfpercent += GlobalData::map_NPCMasterData[nid].dfpercent / 100;
			masternpc = nid;
		}
	}

	std::string ngstr;
	int ngvalue = 0;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
	{
		std::string nid = fite->first;
		ngvalue = GlobalData::map_fighterPlayerData[nid] - 1;

		if (nid.compare(0, 1, "w") == 0)
			ngstr = nid;
	}
	if (ngstr.length() > 0)
	{
		int gflv = ngvalue;

		if (gflv > GlobalData::map_wgngs[ngstr].maxlv - 1)
			gflv = GlobalData::map_wgngs[ngstr].maxlv - 1;
		ngdf = GlobalData::map_wgngs[ngstr].vec_bns[gflv];

		int masterdf = 0;
		if (masternpc.length() > 0)
		{
			for (unsigned int i = 0; i < GlobalData::map_NPCMasterData[masternpc].vec_gfid.size(); i++)
			{
				if (ngstr.compare(GlobalData::map_NPCMasterData[masternpc].vec_gfid[i]) == 0)
				{
					int msbdf = ngdf * GlobalData::map_NPCMasterData[masternpc].vec_gfbonus[i] / 100;
					masterdf += msbdf;

					break;
				}
			}
		}

		int mixdf = 0;
		std::string mixgf = GlobalData::fightPlayerMixgf;
		if (mixgf.length() > 0)
		{
			MixGfData mdata = GlobalData::map_MixGfData[mixgf];

			std::map<std::string, int>::iterator fite;
			for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
			{
				std::string nid = fite->first;
				int value = GlobalData::map_fighterPlayerData[nid];

				if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
					mixdf += ngdf * mdata.dfpercent / 100;
			}
		}
		ngdf = ngdf + masterdf + mixdf;
	}

	std::map<std::string, int>::iterator eite;
	for (eite = GlobalData::map_fighterPlayerData.begin(); eite != GlobalData::map_fighterPlayerData.end(); ++eite)
	{
		std::string nid = eite->first;
		int value = GlobalData::map_fighterPlayerData[nid];

		int goodvalue = value % 1000 - 1;
		if (nid.compare(0, 1, "a") == 0 && goodvalue > 0)
		{
			std::string aname = nid;
			adf = GlobalData::map_equips[aname].df;
			int slv = value/1000;
			slvdf = slv * (slv + adf / 10);
		}
	}
	//防御属性
	float fdf = GlobalData::map_heroAtr[m_playerData->herotype].vec_df[m_playerData->herolv] + ngdf + adf;
	fdf += slvdf;

	fdf += fdf*frienddfpercent;

	if (GlobalData::fightPlayerFactionLv == 4)
		fdf += fdf*0.02f;

	adf = int(fdf + 0.5f);
	return adf;
}
void WXPlayerChallengeLayer::disable()
{
	m_stageIcon->stopAllActions();
	m_node->setScale(1);
	std::string stagestr = "stage0.png";
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
}
int WXPlayerChallengeLayer::getPlayerAtk()
{
	int weaponAtk = 0;
	int wgAtk = 0;
	float slvAtk = 0.0f;//强化等级加攻

	std::map<std::string, int>::iterator eite;
	for (eite = GlobalData::map_fighterPlayerData.begin(); eite != GlobalData::map_fighterPlayerData.end(); ++eite)
	{
		std::string nid = eite->first;
		int value = GlobalData::map_fighterPlayerData[nid];
		int goodvalue = value % 1000 - 1;
		if (nid.compare(0, 1, "e") == 0 && goodvalue > 0)
		{
			std::string aname = nid;
			weaponAtk = GlobalData::map_equips[aname].atk;
			int slv = value / 1000;
			slvAtk = slv * (slv + weaponAtk / 10);
		}
	}

	float friendatkpercent = 0.0f;
	std::string masternpc;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::map_fightPlayerfriendly.begin(); it != GlobalData::map_fightPlayerfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::map_fightPlayerfriendly[nid].relation == F_FRIEND)
		{
			friendatkpercent += GlobalData::map_NPCFriendData[nid].atkpercent / 100;
		}
		else if (GlobalData::map_fightPlayerfriendly[nid].relation == F_MASTER)
		{
			friendatkpercent += GlobalData::map_NPCMasterData[nid].atkpercent / 100;
			masternpc = nid;
		}
	}

	std::string wgstr;
	int wgvalue = 0;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
	{
		std::string nid = fite->first;
		wgvalue = GlobalData::map_fighterPlayerData[nid] - 1;

		if (nid.compare(0, 1, "w") == 0)
			wgstr = nid;
	}
	if (wgstr.length() > 0)
	{
		int gflv = wgvalue;
		if (gflv > GlobalData::map_wgngs[wgstr].maxlv - 1)
			gflv = GlobalData::map_wgngs[wgstr].maxlv - 1;
		wgAtk = GlobalData::map_wgngs[wgstr].vec_bns[gflv];

		int masteratk = 0;
		if (masternpc.length() > 0)
		{
			for (unsigned int i = 0; i < GlobalData::map_NPCMasterData[masternpc].vec_gfid.size(); i++)
			{
				if (wgstr.compare(GlobalData::map_NPCMasterData[masternpc].vec_gfid[i]) == 0)
				{
					int msbatk = wgAtk * GlobalData::map_NPCMasterData[masternpc].vec_gfbonus[i] / 100;
					masteratk += msbatk;
					break;
				}
			}
		}

		int mixatk = 0;
		std::string mixgf = GlobalData::fightPlayerMixgf;
		if (mixgf.length() > 0)
		{
			MixGfData mdata = GlobalData::map_MixGfData[mixgf];

			std::map<std::string, int>::iterator fite;
			for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
			{
				std::string nid = fite->first;
				int value = GlobalData::map_fighterPlayerData[nid];

				if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
					mixatk += wgAtk * mdata.atkpercent / 100;
			}
		}
		wgAtk = wgAtk + masteratk + mixatk;
	}

	//攻击属性
	float fack = GlobalData::map_heroAtr[m_playerData->herotype].vec_atk[m_playerData->herolv] + weaponAtk + wgAtk;

	if (wgstr.length() > 0)
	{
		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::map_fighterPlayerData[nid] - 1;

			if (nid.compare(0, 1, "e") == 0 && value > 0)
			{
				float back = fack * 0.05f;
				fack += back;
			}
		}
	}
	fack += slvAtk;

	fack += fack* friendatkpercent;

	if (GlobalData::fightPlayerFactionLv == 5)
		fack += fack*0.02f;

	int tatk = int(fack + 0.5f);
	return tatk;
}
void WXPlayerChallengeLayer::nomal()
{
	m_stageIcon->stopAllActions();
	m_node->setScale(1);
	std::string stagestr;
	if (m_isboss)
	{
		stagestr = "sboss1.png";
	}
	else
	{
		stagestr = "stage1.png";
	}
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
}


int WXPlayerChallengeLayer::getPlayerDodde()
{
	int dodgernd = GlobalData::map_heroAtr[m_playerData->herotype].vec_dodge[m_playerData->herolv];

	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
	{
		std::string nid = fite->first;
		int ngvalue = GlobalData::map_fighterPlayerData[nid] - 1;

		if (nid.compare(0, 1, "x") == 0)
			dodgernd += GlobalData::map_wgngs[nid].vec_dodge[ngvalue];
	}



	float frienddogdernd = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::map_fightPlayerfriendly.begin(); it != GlobalData::map_fightPlayerfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::map_fightPlayerfriendly[nid].relation == F_FRIEND)
		{
			frienddogdernd += GlobalData::map_NPCFriendData[nid].dodgepercent;
		}
		else if (GlobalData::map_fightPlayerfriendly[nid].relation == F_MASTER)
		{
			frienddogdernd += GlobalData::map_NPCMasterData[nid].dodgepercent;
		}
	}
	dodgernd += frienddogdernd;

	float mixdodge = 0.0f;
	std::string mymixgf = GlobalData::getMixGF();
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mymixgf];

		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::map_fighterPlayerData[nid];

			if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
				mixdodge += mdata.dodgepercent;
		}
	}
	dodgernd += mixdodge;
	if (GlobalData::fightPlayerFactionLv == 1)
		dodgernd += 2;

	return dodgernd;
}

void WXPlayerChallengeLayer::showLock(int starnum)
{
	if (lockNode == NULL)
	{
		lockNode = Sprite::createWithSpriteFrameName("lock.png");
		lockNode->setPosition(Vec2(m_node->getContentSize().width / 2, 10));
		m_node->addChild(lockNode, 2);

		//Sprite* lockbox = Sprite::createWithSpriteFrameName("unlockbox.png");
		//lockbox->setPosition(Vec2(lockNode->getContentSize().width / 2, lockNode->getContentSize().height - 60));
		//lockNode->addChild(lockbox);

		//Sprite* star = Sprite::createWithSpriteFrameName("star1.png");
		//star->setPosition(Vec2(lockbox->getContentSize().width / 2, lockbox->getContentSize().height / 2));
		//lockbox->addChild(star);

		//std::string desc = StringUtils::format("%3d     开启", starnum);
		//Label* stardesclbl = Label::createWithBMFont("fonts/tips.fnt", CommonFuncs::gbk2utf(desc.c_str()));
		//stardesclbl->setPosition(Vec2(lockbox->getContentSize().width / 2, lockbox->getContentSize().height / 2));
		//stardesclbl->setScale(0.6f);
		//lockbox->addChild(stardesclbl);
	}
}

int WXPlayerChallengeLayer::getPlayerCrit()
{
	int critrnd = GlobalData::map_heroAtr[m_playerData->herotype].vec_crit[m_playerData->herolv];
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
	{
		std::string nid = fite->first;
		int wgvalue = GlobalData::map_fighterPlayerData[nid] - 1;

		if (nid.compare(0, 1, "w") == 0)
			critrnd += GlobalData::map_wgngs[nid].vec_cirt[wgvalue];
	}


	float friendcritrnd = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::map_fightPlayerfriendly.begin(); it != GlobalData::map_fightPlayerfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::map_fightPlayerfriendly[nid].relation == F_FRIEND)
		{
			friendcritrnd += GlobalData::map_NPCFriendData[nid].critpercent;
		}
		else if (GlobalData::map_fightPlayerfriendly[nid].relation == F_MASTER)
		{
			friendcritrnd += GlobalData::map_NPCMasterData[nid].critpercent;
		}
	}
	critrnd += friendcritrnd;

	float mixcrit = 0.0f;
	std::string mixgf = GlobalData::fightPlayerMixgf;
	if (mixgf.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mixgf];
		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::map_fighterPlayerData.begin(); fite != GlobalData::map_fighterPlayerData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::map_fighterPlayerData[nid];

			if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
				mixcrit += mdata.critpercent;
		}
	}
	critrnd += mixcrit;
	if (GlobalData::fightPlayerFactionLv == 2)
		critrnd += 2;
	return critrnd;
}


void WXPlayerChallengeLayer::removeLock()
{
	if (lockNode != NULL)
	{
		lockNode->removeFromParentAndCleanup(true);
		lockNode = NULL;
	}
}


void WXPlayerChallengeLayer::showResultLayer(int result)
{
	win = result;
	this->scheduleOnce(schedule_selector(WXPlayerChallengeLayer::delayShowResultLayer), 1.0f);
}