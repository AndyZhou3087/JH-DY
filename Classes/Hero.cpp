#include "Hero.h"
#include "Const.h"
#include "Nature.h"
#include "GameScene.h"
#include "NewerGuideLayer.h"
float Hero::MAXInnerinjuryValue = 100.0f;
float Hero::MAXOutinjuryValue = 100.0f;
float Hero::MAXHungerValue = 100.0f;
float Hero::MAXSpiritValue = 100.0f;

#define HungerSpeed 2//饱食度下降速度
#define InnerinjurySpeed 1//内伤下降速度
#define OutinjurySpeed 1//外伤下降速度
#define SpiritSpeed 1.5f//精神下降速度
#define LifeLostSpeed 2//生命下降速度（按百分比）

#define SeriousInnerinjury 40.0f//严重内伤界限
#define SeriousOutinjury 40.0f//严重外伤界限
#define SeriousHunger 40.0f//严重饥饿界限

Hero::Hero()
{
	maxlifepercent = 1.0f;
	liferecoverpercent = 1.0f;
	m_atkpercent = 1.0f;
	m_dfpercent = 1.0f;
	m_maxHunger = MAXHungerValue;
	m_maxInnerinjury = MAXInnerinjuryValue;
	m_maxOutinjury = MAXOutinjuryValue;
	m_maxSpirit = MAXSpiritValue;

	injuryrecoverpercent = 1.0f;
	outjuryrecoverpercent = 1.0f;
	hungerrecoverpercent = 1.0f;
	sleepLostPercent = 1.0f;

	m_pastmin = 0;

	m_totalAtkBonusPercent = 0.0f;
	m_totalDfBonusPercent = 0.0f;
	m_isWDChallenge = false;
	m_isMoving = false;
}


Hero::~Hero()
{

}

bool Hero::init()
{
	this->schedule(schedule_selector(Hero::updateData), 1.0f/*60.0f / TIMESCALE*/);
	this->schedule(schedule_selector(Hero::checkMaxVaule), 1.0f);
	return true;
}
void Hero::onGameStart() {
	playBossShowEffect();
}

