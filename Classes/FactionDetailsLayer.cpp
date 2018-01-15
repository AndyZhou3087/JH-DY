#include "FactionDetailsLayer.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "CommonFuncs.h"

std::string desc = "1.帮派解散规则：\
\n帮主只可以解散帮派。\
\n解散帮派需要消耗50元宝。\n\
\n2.帮派等级规则：\
\n创建帮派初始为1级。\
\n帮派等级最高为5级。\
\n1级帮派人数限制为20人。\
\n帮派等级每提升一级，人数上限增加5人。\n\
\n3.帮派经验值获取：\
\n通过会员贡献资源获取经验值。\
\n可贡献的资源类型：银两、元宝。\
\n10银两可获得1点经验值。\
\n10元宝可获得10点经验值。\
\n单个用户每日最多只可贡献100银两、100元宝。\n\
\n4.帮派会员等级：\
\n帮主：每个帮派只可有1个帮主。\
\n副帮主：1级帮会最多只可有3个副帮主，帮派等级每提升1级可增加1个副帮主。\
\n长老：1级帮会最多只可有5个长老，帮派等级每提升1级可增加2个长老。\
\n帮众：不限制。\
\n贡献度：帮众每次捐赠都可获得贡献度，帮主可根据帮众的贡献度来进行会员等级的配置。\n\
\n5.贡献度获得：\
\n捐赠1银两可获得1点贡献度。\
\n捐赠1元宝可获得10点贡献度。";
FactionDetailsLayer::FactionDetailsLayer()
{
}


FactionDetailsLayer::~FactionDetailsLayer()
{

}

FactionDetailsLayer* FactionDetailsLayer::create()
{
	FactionDetailsLayer *pRet = new FactionDetailsLayer();
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

bool FactionDetailsLayer::init()
{
	//蒙版
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	//加载csb文件
	Node* csbnode = CSLoader::createNode("factionDetailsLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(FactionDetailsLayer::onClose, this));


	cocos2d::ui::ScrollView* scrollview = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");
	scrollview->setScrollBarEnabled(false);
	scrollview->setBounceEnabled(true);

	Label* contentlbl = Label::createWithTTF(CommonFuncs::gbk2utf(desc.c_str()), "fonts/STXINGKA.TTF", 30);
	contentlbl->setAnchorPoint(Vec2(0, 1));
	contentlbl->setColor(Color3B(0, 0, 0));
	contentlbl->setHorizontalAlignment(TextHAlignment::LEFT);
	contentlbl->setLineBreakWithoutSpace(true);
	contentlbl->setMaxLineWidth(scrollview->getContentSize().width);
	scrollview->addChild(contentlbl);
	int innerheight = contentlbl->getStringNumLines() * 31;//contentlbl->getHeight();
	int contentheight = scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollview->setInnerContainerSize(Size(scrollview->getContentSize().width, innerheight));

	contentlbl->setPosition(Vec2(0, innerheight));

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


void FactionDetailsLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
