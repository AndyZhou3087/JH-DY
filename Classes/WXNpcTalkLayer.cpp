#include "WXNpcTalkLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GameScene.h"

WXNpcTalkLayer::WXNpcTalkLayer()
{
	m_wordindex = 0;
	m_wordcount = 0;
	lasttalklbl = NULL;
}
WXNpcTalkLayer::~WXNpcTalkLayer()
{

}
void WXNpcTalkLayer::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}

time_t WXNpcTalkLayer::getNowTime()
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
WXNpcTalkLayer* WXNpcTalkLayer::create(std::vector<std::string> vec_words, std::string npcid)
{
	WXNpcTalkLayer *pRet = new WXNpcTalkLayer();
	if (pRet && pRet->init(vec_words, npcid))
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
long long WXNpcTalkLayer::getNowTimeMs() {
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

bool WXNpcTalkLayer::isBeforeToday(time_t sec) {
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

bool WXNpcTalkLayer::init(std::vector<std::string> vec_words, std::string npcid)
{
	if (!Layer::init())
	{
		return false;
	}

	m_wordslist = vec_words;
	auto colorlayer = LayerColor::create(Color4B(0, 0, 0, 200));
	this->addChild(colorlayer);

	Node* csbnode = CSLoader::createNode("npctalkLayer.csb");
	this->addChild(csbnode, 2);

	m_talknode = (cocos2d::ui::Widget*)csbnode->getChildByName("node");
	npcimg = (cocos2d::ui::ImageView*)m_talknode->getChildByName("npcimg");
	std::string npcimgstr = StringUtils::format("ui/%s.png", npcid.c_str());
	npcimg->loadTexture(npcimgstr, cocos2d::ui::Widget::TextureResType::PLIST);
	npcimg->setOpacity(150);

	heroimg = (cocos2d::ui::ImageView*)m_talknode->getChildByName("heroimg");

	std::string heroidstr = StringUtils::format("ui/fhero%d.png", g_hero->getHeadID());
	heroimg->loadTexture(heroidstr, cocos2d::ui::TextureResType::PLIST);

	cocos2d::ui::Text* wordlbl = (cocos2d::ui::Text*)m_talknode->getChildByName("text");
	wordlbl->setVisible(false);

	heroimg->setOpacity(150);
	npcimg->setOpacity(255);

	checkWordLblColor(vec_words[0]);

	//float dt = 0.0f;
	//for (unsigned int i = 0; i < vec_words.size(); i++)
	//{
	//	this->scheduleOnce(schedule_selector(WXNpcTalkLayer::showTypeText), dt);
	//	dt += vec_words[i].size() / 3 * 0.1f + 1.0f;
	//}

	//layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{

		return true;
	};
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		if (isShowWord)
		{
			if (m_wordslist.size() <= 0)
				return;

			if (m_wordslist[m_wordindex].length() > 0)
			{
				fastShowWord();
				return;
			}
		}
		m_wordindex++;

		int size = m_wordslist.size();

		if (m_wordindex >= size)
		{
			this->removeFromParentAndCleanup(true);
			return;
		}
		else
		{
			checkWordLblColor(vec_words[m_wordindex]);
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

long long WXNpcTalkLayer::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}

void WXNpcTalkLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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

void WXNpcTalkLayer::checkWordLblColor(std::string wordstr)
{
	m_wordlbl = Label::createWithTTF(wordstr, "fonts/STXINGKA.TTF", 26);
	m_wordlbl->setLineBreakWithoutSpace(true);
	m_wordlbl->setMaxLineWidth(610);
	m_wordlbl->setPosition(Vec2(0,-115));
	m_talknode->addChild(m_wordlbl, 0, "talklbl");


	int index = 0;
	while (m_wordlbl->getLetter(index) != NULL)
	{
		m_wordlbl->getLetter(index)->setColor(Color3B::BLACK);
		m_wordlbl->getLetter(index)->setScale(0);
		index++;
	}

	std::map<std::string, NpcData>::iterator it;
	for (it = GlobalData::map_npcs.begin(); it != GlobalData::map_npcs.end(); ++it)
	{
		std::string npcname = GlobalData::map_npcs[it->first].name;
		std::size_t findpos = wordstr.find(npcname);
		if (findpos != std::string::npos)
		{
			int sindex = findpos / 3;
			int len = npcname.size() / 3;
			for (int i = sindex; i < sindex + len; i++)
			{
				m_wordlbl->getLetter(i)->setColor(Color3B(230, 35, 35));
			}
		}
	}
	std::size_t findpos = wordstr.find(g_hero->getMyName());
	if (findpos != std::string::npos)
	{
		int sindex = findpos / 3;
		int len = g_hero->getMyName().size() / 3;
		for (int i = sindex; i < sindex + len; i++)
		{
			m_wordlbl->getLetter(i)->setColor(Color3B(27, 141, 0));
		}
	}

	this->scheduleOnce(schedule_selector(WXNpcTalkLayer::showTypeText), 0.1f);
}

void WXNpcTalkLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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

void WXNpcTalkLayer::showTypeText(float dt)
{
	if (lasttalklbl != NULL)
	{
		lasttalklbl->setVisible(false);
		lasttalklbl->removeFromParentAndCleanup(true);
		lasttalklbl = NULL;
	}
	if (m_wordindex % 2 == 0)
	{
		heroimg->setOpacity(150);
		npcimg->setOpacity(255);
	}
	else
	{
		heroimg->setOpacity(255);
		npcimg->setOpacity(150);
	}
	lasttalklbl = m_wordlbl;
	m_wordlbl->schedule([&](float dt){
		isShowWord = true;
		m_wordcount += 3;
		int letterindex = m_wordcount / 3 - 1;
		m_wordlbl->getLetter(letterindex)->setScale(1.0f);
		int len = m_wordlbl->getString().length();
		if (m_wordcount >= len)
		{
			//m_wordindex++;
			m_wordcount = 0;
			isShowWord = false;
			m_wordlbl->unschedule("schedule_typecallback");
			//int size = m_wordslist.size();
			//if (m_wordindex < size)
			//{
			//	checkWordLblColor(m_wordslist[m_wordindex]);
			//}
		}

	}, 0.1f, "schedule_typecallback");
}
void WXNpcTalkLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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
void WXNpcTalkLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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
				DelayTime::create(20*0.1),
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
				DelayTime::create(30*0.1),
				action,
				NULL));
		}
		else {
			node->runAction(action);
		}
	}
}

void WXNpcTalkLayer::fastShowWord()
{
	isShowWord = false;
	m_wordcount = 0;

	this->unscheduleAllCallbacks();
	m_wordlbl->unscheduleAllCallbacks();
	int index = 0;
	while (m_wordlbl->getLetter(index) != NULL)
	{
		m_wordlbl->getLetter(index)->setScale(1);
		index++;
	}
	int wordsize = m_wordslist.size();
	if (m_wordindex >= wordsize)
	{
		return;
	}
}