#include "FightLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "GameScene.h"
#include "Winlayer.h"
#include "SoundManager.h"
#include "NewerGuideLayer.h"
#include "AnalyticUtil.h"
#include "MapLayer.h"
#include "Shake.h"
#include "ChallengeCountLayer.h"
#include "MyActionProgressTimer.h"

FightLayer::FightLayer()
{
	isecapeok = false;
	isUseWg = false;
	winnpcount = 0;
	winProperCount = 0;
	isWin = false;
}


FightLayer::~FightLayer()
{
	if (g_hero != NULL)
	{
		g_hero->setTotalAtkBonusPercent(0.0f);
		g_hero->setTotalDfBonusPercent(0.0f);
		g_hero->setIsWDChallenge(false);
	}
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_ENTER_MAPADDR);
}

FightLayer* FightLayer::create(std::string addrid, std::string npcid)
{
	FightLayer *pRet = new FightLayer();
	if (pRet && pRet->init(addrid, npcid))
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

bool FightLayer::init(std::string addrid, std::string npcid)
{
	Node* csbnode = CSLoader::createNode("fightLayer.csb");
	this->addChild(csbnode);

	m_addrid = addrid;
	m_npcid = npcid;

	//地点名称
	cocos2d::ui::Text* addrnametxt = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	addrnametxt->setString(GlobalData::map_maps[m_addrid].cname);

	if (m_addrid.compare("m13-1") == 0)
	{
		g_hero->setIsWDChallenge(true);
		m_npcid = GlobalData::map_maps[m_addrid].npcs[0];
		continuefight = GlobalData::map_maps[m_addrid].npcs.size() - 1;
	}
	else if (m_addrid.compare("m1-6") == 0)
	{
		m_npcid = GlobalData::map_maps[m_addrid].npcs[0];
		continuefight = GlobalData::map_maps[m_addrid].npcs.size() - 1;
	}
	else
	{
		continuefight = 0;
	}

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
	// NPC 图标
	npchead = (cocos2d::ui::ImageView*)npcheadbox->getChildByName("npcicon");
	std::string npcheadstr = StringUtils::format("ui/%s.png", m_npcid.c_str());
	npchead->loadTexture(npcheadstr, cocos2d::ui::TextureResType::PLIST);

	//NPC名称
	npcnametxt = (cocos2d::ui::Text*)npcheadbox->getChildByName("npcname");
	npcnametxt->setString(GlobalData::map_npcs[m_npcid].name);
	
	//角色名
	cocos2d::ui::Text* heronametxt = (cocos2d::ui::Text*)heroheadbox->getChildByName("heroname");
	heronametxt->setString(g_hero->getMyName());

	int maxlife = g_hero->getMaxLifeValue();;
	//角色血量显示
	herohpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("herohpvaluetext");
	std::string hpstr = StringUtils::format("%d/%d", (int)g_hero->getLifeValue(), maxlife);
	herohpvaluetext->setString(hpstr);

	//角色血量进度
	float herohppercent = 100 * g_hero->getLifeValue() / maxlife;

	herohpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("herohpbar");
	herohpbar->setPercent(herohppercent);

	/*herohpbar2 = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("herohpbar2");
	herohpbar2->setPercent(herohppercent);*/

	npcmaxhp = GlobalData::map_npcs[m_npcid].life;
	npchp = npcmaxhp;

	npcatk = GlobalData::map_npcs[m_npcid].atk;
	npcdf = GlobalData::map_npcs[m_npcid].df;

	//NPC血量显示
	npchpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("npchpvaluetext");
	hpstr = StringUtils::format("%d/%d", npchp, npcmaxhp);
	npchpvaluetext->setString(hpstr);

	//NCP血量进度
	int npchppercent = 100 * npchp / npcmaxhp;
	npchpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("npchpbar");
	npchpbar->setPercent(npchppercent);

	/*npchpbar2 = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("npchpbar2");
	npchpbar2->setPercent(npchppercent);*/

	//逃跑按钮
	m_escapebtn = (cocos2d::ui::Button*)csbnode->getChildByName("escapebtn");
	m_escapebtn->addTouchEventListener(CC_CALLBACK_2(FightLayer::onEscape, this));
	m_escapebtn->setTag(0);

	escapetext = (cocos2d::ui::Text*)m_escapebtn->getChildByName("text");

	if (NewerGuideLayer::checkifNewerGuide(39))
		m_escapebtn->setVisible(false);
	// 滚动文字
	m_fihgtScorll = UIScroll::create(610.0f, 435.0f);
	m_fihgtScorll->setPosition(Vec2(360, 350));
	csbnode->addChild(m_fihgtScorll);

	heroactimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroactimg");
	npcactimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcactimg");

	herocritfnt = (cocos2d::ui::TextBMFont*)heroactimg->getChildByName("herocritfnt");
	npccritfnt = (cocos2d::ui::TextBMFont*)npcactimg->getChildByName("npccritfnt");

	if (npcid.compare("n001") != 0)
		this->scheduleOnce(schedule_selector(FightLayer::delayHeroFight), 1.0f);//1.0s，hero->npc
	else
	{
		addrnametxt->setString(CommonFuncs::gbk2utf("路上"));
		m_fightbtn = (cocos2d::ui::Button*)csbnode->getChildByName("fightbtn");
		m_fightbtn->addTouchEventListener(CC_CALLBACK_2(FightLayer::onFihgt, this));
		m_fightbtn->setVisible(true);
		m_escapebtn->setPositionX(480);
		this->schedule(schedule_selector(FightLayer::checkHeroLife), 0.5f);
	}

	herohurticon = (cocos2d::ui::Widget*)csbnode->getChildByName("herohurt");
	npchurticon = (cocos2d::ui::Widget*)csbnode->getChildByName("npchurt");

	resetSkills();
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

void FightLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	if (m_npcid.compare("n001") == 0)
	{
        if (g_maplayer != NULL)
            g_maplayer->heroPauseMoving();
	}

}

void FightLayer::onEscape(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		GlobalData::isFightMaster = false;
		if (isWin)
			return;

		if (node->getTag() == 0)
		{
			int r = GlobalData::createRandomNum(100);
			if (r < GlobalData::map_npcs[m_npcid].escapernd)
			{
				HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("逃跑成功！"));
				addChild(hbox);

				isecapeok = true;
			}
			else
			{
				g_uiScroll->addEventText(CommonFuncs::gbk2utf("你乘机逃跑，可惜失败了！！"), 25, Color3B(204, 4, 4));
				m_escapebtn->setEnabled(false);
				if (m_npcid.compare("n001") == 0)
				{
					fightRobber();
				}
			}
			escapetext->setString(CommonFuncs::gbk2utf("返回"));
			m_escapebtn->setTag(1);
			if (isecapeok && m_npcid.compare("n001") == 0)//山贼
			{
                if (g_maplayer != NULL)
                    g_maplayer->heroResumeMoving();
				this->removeFromParentAndCleanup(true);
			}
		}
		else
		{
			this->removeFromParentAndCleanup(true);
		}
	}
}

