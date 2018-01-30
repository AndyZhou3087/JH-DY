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


void FactionDetailsLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
	if (nullptr == node) {
		return;
	}

	ActionInterval * action = Sequence::create(
		ScaleTo::create(0.2, 1.05, 0.95, 1),
		Spawn::create(
		EaseExponentialOut::create(ScaleTo::create(0.1, 0.95, 1.05, 1)),
		MoveBy::create(0.2, Vec2(0, dt)),
		ac,
		NULL),
		Spawn::create(
		EaseExponentialIn::create(ScaleTo::create(0.1, 1.1, 0.95, 1)),
		MoveBy::create(0.2, Vec2(0, -dt)),
		NULL),
		ScaleTo::create(0.1, 0.98, 1.08, 1),
		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	if (repeat) {
		if (0 != tag) {
			action->setTag(tag);
		}

		node->runAction(RepeatForever::create(
			Sequence::create(
			action,
			DelayTime::create(intrval),
			NULL)
			));
	}
	else {
		node->runAction(action);
	}
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


bool FactionDetailsLayer::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
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


void FactionDetailsLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
	if (nullptr == node) {
		return;
	}

	ActionInterval * action = Sequence::create(
		EaseSineIn::create(ScaleTo::create(0.08, 0.95, 1.05, 1)),
		EaseSineOut::create(ScaleTo::create(0.2, 1.15, 0.95, 1)),
		ScaleTo::create(0.1, 0.98, 1.08, 1),
		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 0.98, 1.08, 1),
		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	if (repeat) {
		if (0 != tag) {
			action->setTag(tag);
		}
		if (delay) {
			node->runAction(RepeatForever::create(
				Sequence::create(
				DelayTime::create(9*0.1),
				action,
				DelayTime::create(intrval),
				NULL)
				));
		}
		else {
			node->runAction(RepeatForever::create(
				Sequence::create(
				action,
				DelayTime::create(intrval),
				NULL)
				));
		}

	}
	else {
		if (delay) {
			node->runAction(Sequence::create(
				DelayTime::create(7*0.1),
				action,
				NULL));
		}
		else {
			node->runAction(action);
		}
	}
}


void FactionDetailsLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}


void FactionDetailsLayer::jumpDown(cocos2d::Node *node, float dt) {
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
