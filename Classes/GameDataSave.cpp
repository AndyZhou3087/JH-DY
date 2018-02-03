#include "GameDataSave.h"
#include "cocos2d.h"
#include "Utility.h"
#include "Hero.h"
#include "Nature.h"
#include "GlobalData.h"
GameDataSave* GameDataSave::_Context = NULL;
std::string GameDataSave::userid = "";
GameDataSave* GameDataSave::getInstance() {
	if (_Context == NULL) {
		_Context = new GameDataSave;
	}
	return _Context;
}

void GameDataSave::purgeGameSave() {
	CC_SAFE_DELETE(_Context);
}

int GameDataSave::getLiveDays()
{
	return loadIntDataByKey(addUIDString("livedays"));
}
void GameDataSave::initA()
{
	const Size size = Director::getInstance()->getVisibleSize();
	auto m_top = CSLoader::createNode("GameTopLayer.csb");
	Director::getInstance()->getRunningScene()->addChild(m_top, 0);
	m_top->ignoreAnchorPointForPosition(false);
	m_top->setAnchorPoint(Vec2(0.5, 1));
	m_top->setPosition(size.width * 0.5, size.height);

	auto m_bottom = CSLoader::createNode("GameBottomLayer.csb");
	m_bottom->ignoreAnchorPointForPosition(false);
	m_bottom->setAnchorPoint(Vec2(0.5, 0));
	m_bottom->setPosition(size.width * 0.5, 0);
	Director::getInstance()->getRunningScene()->addChild(m_bottom, 1);
}
void GameDataSave::setLiveDays(int val)
{
	saveIntDataByKey(addUIDString("livedays"), val);
}

float GameDataSave::getHeroOutinjury()
{
	return loadFloatDataByKey(addUIDString("outinjury"), Hero::MAXOutinjuryValue);
}
void GameDataSave::setHeroOutinjury(float val)
{
	saveFloatDataByKey(addUIDString("outinjury"), val);
}

void GameDataSave::initBg()
{
	const Size size = Director::getInstance()->getVisibleSize();
	auto m_bg = Sprite::create("");
	Director::getInstance()->getRunningScene()->addChild(m_bg, 0);
	m_bg->setAnchorPoint(Vec2(0.5, 0));
	m_bg->setPosition(size.width * 0.5, 0);

	auto m_bgGround = Sprite::create("");
	Director::getInstance()->getRunningScene()->addChild(m_bgGround, 1);
	m_bgGround->setPosition(m_bg->getContentSize().width * 0.5, 144 + m_bgGround->getContentSize().height * 0.5);

	auto tree = Sprite::create("");
	Director::getInstance()->getRunningScene()->addChild(tree);
	tree->setAnchorPoint(Vec2(1, 0.5));
	tree->setPosition(size.width, 120);
}
float GameDataSave::getHeroInnerinjury()
{
	return loadFloatDataByKey(addUIDString("innerinjury"), Hero::MAXInnerinjuryValue);
}
void GameDataSave::setHeroInnerinjury(float val)
{
	saveFloatDataByKey(addUIDString("innerinjury"), val);
}

float GameDataSave::getHeroHunger()
{
	return loadFloatDataByKey(addUIDString("hunger"), Hero::MAXHungerValue);
}
void GameDataSave::setHeroHunger(float val)
{
	saveFloatDataByKey(addUIDString("hunger"), val);
}

void GameDataSave::onGameStart() {
	playBossShowEffect();
}

void GameDataSave::updateBloodBar() {

	float rate = 0;
	if (rate > 0) {
		rate = 1 - 100 * 1.0f / 20;
	}

	if (nullptr != m_bloodBar) {
		static const Vec2 offSize = m_bloodBar->getTextureRect().origin;
		static const float h = m_bloodBar->getContentSize().height;
		static const float w = m_bloodBar->getContentSize().width;
		float width = rate * w;
		m_bloodBar->setTextureRect(CCRectMake(offSize.x, offSize.y, width, h));
	}
}

float GameDataSave::getHeroSpirit()
{
	return loadFloatDataByKey(addUIDString("spirit"), Hero::MAXSpiritValue);
}
void GameDataSave::setHeroSpirit(float val)
{
	saveFloatDataByKey(addUIDString("spirit"), val);
}

