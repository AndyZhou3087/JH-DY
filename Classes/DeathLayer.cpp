#include "DeathLayer.h"
#include "CommonFuncs.h"
#include "StartScene.h"
#include "SoundManager.h"
#include "Const.h"
#include "AnalyticUtil.h"
#include "GameScene.h"
DeathLayer::DeathLayer()
{

}


DeathLayer::~DeathLayer()
{

}

bool DeathLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 255));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("deathLayer.csb");
	this->addChild(csbnode);

	csbnode->runAction(FadeIn::create(3.0f));

	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		Director::getInstance()->replaceScene(StartScene::createScene());
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_DEATH);

#ifdef ANALYTICS
	std::string pastdaystr = StringUtils::format("pastday%d", g_nature->getPastDays());
	AnalyticUtil::onEvent(pastdaystr.c_str());
#endif

	return true;
}

DeathLayer* DeathLayer::create()
{
	DeathLayer *pRet = new DeathLayer();
	if (pRet && pRet->init())
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