void FightLayer::onFihgt(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_escapebtn->setEnabled(false);
		fightRobber();
	}
}

void FightLayer::fightRobber()
{
	m_fightbtn->setEnabled(false);
	this->unschedule(schedule_selector(FightLayer::checkHeroLife));
	this->scheduleOnce(schedule_selector(FightLayer::delayHeroFight), 1.0f);
}

void FightLayer::npcDie()
{
	if (m_addrid.compare("m1-6") == 0)
	{
		if (continuefight <= 0)
		{
			isWin = true;
			this->scheduleOnce(schedule_selector(FightLayer::delayShowWinLayer), 1.5f);
		}
		else
		{
			int totalnpc = GlobalData::map_maps[m_addrid].npcs.size();
			m_npcid = GlobalData::map_maps[m_addrid].npcs[totalnpc - continuefight];
			continuefight--;
			updateFightNextNpc();
		}

	}
	else
	{
		isWin = true;
		this->scheduleOnce(schedule_selector(FightLayer::delayShowWinLayer), 1.5f);
	}
}

int FightLayer::getNpcHurt()
{
	int heroCurAck = g_hero->getTotalAtck();

	int npchurt = heroCurAck - npcdf;
	float fminack = 0.1f * heroCurAck;
	int intminack = int(fminack + 0.5f);
	if (npchurt < intminack)
		npchurt = intminack;

	return npchurt;
}

