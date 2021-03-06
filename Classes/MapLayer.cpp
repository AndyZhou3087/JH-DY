﻿#include "MapLayer.h"
#include "json.h"
#include "HomeLayer.h"
#include "WXHomeHill.h"
#include "WXGoWhereLayer.h"
#include "GameDataSave.h"
#include "Const.h"
#include "GameScene.h"
#include "CommonFuncs.h"
#include "GlobalData.h"
#include "ShopLayer.h"
#include "SoundManager.h"
#include "UnlockLayer.h"
#include "NewerGuideLayer.h"
#include "AnalyticUtil.h"
#include "WXFightLayer.h"
#include "StoryScene.h"
#include "VipShopLayer.h"
#include "TimeGiftLayer.h"
#include "WXRelationLayer.h"
#include "WXRankLayer.h"
#include "WXFactionMainLayer.h"
#include "NewerGuide2Layer.h"
//#include "RaffleLayer.h"
#include "WXAchiveLayer.h"
//#include "PrizeLayer.h"
#include "WXHSLJMainLayer.h"
#include "WXBranchMissionLayer.h"
#include "HintBox.h"
#include "WXHelpMainLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iosfunc.h"
#endif

MapLayer* g_maplayer = NULL;
MapLayer::MapLayer()
{
	m_lotteryimg = NULL;
	ismoving = false;
	if (g_hero != NULL)
		g_hero->setIsMoving(false);
	m_isDraging = false;
	m_distance = 0.0f;
}


MapLayer::~MapLayer()
{
	ismoving = false;
	if (g_hero != NULL)
		g_hero->setIsMoving(false);
	g_maplayer = NULL;
}
void MapLayer::onGameStart() {
	playBossShowEffect();
}

void MapLayer::updateBloodBar() {

	float rate = 0;
	if (rate> 0) {
		rate = 1 - 20 * 1.0f / 20;
	}

	if (nullptr != m_bloodBar) {
		static const Vec2 offSize = m_bloodBar->getTextureRect().origin;
		static const float h = m_bloodBar->getContentSize().height;
		static const float w = m_bloodBar->getContentSize().width;
		float width = rate * w;
		m_bloodBar->setTextureRect(CCRectMake(offSize.x, offSize.y, width, h));
	}
}

