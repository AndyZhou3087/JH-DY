#include "WXFactionPromotionLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "WaitingProgress.h"
#include "HintBox.h"
#include "WXFactionMemberLayer.h"

WXFactionPromotionLayer::WXFactionPromotionLayer()
{
	m_select = -1;
}


WXFactionPromotionLayer::~WXFactionPromotionLayer()
{

}

void WXFactionPromotionLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
	if (nullptr == node) {
		return;
	}

	ActionInterval * action = Sequence::create(
		ScaleTo::create(0.2, 1.1, 0.9, 1),
		Spawn::create(
		EaseExponentialOut::create(ScaleTo::create(0.1, 0.9, 1.1, 1)),
		MoveBy::create(0.2, Vec2(0, dt)),
		NULL),
		Spawn::create(
		EaseExponentialIn::create(ScaleTo::create(0.1, 1.2, 0.9, 1)),
		MoveBy::create(0.2, Vec2(0, -dt)),
		NULL),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	if (repeat) {
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


WXFactionPromotionLayer* WXFactionPromotionLayer::create(FactionMemberData *data, Node* target)
{
	WXFactionPromotionLayer *pRet = new WXFactionPromotionLayer();
	if (pRet && pRet->init(data, target))
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

void WXFactionPromotionLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
	if (nullptr == node) {
		return;
	}

	ActionInterval * action = Sequence::create(
		ScaleTo::create(0.2, 1.1, 0.9, 1),
		Spawn::create(
		EaseExponentialOut::create(ScaleTo::create(0.1, 0.9, 1.1, 1)),
		MoveBy::create(0.2, Vec2(0, dt)),
		NULL),
		Spawn::create(
		EaseExponentialIn::create(ScaleTo::create(0.1, 1.2, 0.9, 1)),
		MoveBy::create(0.2, Vec2(0, -dt)),
		NULL),
		ScaleTo::create(0.1, 0.95, 1.05, 1),
		ScaleTo::create(0.1, 1.05, 0.95, 1),
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


bool WXFactionPromotionLayer::init(FactionMemberData *data, Node* target)
{
	//蒙版
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	//加载csb文件
	Node* csbnode = CSLoader::createNode("promotionLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(WXFactionPromotionLayer::onOk, this));

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(WXFactionPromotionLayer::onBack, this));

	//checkbox

	for (int i = 0; i < 3; i++)
	{
		std::string str = StringUtils::format("check%d", i + 1);
		m_checkBox[i] = (cocos2d::ui::CheckBox*)csbnode->getChildByName(str);
		m_checkBox[i]->addEventListener(CC_CALLBACK_2(WXFactionPromotionLayer::checkBoxCallback, this));
		m_checkBox[i]->setTag(i + 2);
	}
	m_target = target;
	m_memberdata = data;
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

void WXFactionPromotionLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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

void WXFactionPromotionLayer::checkBoxCallback(cocos2d::Ref* pSender, cocos2d::ui::CheckBox::EventType type)
{
	Node* checkbtn = (Node*)pSender;
	int tag = checkbtn->getTag();
	switch(type)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		case cocos2d::ui::CheckBox::EventType::SELECTED://选中
			for (int i = 0; i < 3; i++)
			{
				if (i != tag - 2)
				{
					m_checkBox[i]->setSelected(false);
				}
				else
				{
					m_select = tag;
				}
			}
			break;
		case cocos2d::ui::CheckBox::EventType::UNSELECTED://不选中
			m_select = -1;
			break;
		default:
			break;
	}
}

void WXFactionPromotionLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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
				DelayTime::create(32*0.1),
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
				DelayTime::create(100*0.1),
				action,
				NULL));
		}
		else {
			node->runAction(action);
		}
	}
}


void WXFactionPromotionLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if (m_select == m_memberdata->position || m_select < 0)
		{
			this->removeFromParentAndCleanup(true);
			return;
		}
		WaitingProgress* waitbox = WaitingProgress::create("处理中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->promotionFaction(m_memberdata->factionid, m_memberdata->userid, m_memberdata->herotype, m_select);
	}
}

void WXFactionPromotionLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void WXFactionPromotionLayer::onSuccess()
{
	FactionMemberItem* mbitem = (FactionMemberItem*)m_target;
	mbitem->updatePosition(m_select);
	this->removeFromParentAndCleanup(true);
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void WXFactionPromotionLayer::jumpDown(cocos2d::Node *node, float dt) {
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

void WXFactionPromotionLayer::onErr(int errcode)
{
    Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox* hintbox;
	if (errcode == -4)
	{
		if (m_select == 2)
			hintbox = HintBox::create(CommonFuncs::gbk2utf("任命失败！ 副帮主人数已超限，可提升帮派等级增加副帮主人数！"));
		else
			hintbox = HintBox::create(CommonFuncs::gbk2utf("任命失败！ 长老人数已超限，可提升帮派等级增加长老人数！"));
	}
	else
	{
		hintbox = HintBox::create(CommonFuncs::gbk2utf("任命失败！请检查网络连接或者稍后重试！"));
	}
	this->addChild(hintbox);
}
