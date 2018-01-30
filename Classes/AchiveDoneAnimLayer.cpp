#include "AchiveDoneAnimLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "GameScene.h"
bool AchiveDoneAnimLayer::init()
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	//蒙版
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	showAchiveDoneAnim(0);
	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void AchiveDoneAnimLayer::initA()
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

AchiveDoneAnimLayer* AchiveDoneAnimLayer::create()
{
	AchiveDoneAnimLayer *pRet = new AchiveDoneAnimLayer();
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

void AchiveDoneAnimLayer::showAchiveDoneAnim(float dt)
{
	this->removeChildByName("achiveanim");
	if (GlobalData::vec_showAchiveNames.size() <= 0)
	{
		GlobalData::saveAchiveAnimData();
		if (g_gameLayer != NULL)
			g_gameLayer->schedule(schedule_selector(GameScene::checkAchiveIsDone), 1.0f);
		removSelf();
		return;
	}

	Node* csbnode = CSLoader::createNode("achiveNodeAnim.csb");
	csbnode->setPosition(Vec2(360, 720));
	this->addChild(csbnode, 0, "achiveanim");
	cocos2d::ui::Text* textname = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	textname->setString(GlobalData::vec_showAchiveNames[0]);
	auto action = CSLoader::createTimeline("achiveNodeAnim.csb");
	csbnode->runAction(action);
	action->gotoFrameAndPlay(0, false);

	GlobalData::vec_showAchiveNames.erase(GlobalData::vec_showAchiveNames.begin());
	float delaytime = action->getEndFrame()*1.0f / (action->getTimeSpeed() * 60);
	this->schedule(schedule_selector(AchiveDoneAnimLayer::showAchiveDoneAnim), delaytime, GlobalData::vec_showAchiveNames.size(), 0);
}

void AchiveDoneAnimLayer::initBg()
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

void AchiveDoneAnimLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