void Hero::updateBloodBar() {

	float rate = 0;
	if (rate > 0) {
		rate = 1 - 100 * 1.0f / 3;
	}

	if (nullptr != m_bloodBar) {
		static const Vec2 offSize = m_bloodBar->getTextureRect().origin;
		static const float h = m_bloodBar->getContentSize().height;
		static const float w = m_bloodBar->getContentSize().width;
		float width = rate * w;
		m_bloodBar->setTextureRect(CCRectMake(offSize.x, offSize.y, width, h));
	}
}
void Hero::updateData(float dt)
{
	if (GlobalData::g_gameStatus != GAMESTART)
		return;

	if (m_isWDChallenge || GlobalData::isPlayerChallenging)
		return;

	//12s，（游戏时间1小时更新一次）
	if (g_nature != NULL)
		m_pastmin += g_nature->getTimeInterval();

	if (m_pastmin >= 60)
	{
		int hour = m_pastmin / 60;
		m_pastmin = m_pastmin % 60;

		//新手引导不扣属性消耗
		if (g_NewerGuideLayer != NULL)
			return;

		//闭关没有分身符不扣属性消耗， 有分身符要扣
		if (GlobalData::isExercising() && !GlobalData::isHasFSF())
			return;

		if (m_outinjury < SeriousOutinjury)//严重外伤，内伤3倍下降
		{
			if (m_innerinjury < MAXInnerinjuryValue)
				m_innerinjury -= hour * InnerinjurySpeed * 3.0f;
			m_life -= hour * LifeLostSpeed * 1.0f * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100.0f;
		}
		if (m_hunger < SeriousHunger)//过度饥饿 2倍外伤，2倍内伤下降
		{
			if (m_outinjury < MAXOutinjuryValue)
				m_outinjury -= hour * OutinjurySpeed * 2.0f;
			if (m_innerinjury < MAXInnerinjuryValue)
				m_innerinjury -= hour * InnerinjurySpeed * 2.0f;
			m_life -= hour * LifeLostSpeed * 1.0f * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100.0f;
		}
		if (m_innerinjury < SeriousInnerinjury)//严重内伤，外伤2倍下降
		{
			if (m_outinjury < MAXOutinjuryValue)
				m_outinjury -= hour * OutinjurySpeed * 3.0f;
			m_life -= hour * LifeLostSpeed * 1.0f * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100.0f;
		}

		//接上严重界限的消耗
		m_hunger -= hour * HungerSpeed * 1.0f * sleepLostPercent;

		if (m_innerinjury < MAXInnerinjuryValue)
		{
			if (m_innerinjury > 90)
				m_innerinjury -= hour * InnerinjurySpeed * 0.3f;
			else if (m_innerinjury > 70)
				m_innerinjury -= hour * InnerinjurySpeed * 0.5f;
			else
				m_innerinjury -= hour * InnerinjurySpeed * 2.0f;
		}
		if (m_outinjury < MAXOutinjuryValue)
		{
			if (m_outinjury > 90)
				m_outinjury -= hour * OutinjurySpeed * 0.3f;
			else if (m_outinjury > 70)
				m_outinjury -= hour * OutinjurySpeed * 0.5f;
			else
				m_outinjury -= hour * OutinjurySpeed * 2.0f;

			if (g_nature->getTemperature() > 20)
			{
				m_outinjury -= hour * OutinjurySpeed * 1.0f;
			}
		}

		else if (g_nature->getTemperature() < -5)
		{
			m_life -= hour * LifeLostSpeed * 1.0f * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100.0f;
		}

		m_spirit -= hour * SpiritSpeed * 1.0f * sleepLostPercent;

		if (m_innerinjury < 0.0f)
			m_innerinjury = 0.0f;
		if (m_outinjury < 0.0f)
			m_outinjury = 0.0f;
		if (m_hunger < 0.0f)
			m_hunger = 0.0f;
		if (m_life < 0.0f)
			m_life = 0.0f;
		if (m_spirit < 0.0f)
			m_spirit = 0.0f;
	}

}