bool MapLayer::init()
{
	Node* csbnode = CSLoader::createNode("mapLayer.csb");
	this->addChild(csbnode);

	m_mapscroll = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");
	m_mapscroll->setScrollBarEnabled(false);
	m_mapscroll->setSwallowTouches(false);

	m_mapbg = (cocos2d::ui::Widget*)m_mapscroll->getChildByName("mapbg");

	int mapnamecount = GlobalData::map_maps.size();
	int heroposindex = 0;
	std::string addr = GameDataSave::getInstance()->getHeroAddr();

	for (int i = 0; i < mapnamecount; i++)
	{
		cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
		mapname->addTouchEventListener(CC_CALLBACK_2(MapLayer::onclick, this));
		mapname->setSwallowTouches(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		mapname->setVisible(false);
#else
		mapname->setVisible(false);
#endif
		if (mapname->getName().compare(addr) == 0)
		{
			heroposindex = i;
		}

		if (mapname->getName().compare("m1-9") == 0)
		{
			m_lotteryimg = mapname;
			m_lotteryimg->setScale(0.0f);
		}
		else if (mapname->getName().compare("m1-10") == 0)
		{
			achiveRedpoint = (cocos2d::ui::Widget*)mapname->getChildByName("redpoint");
			achiveRedpoint->setVisible(false);
		}
		else if (mapname->getName().compare("m13-1") == 0)
		{
			mapname->setVisible(true);
		}
	}

	Vec2 pos = m_mapbg->getChildren().at(heroposindex)->getPosition();

	mapMoveTo(pos);

	m_heroPos = m_mapbg->getChildByName(addr)->getPosition();

	std::string heroidstr = StringUtils::format("ui/herohead%d.png", g_hero->getHeadID());
	m_herohead = Sprite::createWithSpriteFrameName(heroidstr);
	m_herohead->setAnchorPoint(Vec2(0.5, 0));
	m_herohead->setPosition(m_heroPos);
	if (addr.compare("m1-9") == 0)
	{
		m_herohead->setVisible(false);
	}
	m_mapscroll->addChild(m_herohead);

	updateUnlockChapter();

	for (int i = 0; i < 2; i++)
	{
		std::string missionstr = StringUtils::format("ui/mapmission%d_0.png", i);
		m_smissionIcon[i] = Sprite::createWithSpriteFrameName(missionstr);
		m_smissionIcon[i]->setAnchorPoint(Vec2(0.5, 0));
		m_smissionIcon[i]->setVisible(false);
		m_mapbg->addChild(m_smissionIcon[i]);

		missionstr = StringUtils::format("ui/mapmission%d_1.png", i);
		m_dmissionIcon[i] = Sprite::createWithSpriteFrameName(missionstr);
		m_dmissionIcon[i]->setAnchorPoint(Vec2(0.5, 0));
		m_dmissionIcon[i]->setVisible(false);
		m_mapbg->addChild(m_dmissionIcon[i]);
		updataPlotMissionIcon(i);
	}

	cocos2d::ui::Widget* shopbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("shopbtn");
	shopbtn->addTouchEventListener(CC_CALLBACK_2(MapLayer::onShop, this));


	cocos2d::ui::Widget* vipbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("vipbtn");
	vipbtn->addTouchEventListener(CC_CALLBACK_2(MapLayer::onVipShop, this));

	brachmissionicon = (cocos2d::ui::Widget*)csbnode->getChildByName("branchmission");
	brachmissionicon->setVisible(false);
	brachmissionicon->addTouchEventListener(CC_CALLBACK_2(MapLayer::onBranchMisson, this));

	m_timegiftbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("timegiftbtn");
	m_timegiftbtn->addTouchEventListener(CC_CALLBACK_2(MapLayer::onTimeGift, this));
	//m_timegiftbtn->setVisible(false);

	m_rafflebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("rafflebtn");
	m_rafflebtn->addTouchEventListener(CC_CALLBACK_2(MapLayer::onRaffle, this));
	m_rafflebtn->setVisible(false);

	m_helpbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("helpbtn");
	m_helpbtn->addTouchEventListener(CC_CALLBACK_2(MapLayer::onHelp, this));

	m_prizebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("prizebtn");
	m_prizebtn->addTouchEventListener(CC_CALLBACK_2(MapLayer::onPrize, this));
	m_prizebtn->setVisible(false);

	m_tgiftlefttimelbl = (cocos2d::ui::Text*)m_timegiftbtn->getChildByName("lefttimelbl");
	m_tgiftlefttimelbl->setString(CommonFuncs::gbk2utf("新手礼包"));
	//if (!GlobalData::isOnline)
	//{
	//	vipbtn->setVisible(false);
	//	m_timegiftbtn->setVisible(true);
	//	m_timegiftbtn->setPosition(vipbtn->getPosition());
	//	m_tgiftlefttimelbl->setString(CommonFuncs::gbk2utf("限时礼包"));
	//}
	checkTimeGift(0);
	this->schedule(schedule_selector(MapLayer::checkTimeGift), 1.0f);
	this->schedule(schedule_selector(MapLayer::checkAchive), 2.0f);

	m_vulture = (cocos2d::ui::Widget*)m_mapbg->getChildByName("m1-1")->getChildByName("vulture");
	m_vulture->setVisible(false);

	if (GlobalData::isHasVulture())
		vultureAnim();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		m_isDraging = false;
		m_startClickX = touch->getLocation().x;
		m_startClickY = touch->getLocation().y;
		return true;
	};

	listener->onTouchMoved = [=](Touch *touch, Event *event)
	{
		if (fabsf(m_startClickX - touch->getLocation().x) > 20 || fabsf(m_startClickY - touch->getLocation().y) > 20)
			m_isDraging = true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (g_nature != NULL)
	{
		if (g_nature->getWeather() == Rainy)
			rain();
		else if (g_nature->getWeather() == Snowy)
			snow();
		else
			removeWeatherPaticle();
	}
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_MAP);
	return true;
}

void MapLayer::initTime() {
	int s = 100;
	if (nullptr != m_time) {
		m_time->setString(String::createWithFormat("%d", s)->_string);
	}
}

void MapLayer::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_isDraging || ismoving)
		{
			return;
		}
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

		Node* node = (Node*)pSender;
		m_addrname = node->getName();

		if (m_addrname.compare("m1-6") == 0)
		{
			WXRelationLayer* relationlayer = WXRelationLayer::create();
			g_gameLayer->addChild(relationlayer, 5);
		}
		else if (m_addrname.compare("m1-7") == 0)
		{
			WXRankLayer* ranklayer = WXRankLayer::create();
			g_gameLayer->addChild(ranklayer, 5, "ranklayer");
		}
		else if (m_addrname.compare("m1-8") == 0)
		{
			WXFactionMainLayer* factionmainlayer = WXFactionMainLayer::create();
			g_gameLayer->addChild(factionmainlayer, 5, "factionmainlayer");
		}
		else if (m_addrname.compare("m1-10") == 0)
		{
			WXAchiveLayer* alayer = WXAchiveLayer::create();
			g_gameLayer->addChild(alayer, 5);
		}
		else if (m_addrname.compare("m1-11") == 0)
		{
			WXHSLJMainLayer* alayer = WXHSLJMainLayer::create();
			g_gameLayer->addChild(alayer, 5, "hsljmainlayer");
		}
		else
		{
			m_destPos = node->getPosition();
			m_distance = fabsf(m_heroPos.distance(m_destPos));
			WHERELAYER_TYPE type = ARRIVE;
			if (m_distance <= 1.0f)
				type = ARRIVE;
			else
				type = GOWHERE;

			if (g_gameLayer != NULL)
				g_gameLayer->addChild(WXGoWhereLayer::create(m_addrname, type, m_distance), 2, "gowherelayer");
		}

	}
}
void MapLayer::updateTime() {
	bool isok = true;
	if (isok) {
		int s = 2;
		s -= 1;
		if (s <= 4 && s >= 0) {
	
		}

		if (s <= 0) {
			s = 0;
		
		}
		else {
			
		}

		if (nullptr != m_time) {
			m_time->setString(String::createWithFormat("%d", s)->_string);
		}
	}
}
void MapLayer::heroPauseMoving()
{
	if (g_nature != NULL)
	{
		g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);
		m_herohead->pause();
	}
}
void MapLayer::resetBoss() {
	initalBoss();

	if (nullptr != m_boss) {
		m_boss->setPositionY(m_bossOriginPosY + 500);
		m_boss->setOpacity(255);
	}

	setHurtBossVisible(false);
}
void MapLayer::heroResumeMoving()
{
	if (ismoving && g_nature != NULL)
	{
		g_nature->setTimeInterval(TIMESCALE * 8);
		m_herohead->resume();
	}
}

