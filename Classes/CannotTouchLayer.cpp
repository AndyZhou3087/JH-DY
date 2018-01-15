#include "CannotTouchLayer.h"
CannotTouchLayer::CannotTouchLayer()
{

}


CannotTouchLayer::~CannotTouchLayer()
{

}

bool CannotTouchLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 0));
	this->addChild(color);


	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

CannotTouchLayer* CannotTouchLayer::create()
{
	CannotTouchLayer *pRet = new CannotTouchLayer();
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