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

bool DeathLayer::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
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

void DeathLayer::jumpDown(cocos2d::Node *node, float dt) {
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