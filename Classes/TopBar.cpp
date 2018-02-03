#include "TopBar.h"
#include "CommonFuncs.h"
#include "GameDataSave.h"
#include "GameScene.h"
#include "SysSmallBox.h"
#include "HeroStateUILayer.h"
#include "SoundManager.h"
#include "Const.h"
#include "WXActivitScene.h"
#include "HomeLayer.h"
#include "MapLayer.h"
#include "NewerGuideLayer.h"
#include "WXFriendExgScene.h"
#include "WXFriendNpcScene.h"
#include "WXFightLayer.h"
#include "WXApprenticeScene.h"

TopBar::TopBar()
{
	pastmin = g_nature->getTime();
	isHunter = false;
	isShowingGuide = false;
	newerstep = 0;
}


TopBar::~TopBar()
{

}

bool TopBar::init()
{
	Node* csbnode = CSLoader::createNode("topBarNode.csb");
	this->addChild(csbnode);

	heroimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroimg");
	heroimg->setName("hero");
	heroimg->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));
	std::string heroidstr = StringUtils::format("ui/tophero%d.png", g_hero->getHeadID());
	heroimg->loadTexture(heroidstr, cocos2d::ui::TextureResType::PLIST);
	heroimg->setContentSize(Sprite::createWithSpriteFrameName(heroidstr)->getContentSize());

	reason = (cocos2d::ui::ImageView*)csbnode->getChildByName("reason");
	reason->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));
	reason->setName("reason");

	std::string str = StringUtils::format("ui/top_r_season%d.png", g_nature->getReason());
	reason->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	reason->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	weather = (cocos2d::ui::ImageView*)csbnode->getChildByName("weather");
	weather->setName("weather");
	weather->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	str = StringUtils::format("ui/top_weather%d.png", g_nature->getWeather());
	weather->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	weather->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	cocos2d::ui::ImageView* livedaysincon = (cocos2d::ui::ImageView*)csbnode->getChildByName("livedaysincon");
	livedaysincon->setName("livedays");
	livedaysincon->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* timeicon = (cocos2d::ui::ImageView*)csbnode->getChildByName("toptimeicon");
	timeicon->setName("time");
	timeicon->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* temperature = (cocos2d::ui::ImageView*)csbnode->getChildByName("toptemperature");
	temperature->setName("temperature");
	temperature->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	livedayslbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("livedayslbl");
	livedayslbl->setName("livedays");
	livedayslbl->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	str = StringUtils::format("%d", g_nature->getPastDays());
	livedayslbl->setString(str);

	timelbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("timelbl");
	timelbl->setName("time");
	timelbl->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	int hour = g_nature->getTime() / 60;
	int minute = (int)g_nature->getTime() % 60;
	str = StringUtils::format("%02d:%02d", hour, minute);

	timelbl->setString(str);

	templbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("templbl");
	templbl->setName("temperature");
	templbl->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	str = StringUtils::format("%d", g_nature->getTemperature());
	templbl->setString(str);

	outinjury = (cocos2d::ui::ImageView*)csbnode->getChildByName("topoutinjurybg");
	outinjury->setName("outinjury");
	outinjury->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	innerinjury = (cocos2d::ui::ImageView*)csbnode->getChildByName("topinnerinjurybg");
	innerinjury->setName("innerinjury");
	innerinjury->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	hunger = (cocos2d::ui::ImageView*)csbnode->getChildByName("tophungerbg");
	hunger->setName("hunger");
	hunger->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	spirit = (cocos2d::ui::ImageView*)csbnode->getChildByName("topspiritbg");
	spirit->setName("spirit");
	spirit->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	life = (cocos2d::ui::ImageView*)csbnode->getChildByName("toplifebg");
	life->setName("life");
	life->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	Sprite* sprite0 = Sprite::createWithSpriteFrameName("ui/topoutinjurybar.png");
	outinjuryBar = ProgressTimer::create(sprite0);
	outinjuryBar->setType(ProgressTimer::Type::BAR);
	outinjuryBar->setBarChangeRate(Vec2(0, 1));
	outinjuryBar->setMidpoint(Vec2(0, 0));
	outinjuryBar->setPercentage(g_hero->getOutinjuryValue());
	outinjuryBar->setPosition(outinjury->getPosition());
	csbnode->addChild(outinjuryBar);

	Sprite* sprite1 = Sprite::createWithSpriteFrameName("ui/topinnerinjurybar.png");
	innerinjuryBar = ProgressTimer::create(sprite1);
	innerinjuryBar->setType(ProgressTimer::Type::BAR);
	innerinjuryBar->setBarChangeRate(Vec2(0, 1));
	innerinjuryBar->setMidpoint(Vec2(0, 0));
	innerinjuryBar->setPercentage(g_hero->getInnerinjuryValue());
	innerinjuryBar->setPosition(innerinjury->getPosition());
	csbnode->addChild(innerinjuryBar);

	Sprite* sprite2 = Sprite::createWithSpriteFrameName("ui/tophungerbar.png");
	hungerBar = ProgressTimer::create(sprite2);
	hungerBar->setType(ProgressTimer::Type::BAR);
	hungerBar->setBarChangeRate(Vec2(0, 1));
	hungerBar->setMidpoint(Vec2(0, 0));
	hungerBar->setPercentage(g_hero->getHungerValue());
	hungerBar->setPosition(hunger->getPosition());
	csbnode->addChild(hungerBar);

	Sprite* sprite3 = Sprite::createWithSpriteFrameName("ui/topspiritbar.png");
	spiritBar = ProgressTimer::create(sprite3);
	spiritBar->setType(ProgressTimer::Type::BAR);
	spiritBar->setBarChangeRate(Vec2(0, 1));
	spiritBar->setMidpoint(Vec2(0, 0));
	spiritBar->setPercentage(g_hero->getSpiritValue());
	spiritBar->setPosition(spirit->getPosition());
	csbnode->addChild(spiritBar);

	Sprite* sprite4 = Sprite::createWithSpriteFrameName("ui/toplifebar.png");
	lifeBar = ProgressTimer::create(sprite4);
	lifeBar->setType(ProgressTimer::Type::BAR);
	lifeBar->setBarChangeRate(Vec2(0, 1));
	lifeBar->setMidpoint(Vec2(0, 0));
	lifeBar->setPercentage(g_hero->getLifeValue() * 100.0f / g_hero->getMaxLifeValue());
	lifeBar->setPosition(life->getPosition());
	csbnode->addChild(lifeBar);
	outinjuryRed = (cocos2d::ui::Widget*)csbnode->getChildByName("topoutinjuryred");
	outinjuryRed->setLocalZOrder(1);
	innerinjuryRed = (cocos2d::ui::Widget*)csbnode->getChildByName("topinnerinjuryred");
	innerinjuryRed->setLocalZOrder(1);
	hungerRed = (cocos2d::ui::Widget*)csbnode->getChildByName("tophungerred");
	hungerRed->setLocalZOrder(1);
	spiritRed = (cocos2d::ui::Widget*)csbnode->getChildByName("topspiritred");
	spiritRed->setLocalZOrder(1);
	lifeRed = (cocos2d::ui::Widget*)csbnode->getChildByName("toplifered");
	lifeRed->setLocalZOrder(1);

	
	int maxlv = GlobalData::map_heroAtr[g_hero->getHeadID()].vec_exp.size();
	int lv = g_hero->getLVValue();
	if (lv >= maxlv)
		lv = maxlv - 1;
		
	int percent = g_hero->getExpValue() * 100 / GlobalData::map_heroAtr[g_hero->getHeadID()].vec_exp[lv];
	
	toplvexpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("toplvexpbar");
	toplvexpbar->setPercent(percent);
	//等级属性
	str = StringUtils::format("%d", lv + 1);
	lvtext = (cocos2d::ui::Text*)csbnode->getChildByName("lvtext");
	lvtext->setString(str);

	m_lastinnerinjury = g_hero->getInnerinjuryValue();
	m_lastoutinjury = g_hero->getOutinjuryValue();
	m_lasthunger = g_hero->getHungerValue();
	m_lastspirit = g_hero->getSpiritValue();
	m_lastlife = g_hero->getLifeValue();

	m_lastweather = g_nature->getWeather();

	m_lastDayOrNigth = g_nature->getDayOrNight();

	m_lastpastLiveDay = g_nature->getPastDays();

	schedule(schedule_selector(TopBar::updataUI), NORMAL_TIMEINTERVAL * 1.0f/TIMESCALE);

	return true;
}

