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

void AchiveDoneAnimLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