void MapLayer::showRobberFight(float dt)
{
	heroPauseMoving();
	g_gameLayer->addChild(WXFightLayer::create(m_addrname, "n001"), 5, "fightlayer");
}
void MapLayer::onScoreChange() {
	updateBloodBar();
}

void MapLayer::showJump(float dt)
{
	heroPauseMoving();

	g_gameLayer->addChild(WXGoWhereLayer::create(m_addrname, ONWAY_JUMP), 2, "gowherelayer");
}
void MapLayer::on1sTimer() {

	if (m_bStop) {
		return;
	}

	updateTime();
}

void MapLayer::showMoveToDest()
{
	ismoving = true;
	g_hero->setIsMoving(true);
	g_nature->setTimeInterval(TIMESCALE * 8);

	float dt = moveToDestTime(m_distance);

	m_herohead->runAction(Sequence::create(MoveTo::create(dt / (TIMESCALE * 8.0f), m_destPos), CallFunc::create(CC_CALLBACK_0(MapLayer::Arrive, this)), NULL));

	//int sec = dt / (TIMESCALE * 8.0f);
	//if (sec >= 2)
	//{
	//	int r = GlobalData::createRandomNum(100);
	//	int rnd = g_nature->getDayOrNight() == Night ? 30 : 20;

	//	if (r < rnd)
	//	{
	//		int r1 = GlobalData::createRandomNum(sec - 1) + 1;

	//		this->scheduleOnce(schedule_selector(MapLayer::showRobberFight), r1);
	//	}
	//}

	if (GlobalData::map_maps[m_addrname].isCliff)
	{
		int sec = dt / (TIMESCALE * 8.0f);
		int r = GlobalData::createRandomNum(100);
		if (r < 5)
		{
			this->scheduleOnce(schedule_selector(MapLayer::showJump), sec - 0.5f);
		}
	}
}
void MapLayer::onTimeChange() {

	updateTime();
}

void MapLayer::Arrive()
{
	g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);
	m_heroPos = m_destPos;
	GameDataSave::getInstance()->setHeroAddr(m_addrname);

	if (m_distance > 1.0f)
	{
		std::string npcnames;
		int npcsize = GlobalData::map_maps[m_addrname].npcs.size();
		if (npcsize > 0)
		{
			npcnames.append(CommonFuncs::gbk2utf("这里有"));
			if (m_addrname.compare("m13-1") == 0)
			{
				npcnames.append(CommonFuncs::gbk2utf("江湖各大高手。"));
			}
			else
			{
				for (int i = 0; i < npcsize; i++)
				{
					npcnames.append(GlobalData::map_npcs[GlobalData::map_maps[m_addrname].npcs[i]].name);
					if (i == npcsize - 1)
						npcnames.append(CommonFuncs::gbk2utf("。"));
					else
						npcnames.append(CommonFuncs::gbk2utf("，"));
				}
			}
		}

		std::string str;
		if (g_hero->getAtrByType(H_MOUNT)->count > 0 && g_hero->getAtrByType(H_MOUNT)->goodvalue> 0)
			str.append(CommonFuncs::gbk2utf("你骑着马儿，一溜烟的来到了")); 
		else
			str.append(CommonFuncs::gbk2utf("你跑得双腿发麻，来到了"));

		str.append(GlobalData::map_maps[m_addrname].cname);
		str.append(CommonFuncs::gbk2utf("，"));
		str.append(GlobalData::map_maps[m_addrname].desc);
		str.append(CommonFuncs::gbk2utf("。"));
		str.append(npcnames);
		g_uiScroll->addEventText(str.c_str());

		if (g_hero->getAtrByType(H_MOUNT)->count > 0 && g_hero->getAtrByType(H_MOUNT)->strid.compare("74") == 0)
		{
			g_hero->getAtrByType(H_MOUNT)->goodvalue--;

			if (g_hero->getAtrByType(H_MOUNT)->goodvalue < 0)
			{
				g_hero->getAtrByType(H_MOUNT)->goodvalue = 0;
			}
			else if (g_hero->getAtrByType(H_MOUNT)->goodvalue == 20 || g_hero->getAtrByType(H_MOUNT)->goodvalue == 10)
			{
				std::string strid = g_hero->getAtrByType(H_MOUNT)->strid;
				std::string descstr = StringUtils::format("%s%s%d", GlobalData::map_allResource[strid].cname.c_str(), CommonFuncs::gbk2utf("生命仅剩").c_str(), g_hero->getAtrByType(H_MOUNT)->goodvalue);
				g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
			}

			if (g_hero->getAtrByType(H_MOUNT)->goodvalue <= 10)
			{
				showTalkGuide();
			}
		}
	}
	if (g_gameLayer != NULL)
		g_gameLayer->addChild(WXGoWhereLayer::create(m_addrname, ARRIVE), 2, "gowherelayer");

	this->scheduleOnce(schedule_selector(MapLayer::finishMove), 0.1f);
}
void MapLayer::onAttrackBoss() {
	playAttrackEffect();
}