void FightLayer::skillComboAtk(float dt)
{
	int count = GlobalData::map_gfskills[S_SKILL_3].leftval;
	int c = getNpcHurt();
	npchp -= c * count / 10;
	GlobalData::map_gfskills[S_SKILL_3].leftval--;

	if (npchp <= 0)
	{
		npchp = 0;
		this->unschedule(schedule_selector(FightLayer::skillComboAtk));
	}
	updateNpcLife();
}

void FightLayer::delayHeroFight(float dt)
{
	if (isecapeok)//逃跑成功
		return;

	bool isnormalAct = true;

	int npchurt = getNpcHurt();

	isHeroAct = -1;
	isNpcAct = - 1;

	heroheadbox->setScale(1.15);
	npcheadbox->setScale(1);
	fightherokuang->setVisible(true);
	fightnpckuang->setVisible(false);

	int critrnd = g_hero->getCritRate() * 100;

	if (checkSkill(H_WG) == S_SKILL_4)
	{
		showSkill(S_SKILL_4);
		if (g_hero->getAtrByType(H_WG)->count > 0)
		{
			critrnd += GlobalData::map_wgngs[g_hero->getAtrByType(H_WG)->strid].skilleffect * 100;
		}
		isnormalAct = false;
	}

	int npcdodgernd = GlobalData::map_npcs[m_npcid].dodge * 100;
	int r = GlobalData::createRandomNum(10000);
	if (r < critrnd)
	{
		isHeroAct = 0;
		npchurt *= 2;
		std::string tmpstr = "ui/crit.png";
		npcactimg->loadTexture(tmpstr, cocos2d::ui::TextureResType::PLIST);
		npcactimg->setContentSize(Sprite::createWithSpriteFrameName(tmpstr)->getContentSize());
		npcactimg->setVisible(true);
		npcactimg->setOpacity(200);
		npcactimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		npcactimg->runAction(Sequence::create(ac1, DelayTime::create(1.0f), Hide::create(), NULL));
		tmpstr = StringUtils::format("%d", npchurt);
		npccritfnt->setString(tmpstr);
		npccritfnt->setVisible(true);
		isnormalAct = false;
	}
	else if (r < npcdodgernd)
	{
		isNpcAct = 1;
		std::string imgstr = "ui/dodge.png";
		npcactimg->loadTexture(imgstr, cocos2d::ui::TextureResType::PLIST);
		npcactimg->setContentSize(Sprite::createWithSpriteFrameName(imgstr)->getContentSize());
		npcactimg->setVisible(true);
		npccritfnt->setVisible(false);
		npcactimg->setOpacity(200);
		npcactimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		npcactimg->runAction(Sequence::create(ac1, Shake::create(0.2f, 20, 1), DelayTime::create(0.8f), Hide::create(), NULL));
		isnormalAct = false;

	}

	if (isNpcAct != 1)
	{
		npchp -= npchurt;
		if (npchp < 0)
			npchp = 0;
	}

	updateNpcLife();

	showFightWord(0, npchurt);

	if (npchp <= 0)// NPC dead 胜利
	{
		npcDie();
	}
	else
	{
		if (checkSkill(H_WG) == S_SKILL_3)
		{
			int count = GlobalData::map_gfskills[S_SKILL_3].leftval;
			if (count > 0)
			{
				showSkill(S_SKILL_3);
				this->schedule(schedule_selector(FightLayer::skillComboAtk), 0.3f, count - 1, 0.2f);
				isnormalAct = false;
			}
		}
		this->scheduleOnce(schedule_selector(FightLayer::delayBossFight), 1.5f);//延迟显示NPC 攻击，主要文字显示，需要看一下，所以延迟下
	}

	if (isnormalAct)
	{
		//ActionInterval* ac1 = Spawn::create(Show::create(), FadeIn::create(0.15f), EaseSineIn::create(ScaleTo::create(0.15f, 1)), NULL);
		//npchurticon->runAction(Sequence::create(ac1, DelayTime::create(0.8f), Hide::create(), NULL));
	}
}

