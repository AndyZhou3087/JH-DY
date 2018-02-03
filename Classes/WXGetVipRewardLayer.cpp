#include "WXGetVipRewardLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "Const.h"
#include "WXGoldGoodsItem.h"
#include "WaitingProgress.h"
#include "WXLoginRewardLayer.h"
#include "GameScene.h"

WXGetVipRewardLayer::WXGetVipRewardLayer()
{
	rgoldcount = 0;
}


WXGetVipRewardLayer::~WXGetVipRewardLayer()
{

}
bool WXGetVipRewardLayer::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
}
bool WXGetVipRewardLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("getVipRewardLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(WXGetVipRewardLayer::onClose, this));

	std::string namestr;

	int golditemcount = sizeof(goldcount) / sizeof(goldcount[0]);

	std::vector<std::string> vec_rewardres;
	for (unsigned int i = 0; i < GlobalData::vec_buyVipIds.size(); i++)
	{
		for (unsigned int m = 0; m < GlobalData::vec_goods.size(); m++)
		{
			std::string vipid = GlobalData::vec_goods[m].icon;
			if (GlobalData::vec_buyVipIds[i].compare(vipid) == 0)
			{
				vec_rgoodsindex.push_back(m);
				namestr.append(GlobalData::vec_goods[m].name);
				namestr.append(CommonFuncs::gbk2utf("、"));
				rgoldcount += vipgoldcount[m - golditemcount];

				for (unsigned int n = 0; n < GlobalData::vec_goods[m].vec_res.size(); n++)
				{
					int intres = atoi(GlobalData::vec_goods[m].vec_res[n].c_str());
					int intid = intres / 1000;
					int count = intres % 1000;
					bool isfind = false;
					for (unsigned int j = 0; j < vec_rewardres.size(); j++)
					{
						int intres1 = atoi(vec_rewardres[j].c_str());
						int intid1 = intres1 / 1000;
						int count1 = intres1 % 1000;
						if (intid == intid1)
						{
							std::string newstr = StringUtils::format("%d", intid * 1000 + count + count1);
							vec_rewardres[j] = newstr;
							isfind = true;
							break;
						}
					}
					if (!isfind)
					{
						vec_rewardres.push_back(GlobalData::vec_goods[m].vec_res[n]);
					}
				}
			}
		}
	}
	namestr = namestr.substr(0, namestr.length() - 3);

	std::string descstr = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("获取").c_str(), namestr.c_str(), CommonFuncs::gbk2utf("福利：").c_str());
	cocos2d::ui::Text* desctext = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desctext->setString(descstr);

	int startx = 145;
	int spacex = 110;
	int starty = 680;
	if (vec_rewardres.size() == 2)
	{
		startx = 160;
		spacex = 200;
	}

	int ressize = vec_rewardres.size() + 1;
	for (int i = 0; i < ressize; i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");
		box->setScale(0.8);
		box->setPosition(Vec2(startx + i*spacex, starty));
		this->addChild(box);


		std::string resstr;
		std::string strcount;
		std::string namstr;
		if (i == 0)
		{
			resstr = "ui/gd0.png";
			strcount = StringUtils::format("x%d", rgoldcount);
			namstr = CommonFuncs::gbk2utf("金元宝");
		}
		else
		{
			std::string resid = vec_rewardres[i - 1];
			int intresid = atoi(resid.c_str());
			resstr = StringUtils::format("ui/%d.png", intresid / 1000);
			strcount = StringUtils::format("x%d", intresid % 1000);
			std::string ridstr = StringUtils::format("%d", intresid / 1000);
			namstr = GlobalData::map_allResource[ridstr].cname;
		}
		Sprite* res = Sprite::createWithSpriteFrameName(resstr);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().width / 2 + 10));
		box->addChild(res);

		Label * coutlbl = Label::createWithTTF(strcount, "fonts/STXINGKA.TTF", 25);//Label::createWithSystemFont(strcount, "", 25);
		//coutlbl->enableOutline(Color4B(0, 0, 0, 255), 3);
		coutlbl->setAnchorPoint(Vec2(0.5, 0.5));
		coutlbl->setColor(Color3B(0, 0, 0));
		coutlbl->setPosition(Vec2(box->getPositionX(), 580));
		this->addChild(coutlbl);

		Label * namelbl = Label::createWithTTF(namstr, "fonts/STXINGKA.TTF", 26);
		namelbl->setColor(Color3B(0, 0, 0));
		namelbl->setPosition(Vec2(box->getPositionX(), 610));
		this->addChild(namelbl);
	}
	WaitingProgress* waitbox = WaitingProgress::create("获取月卡中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->isGetVip(GlobalData::vec_buyVipIds);
	GlobalData::vec_buyVipIds.clear();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void WXGetVipRewardLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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

WXGetVipRewardLayer* WXGetVipRewardLayer::create()
{
	WXGetVipRewardLayer *pRet = new WXGetVipRewardLayer();
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


void WXGetVipRewardLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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

void WXGetVipRewardLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);

		if (GlobalData::continueLoginDays > 0)
		{
			WXLoginRewardLayer* llayer = WXLoginRewardLayer::create();
			g_gameLayer->addChild(llayer, 100);
		}
	}
}

void WXGetVipRewardLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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

void WXGetVipRewardLayer::getVipGoods()
{
	GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() + rgoldcount);

	for (unsigned int m = 0; m < vec_rgoodsindex.size();m++)
		WXGoldGoodsItem::addBuyGoods(&GlobalData::vec_goods[vec_rgoodsindex[m]]);

}
void WXGetVipRewardLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	getVipGoods();
}

void WXGetVipRewardLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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
				DelayTime::create(50*0.1),
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

void WXGetVipRewardLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	this->removeFromParentAndCleanup(true);
}


void WXGetVipRewardLayer::jumpDown(cocos2d::Node *node, float dt) {
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