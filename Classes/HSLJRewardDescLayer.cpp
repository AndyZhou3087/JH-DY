#include "HSLJRewardDescLayer.h"
#include "SoundManager.h"
#include "CommonFuncs.h"
HSLJRewardDescLayer::HSLJRewardDescLayer()
{
}


HSLJRewardDescLayer::~HSLJRewardDescLayer()
{

}

HSLJRewardDescLayer* HSLJRewardDescLayer::create()
{
	HSLJRewardDescLayer *pRet = new HSLJRewardDescLayer();
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

bool HSLJRewardDescLayer::init()
{
	//蒙版
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	//加载csb文件
	Node* csbnode = CSLoader::createNode("HSLJRewardDescLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(HSLJRewardDescLayer::onClose, this));

	//layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}


void HSLJRewardDescLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
