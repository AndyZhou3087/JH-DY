#include "HelpDescLayer.h"
#include "CommonFuncs.h"

HelpDescLayer::HelpDescLayer()
{
	
}


HelpDescLayer::~HelpDescLayer()
{
	
}

bool HelpDescLayer::init( std::string title, std::string text)
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
	okbtn->addTouchEventListener(CC_CALLBACK_2(HelpDescLayer::onOk, this));

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

HelpDescLayer* HelpDescLayer::create(std::string title, std::string text)
{
	HelpDescLayer *pRet = new HelpDescLayer();
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

void HelpDescLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}