void TopBar::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		Node* cnode = (Node*)pSender;
		SysSmallBox* sbox = NULL;

		if (g_NewerGuideLayer != NULL)
		{
			g_NewerGuideLayer->removeFromParentAndCleanup(true);
			g_NewerGuideLayer = NULL;
		}

		if (cnode->getName().compare("hero") == 0)
		{
			g_gameLayer->addChild(HeroStateUILayer::create(), 4, "HeroStateUILayer");
		}
		else if (cnode->getName().compare("reason") == 0)
		{
			int rv = g_nature->getReason();
			std::string str = StringUtils::format("ui/top_r_season%d.png", rv);
			sbox = SysSmallBox::create(BoxType::REASON, str, reasonname[rv], reasondesc1[rv], reasondesc[rv]);
		}
		else if (cnode->getName().compare("weather") == 0)
		{
			int rw = g_nature->getWeather();
			std::string str = StringUtils::format("ui/top_weather%d.png", rw);
			sbox = SysSmallBox::create(BoxType::WEATHER, str, weathername[rw], weatherdesc1[rw], weatherdesc[rw]);
		}
		else if (cnode->getName().compare("livedays") == 0)
		{
			std::string str = StringUtils::format("%d天", g_nature->getPastDays());
			sbox = SysSmallBox::create(BoxType::LIVEDAYS, "ui/toplivedaysicon.png", str,"", liveDayDesc);

		}
		else if (cnode->getName().compare("time") == 0)
		{
			int hour = g_nature->getTime() / 60;
			int minute = (int)g_nature->getTime() % 60;
			std::string str = StringUtils::format("%02d:%02d", hour, minute);
			sbox = SysSmallBox::create(BoxType::TIME, "ui/toptimeicon.png", str, "", timeDesc);
		}
		else if (cnode->getName().compare("temperature") == 0)
		{
			std::string str = StringUtils::format("%d℃", g_nature->getTemperature());
			sbox = SysSmallBox::create(BoxType::TEMPERATURE, "ui/toptemperature.png", str, "", tempeDesc);
		}
		else if (cnode->getName().compare("outinjury") == 0)
		{
			std::string str = "外伤";
			int index = 0;
			int valuerange[] = { 90, 70, 40, 30, 20, 10, 0 };
			for (int i = 0; i < 7; i++)
			{
				if (g_hero->getOutinjuryValue() >= valuerange[i])
				{
					index = i;
					break;
				}
			}
			sbox = SysSmallBox::create(BoxType::OUTERINJURY, "ui/topoutinjurybg.png", str, outInjurydesc1[index], outInjurydesc);
		}
		else if (cnode->getName().compare("innerinjury") == 0)
		{
			std::string str = "内伤";

			int index = 0;
			int valuerange[] = { 90, 70, 40, 30, 20, 10, 0 };
			for (int i = 0; i < 7; i++)
			{
				if (g_hero->getInnerinjuryValue() >= valuerange[i])
				{
					index = i;
					break;
				}
			}

			sbox = SysSmallBox::create(BoxType::INNERINJURY, "ui/topinnerinjurybg.png", str, innerInjurydesc1[index], innerInjurydesc);
		}
		else if (cnode->getName().compare("hunger") == 0)
		{
			std::string str = "饱食度";

			int index = 0;
			int valuerange[] = { 90, 70, 40, 30, 20, 10, 0 };
			for (int i = 0; i < 7; i++)
			{
				if (g_hero->getHungerValue() >= valuerange[i])
				{
					index = i;
					break;
				}
			}

			sbox = SysSmallBox::create(BoxType::HUNGER, "ui/tophungerbg.png", str, hungerdesc1[index], hungerdesc);
		}
		else if (cnode->getName().compare("spirit") == 0)
		{
			std::string str = "精神";


			int index = 0;
			int valuerange[] = { 90, 70, 40, 30, 20, 10, 0 };
			for (int i = 0; i < 7; i++)
			{
				if (g_hero->getSpiritValue() >= valuerange[i])
				{
					index = i;
					break;
				}
			}

			sbox = SysSmallBox::create(BoxType::SPIRIT, "ui/topspiritbg.png", str, spiritInjurydesc1[index], spiritInjurydesc);
		}
		else if (cnode->getName().compare("life") == 0)
		{
			std::string str = "气血";
			std::string livevaluestr = StringUtils::format("%d/%d", (int)g_hero->getLifeValue(), (int)g_hero->getMaxLifeValue());
			sbox = SysSmallBox::create(BoxType::LIFE, "ui/toplifebg.png", str, livevaluestr, lifedesc);
		}
		if (sbox != NULL)
			g_gameLayer->addChild(sbox, 4);
	}
}

