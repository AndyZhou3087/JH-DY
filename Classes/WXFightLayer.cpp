#include "WXFightLayer.h"
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
#include "WXChallengeCountLayer.h"
#include "MyActionProgressTimer.h"

WXFightLayer::WXFightLayer()
{
	isecapeok = false;
	isUseWg = false;
	winnpcount = 0;
	winProperCount = 0;
	isWin = false;
}


WXFightLayer::~WXFightLayer()
{
	if (g_hero != NULL)
	{
		g_hero->setTotalAtkBonusPercent(0.0f);
		g_hero->setTotalDfBonusPercent(0.0f);
		g_hero->setIsWDChallenge(false);
	}
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_ENTER_MAPADDR);
}

void WXFightLayer::onGameStart() {
		playBossShowEffect();
}

void WXFightLayer::updateBloodBar() {
	float rate = 0;
	if (rate> 0) {
		rate = 1 - 120 * 1.0f / 100;
	}

	if (nullptr != m_bloodBar) {
		static const Vec2 offSize = m_bloodBar->getTextureRect().origin;
		static const float h = m_bloodBar->getContentSize().height;
		static const float w = m_bloodBar->getContentSize().width;
		float width = rate * w;
		m_bloodBar->setTextureRect(CCRectMake(offSize.x, offSize.y, width, h));
	}
}