void Hero::initTime() {
	int s = 10;
	if (nullptr != m_time) {
		m_time->setString(String::createWithFormat("%d", s)->_string);
	}
}
void Hero::sleep(int losttime, int hour)
{
	//按次恢复生命
	float delay = 0.0f;
	if (losttime > TIMESCALE * ACTION_BAR_TIME)
		delay = losttime / TIMESCALE;
	else
		delay = ACTION_BAR_TIME;

	float pertime = delay / (TIMESCALE* ACTION_BAR_TIME);
	sleephour = hour;
	this->schedule(schedule_selector(Hero::sleepbystep), pertime, TIMESCALE* ACTION_BAR_TIME - 1, 0.0f);
	sleepLostPercent = 0.5f;


	this->scheduleOnce(schedule_selector(Hero::sleepDone), delay);
}
void Hero::updateTime() {
	bool isok = true;
	if (isok) {
		int s =20;
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
void Hero::sleepbystep(float dt)
{
	//每次恢复的生命值
	m_life += getMaxLifeValue() * sleephour * 0.1f * liferecoverpercent / (TIMESCALE* ACTION_BAR_TIME);
	if (m_life > getRecoverLifeMaxValue())
	{
		m_life = getRecoverLifeMaxValue();
		this->unschedule(schedule_selector(Hero::sleepbystep));
	}
}

void Hero::sleepDone(float dt)
{
	sleepLostPercent = 1.0f;
}
void Hero::resetBoss() {
	initalBoss();

	if (nullptr != m_boss) {
		m_boss->setPositionY(m_bossOriginPosY + 500);
		m_boss->setOpacity(255);
	}

	setHurtBossVisible(false);
}

void Hero::onScoreChange() {


	updateBloodBar();
}

void Hero::drinking()
{
	//按次恢复精神值
	this->schedule(schedule_selector(Hero::drinkbystep), 0.2f, TIMESCALE* ACTION_BAR_TIME - 1, 0.0f);
}
void Hero::on1sTimer() {


	if (m_bStop) {
		return;
	}

	updateTime();
}

void Hero::onTimeChange() {

	updateTime();
}

void Hero::onAttrackBoss() {
	playAttrackEffect();
}
void Hero::drinkbystep(float dt)
{
	//每次恢复精神值
	m_spirit += 2.0f;
	if (m_spirit > getMaxSpiritValue())
	{
		m_spirit = getMaxSpiritValue();
		this->unschedule(schedule_selector(Hero::drinkbystep));
	}
}

int Hero::getAtkValue()
{
	return GlobalData::map_heroAtr[getHeadID()].vec_atk[getLVValue()];
}
void Hero::playAttrackEffect() {
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

int Hero::getDfValue()
{
	return GlobalData::map_heroAtr[getHeadID()].vec_df[getLVValue()];
}
void Hero::playBossShowEffect(CallFunc * callback) {
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

void Hero::playBossDeathEffect() {
	playBombEffect();
}

void Hero::initBossBombParticleSystem() {
	if (nullptr == m_boss) {
		return;
	}

	m_emitterBomb = ParticleSystemQuad::create("");
	m_emitterBomb->setTexture(Director::getInstance()->getTextureCache()->addImage(""));
	m_boss->addChild(m_emitterBomb, 100);
	m_emitterBomb->setPosition(Vec2(m_boss->getContentSize().width * 0.5, m_boss->getContentSize().height * 0.5));
	m_emitterBomb->stopSystem();
}
float Hero::getMaxLifeValue()
{
	float friendhppercent = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::map_myfriendly.begin(); it != GlobalData::map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::map_myfriendly[nid].relation == F_FRIEND)
		{
			friendhppercent += GlobalData::map_NPCFriendData[nid].hppercent / 100;
		}
		else if (GlobalData::map_myfriendly[nid].relation == F_MASTER)
		{
			friendhppercent += GlobalData::map_NPCMasterData[nid].hppercent / 100;
		}
	}

	float mixfglifepercent = 0.0f;

	std::string mymixgf = GlobalData::getMixGF();
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mymixgf];
		if ((g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WG)->strid.compare(mdata.mastergf) == 0) || (g_hero->getAtrByType(H_NG)->count > 0 && g_hero->getAtrByType(H_NG)->strid.compare(mdata.mastergf) == 0))
			mixfglifepercent += mdata.hppercent / 100;
	}

	float flife = GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] * 1.0f;
	flife = flife + flife*friendhppercent + flife * mixfglifepercent;

	if (GlobalData::myFactionlv == 3)
		flife += flife*0.02f;
	return flife;
}

float Hero::getRecoverLifeMaxValue()
{
	float flife = maxlifepercent * getMaxLifeValue();
	return flife;
}


void Hero::onGameOver() {
	stopBossActiveEffect();
	playBossDeathEffect();
}

void Hero::setAtrByType(HeroAtrType type, PackageData pData)
{
	map_heroAtr[type] = pData;
	saveProperData();
}
void Hero::playBombEffect() {
	if (nullptr == m_boss) {
		return;
	}

	Texture2D * txt2d = TextureCache::getInstance()->addImage("");
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width / 5;
	float h = txt2d->getContentSize().height;

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.2);
	for (int i = 0; i<7; i++) {
		ani->addSpriteFrameWithTexture(txt2d, Rect(i*w, i*h, w, h));
	}

	Sprite * sprite = Sprite::create("", Rect(0, 0, w, h));
	Sprite* m_layer;
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
PackageData* Hero::getAtrByType(HeroAtrType type)
{
	return &map_heroAtr[type];
}

