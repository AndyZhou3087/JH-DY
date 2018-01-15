#include "MixGFDoneLayer.h"
#include "GlobalData.h"
#include "Const.h"
MixGFDoneLayer::MixGFDoneLayer()
{

}


MixGFDoneLayer::~MixGFDoneLayer()
{
}

bool MixGFDoneLayer::init(int ret)
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("mixDoneLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::ImageView* doneimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("doneimg");
	std::string str;
	if (ret == 0)
		str = StringUtils::format("images/mixdoneok.png");
	else
		str = StringUtils::format("images/mixdonefail.png");
	doneimg->loadTexture(str, cocos2d::ui::Widget::TextureResType::LOCAL);

	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleOnce(schedule_selector(MixGFDoneLayer::removeself), 3.0f);

	return true;
}

MixGFDoneLayer* MixGFDoneLayer::create(int ret)
{
	MixGFDoneLayer *pRet = new MixGFDoneLayer();
	if (pRet && pRet->init(ret))
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

void MixGFDoneLayer::removeself(float dt)
{
	this->removeFromParentAndCleanup(true);
}