void TopBar::updataUI(float dt)
{
	if (GlobalData::g_gameStatus != GAMESTART || GlobalData::isPlayerChallenging)
		return;

	if (g_hero != NULL && g_hero->getIsWDChallenge())
	{
		outinjuryBar->setPercentage(g_hero->getOutinjuryValue());
		innerinjuryBar->setPercentage(g_hero->getInnerinjuryValue());
		lifeBar->setPercentage(g_hero->getLifeValue() * 100.0f / g_hero->getMaxLifeValue());

		if (m_lastinnerinjury != (int)g_hero->getInnerinjuryValue())
		{
			innerinjuryRed->runAction(Sequence::create(Show::create(), Blink::create(1.3f, 2), Hide::create(), NULL));
			m_lastinnerinjury = (int)g_hero->getInnerinjuryValue();
		}
		if (m_lastoutinjury != (int)g_hero->getOutinjuryValue())
		{
			outinjuryRed->runAction(Sequence::create(Show::create(), Blink::create(1.3f, 2), Hide::create(), NULL));
			m_lastoutinjury = (int)g_hero->getOutinjuryValue();
		}
		if (m_lastlife != (int)g_hero->getLifeValue())
		{
			lifeRed->runAction(Sequence::create(Show::create(), Blink::create(1.3f, 2), Hide::create(), NULL));
			m_lastlife = (int)g_hero->getLifeValue();
		}
		return;
	}

	std::string str;
	pastmin += g_nature->getTimeInterval();
	if (pastmin >= 1440.0f)
	{
		pastmin -= 1440.0f;
		int livedays = g_nature->getPastDays();
		str = StringUtils::format("%d", livedays);
		livedayslbl->setString(str);

		str = StringUtils::format("ui/top_r_season%d.png", g_nature->getReason());
		reason->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		reason->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		GameDataSave::getInstance()->setLiveDays(livedays);
		GameDataSave::getInstance()->setNatureReason(g_nature->getReason());
	}
	Scene* activityScene = NULL;
	bool isthieves = false;
	if (g_nature->getPastDays() >= 1 && m_lastpastLiveDay != g_nature->getPastDays())
	{
		m_lastpastLiveDay = g_nature->getPastDays();

		checkNpcRandMap();
		if (g_nature->getReason() == Autumn && !g_nature->getIsShowInsect())
		{
			int r = GlobalData::createRandomNum(100);
			if (r < 10)
			{
				g_nature->setIsShowInsect(true);
				activityScene = WXActivitScene::createScene("images/insect.jpg", CommonFuncs::gbk2utf("蝗虫泛滥"));
				for (unsigned int i = 0; i < GlobalData::vec_hillResid.size(); i++)
				{
					for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
					{
						ResData* data = &GlobalData::vec_resData[m];
						int type = data->type - 1;
						if (data->strid.compare(GlobalData::vec_hillResid[i]) == 0 && (type == FOOD || type == MEDICINAL))
						{
							int count = data->count % 2 == 0 ? data->count / 2 : (data->count / 2 + 1);
							data->count = count;
						}
					}
				}
			}
		}

		if (!g_nature->getIsShowInsect() && !isHunter && g_hero->getLVValue() >= 10)
		{
			int r = GlobalData::createRandomNum(100);
			if (r < 10)
			{
				isHunter = true;
				activityScene = WXActivitScene::createScene("images/hunter.jpg", CommonFuncs::gbk2utf("猎人来啦"));
				for (unsigned int i = 0; i < GlobalData::vec_hillResid.size(); i++)
				{
					for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
					{
						ResData* data = &GlobalData::vec_resData[m];
						int type = data->type - 1;
						if (data->strid.compare(GlobalData::vec_hillResid[i]) == 0 && (data->strid.compare("67") == 0 || data->strid.compare("68") == 0))
						{
							int count = data->count % 2 == 0 ? data->count / 2 : (data->count / 2 + 1);
							data->count = count;
						}
					}
				}
			}
		}

		if (!g_nature->getIsShowInsect() && !isHunter)
		{
			if (g_hero->getIsOut())
			{
				int r = GlobalData::createRandomNum(100);
				if (r < 15)
					isthieves = true;
			}
			if (isthieves)
			{
				activityScene = WXActivitScene::createScene("images/thieves.jpg", CommonFuncs::gbk2utf("盗贼到来，丢失以下物品..."));
			}
			else
			{
				activityScene = WXActivitScene::createScene("images/cday.jpg", CommonFuncs::gbk2utf("今夜很平静，新的一天开始..."));
			}
		}

		g_nature->setIsShowInsect(false);
		isHunter = false;
	}

	if (m_lastweather != g_nature->getWeather())
	{
		str = StringUtils::format("ui/top_weather%d.png", g_nature->getWeather());
		weather->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		weather->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		if (g_nature->getWeather() == Rainy)
		{
			activityScene = WXActivitScene::createScene("images/cweatherrain.jpg", CommonFuncs::gbk2utf("下雨了..."));
			if (g_maplayer != NULL)
				g_maplayer->rain();
		}
		else if (g_nature->getWeather() == Snowy)
		{
			activityScene = WXActivitScene::createScene("images/cweathersnow.jpg", CommonFuncs::gbk2utf("下雪了..."));
			if (g_maplayer != NULL)
				g_maplayer->snow();
		}
		else
		{
			if (g_maplayer != NULL)
				g_maplayer->removeWeatherPaticle();
		}

		m_lastweather = g_nature->getWeather();
	}

	if (m_lastDayOrNigth != g_nature->getDayOrNight())
	{
		if (g_nature->getDayOrNight() == Day)
		{
			int aday = GameDataSave::getInstance()->getApprenticeDay();

			int r = GlobalData::createRandomNum(100);
			if (r < 10)
			{
				int r1 = GlobalData::createRandomNum(100);
				int type = r1 < 50 ? 0 : 1;
				activityScene = WXFriendExgScene::createScene(type);
			}
			else if (r < 20)
			{
				WXFightLayer* fightlayer = (WXFightLayer*)g_gameLayer->getChildByName("fightlayer");
				if (fightlayer == NULL)
					activityScene = WXFriendNpcScene::createScene();
			}
			else
			{
				if (g_hero->getLVValue() >= 29)
				{
					if (aday == 0 || (aday < 0 && r < 30))
						activityScene = WXApprenticeScene::createScene(0);
				}
			}
			if (activityScene == NULL)
			{
				if (aday > 0 && g_nature->getPastDays() - aday >= 30)
					activityScene = WXApprenticeScene::createScene(1);
			}
		}
		if (g_nature->getDayOrNight() == Day)
		{
			std::vector<std::string> tips;
			if (g_hero->getLVValue() < 10)
			{
				for (unsigned int i = 0; i < sizeof(tipswords1) / sizeof(tipswords1[0]); i++)
				{
					tips.push_back(tipswords1[i]);
				}
			}
			else if (g_hero->getLVValue() < 30)
			{
				for (unsigned int i = 0; i < sizeof(tipswords2) / sizeof(tipswords2[0]); i++)
				{
					tips.push_back(tipswords2[i]);
				}
			}
			else
			{
				for (unsigned int i = 0; i < sizeof(tipswords3) / sizeof(tipswords3[0]); i++)
				{
					tips.push_back(tipswords3[i]);
				}
			}

			if (g_nature->getReason() == Winter)
			{
				for (unsigned int i = 0; i < sizeof(tipswords0) / sizeof(tipswords0[0]); i++)
				{
					tips.push_back(tipswords0[i]);
				}
			}
			int tsize = tips.size();
			int r = GlobalData::createRandomNum(tsize);
			g_uiScroll->addEventText(CommonFuncs::gbk2utf(tips[r].c_str()), 26, Color3B(27, 141, 0));
			//activityScene = WXActivitScene::createScene("images/cday.jpg", CommonFuncs::gbk2utf("天亮了..."));
		}
		else
		{
			//activityScene = WXActivitScene::createScene("images/cday.jpg", CommonFuncs::gbk2utf("黑夜降临..."));
		}
		m_lastDayOrNigth = g_nature->getDayOrNight();
	}

	if (activityScene != NULL)
	{
		auto transition = TransitionCrossFade::create(0.5f, activityScene);
		Director::getInstance()->pushScene(transition);
	}

	GameDataSave::getInstance()->setNatureTemperature(g_nature->getTemperature());
	int showtime = g_nature->getTime();
	int hour = showtime / 60;
	int minute = (int)showtime % 60;

	str = StringUtils::format("%02d:%02d", hour, minute);
	timelbl->setString(str);
	str = StringUtils::format("%d", g_nature->getTemperature());
	templbl->setString(str);

	outinjuryBar->setPercentage(g_hero->getOutinjuryValue());
	innerinjuryBar->setPercentage(g_hero->getInnerinjuryValue());
	hungerBar->setPercentage(g_hero->getHungerValue());
	spiritBar->setPercentage(g_hero->getSpiritValue());
	lifeBar->setPercentage(g_hero->getLifeValue() * 100.0f / g_hero->getMaxLifeValue());

	int lv = g_hero->getLVValue();
	int percent = g_hero->getExpValue() * 100 / GlobalData::map_heroAtr[g_hero->getHeadID()].vec_exp[lv];
	toplvexpbar->setPercent(percent);
	//等级属性
	str = StringUtils::format("%d", lv + 1);
	lvtext->setString(str);

	bool isnewer = false;
	if (m_lastinnerinjury != (int)g_hero->getInnerinjuryValue())
	{
		innerinjuryRed->runAction(Sequence::create(DelayTime::create(GlobalData::createRandomNum(1)), Show::create(), Blink::create(2.0f, 3), Hide::create(), NULL));
		m_lastinnerinjury = (int)g_hero->getInnerinjuryValue();
		if (g_hero->getInnerinjuryValue() <= g_hero->getMaxInnerinjuryValue() * 0.5f)
			isnewer = true;
	}
	if (m_lastoutinjury != (int)g_hero->getOutinjuryValue())
	{
		outinjuryRed->runAction(Sequence::create(DelayTime::create(GlobalData::createRandomNum(1)), Show::create(), Blink::create(2.0f, 3), Hide::create(), NULL));
		m_lastoutinjury = (int)g_hero->getOutinjuryValue();
		if (g_hero->getOutinjuryValue() <= g_hero->getMaxOutinjuryValue() * 0.5f)
			isnewer = true;
	}
	if (m_lasthunger != (int)g_hero->getHungerValue())
	{
		hungerRed->runAction(Sequence::create(DelayTime::create(GlobalData::createRandomNum(1)), Show::create(), Blink::create(2.0f, 3), Hide::create(), NULL));
		m_lasthunger = (int)g_hero->getHungerValue();
		if (g_hero->getHungerValue() <= g_hero->getMaxHungerValue() * 0.5f)
			isnewer = true;
	}
	if (m_lastspirit != (int)g_hero->getSpiritValue())
	{
		spiritRed->runAction(Sequence::create(DelayTime::create(GlobalData::createRandomNum(1)), Show::create(), Blink::create(2.0f, 3), Hide::create(), NULL));
		m_lastspirit = (int)g_hero->getSpiritValue();
		if (g_hero->getSpiritValue() <= g_hero->getMaxSpiritValue() * 0.5f)
			isnewer = true;
	}
	if (m_lastlife != (int)g_hero->getLifeValue())
	{
		lifeRed->runAction(Sequence::create(DelayTime::create(GlobalData::createRandomNum(1)), Show::create(), Blink::create(2.0f, 3), Hide::create(), NULL));
		m_lastlife = (int)g_hero->getLifeValue();
		if (g_hero->getLifeValue() <= g_hero->getMaxLifeValue() * 0.5f)
			isnewer = true;
	}
	if (isnewer && g_NewerGuideLayer == NULL && !isShowingGuide)
	{
		WXFightLayer* fightlayer = (WXFightLayer*)g_gameLayer->getChildByName("fightlayer");
		if (fightlayer == NULL)
		{
			isShowingGuide = true;
			if (NewerGuideLayer::checkifNewerGuide(59))
				showNewerGuide(59);
		}
	}
}