void Hero::saveProperData()
{
	//保存装备栏数据
	std::string str;
	for (int i = H_WEAPON; i < H_MAX; i++)
	{
		PackageData* sdata = g_hero->getAtrByType((HeroAtrType)i);

		std::string idstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", sdata->strid.c_str(), sdata->type, sdata->count, sdata->extype, sdata->lv, sdata->exp, sdata->goodvalue, sdata->slv, sdata->tqu);
		str.append(idstr);
	}
	GlobalData::setHeroProperData(str.substr(0, str.length() - 1));
}
void Hero::initalBoss() {
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

void Hero::revive()
{
	//复活-满状态复活

	maxlifepercent = 1.0f;
	liferecoverpercent = 1.0f;
	m_atkpercent = 1.0f;
	m_dfpercent = 1.0f;

	setOutinjuryValue(MAXOutinjuryValue);
	setMaxOutinjuryValue(MAXOutinjuryValue);

	setInnerinjuryValue(MAXInnerinjuryValue);
	setMaxInnerinjuryValue(MAXInnerinjuryValue);

	setLifeValue(getMaxLifeValue());
	
	setHungerValue(MAXHungerValue);
	setMaxHungerValue(MAXHungerValue);

	setSpiritValue(MAXSpiritValue);
	setMaxSpiritValue(MAXSpiritValue);
}

void Hero::setHurtBossVisible(bool isVisible) {
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

//功法每种只能有一本，不能重复
bool Hero::checkifHasGF_Equip(std::string gfeid)
{
	HeroAtrType heroproptype[] = { H_WG, H_NG, H_ARMOR, H_WEAPON };

	//装备栏是否有
	for (int i = 0; i < sizeof(heroproptype) / sizeof(heroproptype[0]); i++)
	{
		if (getAtrByType(heroproptype[i])->count > 0 && getAtrByType(heroproptype[i])->strid.compare(gfeid) == 0)
			return true;
	}

	//背包中是否有
	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		PackageData* pone = &MyPackage::vec_packages[i];
		if (pone->strid.compare(gfeid) == 0 && (pone->type == N_GONG || pone->type == W_GONG || pone->type == WEAPON || pone->type == PROTECT_EQU))
			return true;
	}
	//仓库中是否有
	StorageType storagetype[] = { N_GONG, W_GONG, WEAPON, PROTECT_EQU };

	for (int m = 0; m < sizeof(storagetype) / sizeof(storagetype[0]); m++)
	{
		StorageType stype = storagetype[m];
		for (unsigned i = 0; i < StorageRoom::map_storageData[stype].size(); i++)
		{
			if (StorageRoom::map_storageData[stype][i].strid.compare(gfeid) == 0)
				return true;
		}
	}

	std::string mymixgf = GlobalData::getMixGF();
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mymixgf];
		for (unsigned int n = 0; n < mdata.vec_secgf.size(); n++)
		{
			if (gfeid.compare(mdata.vec_secgf[n]) == 0)
				return true;
		}
	}

	return false;
}

void Hero::playBossActiveEffect() {
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
		shakeAction->setTag(4);
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
		shakeAction->setTag(6);
		m_normallBoss_hand_right->runAction(shakeAction);
	}
}

PackageData* Hero::getGF_Equip(std::string gfeid)
{
	HeroAtrType heroproptype[] = { H_WG, H_NG, H_ARMOR, H_WEAPON };

	for (int i = 0; i < sizeof(heroproptype) / sizeof(heroproptype[0]); i++)
	{
		if (getAtrByType(heroproptype[i])->count > 0 && getAtrByType(heroproptype[i])->strid.compare(gfeid) == 0)
			return getAtrByType(heroproptype[i]);
	}

	//背包中是否有
	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		PackageData* pone = &MyPackage::vec_packages[i];
		if (pone->strid.compare(gfeid) == 0 && (pone->type == N_GONG || pone->type == W_GONG || pone->type == WEAPON || pone->type == PROTECT_EQU))
			return &MyPackage::vec_packages[i];
	}
	//仓库中是否有
	StorageType storagetype[] = { N_GONG, W_GONG, WEAPON, PROTECT_EQU};

	for (int m = 0; m < sizeof(storagetype) / sizeof(storagetype[0]); m++)
	{
		StorageType stype = storagetype[m];
		for (unsigned i = 0; i < StorageRoom::map_storageData[stype].size(); i++)
		{
			if (StorageRoom::map_storageData[stype][i].strid.compare(gfeid) == 0)
				return &StorageRoom::map_storageData[stype][i];
		}
	}



	return NULL;
}