void MapLayer::playAttrackEffect() {
	m_emitterBomb->resetSystem();
	if (nullptr != m_boss) {
		ActionInterval *action = Sequence::create(
			CallFunc::create([=](){ setHurtBossVisible(true); }),
			DelayTime::create(0.2),
			CallFunc::create([=](){ setHurtBossVisible(false); }),
			NULL);

		m_boss->runAction(action);

		ActionInterval * shakeAction = Sequence::create(
			ScaleTo::create(0.1, 1.4, 1.4, 1),
			ScaleTo::create(0.1, 1, 1, 1),
			NULL);

		m_boss->runAction(shakeAction);
	}
}
void MapLayer::finishMove(float dt)
{
	ismoving = false;
	g_hero->setIsMoving(false);
}

void MapLayer::onShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		ShopLayer* shopLayer = ShopLayer::create();
		g_gameLayer->addChild(shopLayer, 5);
	}
}
void MapLayer::playBossShowEffect(CallFunc * callback) {
	if (nullptr == m_boss) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	Point midPos = Vec2(size.width * 0.5, size.height * 0.5);
	ccBezierConfig config;
	config.endPosition = Point(m_bossOriginPosX, m_bossOriginPosY);

	config.controlPoint_1 = Point(midPos.x, midPos.y + 50);
	config.controlPoint_2 = Point(midPos.x, midPos.y + 100);


	ActionInterval * showAction = Sequence::create(
		EaseSineOut::create(MoveTo::create(0.8, midPos)),
		ScaleTo::create(0.2, 5, 5, 1),
		Spawn::create(
		ScaleTo::create(0.8, 1, 1, 1),
		BezierTo::create(0.8, config),
		NULL
		),
		CallFunc::create([=](){
		playBossActiveEffect();
	}),
		callback,
		NULL);
	m_boss->runAction(showAction);
}

void MapLayer::onVipShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		VipShopLayer* shopLayer = VipShopLayer::create();
		g_gameLayer->addChild(shopLayer, 5, "vipshoplayer");
	}
}
void MapLayer::playBossDeathEffect() {
	playBombEffect();
}

void MapLayer::initBossBombParticleSystem() {
	if (nullptr == m_boss) {
		return;
	}

	m_emitterBomb = ParticleSystemQuad::create("");
	m_emitterBomb->setTexture(Director::getInstance()->getTextureCache()->addImage(""));
	m_boss->addChild(m_emitterBomb, 100);
	m_emitterBomb->setPosition(Vec2(m_boss->getContentSize().width * 0.5, m_boss->getContentSize().height * 0.5));
	m_emitterBomb->stopSystem();
}
void MapLayer::onTimeGift(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		TimeGiftLayer* giftLayer = TimeGiftLayer::create(TIMEGIFT);
		g_gameLayer->addChild(giftLayer, 5, "gift");

	}
}
void MapLayer::onGameOver() {
	stopBossActiveEffect();
	playBossDeathEffect();
}

