﻿#include "WXChallengeCountLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "SoundManager.h"
#include "HintBox.h"
#include "WXFightLayer.h"
#include "HeroStateUILayer.h"

int usecount[] = { 1, 1, 3, 3, 5, 5, 7, 7, 8, 8, 10, 10, 13, 13, 15, 15, 18, 18, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 };
WXChallengeCountLayer::WXChallengeCountLayer()
{
	lastclickindex = -1;
}


WXChallengeCountLayer::~WXChallengeCountLayer()
{
}

bool WXChallengeCountLayer::init(int* wincount, int winnpccount, bool isRevive)
{
	m_wincount = wincount;
	m_winnpcount = winnpccount;
	m_isRevive = isRevive;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("challengeCountLayer.csb");
	this->addChild(csbnode);

	/*cocos2d::ui::ImageView* heroimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroimg");
	heroimg->addTouchEventListener(CC_CALLBACK_2(WXChallengeCountLayer::onHeroimg, this));
	std::string heroidstr = StringUtils::format("ui/tophero%d.png", g_hero->getHeadID());
	heroimg->loadTexture(heroidstr, cocos2d::ui::TextureResType::PLIST);
	heroimg->setContentSize(Sprite::createWithSpriteFrameName(heroidstr)->getContentSize());*/

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(WXChallengeCountLayer::onCancel, this));

	cocos2d::ui::Button* continuebtn = (cocos2d::ui::Button*)csbnode->getChildByName("continuebtn");
	continuebtn->addTouchEventListener(CC_CALLBACK_2(WXChallengeCountLayer::onContinue, this));

	totalwincountlbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("wincountlbl");

	std::string str = StringUtils::format("%d", *m_wincount);
	totalwincountlbl->setString(str);

	for (int i = 0; i < 3; i++)
	{
		str = StringUtils::format("btn%d", i);
		selectbtn[i] = (cocos2d::ui::Button*)csbnode->getChildByName(str);
		selectbtn[i]->setTag(i);
		selectbtn[i]->addTouchEventListener(CC_CALLBACK_2(WXChallengeCountLayer::onSelect, this));
		/*valuelbl[i] = (cocos2d::ui::Text*)selectbtn[i]->getChildByName("valuelbl");
		str = StringUtils::format("%d%%", winnpccount / 3);
		valuelbl[i]->setString(str);*/

		str = StringUtils::format("usesprite%d",i);

		cocos2d::ui::Widget* usespite = (cocos2d::ui::Widget*)csbnode->getChildByName(str);

		cocos2d::ui::Text* uselbl = (cocos2d::ui::Text*)usespite->getChildByName("neednum");
		str = StringUtils::format("胜点%d", usecount[winnpccount / 3 - 1]);
		uselbl->setString(CommonFuncs::gbk2utf(str.c_str()));
		if (*m_wincount < usecount[winnpccount / 3 - 1])
			uselbl->setTextColor(Color4B(204,4,4,255));


		cocos2d::ui::Widget* particle = (cocos2d::ui::Widget*)selectbtn[i]->getChildByName("particle");
		particle->setVisible(false);
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void WXChallengeCountLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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

WXChallengeCountLayer* WXChallengeCountLayer::create(int* wincount, int winnpccount, bool isRevive)
{
	WXChallengeCountLayer *pRet = new WXChallengeCountLayer();
	if (pRet && pRet->init(wincount, winnpccount, isRevive))
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

void WXChallengeCountLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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

void WXChallengeCountLayer::onHeroimg(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		g_gameLayer->addChild(HeroStateUILayer::create(), 4, "HeroStateUILayer");
	}
}


void WXChallengeCountLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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

void WXChallengeCountLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		g_hero->setIsWDChallenge(false);
		this->removeFromParentAndCleanup(true);

	}
}

void WXChallengeCountLayer::onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (lastclickindex == 0)
		{
			float val = g_hero->getLifeValue();
			float addval = g_hero->getMaxLifeValue() * m_winnpcount / 300;
			if (val + addval > g_hero->getMaxLifeValue())
				g_hero->setLifeValue(g_hero->getMaxLifeValue());
			else
				g_hero->setLifeValue(val+addval);
		}
		else if (lastclickindex == 1)
		{
			float lastbonus = g_hero->getTotalAtkBonusPercent();
			float bonus = lastbonus + m_winnpcount * 1.0f / 300.0f;
			g_hero->setTotalAtkBonusPercent(bonus);

		}
		else if (lastclickindex == 2)
		{
			float lastbonus = g_hero->getTotalDfBonusPercent();
			float bonus = lastbonus + m_winnpcount *1.0f / 300.0f;
			g_hero->setTotalDfBonusPercent(bonus);
		}
		WXFightLayer* figherlayer = (WXFightLayer*)g_gameLayer->getChildByName("fightlayer");
		if (!m_isRevive)
			figherlayer->updateFightNextNpc();
		else
			figherlayer->reviveContinueChallege();
		this->removeFromParentAndCleanup(true);
	}
}

void WXChallengeCountLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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
				DelayTime::create(10*0.1),
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
				DelayTime::create(5*0.1),
				action,
				NULL));
		}
		else {
			node->runAction(action);
		}
	}
}

void WXChallengeCountLayer::onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btnnode = (cocos2d::ui::Button*)pSender;

		int tag = btnnode->getTag();
		if (tag == lastclickindex)
		{
			cocos2d::ui::Widget* particle = (cocos2d::ui::Widget*)btnnode->getChildByName("particle");
			particle->setVisible(true);
			btnnode->setBright(true);
			lastclickindex = -1;
			*m_wincount += usecount[m_winnpcount / 3 - 1];
		}
		else
		{

			if (*m_wincount < usecount[m_winnpcount / 3 - 1])
			{
				HintBox* hintBox = HintBox::create(CommonFuncs::gbk2utf("胜点不足！!"));
				this->addChild(hintBox);
				return;
			}
			if (lastclickindex >= 0)
			{
				selectbtn[lastclickindex]->setBright(true);
			}
			else
			{
				*m_wincount -= usecount[m_winnpcount / 3 - 1];
			}
			selectbtn[tag]->setBright(false);
			lastclickindex = tag;

			cocos2d::ui::Widget* particle = (cocos2d::ui::Widget*)selectbtn[tag]->getChildByName("particle");
			particle->setVisible(false);

		}
		std::string str = StringUtils::format("%d", *m_wincount);
		totalwincountlbl->setString(str);
	}
}