void Hero::stopBossActiveEffect() {
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

PackageData* Hero::getMeHas(std::string strid)
{
	//装备栏是否有
	for (int i = H_WEAPON; i < H_MAX; i++)
	{
		if (getAtrByType((HeroAtrType)i)->count > 0 && getAtrByType((HeroAtrType)i)->strid.compare(strid) == 0)
		{
			return getAtrByType((HeroAtrType)i);
		}
	}
	//背包中是否有
	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		if (MyPackage::vec_packages[i].strid.compare(strid) == 0)
			return &MyPackage::vec_packages[i];
	}
	//仓库中是否有

	std::map<int, std::vector<PackageData>>::iterator it;
	for (it = StorageRoom::map_storageData.begin(); it != StorageRoom::map_storageData.end(); ++it)
	{
		int size = StorageRoom::map_storageData[it->first].size();
		for (int j = 0; j < size; j++)
		{
			PackageData sdata = StorageRoom::map_storageData[it->first][j];
			if (sdata.strid.compare(strid) == 0)
				return &StorageRoom::map_storageData[it->first][j];
		}
	}
	return NULL;
}
void Hero::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}
int Hero::getGfCountByLv(int lv)
{
	int count = 0;
	//装备栏是否有
	for (int i = H_WG; i <= H_NG; i++)
	{
		if (getAtrByType((HeroAtrType)i)->count > 0 && getAtrByType((HeroAtrType)i)->lv >= lv-1)
		{
			count++;
		}
	}
	//背包中是否有
	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		if ((MyPackage::vec_packages[i].type == W_GONG || MyPackage::vec_packages[i].type == N_GONG) && MyPackage::vec_packages[i].lv >= lv-1)
			count++;
	}
	//仓库中是否有
	for (int i = N_GONG; i <= W_GONG; i++)
	{
		int size = StorageRoom::map_storageData[i].size();
		for (int j = 0; j < size; j++)
		{
			PackageData sdata = StorageRoom::map_storageData[i][j];
			if (sdata.lv >= lv - 1)
				count++;
		}
	}
	return count;
}
time_t Hero::getNowTime()
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
void Hero::checkMaxVaule(float dt)
{
	float mlife = 1.0f;

	float matk = 1.0f;
	float mdf = 1.0f;
	if (m_spirit <= 10.0f)
	{
		liferecoverpercent = 0.3f;
		injuryrecoverpercent = 0.3f;
		outjuryrecoverpercent = 0.3f;
		hungerrecoverpercent = 0.3f;
		matk = 0.7f;
		mdf = 0.7f;
	}
	else if (m_spirit <= 20.0f)
	{
		liferecoverpercent = 0.4f;
		injuryrecoverpercent = 0.4f;
		outjuryrecoverpercent = 0.4f;
		hungerrecoverpercent = 0.4f;
		matk = 0.9f;
		mdf = 0.9f;
	}

	else if (m_spirit <= 30.0f)
	{
		liferecoverpercent = 0.7f;
		injuryrecoverpercent = 0.7f;
		outjuryrecoverpercent = 0.7f;
		hungerrecoverpercent = 0.7f;
		matk =  1.0f;
		mdf = 1.0f;
	}

	else if (m_spirit <= 40.0f)
	{
		liferecoverpercent = 0.9f;
		injuryrecoverpercent = 0.9f;
		outjuryrecoverpercent = 0.9f;
		hungerrecoverpercent = 0.9f;
		matk = 1.0f;
		mdf = 1.0f;
	}
	else
	{
		liferecoverpercent = 1.0f;
		injuryrecoverpercent = 1.0f;
		outjuryrecoverpercent = 1.0f;
		hungerrecoverpercent = 1.0f;
		matk = 1.0f;
		mdf = 1.0f;
	}

	if (m_hunger <= 10.0f)
	{
		mlife = MIN(mlife, 0.3f);
		matk = MIN(matk, 0.3f);
		mdf = MIN(mdf, 0.3);
	}
	else if (m_hunger <= 20.0f)
	{
		mlife = MIN(mlife, 0.4f);
		matk = MIN(matk, 0.4f);
		mdf = MIN(mdf, 0.4);
	}
	else if (m_hunger <= 30.0f)
	{
		mlife = MIN(mlife, 1.0f);
		matk = MIN(matk, 0.7f);
		mdf = MIN(mdf, 0.7);
	}
	else if (m_hunger <= 40.0f)
	{
		mlife = MIN(mlife, 1.0f);
		matk = MIN(matk, 0.9f);
		mdf = MIN(mdf, 0.9);
	}
	else
	{
		mlife = MIN(mlife, 1.0f);
		matk = MIN(matk, 1.0f);
		mdf = MIN(mdf, 1.0);
	}

	if (m_innerinjury <= 10.0f)
	{
		mlife = MIN(mlife, 0.3f);
	}
	else if (m_innerinjury <= 20.0f)
	{
		mlife = MIN(mlife, 0.4f);
	}
	else if (m_innerinjury <= 30.0f)
	{
		mlife = MIN(mlife, 0.7f);
	}
	else if (m_innerinjury <= 40.0f)
	{
		mlife = MIN(mlife, 0.9f);
	}
	else
	{
		mlife = MIN(mlife, 1.0f);
	}

	if (m_outinjury <= 10.0f)
	{
		mlife = MIN(mlife, 0.3f);
	}
	else if (m_outinjury <= 20.0f)
	{
		mlife = MIN(mlife, 0.4f);
	}
	else if (m_outinjury <= 30.0f)
	{
		mlife = MIN(mlife, 0.7f);
	}
	else if (m_outinjury <= 40.0f)
	{
		mlife = MIN(mlife, 0.9f);
	}
	else
	{
		mlife = MIN(mlife, 1.0f);
	}

	maxlifepercent = mlife;

	if (m_life > getRecoverLifeMaxValue())
		setLifeValue(getRecoverLifeMaxValue());

	setAtkPercent(matk);
	setDfPercent(mdf);
}
long long Hero::getNowTimeMs() {
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

void Hero::recoverOutjury(int val)
{
	float fval = val * outjuryrecoverpercent;
	m_outinjury += fval;
}

void Hero::recoverInjury(int val)
{
	float fval = val * injuryrecoverpercent;
	m_innerinjury += fval;
}
bool Hero::isBeforeToday(time_t sec) {
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

long long Hero::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}
void Hero::recoverHunger(int val)
{
	float fval = val* hungerrecoverpercent;
	m_hunger += fval;
}

int Hero::getTotalDf()
{
	int adf = 0;
	int ngdf = 0;
	float slvdf = 0.0f;//强化等级加防

	float frienddfpercent = 0.0f;
	std::string masternpc;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::map_myfriendly.begin(); it != GlobalData::map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::map_myfriendly[nid].relation == F_FRIEND)
		{
			frienddfpercent += GlobalData::map_NPCFriendData[nid].dfpercent / 100;
		}
		else if (GlobalData::map_myfriendly[nid].relation == F_MASTER)
		{
			frienddfpercent += GlobalData::map_NPCMasterData[nid].dfpercent / 100;
			masternpc = nid;
		}
	}

	if (g_hero->getAtrByType(H_NG)->count > 0)
	{
		std::string gfname = g_hero->getAtrByType(H_NG)->strid;
		int gflv = g_hero->getAtrByType(H_NG)->lv;

		if (gflv > GlobalData::map_wgngs[gfname].maxlv - 1)
			gflv = GlobalData::map_wgngs[gfname].maxlv - 1;
		ngdf = GlobalData::map_wgngs[gfname].vec_bns[gflv];

		int masterdf = 0;
		if (masternpc.length() > 0)
		{
			for (unsigned int i = 0; i < GlobalData::map_NPCMasterData[masternpc].vec_gfid.size(); i++)
			{
				if (gfname.compare(GlobalData::map_NPCMasterData[masternpc].vec_gfid[i]) == 0)
				{
					int msbdf = ngdf * GlobalData::map_NPCMasterData[masternpc].vec_gfbonus[i] / 100;
					masterdf += msbdf;

					break;
				}
			}
		}

		int mixdf = 0;
		std::string mymixgf = GlobalData::getMixGF();
		if (mymixgf.length() > 0)
		{
			MixGfData mdata = GlobalData::map_MixGfData[mymixgf];
			if ((g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WG)->strid.compare(mdata.mastergf) == 0) || (g_hero->getAtrByType(H_NG)->count > 0 && g_hero->getAtrByType(H_NG)->strid.compare(mdata.mastergf) == 0))
				mixdf += ngdf * mdata.dfpercent / 100;
		}
		ngdf = ngdf + masterdf + mixdf;
	}

	if (g_hero->getAtrByType(H_ARMOR)->count > 0 && g_hero->getAtrByType(H_ARMOR)->goodvalue > 0)
	{
		std::string aname = g_hero->getAtrByType(H_ARMOR)->strid;
		adf = GlobalData::map_equips[aname].df;
		int slv = g_hero->getAtrByType(H_ARMOR)->slv;
		slvdf = slv * (slv + adf / 10);
	}
	//防御属性
	float fdf = g_hero->getDfPercent() *(g_hero->getDfValue() + ngdf + adf);
	fdf += slvdf;

	fdf += fdf*m_totalDfBonusPercent;


	fdf += fdf*frienddfpercent;

	if (GlobalData::myFactionlv == 4)
		fdf += fdf*0.02f;

	adf = int(fdf + 0.5f);
	return adf;
}
bool Hero::getRandomBoolean(float rate) {

	int rate10 = (int)(rate*10.0);
	int randNum = rand();
	if (randNum % 10 <= rate10) {
		return true;
	}
	else {
		return false;
	}
}