void MapLayer::playBombEffect() {

	Texture2D * txt2d = TextureCache::getInstance()->addImage("");
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width / 4;
	float h = txt2d->getContentSize().height;

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.2);
	for (int i = 0; i<5; i++) {
		ani->addSpriteFrameWithTexture(txt2d, Rect(i*w, i*h, w, h));
	}

	Sprite * sprite = Sprite::create("", Rect(0, 0, w, h));
	Sprite  * m_layer;
	m_layer->addChild(sprite, m_boss->getZOrder() + 1);
	sprite->setPosition(m_boss->getPositionX(), m_boss->getPositionY());

	sprite->runAction(Sequence::create(
		Animate::create(ani),
		CallFunc::create([=](){
		m_layer->removeChild(sprite, true);
	}),
		NULL));

	m_boss->runAction(FadeOut::create(0.8));
}
void MapLayer::onRaffle(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		//RaffleLayer* rlayer = RaffleLayer::create();
		//g_gameLayer->addChild(rlayer, 5);
	}
}
void MapLayer::initalBoss() {
	EnumBoss bossType = Boss_unknow;
	switch (bossType) {
	case Boss_Snow:
		bossType = Boss_Snow;
		break;
	case Boss_Bear:
		bossType = Boss_Bear;
		break;
	default:
		bossType = Boss_Snow;
		break;
	}
	Sprite* m_layer;
	auto bossSnow = dynamic_cast<Sprite*>(m_layer->getChildByName("boss"));
	auto bossBear = dynamic_cast<Sprite*>(m_layer->getChildByName("boss2"));

	if (Boss_Snow == bossType) {
		m_boss = bossSnow;
		bossSnow->setVisible(true);
		bossBear->setVisible(false);

		m_normalBoss_head = dynamic_cast<Sprite*>(m_layer->getChildByName("boss")
			->getChildByName("normal_body")
			->getChildByName("normal_head"));
		m_normallBoss_hand_left = dynamic_cast<Sprite*>(m_layer->getChildByName("boss")
			->getChildByName("normal_hand_left"));

		m_normallBoss_hand_right = dynamic_cast<Sprite*>(m_layer->getChildByName("boss")
			->getChildByName("normal_hand_right"));

		m_hurtBoss_head = dynamic_cast<Sprite*>(m_normalBoss_head->getChildByName("hurt_head"));
		m_hurtBoss_hand_left = dynamic_cast<Sprite*>(m_normallBoss_hand_left->getChildByName("hurt_hand_left"));
		m_hurtBoss_hand_right = dynamic_cast<Sprite*>(m_normallBoss_hand_right->getChildByName("hurt_hand_right"));
		m_hurtBoss_body = dynamic_cast<Sprite*>(m_layer->getChildByName("boss")
			->getChildByName("normal_body")
			->getChildByName("hurt_body"));
	}

	if (Boss_Bear == bossType) {
		m_boss = bossBear;
		bossSnow->setVisible(false);
		bossBear->setVisible(true);
		m_normalBoss_head = dynamic_cast<Sprite*>(m_layer->getChildByName("boss2")
			->getChildByName("normal_body")
			->getChildByName("normal_head"));
		m_normallBoss_hand_left = dynamic_cast<Sprite*>(m_layer->getChildByName("boss2")
			->getChildByName("normal_hand_left"));

		m_normallBoss_hand_right = dynamic_cast<Sprite*>(m_layer->getChildByName("boss2")
			->getChildByName("normal_hand_right"));

		m_hurtBoss_head = dynamic_cast<Sprite*>(m_normalBoss_head->getChildByName("hurt_head"));
		m_hurtBoss_hand_left = dynamic_cast<Sprite*>(m_normallBoss_hand_left->getChildByName("hurt_hand_left"));
		m_hurtBoss_hand_right = dynamic_cast<Sprite*>(m_normallBoss_hand_right->getChildByName("hurt_hand_right"));
		m_hurtBoss_body = dynamic_cast<Sprite*>(m_layer->getChildByName("boss2")
			->getChildByName("normal_body")
			->getChildByName("hurt_body"));
	}

	if (nullptr != m_normallBoss_hand_left) {
		m_normallBoss_hand_left->setRotation(-20);
	}

	if (nullptr != m_normallBoss_hand_right) {
		m_normallBoss_hand_right->setRotation(20);
	}


}

void MapLayer::onPrize(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		//PrizeLayer* pzlayer = PrizeLayer::create();
		//g_gameLayer->addChild(pzlayer, 5);
	}
}

void MapLayer::setHurtBossVisible(bool isVisible) {
	if (nullptr != m_hurtBoss_body) {
		m_hurtBoss_body->setVisible(isVisible);
	}

	if (nullptr != m_hurtBoss_head) {
		m_hurtBoss_head->setVisible(isVisible);
	}

	if (nullptr != m_hurtBoss_hand_left) {
		m_hurtBoss_hand_left->setVisible(isVisible);
	}

	if (nullptr != m_hurtBoss_hand_right) {
		m_hurtBoss_hand_right->setVisible(isVisible);
	}
}

