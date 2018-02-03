#include "WXFactionMainLayer.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "WXFactionCreateLayer.h"
#include "Const.h"
#include "GameScene.h"
#include "WXFactionMemberLayer.h"
#include "WXFactionDetailsLayer.h"

WXFactionMainLayer::WXFactionMainLayer()
{
}


WXFactionMainLayer::~WXFactionMainLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}
void WXFactionMainLayer::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

WXFactionMainLayer* WXFactionMainLayer::create()
{
	WXFactionMainLayer *pRet = new WXFactionMainLayer();
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

void WXFactionMainLayer::shake(Node * node) {
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

bool WXFactionMainLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("factionMainLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(WXFactionMainLayer::onBack, this));

	enterbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("enterbtn");
	enterbtn->addTouchEventListener(CC_CALLBACK_2(WXFactionMainLayer::onEnterFaction, this));

	createbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("createbtn");
	createbtn->addTouchEventListener(CC_CALLBACK_2(WXFactionMainLayer::onCreateFaction, this));

	cocos2d::ui::Widget* morebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("morebtn");
	morebtn->addTouchEventListener(CC_CALLBACK_2(WXFactionMainLayer::onMore, this));

	srollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scrollview");
	srollView->setScrollBarEnabled(false);
	srollView->setBounceEnabled(true);

	GlobalData::g_gameStatus = GAMEPAUSE;

	getFactionListData();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void WXFactionMainLayer::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
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


void WXFactionMainLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void WXFactionMainLayer::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
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

void WXFactionMainLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void WXFactionMainLayer::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac) {
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

void WXFactionMainLayer::onEnterFaction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		FactionListData* fldata = NULL;
		for (unsigned int i = 0; i < GlobalData::vec_factionListData.size(); i++)
		{
			if (GlobalData::vec_factionListData[i].id == GlobalData::myFaction)
			{
				fldata = &GlobalData::vec_factionListData[i];

			}
		}
		if (fldata != NULL)
		{
			WXFactionMemberLayer * fmayer = WXFactionMemberLayer::create(fldata);
			g_gameLayer->addChild(fmayer, 5, "factionmemberlayer");
		}
	}
}


void WXFactionMainLayer::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
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

void WXFactionMainLayer::onCreateFaction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (g_hero->getLVValue() < 49)
		{
			std::string str = StringUtils::format("%s", CommonFuncs::gbk2utf("等级达到50级才能创建帮派！！").c_str());
			HintBox * box = HintBox::create(str);
			this->addChild(box);
			return;
		}

		WXFactionCreateLayer* flayer = WXFactionCreateLayer::create(0);
		this->addChild(flayer);
	}
}

void WXFactionMainLayer::onMore(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		WXFactionDetailsLayer* fdlayer = WXFactionDetailsLayer::create();
		g_gameLayer->addChild(fdlayer, 5);
	}
}

void WXFactionMainLayer::getFactionListData()
{
	WaitingProgress* waitbox = WaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->getFactionList();
}

void WXFactionMainLayer::delayShowData(float dt)
{
	srollView->removeAllChildrenWithCleanup(true);
	int size = GlobalData::vec_factionListData.size();

	int itemheight = 78;
	int innerheight = itemheight * size;
	int contentheight = srollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	srollView->setInnerContainerSize(Size(srollView->getContentSize().width, innerheight));

	for (unsigned int i = 0; i < GlobalData::vec_factionListData.size(); i++)
	{
		FactionListItem* node = FactionListItem::create(&GlobalData::vec_factionListData[i]);
		node->setPosition(Vec2(srollView->getContentSize().width/2, innerheight - itemheight / 2 - i * itemheight));
		std::string nodestr = StringUtils::format("flitem%d", i);
		srollView->addChild(node, 0, nodestr);
	}

	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void WXFactionMainLayer::updateLvAndMember()
{
	for (unsigned int i = 0; i < GlobalData::vec_factionListData.size(); i++)
	{
		std::string nodestr = StringUtils::format("flitem%d", i);
		FactionListItem* itemnode = (FactionListItem*)srollView->getChildByName(nodestr);
		itemnode->updateLvAndPeopleCount();
	}
	
}

void WXFactionMainLayer::onSuccess()
{
	if (GlobalData::myFaction > 0)
	{
		createbtn->setEnabled(false);
		if (GlobalData::mytitle == 0)
			enterbtn->setEnabled(false);
		else if (GlobalData::mytitle > 0)
			enterbtn->setEnabled(true);
		else
		{
			enterbtn->setEnabled(false);
			createbtn->setEnabled(true);
			std::string str = StringUtils::format("%s", CommonFuncs::gbk2utf("你申请帮派的请求被拒绝了！！请重新申请吧~~").c_str());
			HintBox * box = HintBox::create(str);
			this->addChild(box);
		}
	}
	else
	{
		enterbtn->setEnabled(false);
		createbtn->setEnabled(true);
	}
	this->scheduleOnce(schedule_selector(WXFactionMainLayer::delayShowData), 0.1f);
}

void WXFactionMainLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}
void WXFactionMainLayer::disBtn()
{
	createbtn->setEnabled(false);
	enterbtn->setEnabled(false);
}