bool Hero::getRandomBoolean() {

	if (0 == rand() % 2) {
		return true;
	}
	else {
		return false;
	}
}
int Hero::getTotalAtck()
{
	int weaponAtk = 0;
	int wgAtk = 0;
	float slvAtk = 0.0f;//强化等级加攻
	if (g_hero->getAtrByType(H_WEAPON)->count > 0 && g_hero->getAtrByType(H_WEAPON)->goodvalue > 0)
	{
		std::string strid = g_hero->getAtrByType(H_WEAPON)->strid;
		weaponAtk = GlobalData::map_equips[strid].atk;
		int slv = g_hero->getAtrByType(H_WEAPON)->slv;
		slvAtk = slv * (slv + weaponAtk / 10);
	}

	float friendatkpercent = 0.0f;
	std::string masternpc;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::map_myfriendly.begin(); it != GlobalData::map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::map_myfriendly[nid].relation == F_FRIEND)
		{
			friendatkpercent += GlobalData::map_NPCFriendData[nid].atkpercent / 100;
		}
		else if (GlobalData::map_myfriendly[nid].relation == F_MASTER)
		{
			friendatkpercent += GlobalData::map_NPCMasterData[nid].atkpercent / 100;
			masternpc = nid;
		}
	}

	if (g_hero->getAtrByType(H_WG)->count > 0)
	{
		std::string strid = g_hero->getAtrByType(H_WG)->strid;
		int gflv = g_hero->getAtrByType(H_WG)->lv;
		if (gflv > GlobalData::map_wgngs[strid].maxlv - 1)
			gflv = GlobalData::map_wgngs[strid].maxlv - 1;
		wgAtk = GlobalData::map_wgngs[strid].vec_bns[gflv];

		int masteratk = 0;
		if (masternpc.length() > 0)
		{
			for (unsigned int i = 0; i < GlobalData::map_NPCMasterData[masternpc].vec_gfid.size(); i++)
			{
				if (strid.compare(GlobalData::map_NPCMasterData[masternpc].vec_gfid[i]) == 0)
				{
					int msbatk = wgAtk * GlobalData::map_NPCMasterData[masternpc].vec_gfbonus[i] / 100;
					masteratk += msbatk;
					break;
				}
			}
		}

		int mixatk = 0;
		std::string mymixgf = GlobalData::getMixGF();
		if (mymixgf.length() > 0)
		{
			MixGfData mdata = GlobalData::map_MixGfData[mymixgf];
			if ((g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WG)->strid.compare(mdata.mastergf) == 0) || (g_hero->getAtrByType(H_NG)->count > 0 && g_hero->getAtrByType(H_NG)->strid.compare(mdata.mastergf) == 0))
				mixatk += wgAtk * mdata.atkpercent / 100;
		}
		wgAtk = wgAtk + masteratk + mixatk;
	}

	//攻击属性
	float fack = g_hero->getAtkPercent() * (g_hero->getAtkValue() + weaponAtk + wgAtk);

	if (g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WEAPON)->count > 0 && g_hero->getAtrByType(H_WEAPON)->goodvalue > 0)
	{
		if (GlobalData::map_wgngs[g_hero->getAtrByType(H_WG)->strid].extype == GlobalData::map_equips[g_hero->getAtrByType(H_WEAPON)->strid].extype)
		{
			float back = fack * 0.05f;
			fack += back;
		}
	}
	fack += slvAtk;

	fack += fack* m_totalAtkBonusPercent;

	fack += fack* friendatkpercent;

	if (GlobalData::myFactionlv == 5)
		fack += fack*0.02f;

	int tatk = int(fack + 0.5f);
	return tatk;
}