void FightLayer::delayBossFight(float dt)
{
	if (isecapeok)//逃跑成功
		return;

	bool isnormalAct = true;
	float curheroHp = g_hero->getLifeValue();

	int curheroDf = g_hero->getTotalDf();

	int herohurt = npcatk - curheroDf;

	float fminack = 0.1f * npcatk;
	int intminack = int(fminack + 0.5f);

	heroheadbox->setScale(1);
	npcheadbox->setScale(1.15);
	fightherokuang->setVisible(false);
	fightnpckuang->setVisible(true);

	if (herohurt < intminack)
		herohurt = intminack;

    int skilltype = checkSkill(H_WG);
    
    if (skilltype == S_SKILL_1 || skilltype == S_SKILL_2 || skilltype == S_SKILL_5)
    {
       GlobalData::map_gfskills[skilltype].leftval--;
    }
    
	if (isNpcAct != 1)
	{
		if (npchp <= 0)
		{
			npcDie();
			return;
		}

		
		if (skilltype == S_SKILL_1 || skilltype == S_SKILL_5)
		{
			isnormalAct = false;
			showSkill(skilltype);

			npchp -= getNpcHurt() * 3 / 10;
			if (npchp < 0)
				npchp = 0;
			updateNpcLife();

			if (npchp <= 0)
			{
				npcDie();
				return;
			}
		}
		else if (skilltype == S_SKILL_2)
		{
			isnormalAct = false;
			showSkill(skilltype);
			this->scheduleOnce(schedule_selector(FightLayer::delayHeroFight), 1.5f);
			return;
		}
		skilltype = checkSkill(H_NG);
		if (skilltype == S_SKILL_6)
		{
			isnormalAct = false;
			showSkill(skilltype);
			if (g_hero->getAtrByType(H_NG)->count > 0)
			{
				herohurt = herohurt * (100 - GlobalData::map_wgngs[g_hero->getAtrByType(H_NG)->strid].skilleffect) / 100;
			}
		}
		else if (skilltype == S_SKILL_7)
		{
			isnormalAct = false;
			showSkill(skilltype);
			int npclosthp = 0;
			if (g_hero->getAtrByType(H_NG)->count > 0)
			{
				npclosthp = npchp * GlobalData::map_wgngs[g_hero->getAtrByType(H_NG)->strid].skilleffect / 100;
			}
			npchp -= npclosthp;
			if (npchp < 0)
				npchp = 0;
			updateNpcLife();
			g_hero->setLifeValue(curheroHp + npclosthp);
			checkHeroLife(0);
			if (npchp <= 0)
			{
				npcDie();
				return;
			}
		}
		
	}


	isHeroAct = -1;
	isNpcAct = -1;

	int dodgernd = g_hero->getdodgeRate() * 100;
	if (checkSkill(H_NG) == S_SKILL_8)
	{
		isnormalAct = false;
		showSkill(S_SKILL_8);
		if (g_hero->getAtrByType(H_NG)->count > 0)
		{
			dodgernd += GlobalData::map_wgngs[g_hero->getAtrByType(H_NG)->strid].skilleffect * 100;
		}
		
	}

	int npccritrnd = GlobalData::map_npcs[m_npcid].crit * 100;
	int r = GlobalData::createRandomNum(10000);
	if (r < npccritrnd)
	{
		isNpcAct = 0;
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
		isnormalAct = false;
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
		isnormalAct = false;
	}

	if (isHeroAct != 1)
	{
		curheroHp -= herohurt;
	}

	showFightWord(1, herohurt);

	if (curheroHp < 0.0f)
		curheroHp = 0.0f;

	g_hero->setLifeValue(curheroHp);
	checkHeroLife(0);

	if (g_hero->getLifeValue() > 0.0f)
	{
		this->scheduleOnce(schedule_selector(FightLayer::delayHeroFight), 1.5f);

		float f1maxlife = g_hero->getMaxLifeValue() * 0.05f;
		if (herohurt >= (int)f1maxlife)//受到大于10%伤害
		{
			int r = GlobalData::createRandomNum(100);
			if (r < 30)//30%概率收内伤
			{
				float curvalue = g_hero->getInnerinjuryValue() - 5.0f;
				if (curvalue < 0.0f)
					curvalue = 0.0f;
				g_hero->setInnerinjuryValue(curvalue);

			}
			else//70%概率收外伤
			{
				float curvalue = g_hero->getOutinjuryValue() - 5.0f;
				if (curvalue < 0.0f)
					curvalue = 0.0f;
				g_hero->setOutinjuryValue(curvalue);
			}
		}

		if (isnormalAct)
		{
			//ActionInterval* ac1 = Spawn::create(Show::create(), FadeIn::create(0.15f), EaseSineIn::create(ScaleTo::create(0.15f, 1)), NULL);
			//herohurticon->runAction(Sequence::create(ac1, DelayTime::create(0.8f), Hide::create(), NULL));
		}
	}
	else
	{
		escapetext->setString(CommonFuncs::gbk2utf("返回"));
		m_escapebtn->setTag(1);

#ifdef ANALYTICS
		std::string deathstr = StringUtils::format("d-%s", m_npcid.c_str());
		AnalyticUtil::onEvent(m_npcid.c_str());
#endif
	}
	
}

