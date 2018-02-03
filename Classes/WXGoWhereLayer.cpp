#include "WXGoWhereLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GlobalData.h"
#include "WXHomeHill.h"
#include "HomeLayer.h"
#include "MapLayer.h"
#include "GameScene.h"
#include "TempStorageLayer.h"
#include "WXNpcLayer.h"
#include "SoundManager.h"
#include "GameDataSave.h"
#include "NewerGuideLayer.h"
#include "WXActivitScene.h"
#include "SepcialNpcLayer.h"
#include "AnalyticUtil.h"
//#include "RollDiceLayer.h"
#include "TopBar.h"
WXGoWhereLayer::WXGoWhereLayer()
{

}


WXGoWhereLayer::~WXGoWhereLayer()
{
}

WXGoWhereLayer* WXGoWhereLayer::create(std::string addrname, WHERELAYER_TYPE type, float distance)
{
	WXGoWhereLayer *pRet = new WXGoWhereLayer();
	if (pRet && pRet->init(addrname, type, distance))
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

bool WXGoWhereLayer::getRandomBoolean(float rate) {

	int rate10 = (int)(rate*10.0);
	int randNum = rand();
	if (randNum % 10 <= rate10) {
		return true;
	}
	else {
		return false;
	}
}

bool WXGoWhereLayer::getRandomBoolean() {

	if (0 == rand() % 2) {
		return true;
	}
	else {
		return false;
	}
}


bool WXGoWhereLayer::init(std::string addrid, WHERELAYER_TYPE type, float distance)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("goWhereLayer.csb");
	this->addChild(csbnode);

	m_addrstr = addrid;
	m_type = type;

	MapData mdata = GlobalData::map_maps[addrid];

	//名称
	cocos2d::ui::Text* title = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	title->setString(mdata.cname);

	//描述
	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desc->setString(mdata.desc);

	//距离 ，换算成时间
	cocos2d::ui::Text* fast = (cocos2d::ui::Text*)csbnode->getChildByName("fast");

    float fmin = 0.0f;
    if (g_maplayer != NULL)
		fmin = g_maplayer->moveToDestTime(distance);

	int minute = (int)fmin;
	std::string faststr;
	if (minute / 60 > 0)
	{
		faststr = StringUtils::format("%d小时%d分钟", minute / 60, minute % 60);
		if (minute % 60 == 0)
			faststr = StringUtils::format("%d小时", minute / 60);
	}
	else
		faststr = StringUtils::format("%d分钟", minute);
	
	fast->setString(CommonFuncs::gbk2utf(faststr.c_str()));

	cocos2d::ui::Text* fasttitle = (cocos2d::ui::Text*)csbnode->getChildByName("text1");
	
	//地点的类型
	cocos2d::ui::ImageView* typeimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("typepng");
	std::string str = StringUtils::format("images/%s.jpg", mdata.tpngname);
	typeimg->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
	
	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(WXGoWhereLayer::onClose, this));
	closebtnlabel = (cocos2d::ui::Text*)closebtn->getChildByName("btnlabel");

	m_gobtn = (cocos2d::ui::Button*)csbnode->getChildByName("gobtn");
	m_gobtn->addTouchEventListener(CC_CALLBACK_2(WXGoWhereLayer::onGO, this));

	//物品存放
	m_stbtn = (cocos2d::ui::Button*)csbnode->getChildByName("stbtn");
	m_stbtn->addTouchEventListener(CC_CALLBACK_2(WXGoWhereLayer::onST, this));

	m_stredpoint = (cocos2d::ui::Widget*)m_stbtn->getChildByName("redpoint");
	checkRedPoint(0);

	m_enterbtn = (cocos2d::ui::Button*)csbnode->getChildByName("enterbtn");
	m_enterbtn->addTouchEventListener(CC_CALLBACK_2(WXGoWhereLayer::onComeIn, this));
	enterbtnlabel = (cocos2d::ui::Text*)m_enterbtn->getChildByName("btnlabel");

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(WXGoWhereLayer::onClose, this));

	if (type == GOWHERE)
	{
		closebtn->setVisible(true);
		m_gobtn->setVisible(true);
		m_stbtn->setVisible(false);
		m_enterbtn->setVisible(false);
		fast->setVisible(true);
		fasttitle->setVisible(true);
	}
	else if (type == ARRIVE)
	{
		closebtn->setVisible(false);
		m_gobtn->setVisible(false);
		m_stbtn->setVisible(true);
		m_enterbtn->setVisible(true);
		fast->setVisible(false);
		fasttitle->setVisible(false);
		if (m_addrstr.compare("m1-1") == 0)//家，没有物品存放
		{
			m_stbtn->setVisible(false);
			m_enterbtn->setPositionX(360);
		}
	}
	else if (type == ONWAY_JUMP)
	{
		closebtn->setVisible(true);
		closebtnlabel->setString(CommonFuncs::gbk2utf("不跳"));
		//closebtn->setTitleText(CommonFuncs::gbk2utf("不跳"));
		m_gobtn->setVisible(false);
		m_stbtn->setVisible(false);
		m_enterbtn->setVisible(true);
		enterbtnlabel->setString(CommonFuncs::gbk2utf("跳入"));
		//m_enterbtn->setTitleText(CommonFuncs::gbk2utf("跳入"));
		fast->setVisible(false);
		fasttitle->setVisible(false);
		title->setString(CommonFuncs::gbk2utf("奇遇"));
		desc->setString(CommonFuncs::gbk2utf("发现悬崖下面紫光闪耀，好像藏着什么好东西，你想下去一探究竟，少侠还请三思，崖下白骨露野，跳下去多半会一命呜呼！"));
		std::string str = StringUtils::format("images/%s.jpg", "t008");
		typeimg->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
#ifdef ANALYTICS
		AnalyticUtil::onEvent("showjump");
#endif
	}
	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->schedule(schedule_selector(WXGoWhereLayer::checkRedPoint), 1.0f);
	checkNewerGuide();
	return true;
}