void TopBar::stopLoseAnim()
{
	innerinjuryRed->stopAllActions();
	innerinjuryRed->setVisible(false);

	outinjuryRed->stopAllActions();
	outinjuryRed->setVisible(false);

	hungerRed->stopAllActions();
	hungerRed->setVisible(false);

	spiritRed->stopAllActions();
	spiritRed->setVisible(false);

	lifeRed->stopAllActions();
	lifeRed->setVisible(false);	
}
void TopBar::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 7)
	{
		nodes.push_back(heroimg);
		std::string str = StringUtils::format("tophero%d", g_hero->getHeadID());
		NewerGuideLayer::pushUserData(str);
	}
	else if (step == 15)
	{
		nodes.push_back(timelbl);
		NewerGuideLayer::pushUserData("timeguide");
	}
	else if (step == 59)
	{

	}
	else if (step == 60)
	{
		newerstep = 60;
		nodes.push_back(life);
		NewerGuideLayer::pushUserData("toplifebar");
	}
	else if (step == 61)
	{
		nodes.push_back(spirit);
		NewerGuideLayer::pushUserData("topspiritbar");
	}
	else if (step == 62)
	{
		nodes.push_back(hunger);
		NewerGuideLayer::pushUserData("tophungerbar");
	}
	else if (step == 63)
	{
		nodes.push_back(innerinjury);
		NewerGuideLayer::pushUserData("topinnerinjurybar");
	}
	else if (step == 64)
	{
		nodes.push_back(outinjury);
		NewerGuideLayer::pushUserData("topoutinjurybar");
	}
	else if (step == 65)
	{
		HomeLayer* homelayer = (HomeLayer*)g_gameLayer->getChildByName("homelayer");
		homelayer->checkNewerGuide();
	}
	if (step == 7 || step == 15 || (step >= 59 && step <= 64))
		g_gameLayer->showNewerGuide(step, nodes);
}