WXFightLayer* WXFightLayer::create(std::string addrid, std::string npcid)
{
	WXFightLayer *pRet = new WXFightLayer();
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


void WXFightLayer::playBossShowEffect(CallFunc * callback) {
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

bool WXFightLayer::init(std::string addrid, std::string npcid)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

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
	m_escapebtn->addTouchEventListener(CC_CALLBACK_2(WXFightLayer::onEscape, this));
	m_escapebtn->setTag(0);

	escapetext = (cocos2d::ui::Text*)m_escapebtn->getChildByName("text");

	if (NewerGuideLayer::checkifNewerGuide(39))
		m_escapebtn->setVisible(false);
	// 滚动文字
	m_fihgtScorll = UIScroll::create(600.0f, 435.0f);
	m_fihgtScorll->setPosition(Vec2(360, 370));
	csbnode->addChild(m_fihgtScorll);

	heroactimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroactimg");
	npcactimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcactimg");

	herocritfnt = (cocos2d::ui::TextBMFont*)heroactimg->getChildByName("herocritfnt");
	npccritfnt = (cocos2d::ui::TextBMFont*)npcactimg->getChildByName("npccritfnt");

	if (npcid.compare("n001") != 0)
		this->scheduleOnce(schedule_selector(WXFightLayer::delayHeroFight), 1.0f);//1.0s，hero->npc
	else
	{
		addrnametxt->setString(CommonFuncs::gbk2utf("路上"));
		m_fightbtn = (cocos2d::ui::Button*)csbnode->getChildByName("fightbtn");
		m_fightbtn->addTouchEventListener(CC_CALLBACK_2(WXFightLayer::onFihgt, this));
		m_fightbtn->setVisible(true);
		m_escapebtn->setPositionX(480);
		this->schedule(schedule_selector(WXFightLayer::checkHeroLife), 0.5f);
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

void WXFightLayer::initTime() {
	int s = 100;
	if (nullptr != m_time) {
		m_time->setString(String::createWithFormat("%d", s)->_string);
	}
}


void WXFightLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	if (m_npcid.compare("n001") == 0)
	{
        if (g_maplayer != NULL)
            g_maplayer->heroPauseMoving();
	}

}

void WXFightLayer::onEscape(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
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

void WXFightLayer::updateTime() {
	bool isok = false;
	if (isok) {
		int s = 100;
		s -= 1;
		if (s <= 8 && s >= 0) {
			
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

void WXFightLayer::onFihgt(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_escapebtn->setEnabled(false);
		fightRobber();
	}
}

void WXFightLayer::fightRobber()
{
	m_fightbtn->setEnabled(false);
	this->unschedule(schedule_selector(WXFightLayer::checkHeroLife));
	this->scheduleOnce(schedule_selector(WXFightLayer::delayHeroFight), 1.0f);
}

void WXFightLayer::resetBoss() {
	initalBoss();

	if (nullptr != m_boss) {
		m_boss->setPositionY(m_bossOriginPosY + 500);
		m_boss->setOpacity(255);
	}

	setHurtBossVisible(false);
}

void WXFightLayer::onScoreChange() {

	updateBloodBar();
}

void WXFightLayer::on1sTimer() {

	if (m_bStop) {
		return;
	}

	updateTime();
}

void WXFightLayer::npcDie()
{
	if (m_addrid.compare("m1-6") == 0)
	{
		if (continuefight <= 0)
		{
			isWin = true;
			this->scheduleOnce(schedule_selector(WXFightLayer::delayShowWinLayer), 1.5f);
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
		this->scheduleOnce(schedule_selector(WXFightLayer::delayShowWinLayer), 1.5f);
	}
}

void WXFightLayer::onTimeChange() {
	updateTime();
}

void WXFightLayer::onAttrackBoss() {
	playAttrackEffect();
}


int WXFightLayer::getNpcHurt()
{
	int heroCurAck = g_hero->getTotalAtck();

	int npchurt = heroCurAck - npcdf;
	float fminack = 0.1f * heroCurAck;
	int intminack = int(fminack + 0.5f);
	if (npchurt < intminack)
		npchurt = intminack;

	return npchurt;
}

void WXFightLayer::skillComboAtk(float dt)
{
	int count = GlobalData::map_gfskills[S_SKILL_3].leftval;
	int c = getNpcHurt();
	npchp -= c * count / 10;
	GlobalData::map_gfskills[S_SKILL_3].leftval--;

	if (npchp <= 0)
	{
		npchp = 0;
		this->unschedule(schedule_selector(WXFightLayer::skillComboAtk));
	}
	updateNpcLife();
}

void WXFightLayer::playAttrackEffect() {
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

void WXFightLayer::delayHeroFight(float dt)
{
	if (isecapeok)//逃跑成功
		return;

	bool isnormalAct = true;

	int npchurt = getNpcHurt();

	isHeroAct = -1;
	isNpcAct = - 1;

	heroheadbox->setScale(1.15);
	npcheadbox->setScale(1);
	/*fightherokuang->setVisible(true);
	fightnpckuang->setVisible(false);*/

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
				this->schedule(schedule_selector(WXFightLayer::skillComboAtk), 0.3f, count - 1, 0.2f);
				isnormalAct = false;
			}
		}
		this->scheduleOnce(schedule_selector(WXFightLayer::delayBossFight), 1.5f);//延迟显示NPC 攻击，主要文字显示，需要看一下，所以延迟下
	}

	if (isnormalAct)
	{
		//ActionInterval* ac1 = Spawn::create(Show::create(), FadeIn::create(0.15f), EaseSineIn::create(ScaleTo::create(0.15f, 1)), NULL);
		//npchurticon->runAction(Sequence::create(ac1, DelayTime::create(0.8f), Hide::create(), NULL));
	}
}

void WXFightLayer::playBossDeathEffect() {
	playBombEffect();
}


void WXFightLayer::initBossBombParticleSystem() {
	if (nullptr == m_boss) {
		return;
	}

	ParticleSystemQuad *m_emitterBomb = ParticleSystemQuad::create("");
	m_emitterBomb->setTexture(Director::getInstance()->getTextureCache()->addImage(""));
	m_boss->addChild(m_emitterBomb, 100);
	m_emitterBomb->setPosition(Vec2(m_boss->getContentSize().width * 0.5, m_boss->getContentSize().height * 0.5));
	m_emitterBomb->stopSystem();
}

void WXFightLayer::delayBossFight(float dt)
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
	/*fightherokuang->setVisible(false);
	fightnpckuang->setVisible(true);*/

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
			this->scheduleOnce(schedule_selector(WXFightLayer::delayHeroFight), 1.5f);
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
		this->scheduleOnce(schedule_selector(WXFightLayer::delayHeroFight), 1.5f);

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

void WXFightLayer::onGameOver() {
	stopBossActiveEffect();
	playBossDeathEffect();
}

void WXFightLayer::delayShowWinLayer(float dt)
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

void WXFightLayer::playBombEffect() {
	if (nullptr == m_boss) {
		return;
	}

	Texture2D * txt2d = TextureCache::getInstance()->addImage("");
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width / 10;
	float h = txt2d->getContentSize().height;

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.2);
	for (int i = 0; i<10; i++) {
		ani->addSpriteFrameWithTexture(txt2d, Rect(i*w, i*h, w, h));
	}

	Sprite * sprite = Sprite::create("", Rect(0, 0, w, h));
	Sprite *m_layer = Sprite::create("", Rect(0, 0, w, h));
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


void WXFightLayer::showFightWord(int type, int value)
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


void WXFightLayer::initalBoss() {
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
	Sprite* m_layer = (Sprite*)this->getChildByName("mlayer");
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


void WXFightLayer::checkWordLblColor(std::string wordstr)
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

void WXFightLayer::setHurtBossVisible(bool isVisible) {
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


std::string WXFightLayer::getGfFightStr()
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

void WXFightLayer::checkHeroLife(float dt)
{
	std::string hpstr = StringUtils::format("%d/%d", (int)g_hero->getLifeValue(), (int)g_hero->getMaxLifeValue());
	herohpvaluetext->setString(hpstr);
	float herohppercent = 100 * g_hero->getLifeValue() / g_hero->getMaxLifeValue();
	herohpbar->setPercent(herohppercent);

	/*MyProgressTo * fromto = MyProgressTo::create(0.5f, herohppercent);
	herohpbar2->runAction(fromto);*/
}

void WXFightLayer::playBossActiveEffect() {
	stopBossActiveEffect();

	if (nullptr != m_boss) {
		ActionInterval * jumpAction = RepeatForever::create(
			Sequence::create(
			MoveBy::create(0.1, Vec2(0, 5)),
			MoveBy::create(0.1, Vec2(0, -10)),
			MoveBy::create(0.1, Vec2(0, 5)),
			NULL)
			);
		jumpAction->setTag(2);
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
		shakeAction->setTag(3);
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
		shakeAction->setTag(1);
		m_normallBoss_hand_right->runAction(shakeAction);
	}
}

void WXFightLayer::stopBossActiveEffect() {
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

void WXFightLayer::updateNpcLife()
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

void WXFightLayer::nextFightNpc(float dt)
{
	this->unschedule(schedule_selector(WXFightLayer::delayHeroFight));

	std::string npcheadstr = StringUtils::format("ui/%s.png", m_npcid.c_str());
	npchead->loadTexture(npcheadstr, cocos2d::ui::TextureResType::PLIST);

	npcnametxt->setString(GlobalData::map_npcs[m_npcid].name);

	npcmaxhp = GlobalData::map_npcs[m_npcid].life;
	npchp = npcmaxhp;

	npcatk = GlobalData::map_npcs[m_npcid].atk;
	npcdf = GlobalData::map_npcs[m_npcid].df;

	updateNpcLife();

	resetSkills();

	this->scheduleOnce(schedule_selector(WXFightLayer::delayHeroFight), 1.5f);
}

void WXFightLayer::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void WXFightLayer::shake(Node * node) {
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

void WXFightLayer::continueChallenge()
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

bool WXFightLayer::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
}

void WXFightLayer::updateFightNextNpc()
{
	isecapeok = false;
	escapetext->setString(CommonFuncs::gbk2utf("逃跑"));
	m_escapebtn->setEnabled(true);
	m_escapebtn->setTag(0);
	checkHeroLife(0);
	this->scheduleOnce(schedule_selector(WXFightLayer::nextFightNpc), 0.5f);
}

void WXFightLayer::reviveContinueChallege()
{
	isecapeok = false;
	escapetext->setString(CommonFuncs::gbk2utf("逃跑"));
	m_escapebtn->setEnabled(true);
	m_escapebtn->setTag(0);
	checkHeroLife(0);
	this->scheduleOnce(schedule_selector(WXFightLayer::delayHeroFight), 1.5f);
}

void WXFightLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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

void WXFightLayer::showChallengeCountLayer(bool isRevive)
{
	WXChallengeCountLayer* layer = WXChallengeCountLayer::create(&winProperCount, winnpcount, isRevive);
	g_gameLayer->addChild(layer, 4, "challengecountlayer");
}

void WXFightLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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
int WXFightLayer::checkSkill(HeroAtrType gftype)
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

void WXFightLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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
void WXFightLayer::showSkill(int skilltype)
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

void WXFightLayer::showHeroTextAmin(std::string filename)
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

void WXFightLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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
				DelayTime::create(15*0.1),
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
				DelayTime::create(20*0.1),
				action,
				NULL));
		}
		else {
			node->runAction(action);
		}
	}
}


void WXFightLayer::showNpcTextAmin(std::string filename)
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

void WXFightLayer::restartFightNpc(std::string npcid)
{
	m_npcid = npcid;
	updateFightNextNpc();
}

void WXFightLayer::jumpDown(cocos2d::Node *node, float dt) {
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

void WXFightLayer::resetSkills()
{
	for (int i = S_SKILL_1; i <= S_SKILL_8; i++)
	{
		GlobalData::map_gfskills[i].leftval = 0;
		GlobalData::map_gfskills[i].fightPlayerleftval = 0;
	}
}

void WXFightLayer::initA()
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

void WXFightLayer::initBg()
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