void GameDataSave::initTime() {
	int s = 10;
	if (nullptr != m_time) {
		m_time->setString(String::createWithFormat("%d", s)->_string);
	}
}

void GameDataSave::updateTime() {
	bool isok = false;
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


float GameDataSave::getHeroLife()
{
	return loadFloatDataByKey(addUIDString("life"), -100.0f);
}
void GameDataSave::setHeroLife(float val)
{
	saveFloatDataByKey(addUIDString("life"), val);
}

void GameDataSave::resetBoss() {
	initalBoss();

	if (nullptr != m_boss) {
		m_boss->setPositionY(m_bossOriginPosY + 500);
		m_boss->setOpacity(255);
	}

	setHurtBossVisible(false);
}

void GameDataSave::onScoreChange() {

	updateBloodBar();
}

void GameDataSave::on1sTimer() {

	if (m_bStop) {
		return;
	}

	updateTime();
}

void GameDataSave::onTimeChange() {

	updateTime();
}


int GameDataSave::getNatureTime()
{
	return loadIntDataByKey(addUIDString("ndaytime"));
}
void GameDataSave::setNatureTime(int val)
{
	saveIntDataByKey(addUIDString("ndaytime"), val);
}

void GameDataSave::onAttrackBoss() {
	playAttrackEffect();
}

void GameDataSave::playAttrackEffect() {
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

int GameDataSave::getNatureWeather()
{
	return loadIntDataByKey(addUIDString("nwhather"), EWeather::Suny);
}
void GameDataSave::setNatureWeather(int val)
{
	saveIntDataByKey(addUIDString("nwhather"), val);
}

void GameDataSave::playBossShowEffect(CallFunc * callback) {
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
int GameDataSave::getNatureWeatherChangeCount()
{
	return loadIntDataByKey(addUIDString("nwhatherchgcount"), 0);
}

void GameDataSave::setNatureWeatherChangeCount(int val)
{
	saveIntDataByKey(addUIDString("nwhatherchgcount"), val);
}

void GameDataSave::playBossDeathEffect() {
	playBombEffect();
}

void GameDataSave::initBossBombParticleSystem() {
	if (nullptr == m_boss) {
		return;
	}

	m_emitterBomb = ParticleSystemQuad::create("");
	m_emitterBomb->setTexture(Director::getInstance()->getTextureCache()->addImage(""));
	m_boss->addChild(m_emitterBomb, 100);
	m_emitterBomb->setPosition(Vec2(m_boss->getContentSize().width * 0.5, m_boss->getContentSize().height * 0.5));
	m_emitterBomb->stopSystem();
}

int GameDataSave::getNatureReason()
{
	return loadIntDataByKey(addUIDString("nreason"), EReason::Spring);
}
void GameDataSave::setNatureReason(int val)
{
	saveIntDataByKey(addUIDString("nreason"), val);
}

void GameDataSave::onGameOver() {
	stopBossActiveEffect();
	playBossDeathEffect();
}

void GameDataSave::playBombEffect() {
	if (nullptr == m_boss) {
		return;
	}

	Texture2D * txt2d = TextureCache::getInstance()->addImage("");
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width / 2;
	float h = txt2d->getContentSize().height;

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.2);
	for (int i = 0; i<7; i++) {
		ani->addSpriteFrameWithTexture(txt2d, Rect(i*w, i*h, w, h));
	}

	Sprite * sprite = Sprite::create("", Rect(0, 0, w, h));
	Sprite *m_layer;
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

int GameDataSave::getNatureTemperature()
{
	//默认温度取随机
	int maxr = tempeRange[getNatureReason()][1] - tempeRange[getNatureReason()][0] + 1;
	int r = tempeRange[getNatureReason()][0] + GlobalData::createRandomNum(maxr);
	return loadIntDataByKey(addUIDString("ntempe"), r);
}
void GameDataSave::setNatureTemperature(int val)
{
	saveIntDataByKey(addUIDString("ntempe"), val);
}

std::string GameDataSave::getStorageData()
{
	return loadStringDataByKey(addUIDString("storage"), "");
}
void GameDataSave::setStorageData(std::string valstr)
{
	saveStringDataByKey(addUIDString("storage"), valstr);
}

void GameDataSave::initalBoss() {
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
int GameDataSave::getBuildLV(std::string buildname)
{
	std::string rname = StringUtils::format("%slv", buildname.c_str());
	return loadIntDataByKey(addUIDString(rname));
}
void GameDataSave::setBuildLV(std::string buildname, int val)
{
	std::string rname = StringUtils::format("%slv", buildname.c_str());
	saveIntDataByKey(addUIDString(rname), val);
}

void GameDataSave::setHurtBossVisible(bool isVisible) {
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
std::string GameDataSave::getHeroAddr()
{
	return loadStringDataByKey(addUIDString("heroaddr"), "m1-1");
}

void GameDataSave::setHeroAddr(std::string addr)
{
	saveStringDataByKey(addUIDString("heroaddr"), addr);
}

std::string GameDataSave::getTempStorage(std::string addrname)
{
	std::string str = StringUtils::format("%s-temps", addrname.c_str());
	return loadStringDataByKey(addUIDString(str));
}

void GameDataSave::playBossActiveEffect() {
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

void GameDataSave::setTempStorage(std::string addrname, std::string vstr)
{
	std::string str = StringUtils::format("%s-temps", addrname.c_str());
	saveStringDataByKey(addUIDString(str), vstr);
}

std::string GameDataSave::getPackage()
{
	return loadStringDataByKey(addUIDString("packages"));
}

void GameDataSave::setPackage(std::string vstr)
{
	saveStringDataByKey(addUIDString("packages"), vstr);
}

void GameDataSave::stopBossActiveEffect() {
	if (nullptr != m_boss) {
		m_boss->stopActionByTag(4);
	}

	if (nullptr != m_normallBoss_hand_left) {
		m_normallBoss_hand_left->stopActionByTag(5);
		m_normallBoss_hand_left->setRotation(-20);
	}

	if (nullptr != m_normallBoss_hand_right) {
		m_normallBoss_hand_right->stopActionByTag(6);
		m_normallBoss_hand_right->setRotation(20);
	}
}

std::string GameDataSave::getResData()
{
	return loadStringDataByKey(addUIDString("resdata"));
}
void GameDataSave::setResData(std::string vstr)
{
	saveStringDataByKey(addUIDString("resdata"), vstr);
}

int GameDataSave::getHeroId()
{
	return loadIntDataByKey(addUIDString("heroid"), 1);
}

int GameDataSave::getHeroIdByUid(std::string uid)
{
	std::string key = StringUtils::format("%sheroid", uid.c_str());
	return loadIntDataByKey(key, 1);
}

void GameDataSave::setHeroId(int id)
{
	saveIntDataByKey(addUIDString("heroid"), id);
}

void GameDataSave::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}

int GameDataSave::getHeroLV()
{
	return loadIntDataByKey(addUIDString("herolv"), 0);
}

int GameDataSave::getHeroLV(std::string localid)
{
	std::string key = localid + "herolv";
	return loadIntDataByKey(key, 0);
}

void GameDataSave::setHeroLV(int lv)
{
	saveIntDataByKey(addUIDString("herolv"), lv);
}

time_t GameDataSave::getNowTime()
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

int GameDataSave::getHeroExp()
{
	return loadIntDataByKey(addUIDString("heroexp"), 0);
}
void GameDataSave::setHeroExp(int val)
{
	saveIntDataByKey(addUIDString("heroexp"), val);
}

bool GameDataSave::getHeroIsOut()
{
	int isout = loadIntDataByKey(addUIDString("heroisout"), 0);
	return isout == 0?false:true;
}
void GameDataSave::setHeroIsOut(bool val)
{
	saveIntDataByKey(addUIDString("heroisout"), val == true ? 1 : 0);
}

long long GameDataSave::getNowTimeMs() {
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
std::string GameDataSave::getHeroProperData()
{
	return loadStringDataByKey(addUIDString("heroproper"));
}

std::string GameDataSave::getHeroProperData(std::string localid)
{
	std::string key = localid + "heroproper";
	return loadStringDataByKey(key);
}

void GameDataSave::setHeroProperData(std::string strval)
{
	saveStringDataByKey(addUIDString("heroproper"), strval);
}

bool GameDataSave::isBeforeToday(time_t sec) {
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

std::string GameDataSave::getHeroUnlockData()
{
	return loadStringDataByKey("herounlock", "1-0-0-0");
}
void GameDataSave::setHeroUnlockData(std::string strval)
{
	saveStringDataByKey("herounlock", strval);
}

std::string GameDataSave::getUserId()
{
	return loadStringDataByKey("uid");
}

long long GameDataSave::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}

bool GameDataSave::getRandomBoolean(float rate) {

	int rate10 = (int)(rate*10.0);
	int randNum = rand();
	if (randNum % 10 <= rate10) {
		return true;
	}
	else {
		return false;
	}
}


void GameDataSave::setUserId(std::string val)
{
	userid = val;
}

void GameDataSave::saveUserId(std::string val)
{
	setUserId(val);
	saveStringDataByKey("uid", val);
}

std::string GameDataSave::addUIDString(std::string val)
{
	return userid + val;
}

bool GameDataSave::getRandomBoolean() {

	if (0 == rand() % 2) {
		return true;
	}
	else {
		return false;
	}
}

void GameDataSave::setPlotMissionIndex(int val)
{
	saveIntDataByKey(addUIDString("plot"), val);
}

int GameDataSave::getPlotMissionIndex()
{
	return loadIntDataByKey(addUIDString("plot"), 0);
}

int GameDataSave::getRandomNum(int range) {

	if (range <= 0) {
		return 0;
	}

	return rand() % range;
}


void GameDataSave::setPlotMissionStatus(std::string strval)
{
	saveStringDataByKey(addUIDString("plotstatus"), strval);
}

std::string GameDataSave::getPlotMissionStatus()
{
	return loadStringDataByKey(addUIDString("plotstatus"));
}

int GameDataSave::getRandomNum(int rangeStart, int rangeEnd) {

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

void GameDataSave::setPlotUnlockChapter(int val)
{
	saveIntDataByKey(addUIDString("unlockchapter"), val);
}

int GameDataSave::getPlotUnlockChapter()
{
	return loadIntDataByKey(addUIDString("unlockchapter"), 1);
}

void GameDataSave::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void GameDataSave::setBranchPlotMissionStatus(std::string strval)
{
	saveStringDataByKey(addUIDString("newbplotstatus"), strval);
}

std::string GameDataSave::getBranchPlotMissionStatus()
{
	return loadStringDataByKey(addUIDString("newbplotstatus"));
}

void GameDataSave::shake(Node * node) {
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
std::string GameDataSave::getSaveListId()
{
	return loadStringDataByKey("savelistid", ";;;");
}

void GameDataSave::setBranchPlotMissionGiveGoods(std::string strval)
{
	saveStringDataByKey(addUIDString("bmggs"), strval);
}

bool GameDataSave::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
}
std::string GameDataSave::getBranchPlotMissionGiveGoods()
{
	return loadStringDataByKey(addUIDString("bmggs"), "");
}

void GameDataSave::setSaveListId(std::string val)
{
	saveStringDataByKey("savelistid", val);
}

bool GameDataSave::getIsNewerGuide(int index)
{
	std::string key = StringUtils::format("guide%d", index);
	int val = loadIntDataByKey(key, 1);
	return val == 1 ? true : false;
}

void GameDataSave::setIsNewerGuide(int index, bool val)
{
	std::string key = StringUtils::format("guide%d", index);
	saveIntDataByKey(key, (int)val);
}

void GameDataSave::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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

void GameDataSave::setExersiceCfg(std::string strval)
{
	saveStringDataByKey(addUIDString("exsercise"), strval);
}

std::string GameDataSave::getExersiceCfg()
{
	return loadStringDataByKey(addUIDString("exsercise"), "");
}


void GameDataSave::setModifyDefaultStorage(int heroindex, std::string strval)
{
	std::string key = StringUtils::format("hero%ddefstorge", heroindex);
	saveStringDataByKey(key, strval);
}


void GameDataSave::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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


std::string GameDataSave::getModifyDefaultStorage(int heroindex)
{
	std::string key = StringUtils::format("hero%ddefstorge", heroindex);
	return loadStringDataByKey(key, "");
}

void GameDataSave::setShareDayOfYear(int day)
{
	saveIntDataByKey("shareday", day);
}

int GameDataSave::getShareDayOfYear()
{
	return loadIntDataByKey("shareday", -1);
}

void GameDataSave::setWarmConfig(std::string strval)
{
	saveStringDataByKey(addUIDString("warmconfig"), strval);
}


void GameDataSave::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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



std::string GameDataSave::getWarmConfig()
{
	return loadStringDataByKey(addUIDString("warmconfig"), "");
}

void GameDataSave::setHeroSex(int val)
{
	saveIntDataByKey(addUIDString("sex"), val);
}

int GameDataSave::getHeroSex()
{
	return loadIntDataByKey(addUIDString("sex"), -1);
}

void GameDataSave::setHeroExpEndTime(int val)
{
	saveIntDataByKey(addUIDString("heroexpetime"), val);
}

void GameDataSave::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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


int GameDataSave::getHeroExpEndTime()
{
	return loadIntDataByKey(addUIDString("heroexpetime"), 0);
}

void GameDataSave::setGfEndTime(int val)
{
	saveIntDataByKey(addUIDString("gfexpetime"), val);
}

int GameDataSave::getGfEndTime()
{
	return loadIntDataByKey(addUIDString("gfexpetime"), 0);
}

std::string GameDataSave::getExgCfgData()
{
	return loadStringDataByKey(addUIDString("exgcfg"), "");
}

void GameDataSave::setExgCfgData(std::string strval)
{
	saveStringDataByKey(addUIDString("exgcfg"), strval);
}

void GameDataSave::jumpDown(cocos2d::Node *node, float dt) {
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

void GameDataSave::setWxbMapPos(int val)
{
	saveIntDataByKey(addUIDString("wxbmap"), val);
}

int GameDataSave::getWxbMapPos()
{
	return loadIntDataByKey(addUIDString("wxbmap"), 0);
}

void GameDataSave::setDgqbMapPos(int val)
{
	saveIntDataByKey(addUIDString("dgqbmap"), val);
}

int GameDataSave::getDgqbMapPos()
{
	return loadIntDataByKey(addUIDString("dgqbmap"), 0);
}

void GameDataSave::setReviveCount(int val)
{
	saveIntDataByKey(addUIDString("revivecount"), val);
}

int GameDataSave::getReviveCount()
{
	return loadIntDataByKey(addUIDString("revivecount"), 1);
}

int GameDataSave::getGoldCount()
{
	return loadIntDataByKey("gold", 0);
}

void GameDataSave::setGoldCount(int count)
{
	saveIntDataByKey("gold", count);
}

bool GameDataSave::getIsPostAllData()
{
	int ret = loadIntDataByKey("postallok", 0);
	return ret == 1 ? true : false;
}

void GameDataSave::setIsPostAllData(bool val)
{
	saveIntDataByKey("postallok", val?1:0);
}

bool GameDataSave::getIsFirstInstall()
{
	int ret = loadIntDataByKey("firstintall", 1);
	return ret == 1 ? true : false;
}

void GameDataSave::setIsFirstInstall(bool val)
{
	saveIntDataByKey("firstintall", val ? 1 : 0);
}

std::string GameDataSave::getMyID()
{
	return loadStringDataByKey("id", "");
}

void GameDataSave::setMyID(std::string str)
{
	saveStringDataByKey("id", str);
}

std::string GameDataSave::getMyNickName()
{
	return loadStringDataByKey("nickname", "");
}

void GameDataSave::setMyNickName(std::string str)
{
	saveStringDataByKey("nickname", str);
}

bool GameDataSave::getIsBuyTimeGift()
{
	int ret = loadIntDataByKey("isbuytimegift", 0);
	return ret == 1 ? true : false;
}

void GameDataSave::setIsBuyTimeGift(bool val)
{
	saveIntDataByKey("isbuytimegift", val ? 1 : 0);
}

void GameDataSave::setUseGold(int val)
{
	saveIntDataByKey("costgold", val);
}

int GameDataSave::getUseGold()
{
	return loadIntDataByKey("costgold", 0);
}

std::string GameDataSave::getFriendly()
{
	return loadStringDataByKey(addUIDString("friendly"), "");
}

std::string GameDataSave::getFriendly(std::string localid)
{
	std::string key = localid + "friendly";
	return loadStringDataByKey(key, "");
}

void GameDataSave::setFriendly(std::string str)
{
	saveStringDataByKey(addUIDString("friendly"), str);
}

std::string GameDataSave::getMixGF()
{
	return loadStringDataByKey(addUIDString("mixgf"), "");
}

std::string GameDataSave::getMixGF(std::string localid)
{
	std::string key = localid + "mixgf";
	return loadStringDataByKey(key, "");
}

void GameDataSave::setMixGF(std::string str)
{
	saveStringDataByKey(addUIDString("mixgf"), str);
}

int GameDataSave::getFreeMixTime(int type)
{
	std::string typestr = StringUtils::format("freemixtime%d", type);
	return loadIntDataByKey(addUIDString(typestr), 0);
}

void GameDataSave::setFreeMixTime(int type, int val)
{
	std::string typestr = StringUtils::format("freemixtime%d", type);
	saveIntDataByKey(addUIDString(typestr), val);
}


int GameDataSave::getMixGfCountByType(int type)
{
	std::string typestr = StringUtils::format("mixcount%d", type);
	return loadIntDataByKey(addUIDString(typestr), 0);
}

void GameDataSave::setMixGfCountByType(int type, int val)
{
	std::string typestr = StringUtils::format("mixcount%d", type);
	saveIntDataByKey(addUIDString(typestr), val);
}

std::string GameDataSave::getSuggestMixGf()
{
	return loadStringDataByKey(addUIDString("suggestmixgf"), "");
}

void GameDataSave::setSuggestMixGf(std::string str)
{
	saveStringDataByKey(addUIDString("suggestmixgf"), str);
}

int GameDataSave::getNoPopNoticeDay()
{
	return loadIntDataByKey("nopopnoticeday", -1);
}

void GameDataSave::setNoPopNoticeDay(int day)
{
	saveIntDataByKey("nopopnoticeday", day);
}

int GameDataSave::getContributionDay()
{
	return loadIntDataByKey(addUIDString("contributeday"), -1);
}

void GameDataSave::setContributionDay(int day)
{
	saveIntDataByKey(addUIDString("contributeday"), day);
}


int GameDataSave::getSliverContribution()
{
	return loadIntDataByKey(addUIDString("slivercontribute"), 0);
}


void GameDataSave::setSliverContribution(int val)
{
	saveIntDataByKey(addUIDString("slivercontribute"), val);
}

int GameDataSave::getGoldContribution()
{
	return loadIntDataByKey(addUIDString("goldcontribute"), 0);
}

void GameDataSave::setGoldContribution(int val)
{
	saveIntDataByKey(addUIDString("goldcontribute"), val);
}

int GameDataSave::getApprenticeDay()
{
	return loadIntDataByKey(addUIDString("apprentice"), 0);
}
void GameDataSave::setApprenticeDay(int val)
{
	saveIntDataByKey(addUIDString("apprentice"), val);
}

std::string GameDataSave::getRaffleStage()
{
	return loadStringDataByKey("rafflestage", "");
}

void GameDataSave::setRaffleStage(std::string strval)
{
	saveStringDataByKey("rafflestage", strval);
}

std::string GameDataSave::getAchiveData()
{
	return loadStringDataByKey(addUIDString("achive"), "");
}

void GameDataSave::setAchiveData(std::string strval)
{
	saveStringDataByKey(addUIDString("achive"), strval);
}

std::string GameDataSave::getAchiveAnimData()
{
	return loadStringDataByKey(addUIDString("achiveanim"), "");
}

void GameDataSave::setAchiveAnimData(std::string strval)
{
	saveStringDataByKey(addUIDString("achiveanim"), strval);
}

void GameDataSave::setPlayerChallengeData(std::string strval)
{
	saveStringDataByKey(addUIDString("playerchallenge"), strval);
}

std::string GameDataSave::getPlayerChallengeData()
{
	return loadStringDataByKey(addUIDString("playerchallenge"), "");
}

void GameDataSave::setHsljMatchPlayer(std::string val)
{
	saveStringDataByKey("shljmatch", val);
}

std::string GameDataSave::getHsljMatchPlayer()
{
	return loadStringDataByKey("shljmatch", "");
}

int GameDataSave::getEnterGameDaysOfYear()
{
	return loadIntDataByKey(addUIDString("daysofyear"), -1);
}

void GameDataSave::setEnterGameDaysOfYear(int days)
{
	saveIntDataByKey(addUIDString("daysofyear"), days);
}