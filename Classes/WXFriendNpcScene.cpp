#include "WXFriendNpcScene.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "GameScene.h"
#include "WXFightLayer.h"

WXFriendNpcScene::WXFriendNpcScene()
{

}

WXFriendNpcScene::~WXFriendNpcScene()
{
	GlobalData::isPopingScene = false;
}

Scene* WXFriendNpcScene::createScene()
{
	if (GlobalData::isPopingScene)
		return NULL;
	GlobalData::isPopingScene = true;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	Layer* layer = WXFriendNpcScene::create();

	if (layer == NULL)
		return NULL;
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

WXFriendNpcScene* WXFriendNpcScene::create()
{
	WXFriendNpcScene *pRet = new WXFriendNpcScene();
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

bool WXFriendNpcScene::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
}


bool WXFriendNpcScene::init()
{
	std::vector<std::string> vec_friends;
	std::map<std::string, FriendlyData>::iterator mit;
	for (mit = GlobalData::map_myfriendly.begin(); mit != GlobalData::map_myfriendly.end(); ++mit)
	{
		if (GlobalData::map_myfriendly[mit->first].relation == F_FRIEND)
		{
			if (GlobalData::map_NPCFriendData[mit->first].vec_fightnpc.size() > 0)
			vec_friends.push_back(mit->first);
		}
	}

	if (vec_friends.size() <= 0)
		return false;

	Node* csbnode = CSLoader::createNode("friendNpcChallengeLayer.csb");
	this->addChild(csbnode);

	int rnd = GlobalData::createRandomNum(vec_friends.size());
	std::string npcid = vec_friends[rnd];

	vec_fightnpc = GlobalData::map_NPCFriendData[npcid].vec_fightnpc;
	std::string enemynpcname;

	GlobalData::map_maps["m1-6"].npcs.clear();
	for (unsigned int i = 0; i < vec_fightnpc.size(); i++)
	{
		enemynpcname.append(GlobalData::map_npcs[vec_fightnpc[i]].name);
		enemynpcname.append(CommonFuncs::gbk2utf("、"));
		GlobalData::map_maps["m1-6"].npcs.push_back(vec_fightnpc[i]);
	}
	enemynpcname = enemynpcname.substr(0, enemynpcname.length() - 3);

	//描述
	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");

	desc1 = (cocos2d::ui::Text*)csbnode->getChildByName("desc1");
	desc1->setVisible(false);

	std::string descstr = StringUtils::format("%s%s%s%s%s", CommonFuncs::gbk2utf("你与").c_str(), GlobalData::map_npcs[npcid].name, CommonFuncs::gbk2utf("成为好友，").c_str(), enemynpcname.c_str(), CommonFuncs::gbk2utf("向你发起挑战！").c_str());
	desc->setString(descstr);

	cocos2d::ui::ImageView* typepng = (cocos2d::ui::ImageView*)csbnode->getChildByName("image");
	typepng->loadTexture("images/cchallenge.jpg", cocos2d::ui::Widget::TextureResType::LOCAL);
	
	escapebtn = (cocos2d::ui::Button*)csbnode->getChildByName("escapebtn");
	escapebtn->addTouchEventListener(CC_CALLBACK_2(WXFriendNpcScene::onEscape, this));

	getbtn = (cocos2d::ui::Button*)csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(WXFriendNpcScene::onGet, this));


	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
void WXFriendNpcScene::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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

void WXFriendNpcScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void WXFriendNpcScene::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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


void WXFriendNpcScene::onEscape(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		int r = GlobalData::createRandomNum(100);
		if (r < 50)
		{
			desc1->setVisible(true);
			escapebtn->setEnabled(false);
			getbtn->setEnabled(false);
			this->scheduleOnce(schedule_selector(WXFriendNpcScene::delayFight), 1.5f);
		}
		else
		{
			Director::getInstance()->popScene();
		}
	}
}

void WXFriendNpcScene::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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


void WXFriendNpcScene::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		escapebtn->setEnabled(false);
		getbtn->setEnabled(false);
		this->scheduleOnce(schedule_selector(WXFriendNpcScene::delayFight), 0.5f);
	}
}


void WXFriendNpcScene::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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
				DelayTime::create(30*0.1),
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
				DelayTime::create(20*0.1),
				action,
				NULL));
		}
		else {
			node->runAction(action);
		}
	}
}

void WXFriendNpcScene::delayFight(float dt)
{
	Director::getInstance()->popScene();

	if (g_gameLayer != NULL)
	{
		g_gameLayer->addChild(WXFightLayer::create("m1-6", vec_fightnpc[0]), 4, "fightlayer");
	}
}

void WXFriendNpcScene::jumpDown(cocos2d::Node *node, float dt) {
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