void MapLayer::updateUnlockChapter()
{
	std::map<std::string, MapData>::iterator it;

	for (it = GlobalData::map_maps.begin(); it != GlobalData::map_maps.end(); ++it)
	{
		std::string mapid = GlobalData::map_maps[it->first].strid;
		if (mapid.length() > 0)
		{
			cocos2d::ui::Widget* mapNamImage = (cocos2d::ui::Widget*)m_mapbg->getChildByName(mapid);

			std::vector<std::string> tmp;

			CommonFuncs::split(mapid, tmp, "-");
			int mapchapter = atoi(tmp[0].substr(1, tmp[0].size() - 1).c_str());
			if (mapchapter <= GlobalData::getUnlockChapter())
			{
				if (mapNamImage != NULL)
				{
					if (GlobalData::isOnline)
						mapNamImage->setVisible(true);
					else
					{
						if (mapid.compare("m1-7") == 0 )
							mapNamImage->setVisible(false);
						else if (mapid.compare("m1-8") == 0)
							mapNamImage->setVisible(false);
						else if (mapid.compare("m1-11") == 0)
							mapNamImage->setVisible(false);
						else
							mapNamImage->setVisible(true);
					}
				}
			}
		}
	}
}
void MapLayer::playBossActiveEffect() {
	stopBossActiveEffect();

	if (nullptr != m_boss) {
		ActionInterval * jumpAction = RepeatForever::create(
			Sequence::create(
			MoveBy::create(0.1, Vec2(0, 5)),
			MoveBy::create(0.1, Vec2(0, -10)),
			MoveBy::create(0.1, Vec2(0, 5)),
			NULL)
			);
		jumpAction->setTag(1);
		m_boss->runAction(jumpAction);
	}

	if (nullptr != m_normallBoss_hand_left) {
		ActionInterval * shakeAction = RepeatForever::create(
			Sequence::create(
			RotateBy::create(0.2, 10),
			RotateBy::create(0.2, -50),
			RotateBy::create(0.2, 40),
			DelayTime::create(1),
			NULL)
			);
		shakeAction->setTag(2);
		m_normallBoss_hand_left->runAction(shakeAction);
	}

	if (nullptr != m_normallBoss_hand_right) {
		ActionInterval * shakeAction = RepeatForever::create(
			Sequence::create(
			RotateBy::create(0.2, -10),
			RotateBy::create(0.2, 50),
			RotateBy::create(0.2, -40),
			DelayTime::create(1),
			NULL)
			);
		shakeAction->setTag(3);
		m_normallBoss_hand_right->runAction(shakeAction);
	}
}
void MapLayer::showUnlockLayer(float dt)
{

	Director::getInstance()->getRunningScene()->addChild(UnlockLayer::create(), 10);
	updateUnlockChapter();
#ifdef ANALYTICS
	std::string unlockstr = StringUtils::format("u%d", GlobalData::getUnlockChapter());
	AnalyticUtil::onEvent(unlockstr.c_str());
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	this->scheduleOnce(schedule_selector(MapLayer::delayShowComments), 2.0f);
#endif
	GlobalData::doAchive(A_5, GlobalData::getUnlockChapter());
}

void MapLayer::delayShowComments(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (!GlobalData::getNoComments())
		alterView();
#endif
}
void MapLayer::stopBossActiveEffect() {
	if (nullptr != m_boss) {
		m_boss->stopActionByTag(1);
	}

	if (nullptr != m_normallBoss_hand_left) {
		m_normallBoss_hand_left->stopActionByTag(2);
		m_normallBoss_hand_left->setRotation(-20);
	}

	if (nullptr != m_normallBoss_hand_right) {
		m_normallBoss_hand_right->stopActionByTag(3);
		m_normallBoss_hand_right->setRotation(20);
	}
}
void MapLayer::updataPlotMissionIcon(int type)
{

	m_smissionIcon[type]->stopAllActions();
	m_smissionIcon[type]->setVisible(false);
	m_dmissionIcon[type]->stopAllActions();
	m_dmissionIcon[type]->setVisible(false);

	int mapnamecount = GlobalData::map_maps.size();

	if (type == 0)
	{
		PlotMissionData* plotData = NULL;
		int plotindex = 0;
		plotindex = GlobalData::getPlotMissionIndex();
		plotData = &GlobalData::vec_PlotMissionData[plotindex];

		std::string snpc = plotData->snpc;
		std::string dnpc = plotData->dnpc;

		if (plotData->mapid.length() > 0)
		{
			for (unsigned int m = 0; m < GlobalData::map_maps[plotData->mapid].npcs.size(); m++)
			{
				if (snpc.compare(GlobalData::map_maps[plotData->mapid].npcs.at(m)) == 0)
				{
					if (plotData->status == M_NONE)
					{
						if (plotData->words.size() <= 0)
						{
							plotData->status = M_DOING;
						}
						else
						{
							m_smissionIcon[type]->setPosition(m_mapbg->getChildByName(plotData->mapid)->getPosition());
							m_smissionIcon[type]->setVisible(true);
							m_smissionIcon[type]->runAction(RepeatForever::create(Blink::create(2, 3)));
						}
					}
					else
					{
						m_smissionIcon[type]->stopAllActions();
						m_smissionIcon[type]->setVisible(false);
					}
				}
			}
		}
		else
		{
			for (int i = mapnamecount - 1; i >= 0; i--)
			{
				cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
				for (unsigned int m = 0; m < GlobalData::map_maps[mapname->getName()].npcs.size(); m++)
				{
					if (snpc.compare(GlobalData::map_maps[mapname->getName()].npcs.at(m)) == 0)
					{
						if (plotData->status == M_NONE)
						{
							if (plotData->words.size() <= 0)
							{
								plotData->status = M_DOING;
							}
							else
							{
								m_smissionIcon[type]->setPosition(mapname->getPosition());
								m_smissionIcon[type]->setVisible(true);
								m_smissionIcon[type]->runAction(RepeatForever::create(Blink::create(2, 3)));
							}
						}
						else
						{
							m_smissionIcon[type]->stopAllActions();
							m_smissionIcon[type]->setVisible(false);
						}
					}
				}
			}
		}

		if (plotData->mapid.length() > 0)
		{
			for (unsigned int m = 0; m < GlobalData::map_maps[plotData->mapid].npcs.size(); m++)
			{
				if (dnpc.compare(GlobalData::map_maps[plotData->mapid].npcs.at(m)) == 0)
				{
					if (plotData->status == M_DOING)
					{
						m_dmissionIcon[type]->setVisible(true);
						m_dmissionIcon[type]->runAction(RepeatForever::create(Blink::create(2, 3)));
						m_dmissionIcon[type]->setPosition(m_mapbg->getChildByName(plotData->mapid)->getPosition());
					}
					else
					{
						m_dmissionIcon[type]->stopAllActions();
						m_dmissionIcon[type]->setVisible(false);
					}

				}
			}
		}
		else
		{
			for (int i = mapnamecount - 1; i >= 0; i--)
			{
				cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
				for (unsigned int m = 0; m < GlobalData::map_maps[mapname->getName()].npcs.size(); m++)
				{
					if (dnpc.compare(GlobalData::map_maps[mapname->getName()].npcs.at(m)) == 0)
					{
						if (plotData->status == M_DOING)
						{
							m_dmissionIcon[type]->setVisible(true);
							m_dmissionIcon[type]->runAction(RepeatForever::create(Blink::create(2, 3)));
							m_dmissionIcon[type]->setPosition(mapname->getPosition());
						}
						else
						{
							m_dmissionIcon[type]->stopAllActions();
							m_dmissionIcon[type]->setVisible(false);
						}
					}
				}
			}
		}
	}
}
void MapLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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