int Hero::getRandomNum(int range) {

	if (range <= 0) {
		return 0;
	}

	return rand() % range;
}

int Hero::getRandomNum(int rangeStart, int rangeEnd) {

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
float Hero::getCritRate()
{
	int critrnd = GlobalData::map_heroAtr[g_hero->getHeadID()].vec_crit[g_hero->getLVValue()];
	if (g_hero->getAtrByType(H_WG)->count > 0)
		critrnd += GlobalData::map_wgngs[g_hero->getAtrByType(H_WG)->strid].vec_cirt[g_hero->getAtrByType(H_WG)->lv];

	float friendcritrnd = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::map_myfriendly.begin(); it != GlobalData::map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::map_myfriendly[nid].relation == F_FRIEND)
		{
			friendcritrnd += GlobalData::map_NPCFriendData[nid].critpercent;
		}
		else if (GlobalData::map_myfriendly[nid].relation == F_MASTER)
		{
			friendcritrnd += GlobalData::map_NPCMasterData[nid].critpercent;
		}
	}
	critrnd += friendcritrnd;

	float mixcrit = 0.0f;
	std::string mymixgf = GlobalData::getMixGF();
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mymixgf];
		if ((g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WG)->strid.compare(mdata.mastergf) == 0) || (g_hero->getAtrByType(H_NG)->count > 0 && g_hero->getAtrByType(H_NG)->strid.compare(mdata.mastergf) == 0))
			mixcrit += mdata.critpercent;
	}
	critrnd += mixcrit;
	if (GlobalData::myFactionlv == 2)
		critrnd += 2;
	return critrnd;
}
void Hero::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void Hero::shake(Node * node) {
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

float Hero::getdodgeRate()
{
	int dodgernd = GlobalData::map_heroAtr[g_hero->getHeadID()].vec_dodge[g_hero->getLVValue()];

	if (g_hero->getAtrByType(H_NG)->count > 0)
		dodgernd += GlobalData::map_wgngs[g_hero->getAtrByType(H_NG)->strid].vec_dodge[g_hero->getAtrByType(H_NG)->lv];

	float frienddogdernd = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::map_myfriendly.begin(); it != GlobalData::map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::map_myfriendly[nid].relation == F_FRIEND)
		{
			frienddogdernd += GlobalData::map_NPCFriendData[nid].dodgepercent;
		}
		else if (GlobalData::map_myfriendly[nid].relation == F_MASTER)
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
		if ((g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WG)->strid.compare(mdata.mastergf) == 0) || (g_hero->getAtrByType(H_NG)->count > 0 && g_hero->getAtrByType(H_NG)->strid.compare(mdata.mastergf) == 0))
			mixdodge += mdata.dodgepercent;
	}
	dodgernd += mixdodge;
	if (GlobalData::myFactionlv == 1)
		dodgernd += 2;

	return dodgernd;
}

bool Hero::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
}


void Hero::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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