int WXGoWhereLayer::getRandomNum(int range) {

	if (range <= 0) {
		return 0;
	}

	return rand() % range;
}

int WXGoWhereLayer::getRandomNum(int rangeStart, int rangeEnd) {

	if (rangeEnd < rangeStart) {
		CCASSERT(false, "get random fail");
		return 0;
	}

	if (rangeStart == rangeEnd) {
		return rangeStart;
	}

	int delta = rand() % (rangeEnd - rangeStart);
	return rangeStart + delta;
}

void WXGoWhereLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	if (m_type == ONWAY_JUMP && g_maplayer != NULL)
		g_maplayer->heroPauseMoving();
}

void WXGoWhereLayer::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void WXGoWhereLayer::shake(Node * node) {
	if (NULL == node) {
		return;
	}

	node->runAction(CCSequence::create(
		MoveBy::create(0.02, Vec2(0, 15)),
		MoveBy::create(0.02, Vec2(0, -27)),
		MoveBy::create(0.02, Vec2(0, 22)),
		MoveBy::create(0.02, Vec2(0, -14)),
		MoveBy::create(0.02, Vec2(0, 4)),
		NULL));
}

void WXGoWhereLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_type == ONWAY_JUMP && g_maplayer != NULL)
		{
			g_maplayer->heroResumeMoving();
		}
		this->removeFromParentAndCleanup(true);
	}
}

bool WXGoWhereLayer::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
}

void WXGoWhereLayer::onGO(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
		if (g_maplayer != NULL)//地图上移动角色头像
			g_maplayer->showMoveToDest();
	}
}

void WXGoWhereLayer::onST(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		TempStorageLayer* layer = TempStorageLayer::create(m_addrstr);

		if (g_gameLayer != NULL)
			g_gameLayer->addChild(layer, 2, "TempStorageLayer");
	}
}


void WXGoWhereLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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


