#include "StoryScene.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "GameDataSave.h"

std::string storytype[] = { "story", "end" };
StoryScene::StoryScene()
{
	m_wordlbl = NULL;
	isCanClick = false;
}


StoryScene::~StoryScene()
{
}

Scene* StoryScene::createScene(int type)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = StoryScene::create(type);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool StoryScene::init(int type)
{
	clickcount = 0;
	m_type = type;
	m_heroindex = GameDataSave::getInstance()->getHeroId();
	std::string storystr = StringUtils::format("%s%d_1.csb", storytype[type].c_str(), m_heroindex);
	m_csbnode = CSLoader::createNode(storystr);
	if (m_heroindex == 1 && type == 0)
		m_csbnode->setPosition(Vec2(360, 640));
	this->addChild(m_csbnode);

	m_action = CSLoader::createTimeline(storystr);
	m_csbnode->runAction(m_action);
	m_action->gotoFrameAndPlay(0, false);

	float delaytime = m_action->getEndFrame()*1.0f / (m_action->getTimeSpeed() * 60);
	this->scheduleOnce(schedule_selector(StoryScene::showClickText), delaytime + 1.0f);

		////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		if (!isCanClick)
		{
			this->unschedule(schedule_selector(StoryScene::showClickText));
			showClickText(0);
			m_action->gotoFrameAndPlay(m_action->getEndFrame(), false);
			return true;
		}
		if (clickcount == 1)
		{
			isCanClick = false;
			this->scheduleOnce(schedule_selector(StoryScene::delayShowNextStory), 0.1f);
		}
		else if (clickcount == 2)
		{
			isCanClick = false;
			this->scheduleOnce(schedule_selector(StoryScene::showNextScene), 0.1f);
		}
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
	return true;
}

StoryScene* StoryScene::create(int type)
{
	StoryScene *pRet = new StoryScene();
	if (pRet && pRet->init(type))
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

void StoryScene::delayShowNextStory(float dt)
{
	m_csbnode->removeFromParentAndCleanup(true);
	if (m_wordlbl != NULL)
		m_wordlbl->removeFromParentAndCleanup(true);

	std::string storystr = StringUtils::format("%s%d_2.csb", storytype[m_type].c_str(), m_heroindex);

	m_csbnode = CSLoader::createNode(storystr);
	if (m_heroindex == 1 && m_type == 0)
		m_csbnode->setPosition(Vec2(360, 640));

	this->addChild(m_csbnode);

	m_action = CSLoader::createTimeline(storystr);
	m_csbnode->runAction(m_action);
	m_action->gotoFrameAndPlay(0, false);
	float delaytime = m_action->getEndFrame()*1.0f / (m_action->getTimeSpeed() * 60);
	this->scheduleOnce(schedule_selector(StoryScene::showClickText), delaytime + 1.0f);
}

void StoryScene::showClickText(float dt)
{
	m_wordlbl = Label::createWithTTF(CommonFuncs::gbk2utf("点击屏幕继续"), "fonts/STXINGKA.TTF", 30);
	m_wordlbl->setPosition(Vec2(360, 20));
	m_wordlbl->runAction(RepeatForever::create(Blink::create(2, 2)));
	this->addChild(m_wordlbl);
	clickcount++;
	isCanClick = true;
}
void StoryScene::showNextScene(float dt)
{
	if (m_type == 0)
	{
		auto transition = TransitionCrossFade::create(0.5f, GameScene::createScene());
		Director::getInstance()->replaceScene(transition);
	}
	else
	{
		Director::getInstance()->popScene();
	}
}