void WXFactionMainLayer::resetBtn()
{
	createbtn->setEnabled(true);
	enterbtn->setEnabled(false);
}

FactionListItem::FactionListItem()
{

}
FactionListItem::~FactionListItem()
{

}

void FactionListItem::jumpDown(cocos2d::Node *node, float dt) {
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
bool FactionListItem::init(FactionListData *data)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_data = data;
	Node* csbnode = CSLoader::createNode("factionlistNode.csb");
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	int rank = data->rank + 1;

	std::string itemstr;
	if (rank % 2 == 0)
	{
		itemstr = "ui/factlistitem0.png";
	}
	if (itemstr.length() > 0)
	{
		/*cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)csbnode->getChildByName("item");
		item->loadTexture(itemstr, cocos2d::ui::TextureResType::PLIST);*/
	}

	cocos2d::ui::Text* ranknumlbl = (cocos2d::ui::Text*)csbnode->getChildByName("num");
	std::string str = StringUtils::format("%d", rank);
	ranknumlbl->setString(str);

	if (rank <= 3)
	{
		ranknumlbl->setVisible(false);
		std::string rankspritename = StringUtils::format("ui/ranknum%d.png", rank);
		Sprite* ranknum = Sprite::createWithSpriteFrameName(rankspritename);
		ranknum->setPosition(ranknumlbl->getPosition());
		csbnode->addChild(ranknum);
	}

	cocos2d::ui::Text* namelbl = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	namelbl->setString(data->factionname);

	countlbl = (cocos2d::ui::Text*)csbnode->getChildByName("count");
	lvlbl = (cocos2d::ui::Text*)csbnode->getChildByName("lv");

	updateLvAndPeopleCount();

	cocos2d::ui::Text* ownerlbl = (cocos2d::ui::Text*)csbnode->getChildByName("owner");
	ownerlbl->setString(data->owner);

	actionbtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
	actionbtn->addTouchEventListener(CC_CALLBACK_2(FactionListItem::onAction, this));
	actionbtn->setSwallowTouches(false);

	actionbtnlabel = (cocos2d::ui::Text*)actionbtn->getChildByName("text");

	if (GlobalData::myFaction == data->id)
	{
		if (GlobalData::mytitle == 0)
		{
			actionbtnlabel->setString(CommonFuncs::gbk2utf("取消申请"));
			actionbtn->loadTextureNormal("ui/buildtagbtn1.png", cocos2d::ui::Widget::TextureResType::PLIST);
			actionbtn->loadTexturePressed("ui/buildtagbtn1.png", cocos2d::ui::Widget::TextureResType::PLIST);
		}
		else if (GlobalData::mytitle == 1)
		{
			actionbtnlabel->setString(CommonFuncs::gbk2utf("已创建"));
			actionbtn->setEnabled(false);
		}
		else if (GlobalData::mytitle == 2 || GlobalData::mytitle == 3 || GlobalData::mytitle == 4)
		{
			actionbtnlabel->setString(CommonFuncs::gbk2utf("已加入"));
			actionbtn->setEnabled(false);
		}
	}

	return true;
}

bool FactionListItem::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
}
void FactionListItem::updateLvAndPeopleCount()
{
	std::string str = StringUtils::format("%d/%d", m_data->membercount, m_data->maxcount);
	countlbl->setString(str);

	int lv = 0;
	for (int i = 99; i >= 0; i--)
	{
		if (m_data->exp >= i*i*(20+(i-1)*5) * 100)
		{
			lv = i;
			break;
		}
	}
	m_data->lv = lv;
	if (m_data->lv > 4)
	{
		m_data->lv = 4;
	}
	str = StringUtils::format("%d", m_data->lv + 1);
	lvlbl->setString(str);
}

FactionListItem* FactionListItem::create(FactionListData *data)
{
	FactionListItem *pRet = new FactionListItem();
	if (pRet && pRet->init(data))
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

bool FactionListItem::getRandomBoolean(float rate) {

	int rate10 = (int)(rate*10.0);
	int randNum = rand();
	if (randNum % 10 <= rate10) {
		return true;
	}
	else {
		return false;
	}
}
void FactionListItem::onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string descstr;
		if (GlobalData::myFaction > 0)
		{
			if (GlobalData::mytitle == 0)
			{ 
				if (actionbtnlabel->getString().compare(CommonFuncs::gbk2utf("取消申请")) == 0)
				{
					WaitingProgress* waitbox = WaitingProgress::create("处理中...");
					Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
					ServerDataSwap::init(this)->cancelFaction(m_data->id);
					return;
				}
				else
				{
					descstr = "已申请帮派！！";
				}
			}
			else if (GlobalData::mytitle == -1)
			{
				checkRequset();
			}
			else if (GlobalData::mytitle == 1)
				descstr = "已创建帮派！！";
			else if (GlobalData::mytitle == 2 || GlobalData::mytitle == 3 || GlobalData::mytitle == 4)
				descstr = "已加入帮派！！";

			if (descstr.length() > 0)
			{
				HintBox* hintbox = HintBox::create(CommonFuncs::gbk2utf(descstr.c_str()));
				Director::getInstance()->getRunningScene()->addChild(hintbox);
			}
		}
		else
		{
			if (m_data->membercount >= m_data->maxcount)
			{
				descstr = StringUtils::format("%s已满员！", m_data->factionname.c_str());
				HintBox* hintbox = HintBox::create(CommonFuncs::gbk2utf(descstr.c_str()));
				Director::getInstance()->getRunningScene()->addChild(hintbox);
				return;
			}
			checkRequset();
		}
	}
}