void WXGoWhereLayer::onComeIn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_addrstr.compare("m1-1") == 0)//进入家
		{
			HomeLayer* homelayer = HomeLayer::create();
			g_gameLayer->addChild(homelayer, 1, "homelayer");
			g_maplayer->removeFromParentAndCleanup(true);
			g_maplayer = NULL;
		}
		else if (m_addrstr.compare("m1-2") == 0)//进入后山
		{
			if (g_gameLayer != NULL)
				g_gameLayer->addChild(WXHomeHill::create(), 2, "homehill");
		}
		else if (m_addrstr.compare("m1-9") == 0)
		{
			//RollDiceLayer* rlayer = RollDiceLayer::create();
			//g_gameLayer->addChild(rlayer, 5);
		}
		else
		{
			if (m_type == ONWAY_JUMP)
			{
				Scene* activityScene = WXActivitScene::createScene("images/findtreasure.jpg", CommonFuncs::gbk2utf(""));
				if (activityScene != NULL)
				{
					Director::getInstance()->pushScene(activityScene);
					this->removeFromParentAndCleanup(true);
				}
				return;
			}
			else//进入NPC
			{
				if (m_addrstr.compare("m13-1") == 0)
				{
					if (g_gameLayer != NULL)
						g_gameLayer->addChild(SepcialNpcLayer::create(m_addrstr), 2, "sepcialnpclayer");
				}
				else
				{
					if (g_gameLayer != NULL)
						g_gameLayer->addChild(WXNpcLayer::create(m_addrstr), 2, "npclayer");
					std::string mapname = GlobalData::map_maps[m_addrstr].cname;
					if (mapname.find(CommonFuncs::gbk2utf("客栈")) != std::string::npos)
					{
						g_uiScroll->addEventText(CommonFuncs::gbk2utf("小二：客官里面请，吃饭一两银子，睡觉二两银子，喝酒二两银子"), 25, Color3B(204, 4, 4));
					}
				}
			}
		}
		this->removeFromParentAndCleanup(true);
	}
}

void WXGoWhereLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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

void WXGoWhereLayer::checkRedPoint(float dt)
{
	if (GameDataSave::getInstance()->getTempStorage(m_addrstr).length() > 0)
		m_stredpoint->setVisible(true);
	else
		m_stredpoint->setVisible(false);
}

void WXGoWhereLayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if ((step == 15) && m_type == GOWHERE)
	{
		TopBar* topbar = (TopBar*)g_gameLayer->getChildByName("topbar");
		topbar->showNewerGuide(15);
	}
	else if ((step == 16) && m_type == GOWHERE)
	{
		nodes.push_back(m_gobtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}
	else if ((step == 17) && m_type == ARRIVE)
	{
		nodes.push_back(m_enterbtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}
	else if ((step == 39) && m_type == GOWHERE)
	{
		nodes.push_back(m_gobtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}
	else if (step == 40 && m_type == ARRIVE)
	{
		nodes.push_back(m_enterbtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}
	else if (step == 69 && m_type == ARRIVE)
	{
		nodes.push_back(m_stbtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}
	if (nodes.size() > 0)
		g_gameLayer->showNewerGuide(step, nodes);
}
void WXGoWhereLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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
void WXGoWhereLayer::checkNewerGuide()
{
	if (NewerGuideLayer::checkifNewerGuide(15))
		showNewerGuide(15);
	else if (NewerGuideLayer::checkifNewerGuide(16))
		showNewerGuide(16);
	else if (NewerGuideLayer::checkifNewerGuide(17))
		showNewerGuide(17);
	else if (NewerGuideLayer::checkifNewerGuide(39))
		showNewerGuide(39);
	else if (NewerGuideLayer::checkifNewerGuide(40))
		showNewerGuide(40);
	else if (NewerGuideLayer::checkifNewerGuide(69))
	{
		if (m_type == ARRIVE)
		{
			std::string datastr = GameDataSave::getInstance()->getTempStorage(m_addrstr);
			std::vector<std::string> vec_retstr;
			CommonFuncs::split(datastr, vec_retstr, ";");
			if (vec_retstr.size() > 0)
				showNewerGuide(69);
		}
	}
}

void WXGoWhereLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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
				DelayTime::create(getRandomNum(1, 10)*0.1),
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
				DelayTime::create(getRandomNum(1, 10)*0.1),
				action,
				NULL));
		}
		else {
			node->runAction(action);
		}
	}
}

void WXGoWhereLayer::jumpDown(cocos2d::Node *node, float dt) {
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

