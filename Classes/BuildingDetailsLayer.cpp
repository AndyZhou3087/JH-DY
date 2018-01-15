#include "BuildingDetailsLayer.h"
#include "SoundManager.h"
#include "CommonFuncs.h"

BuildingDetailsLayer::BuildingDetailsLayer()
{

}

BuildingDetailsLayer::~BuildingDetailsLayer()
{

}

bool BuildingDetailsLayer::init(Building* pdata)
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	Node* csbnode = CSLoader::createNode("buildingDetailsLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(BuildingDetailsLayer::onOk, this));


	cocos2d::ui::ImageView* resimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("buildsmall")->getChildByName("Image");

	std::string str = StringUtils::format("ui/s%s.png", pdata->data.name);
	resimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	resimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	cocos2d::ui::Text* buildingname = (cocos2d::ui::Text*)csbnode->getChildByName("namelbl");

	buildingname->setString(pdata->data.cname);

	cocos2d::ui::Text* desclbl = (cocos2d::ui::Text*)csbnode->getChildByName("desclbl");
	desclbl->setString(pdata->data.desc);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		removSelf();
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

BuildingDetailsLayer* BuildingDetailsLayer::create(Building* pdata)
{
	BuildingDetailsLayer *pRet = new BuildingDetailsLayer();
	if (pRet && pRet->init(pdata))
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

void BuildingDetailsLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void BuildingDetailsLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