void TopBar::checkNpcRandMap()
{
	int wxbmapsize = sizeof(wxbinmap) / sizeof(wxbinmap[0]);
	std::vector<std::string> vec_map;
	for (int i = 0; i < wxbmapsize;i++)
	{
		std::vector<std::string> tmp;
		CommonFuncs::split(wxbinmap[i], tmp, "-");
		int mapchapter = atoi(tmp[0].substr(1, tmp[0].size() - 1).c_str());
		if (mapchapter <= GlobalData::getUnlockChapter())
		{
			vec_map.push_back(wxbinmap[i]);
		}
	}
	int siez = vec_map.size();
	if (vec_map.size() > 0)
	{
		int rpos = GlobalData::createRandomNum(siez);
		int lastpos = GlobalData::getWxbMapPos();

		if (lastpos != rpos)
		{
			std::vector<std::string>::iterator it;
			for (it = GlobalData::map_maps[wxbinmap[GlobalData::getWxbMapPos()]].npcs.begin(); it != GlobalData::map_maps[wxbinmap[GlobalData::getWxbMapPos()]].npcs.end();)
			{
				if (it->compare("n012") == 0)
					it = GlobalData::map_maps[wxbinmap[GlobalData::getWxbMapPos()]].npcs.erase(it);
				else
					++it;
			}
			GlobalData::setWxbMapPos(rpos);
			GlobalData::map_maps[wxbinmap[rpos]].npcs.push_back("n012");
			if (g_maplayer != NULL)
			{
				g_maplayer->updataPlotMissionIcon(0);
			}
		}
	}

	int dgqbmapsize = sizeof(dgqbinmap) / sizeof(dgqbinmap[0]);
	int r1 = GlobalData::createRandomNum(dgqbmapsize);
	if (GlobalData::getDgqbMapPos() != r1)
	{
		std::vector<std::string>::iterator it;
		for (it = GlobalData::map_maps[dgqbinmap[GlobalData::getDgqbMapPos()]].npcs.begin(); it != GlobalData::map_maps[dgqbinmap[GlobalData::getDgqbMapPos()]].npcs.end();)
		{
			if (it->compare("n091") == 0)
				it = GlobalData::map_maps[dgqbinmap[GlobalData::getDgqbMapPos()]].npcs.erase(it);
			else
				++it;
		}

		GlobalData::setDgqbMapPos(r1);
		GlobalData::map_maps[dgqbinmap[r1]].npcs.push_back("n091");
		if (g_maplayer != NULL)
		{
			g_maplayer->updataPlotMissionIcon(0);
		}
	}
}