void FightLayer::delayShowWinLayer(float dt)
{
	escapetext->setString(CommonFuncs::gbk2utf("返回"));
	m_escapebtn->setEnabled(true);
	m_escapebtn->setTag(1);

	Winlayer* layer = Winlayer::create(m_addrid, m_npcid);
	if (g_gameLayer != NULL)
		g_gameLayer->addChild(layer, 10, "Winlayer");

	if (continuefight > 0)
	{
		winnpcount++;
		winProperCount++;
		int totalnpc = GlobalData::map_maps[m_addrid].npcs.size();
		m_npcid = GlobalData::map_maps[m_addrid].npcs[totalnpc - continuefight];
		continuefight--;
	}
	isWin = false;
	if (m_addrid.compare("m1-6") == 0)
		this->removeFromParentAndCleanup(true);
}

void FightLayer::showFightWord(int type, int value)
{
	std::string wordstr;
	int size = 0;
	int r = 0;
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
				if (m_npcid.compare("n003") == 0 || m_npcid.compare("n002") == 0)
					isUseWg = false;
			}
			if (isUseWg)
			{
				herowordstr = getGfFightStr();
			}
			else
			{
				int extype = g_hero->getAtrByType(H_WEAPON)->extype;
				size = sizeof(herofightdesc1[extype - 1]) / sizeof(herofightdesc1[extype - 1][0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc1[extype - 1][r];
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), g_hero->getMyName().c_str(), GlobalData::map_allResource[wstrid].cname.c_str(), GlobalData::map_npcs[m_npcid].name);
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
				herowordstr = getGfFightStr();
			}
			else
			{
				size = sizeof(herofightdesc) / sizeof(herofightdesc[0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc[r];
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), g_hero->getMyName().c_str(), GlobalData::map_npcs[m_npcid].name);
			}
		}
		
		if (m_npcid.compare("n003") == 0)
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightworfword[0].c_str()).c_str(), g_hero->getMyName().c_str(), GlobalData::map_npcs[m_npcid].name);
		else if (m_npcid.compare("n002") == 0)
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightRabbitword[0].c_str()).c_str(), g_hero->getMyName().c_str(), GlobalData::map_npcs[m_npcid].name);

		checkWordLblColor(herowordstr);

		if (isUseWg)
		{
			int r1 = GlobalData::createRandomNum(4);
			wordstr = bossgetherowg[r1];
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, value);
		}
		else
		{
			if (g_hero->getAtrByType(H_WEAPON)->count > 0)
			{
				int extype = g_hero->getAtrByType(H_WEAPON)->extype;
				size = sizeof(herofightdesc2[extype - 1]) / sizeof(herofightdesc2[extype - 1][0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc2[extype - 1][r];
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, value);
			}
			else
			{
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightdesc3[0].c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, value);
			}
		}

		if (m_npcid.compare("n003") == 0)
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightworfword2[0].c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, value);
		else if (m_npcid.compare("n002") == 0)
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightRabbitword2[0].c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, value);

		if (isNpcAct == 1)//npc闪避
		{
			int dcount = sizeof(npcdodgedesc) / sizeof(npcdodgedesc[0]);
			int dodgernd = GlobalData::createRandomNum(dcount);
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(npcdodgedesc[dodgernd].c_str()).c_str(), GlobalData::map_npcs[m_npcid].name);

			if (m_npcid.compare("n003") == 0 || m_npcid.compare("n002") == 0)
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(worfdodgedesc.c_str()).c_str(), GlobalData::map_npcs[m_npcid].name);
		}

		checkWordLblColor(herowordstr);

		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_ATTACK);
	}
	else//
	{
		std::string bosswordstr;

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
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str(), GlobalData::map_allResource[astrid].cname.c_str(), value);
		}
		else//没有防具
		{
			size = sizeof(bossfight) / sizeof(bossfight[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = bossfight[r];
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str(), value);
		}

		if (m_npcid.compare("n003") == 0)
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(worffight[0].c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str(), value);
		else if (m_npcid.compare("n002") == 0)
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(rabbitfightword[0].c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str(), value);

		if (isHeroAct == 1)
		{
			int dcount = sizeof(herododgedesc) / sizeof(herododgedesc[0]);
			int dodgernd = GlobalData::createRandomNum(dcount);
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(herododgedesc[dodgernd].c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str());
		}
		checkWordLblColor(bosswordstr);

		isUseWg = false;
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_HURT);
	}
}