void MapLayer::mapMoveTo(Vec2 pos)
{
	float offsetx = 0.0f;
	float offsety = 0.0f;
	Size scollviewsize = m_mapscroll->getContentSize();
	if (pos.x > scollviewsize.width / 2)
		offsetx = pos.x - scollviewsize.width / 2;
	if (pos.y > scollviewsize.height / 2)
		offsety = pos.y - scollviewsize.height / 2;

	if (pos.x + scollviewsize.width / 2 > m_mapscroll->getInnerContainerSize().width)
		offsetx = m_mapscroll->getInnerContainerSize().width - scollviewsize.width;
	if (pos.y + scollviewsize.height / 2 > m_mapscroll->getInnerContainerSize().height)
		offsety = m_mapscroll->getInnerContainerSize().height - scollviewsize.height;

	m_mapscroll->setInnerContainerPosition(Vec2(-offsetx, -offsety));
}

void MapLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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

void MapLayer::checkNewerGuide()
{
	if (NewerGuideLayer::checkifNewerGuide(14))
		showNewerGuide(14);
	else if (NewerGuideLayer::checkifNewerGuide(38))
		showNewerGuide(38);
	else if (NewerGuideLayer::checkifNewerGuide(58))
		showNewerGuide(58);
}
void MapLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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

float MapLayer::moveToDestTime(float distance)
{
	float dt = distance * HERO_MOVE_SPEED;
	if (g_hero->getAtrByType(H_MOUNT)->count > 0 && g_hero->getAtrByType(H_MOUNT)->goodvalue > 0)
	{
		int index = -1;
		for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
		{
			ResData rdata = GlobalData::vec_resData[i];
			if (rdata.strid.compare(g_hero->getAtrByType(H_MOUNT)->strid) == 0)
			{
				index = i;
				break;
			}
		}
		if (index >= 0)
		{
			int ep = GlobalData::vec_resData[index].ep[0];
			dt *= 100.0f / (100.0f + ep);
		}
	}
	return dt;
}

void MapLayer::showEndAnim(float dt)
{
	Director::getInstance()->pushScene(StoryScene::createScene(1));
}

void MapLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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

void MapLayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 14)
	{
		nodes.push_back(m_mapbg->getChildByName("m1-2"));
		NewerGuideLayer::pushUserData("m1-2");
	}
	else if (step == 38)
	{
		nodes.push_back(m_mapbg->getChildByName("m1-1"));
		NewerGuideLayer::pushUserData("m1-1");
	}
	else if (step == 58)
	{
		nodes.push_back(m_mapbg->getChildByName("m1-4"));
		NewerGuideLayer::pushUserData("m1-4");
	}
	if (step == 14 || step == 38 || step == 58)
		g_gameLayer->showNewerGuide(step, nodes);
}

void MapLayer::rain()
{
	removeWeatherPaticle();
	ParticleRain *rain = ParticleRain::create();
	rain->setScale(5);
	rain->setTotalParticles(1500);
	rain->setPosition(Vec2(360, 1400));
	rain->setTextureWithRect(Director::getInstance()->getTextureCache()->addImage("particle/rain.png"), Rect(0, 0, 96, 96));
	this->addChild(rain, 1000, "rain");
}

void MapLayer::snow()
{
	removeWeatherPaticle();
	ParticleSnow *snow = ParticleSnow::create();
	snow->setSpeed(100);
	snow->setPosition(Vec2(360, 1400));
	snow->setTextureWithRect(Director::getInstance()->getTextureCache()->addImage("particle/snow.png"), Rect(0, 0, 32, 32));
	this->addChild(snow, 1000, "snow");
}

void MapLayer::removeWeatherPaticle()
{
	if (g_maplayer != NULL)
	{
		g_maplayer->removeChildByName("rain");
		g_maplayer->removeChildByName("snow");
	}
}

