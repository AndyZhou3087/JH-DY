﻿#include "WXHelpDescLayer.h"
#include "CommonFuncs.h"

WXHelpDescLayer::WXHelpDescLayer()
{
	
}


WXHelpDescLayer::~WXHelpDescLayer()
{
	
}

bool WXHelpDescLayer::init( std::string title, std::string text)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("helpDescLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Text* titleTxt = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	titleTxt->setString(title);

	cocos2d::ui::Text* contentTxt = (cocos2d::ui::Text*) csbnode->getChildByName("content");
	contentTxt->setString(text);

	cocos2d::ui::Widget *okbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(WXHelpDescLayer::onOk, this));

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

WXHelpDescLayer* WXHelpDescLayer::create(std::string title, std::string text)
{
	WXHelpDescLayer *pRet = new WXHelpDescLayer();
	if (pRet && pRet->init(title, text))
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

void WXHelpDescLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}