void FightLayer::checkWordLblColor(std::string wordstr)
{
	Label* wordlbl = Label::createWithTTF(wordstr, "fonts/STXINGKA.TTF", 28);
	wordlbl->setLineBreakWithoutSpace(true);
	wordlbl->setMaxLineWidth(610);
	int index = 0;
	//getletter --是每个字，中文，英文都算一个字
	//UFT8 中文是3个字符所以除以3转换下
	while (wordlbl->getLetter(index) != NULL)
	{
		wordlbl->getLetter(index)->setColor(Color3B::BLACK);
		index++;
	}

	//NPC名称颜色 红色
	std::map<std::string, NpcData>::iterator it;
	for (it = GlobalData::map_npcs.begin(); it != GlobalData::map_npcs.end(); ++it)
	{
		std::string npcname = GlobalData::map_npcs[it->first].name;
		std::size_t findpos;
		std::string temp = wordstr;
		int count = 0;
		while (1){
			findpos = temp.find(npcname);
			if (findpos != std::string::npos){
				temp = temp.substr(findpos + npcname.length(), temp.length() - npcname.length());
				int sindex = (wordstr.length() - temp.length() - npcname.length()) / 3;
				int len = npcname.length() / 3;
				for (int i = sindex; i < sindex + len; i++)
				{
					wordlbl->getLetter(i)->setColor(Color3B(230, 35, 35));
				}
			}
			else 
				break;
		}

		//std::size_t findpos = wordstr.find(npcname);
		//if (findpos != std::string::npos)
		//{
		//	int sindex = findpos / 3;
		//	int len = npcname.size() / 3;
		//	for (int i = sindex; i < sindex + len; i++)
		//	{
		//		wordlbl->getLetter(i)->setColor(Color3B(230,35,35));
		//	}
		//}
	}
	//角色名称颜色 绿色

	std::size_t findpos;
	std::string temp = wordstr;
	std::string heroname = g_hero->getMyName();
	while (1){
		findpos = temp.find(g_hero->getMyName());
		if (findpos != std::string::npos){
			temp = temp.substr(findpos + heroname.length(), temp.length() - heroname.length());
			int sindex = (wordstr.length() - temp.length() - heroname.length()) / 3;
			int len = heroname.length() / 3;
			for (int i = sindex; i < sindex + len; i++)
			{
				wordlbl->getLetter(i)->setColor(Color3B(27, 141, 0));
			}
		}
		else
			break;
	}
	//武器，功法名称颜色 紫色
	std::map<std::string, EquipData>::iterator ite;
	for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
	{
		std::string ename = GlobalData::map_allResource[ite->first].cname;
		std::size_t findpos;
		std::string temp = wordstr;
		while (1){
			findpos = temp.find(ename);
			if (findpos != std::string::npos){
				temp = temp.substr(findpos + ename.length(), temp.length() - ename.length());
				int sindex = (wordstr.length() - temp.length() - ename.length()) / 3;
				int len = ename.length() / 3;
				for (int i = sindex; i < sindex + len; i++)
				{
					wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
				}
			}
			else
				break;
		}

		//std::size_t findpos = wordstr.find(ename);
		//if (findpos != std::string::npos)
		//{
		//	int sindex = findpos / 3;
		//	int len = ename.size() / 3;
		//	for (int i = sindex; i < sindex + len; i++)
		//	{
		//		wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
		//	}
		//}
	}
	if (g_hero->getAtrByType(H_WG)->count > 0)
	{
		GFTrickData gftrickdata = GlobalData::map_gftricks[g_hero->getAtrByType(H_WG)->strid];
		int mygftricksize = gftrickdata.snames.size();
		
		for (int n = 0; n < mygftricksize; n++)
		{
			std::string gfname = gftrickdata.snames[n];
			std::size_t findpos;
			std::string temp = wordstr;
			while (1){
				findpos = temp.find(gfname);
				if (findpos != std::string::npos){
					temp = temp.substr(findpos + gfname.length(), temp.length() - gfname.length());
					int sindex = (wordstr.length() - temp.length() - gfname.length()) / 3;
					int len = gfname.length() / 3;
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

	//数字颜色 红色
	//目前只实现了一段中只能改变一组数字的颜色，并且数字要在上面 关键字的后面
	int sindex = -1;
	findpos = 0;
	for (unsigned int i = 0; i < wordstr.length(); i++)
	{
		char a = wordstr[i];
		if (wordstr[i] >= '0' && wordstr[i] <= '9')
		{
			sindex = i / 3;
			findpos = i;
			wordlbl->getLetter(sindex)->setColor(Color3B(230, 35, 35));
			break;
		}
	}

	if (sindex > 0)
	{
		//找到地一个数字后，往后再找4位
		for (int i = 1; i <= 4; i++)
		{
			char a = wordstr[findpos + i];
			if (wordstr[findpos + i] >= '0' && wordstr[findpos + i] <= '9')
			{
				wordlbl->getLetter(sindex + i)->setColor(Color3B(230, 35, 35));
			}
		}
	}

	m_fihgtScorll->addEventLabel(wordlbl);
}

std::string FightLayer::getGfFightStr()
{
	std::string retstr;

	int r1 = GlobalData::createRandomNum(2);
	int r2 = GlobalData::createRandomNum(4);

	std::string gfstr = g_hero->getAtrByType(H_WG)->strid;
	GFTrickData ssdata = GlobalData::map_gftricks[g_hero->getAtrByType(H_WG)->strid];
	int r3 = GlobalData::createRandomNum(ssdata.snames.size());
	std::string gfsname = ssdata.snames[r3];
	std::string	wordstr = CommonFuncs::gbk2utf(herousewgfight[r1][r2].c_str());
	if (r1 == 0)
		retstr = StringUtils::format(wordstr.c_str(), g_hero->getMyName().c_str(), gfsname.c_str(), GlobalData::map_npcs[m_npcid].name);
	else
		retstr = StringUtils::format(wordstr.c_str(), g_hero->getMyName().c_str(), GlobalData::map_npcs[m_npcid].name, gfsname.c_str(), gfsname.c_str(), GlobalData::map_npcs[m_npcid].name);

	return retstr;
}

void FightLayer::checkHeroLife(float dt)
{
	std::string hpstr = StringUtils::format("%d/%d", (int)g_hero->getLifeValue(), (int)g_hero->getMaxLifeValue());
	herohpvaluetext->setString(hpstr);
	float herohppercent = 100 * g_hero->getLifeValue() / g_hero->getMaxLifeValue();
	herohpbar->setPercent(herohppercent);

	/*MyProgressTo * fromto = MyProgressTo::create(0.5f, herohppercent);
	herohpbar2->runAction(fromto);*/
}

void FightLayer::updateNpcLife()
{
	//NPC血量显示
	std::string hpstr = StringUtils::format("%d/%d", npchp, npcmaxhp);
	npchpvaluetext->setString(hpstr);

	//NCP血量进度
	int npchppercent = 100 * npchp / npcmaxhp;
	npchpbar->setPercent(npchppercent);
	/*MyProgressTo * to = MyProgressTo::create(0.5f, npchppercent);
	npchpbar2->runAction(to);*/
}

void FightLayer::nextFightNpc(float dt)
{
	this->unschedule(schedule_selector(FightLayer::delayHeroFight));

	std::string npcheadstr = StringUtils::format("ui/%s.png", m_npcid.c_str());
	npchead->loadTexture(npcheadstr, cocos2d::ui::TextureResType::PLIST);

	npcnametxt->setString(GlobalData::map_npcs[m_npcid].name);

	npcmaxhp = GlobalData::map_npcs[m_npcid].life;
	npchp = npcmaxhp;

	npcatk = GlobalData::map_npcs[m_npcid].atk;
	npcdf = GlobalData::map_npcs[m_npcid].df;

	updateNpcLife();

	resetSkills();

	this->scheduleOnce(schedule_selector(FightLayer::delayHeroFight), 1.5f);
}

void FightLayer::continueChallenge()
{
	if (winnpcount % 3 == 0)
	{
		escapetext->setString(CommonFuncs::gbk2utf("返回"));
		m_escapebtn->setEnabled(true);
		m_escapebtn->setTag(1);
		showChallengeCountLayer(false);
	}
	else
	{
		updateFightNextNpc();
	}
}

void FightLayer::updateFightNextNpc()
{
	isecapeok = false;
	escapetext->setString(CommonFuncs::gbk2utf("逃跑"));
	m_escapebtn->setEnabled(true);
	m_escapebtn->setTag(0);
	checkHeroLife(0);
	this->scheduleOnce(schedule_selector(FightLayer::nextFightNpc), 0.5f);
}

void FightLayer::reviveContinueChallege()
{
	isecapeok = false;
	escapetext->setString(CommonFuncs::gbk2utf("逃跑"));
	m_escapebtn->setEnabled(true);
	m_escapebtn->setTag(0);
	checkHeroLife(0);
	this->scheduleOnce(schedule_selector(FightLayer::delayHeroFight), 1.5f);
}

void FightLayer::showChallengeCountLayer(bool isRevive)
{
	ChallengeCountLayer* layer = ChallengeCountLayer::create(&winProperCount, winnpcount, isRevive);
	g_gameLayer->addChild(layer, 4, "challengecountlayer");
}

int FightLayer::checkSkill(HeroAtrType gftype)
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

void FightLayer::showSkill(int skilltype)
{
	if (skilltype >= S_SKILL_1 && skilltype <= S_SKILL_5)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showNpcTextAmin(str);
	}
	else if (skilltype >= S_SKILL_6 && skilltype <= S_SKILL_8)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showHeroTextAmin(str);
	}
}

void FightLayer::showHeroTextAmin(std::string filename)
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


void FightLayer::showNpcTextAmin(std::string filename)
{
	npcactimg->loadTexture(filename, cocos2d::ui::TextureResType::PLIST);
	npcactimg->setContentSize(Sprite::createWithSpriteFrameName(filename)->getContentSize());
	npcactimg->setVisible(true);
	npcactimg->setOpacity(200);
	npcactimg->setScale(3);
	ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
	npcactimg->runAction(Sequence::create(ac1, DelayTime::create(0.8f), Hide::create(), NULL));
	npccritfnt->setVisible(false);
}

void FightLayer::restartFightNpc(std::string npcid)
{
	m_npcid = npcid;
	updateFightNextNpc();
}

void FightLayer::resetSkills()
{
	for (int i = S_SKILL_1; i <= S_SKILL_8; i++)
	{
		GlobalData::map_gfskills[i].leftval = 0;
		GlobalData::map_gfskills[i].fightPlayerleftval = 0;
	}
}
