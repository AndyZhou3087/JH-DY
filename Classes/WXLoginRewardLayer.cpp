#include "WXLoginRewardLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "Const.h"
#include "StorageRoom.h"

WXLoginRewardLayer::WXLoginRewardLayer()
{

}


WXLoginRewardLayer::~WXLoginRewardLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}
void WXLoginRewardLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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
bool WXLoginRewardLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("loginRewardLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *closebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(WXLoginRewardLayer::onClose, this));

	particle = (cocos2d::ui::Widget*)csbnode->getChildByName("particle");

	int logindays = GlobalData::continueLoginDays;


	for (int i = 1; i<= 7; i++)
	{
		std::string resid = LOGINREWARDS[i - 1];
		int res = atoi(resid.c_str());
		int count = 1;

		if (res > 0)
		{
			resid = StringUtils::format("%d", res / 1000);
			count = res % 1000;
		}

		std::string str = StringUtils::format("item%d", i);
		cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)csbnode->getChildByName(str);

		cocos2d::ui::ImageView* getimg = (cocos2d::ui::ImageView*)item->getChildByName("getimg");
		getimg->addTouchEventListener(CC_CALLBACK_2(WXLoginRewardLayer::onGet, this));
		getimg->setVisible(true);

		cocos2d::ui::Text* statutext = (cocos2d::ui::Text*)item->getChildByName("statutext");

		cocos2d::ui::Widget* loginritemlight = (cocos2d::ui::Widget*)item->getChildByName("loginritemlight");

		//cocos2d::ui::ImageView* rwdboximg = (cocos2d::ui::ImageView*)item->getChildByName("rwdboximg");
		//cocos2d::ui::ImageView* rwdimg = (cocos2d::ui::ImageView*)item->getChildByName("rwdimg");

		cocos2d::ui::Text* name = (cocos2d::ui::Text*)item->getChildByName("name");

		std::string namsstr;
		if (resid.compare(0, 1, "g") == 0)
		{
			count = atoi(resid.substr(1).c_str());
			namsstr = StringUtils::format("%sx%d", CommonFuncs::gbk2utf("金元宝").c_str(), count);
			resid = "gd0";
		}

		else
			namsstr = StringUtils::format("%sx%d", GlobalData::map_allResource[resid].cname.c_str(), count);
		name->setString(namsstr.c_str());

		str = "ui/buildsmall.png";
		int type = GlobalData::getResType(resid);
		if (type == WEAPON || type == PROTECT_EQU)
		{
			str = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[resid].qu);
		}
		else if (type == N_GONG || type == W_GONG)
		{
			str = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[resid].qu);
		}

		/*Sprite * rwdboximg = Sprite::createWithSpriteFrameName(str);
		rwdboximg->setPosition(Vec2(72, 130));
		item->addChild(rwdboximg, 0, "rwdboximg");*/

		str = StringUtils::format("ui/%s.png", resid.c_str());
		Sprite * rwdimg = Sprite::createWithSpriteFrameName(str);
		rwdimg->setScale(0.8);
		rwdimg->setPosition(Vec2(80, 130));
		item->addChild(rwdimg, 0, "rwdimg");

		if (i < logindays)
		{
			item->setColor(Color3B(200, 200, 200));
			loginritemlight->setVisible(false);
			//item->loadTexture("ui/loginritem0.png", cocos2d::ui::Widget::TextureResType::PLIST);
			getimg->loadTexture("ui/buildtagbtn2.png", cocos2d::ui::Widget::TextureResType::PLIST);
			getimg->setEnabled(false);
			//statutext->setString(CommonFuncs::gbk2utf("已领取"));
			//CommonFuncs::changeGray(rwdboximg);
			CommonFuncs::changeGray(rwdimg);
		}
		else if (i == logindays)
		{
			particle->setPosition(Vec2(item->getPositionX(), item->getPositionY() - 10));
			loginritemlight->setVisible(true);
			item->setColor(Color3B(255, 255, 255));
			//item->loadTexture("ui/loginritem1.png", cocos2d::ui::Widget::TextureResType::PLIST);
			getimg->loadTexture("ui/buildtagbtn1.png", cocos2d::ui::Widget::TextureResType::PLIST);
			//statutext->setString(CommonFuncs::gbk2utf("领取"));
		}
		else
		{
			getimg->setVisible(false);
			loginritemlight->setVisible(false);
			item->setColor(Color3B(255, 255, 255));
			getimg->setEnabled(false);
		}

		const std::string cndays[] = { "一", "二", "三", "四", "五", "六", "七" };
		str = StringUtils::format("第%s天", cndays[i - 1].c_str());
		statutext->setString(CommonFuncs::gbk2utf(str.c_str()));
	}

	GlobalData::g_gameStatus = GAMEPAUSE;
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
void WXLoginRewardLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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

WXLoginRewardLayer* WXLoginRewardLayer::create()
{
	WXLoginRewardLayer *pRet = new WXLoginRewardLayer();
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
void WXLoginRewardLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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

void WXLoginRewardLayer::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::ImageView* getimg = (cocos2d::ui::ImageView*)pSender;

		particle->setVisible(false);

		cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)getimg->getParent();
		//item->loadTexture("ui/loginritem0.png", cocos2d::ui::Widget::TextureResType::PLIST);
		item->setColor(Color3B(200, 200, 200));
		cocos2d::ui::Widget* loginritemlight = (cocos2d::ui::Widget*)item->getChildByName("loginritemlight");
		loginritemlight->setVisible(false);
		getimg->loadTexture("ui/buildtagbtn2.png", cocos2d::ui::Widget::TextureResType::PLIST);
		getimg->setEnabled(false);
		//cocos2d::ui::Text* statutext = (cocos2d::ui::Text*)getimg->getChildByName("statutext");
		//statutext->setString(CommonFuncs::gbk2utf("已领取"));
		//CommonFuncs::changeGray(item->getChildByName("rwdboximg"));
		CommonFuncs::changeGray(item->getChildByName("rwdimg"));

		std::string resid = LOGINREWARDS[GlobalData::continueLoginDays - 1];
		int res = atoi(resid.c_str());
		int count = 1;
		if (res > 0)
		{
			resid = StringUtils::format("%d", res / 1000);
			count = res % 1000;
		}
		if (resid.compare(0, 1, "g") == 0)
		{
			count = atoi(resid.substr(1).c_str());
			GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() + count);

		}
		else
		{
			PackageData pdata;
			pdata.strid = resid;
			pdata.count = count;
			pdata.type = GlobalData::getResType(resid);
			pdata.extype = GlobalData::getResExType(resid);
			StorageRoom::add(pdata);
		}

		GlobalData::continueLoginDays = 0;

	}
}
void WXLoginRewardLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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
				DelayTime::create(20*0.1),
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
				DelayTime::create(30*0.1),
				action,
				NULL));
		}
		else {
			node->runAction(action);
		}
	}
}
void WXLoginRewardLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}