bool FactionListItem::getRandomBoolean() {

	if (0 == rand() % 2) {
		return true;
	}
	else {
		return false;
	}
}


void FactionListItem::checkRequset()
{
	if (g_hero->getLVValue() < m_data->lvlimit - 1)
	{
		std::string str = StringUtils::format("%s%s%d%s", m_data->factionname.c_str(), CommonFuncs::gbk2utf("需要申请者达到").c_str(), m_data->lvlimit, CommonFuncs::gbk2utf("级").c_str());
		HintBox* hintbox = HintBox::create(str);
		Director::getInstance()->getRunningScene()->addChild(hintbox);
		return;
	}
	int selectsex = g_hero->getSex();
	if (m_data->sexlimit < 4)
	{
		bool isok = true;
		if (m_data->sexlimit == 3)
		{
			if (selectsex = 0)
			{
				isok = false;
			}
		}
		else if (selectsex != m_data->sexlimit)
		{
			isok = false;
		}

		if (!isok)
		{
			std::string sexstr[] = { "只收自宫者", "只收男性", "只收女性", "只收男性和女性" };
			std::string str = StringUtils::format("%s%s", m_data->factionname.c_str(), CommonFuncs::gbk2utf(sexstr[m_data->sexlimit].c_str()).c_str());
			HintBox* hintbox = HintBox::create(str);
			Director::getInstance()->getRunningScene()->addChild(hintbox);
			return;
		}
	}
	WaitingProgress* waitbox = WaitingProgress::create("处理中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->requestFaction(m_data->id);
}

int FactionListItem::getRandomNum(int range) {

	if (range <= 0) {
		return 0;
	}

	return rand() % range;
}


void FactionListItem::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	if (actionbtnlabel->getString().compare(CommonFuncs::gbk2utf("申请加入")) == 0)
	{
		actionbtnlabel->setString(CommonFuncs::gbk2utf("取消申请"));
		GlobalData::myFaction = m_data->id;
		GlobalData::mytitle = 0;
		actionbtn->loadTextureNormal("ui/buildtagbtn1.png", cocos2d::ui::Widget::TextureResType::PLIST);
		actionbtn->loadTexturePressed("ui/buildtagbtn1.png", cocos2d::ui::Widget::TextureResType::PLIST);
		WXFactionMainLayer* fmainlayer = (WXFactionMainLayer*)g_gameLayer->getChildByName("factionmainlayer");
		if (fmainlayer != NULL)
			fmainlayer->disBtn();
	}
	else if (actionbtnlabel->getString().compare(CommonFuncs::gbk2utf("取消申请")) == 0)
	{
		actionbtnlabel->setString(CommonFuncs::gbk2utf("申请加入"));
		GlobalData::myFaction = 0;
		GlobalData::mytitle = 0;
		actionbtn->loadTextureNormal("ui/buildtagbtn0.png", cocos2d::ui::Widget::TextureResType::PLIST);
		actionbtn->loadTexturePressed("ui/buildtagbtn0.png", cocos2d::ui::Widget::TextureResType::PLIST);

		WXFactionMainLayer* fmainlayer = (WXFactionMainLayer*)g_gameLayer->getChildByName("factionmainlayer");
		if (fmainlayer != NULL)
			fmainlayer->resetBtn();
	}
}


int FactionListItem::getRandomNum(int rangeStart, int rangeEnd) {

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
void FactionListItem::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	std::string errstr = "申请失败，请检查网络设置稍后重试！！";
	if (errcode == 1)
	{
		errstr = "参数错误，请与客服联系！！";
	}
	else if (errcode == 2)
	{
		errstr = "帮派不存在？！请与客服联系！！";
	}
	else if (errcode == 3)
	{
		errstr = "已申请过该帮派！！";
	}
	else if (errcode == 4)
	{
		errstr = "帮派人数已达上限！！";
	}

	HintBox * box = HintBox::create(CommonFuncs::gbk2utf(errstr.c_str()));
	Director::getInstance()->getRunningScene()->addChild(box);
}