void MapLayer::vultureAnim()
{
	m_vulture->stopAllActions();
	m_vulture->setVisible(true);

	auto animation = Animation::create();
	for (int i = 1; i < 10; i++)
	{
		char szName[20] = { 0 };
		sprintf(szName, "ui/vulture%d.png", i);
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szName);
		animation->addSpriteFrame(frame);
	}
	animation->setDelayPerUnit(0.1f);
	//animation->setRestoreOriginalFrame(true);

	auto action = Animate::create(animation);
	PointArray *arrayPoint = PointArray::create(5);
	arrayPoint->addControlPoint(Vec2(30, 110));
	arrayPoint->addControlPoint(Vec2(55, 90));
	arrayPoint->addControlPoint(Vec2(30, 70));
	arrayPoint->addControlPoint(Vec2(10,90));
	arrayPoint->addControlPoint(Vec2(30, 110));
	ActionInterval *forward = CCCardinalSplineTo::create(arrayPoint->count(), arrayPoint, 0);
	m_vulture->runAction(RepeatForever::create(forward));
	m_vulture->runAction(RepeatForever::create(action));
	//m_vulture->runAction(Sequence::create(action, action->reverse(), NULL));
}
void MapLayer::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void MapLayer::shake(Node * node) {
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
void MapLayer::checkTimeGift(float dt)
{
	//int lefttime = GlobalData::getTimeGiftLeftTime();
	//if (lefttime > 0 && !GlobalData::getIsBuyTimeGift())
	//{
	//	m_timegiftbtn->setVisible(true);
	//	int hour = lefttime / 3600;
	//	int min = lefttime % 3600 / 60;
	//	int sec = lefttime % 3600 % 60;
	//	std::string timestr = StringUtils::format("%02d:%02d:%02d", hour,min,sec);
	//	m_tgiftlefttimelbl->setString(timestr);
	//}
	//else
	//{
	//	if (!GlobalData::isOnline)
	//		m_timegiftbtn->setVisible(true);
	//	else
	//		m_timegiftbtn->setVisible(false);
	//}
	if (GlobalData::myRaffleData.isshow)
	{
		m_rafflebtn->setVisible(true);
		if (m_timegiftbtn->isVisible())
		{
			m_rafflebtn->setPositionY(500);
		}
		else
		{
			m_rafflebtn->setPositionY(665);
		}
	}
	if (GlobalData::myLotteryData.isshow && m_lotteryimg != NULL)
	{
		m_herohead->setVisible(true);
		m_lotteryimg->setScale(1.0f);
	}

	//m_prizebtn->setVisible(GlobalData::isExchangeGift);

	updateBranchMissionTime();
}
int MapLayer::getRandomNum(int range) {

	if (range <= 0) {
		return 0;
	}

	return rand() % range;
}

int MapLayer::getRandomNum(int rangeStart, int rangeEnd) {

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

void MapLayer::checkAchive(float dt)
{
	bool ishas = false;
	for (unsigned int i = 0; i < GlobalData::vec_achiveData.size(); i++)
	{
		if (GlobalData::vec_achiveData[i].finish >= GlobalData::getAchiveFinishCount(GlobalData::vec_achiveData[i]))
		{
			ishas = true;
		}
	}
	if (ishas)
	{
		achiveRedpoint->setVisible(true);
	}
	else
	{
		achiveRedpoint->setVisible(false);
	}
}

void MapLayer::showTalkGuide()
{
	if (NewerGuide2Layer::checkifNewerGuide(103))
	{
		std::vector<Node*> vec_node;
		NewerGuide2Layer *layer = NewerGuide2Layer::create(103, vec_node);
		g_gameLayer->addChild(layer, NEWERLAYERZOER);
	}
}

void MapLayer::updateBranchMissionTime()
{
	std::string curmid = GlobalData::getCurBranchPlotMissison();

	if (curmid.length() > 0)
	{
		string str;
		brachmissionicon->setVisible(true);
		cocos2d::ui::Text* timetext = (cocos2d::ui::Text*)brachmissionicon->getChildByName("timetext");
		if (GlobalData::map_BranchPlotMissionItem[curmid].time > 0)
			str = StringUtils::format("%02d:%02d", GlobalData::map_BranchPlotMissionItem[curmid].time / 60, GlobalData::map_BranchPlotMissionItem[curmid].time % 60);
		else
			str = "不限时";
		timetext->setString(str);
	}
	else
	{
		brachmissionicon->setVisible(false);
		if (g_gameLayer != NULL)
			g_gameLayer->removeChildByName("bmissionlayer");
	}
}
bool MapLayer::getRandomBoolean(float rate) {

	int rate10 = (int)(rate*10.0);
	int randNum = rand();
	if (randNum % 10 <= rate10) {
		return true;
	}
	else {
		return false;
	}
}

bool MapLayer::getRandomBoolean() {

	if (0 == rand() % 2) {
		return true;
	}
	else {
		return false;
	}
}
void MapLayer::onBranchMisson(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		if (GlobalData::getCurBranchPlotMissison().length() > 0)
		{
			WXBranchMissionLayer* layer = WXBranchMissionLayer::create();
			g_gameLayer->addChild(layer, 5, "bmissionlayer");
		}

	}
}

void MapLayer::onHelp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		WXHelpMainLayer* layer = WXHelpMainLayer::create();
		g_gameLayer->addChild(layer, 5);
	}
}
