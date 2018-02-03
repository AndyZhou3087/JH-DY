#include "WXApprenticeScene.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "StorageRoom.h"
#include "HintBox.h"
#include "GameDataSave.h"
#include "MD5.h"
#include "GameScene.h"
#include "ShopLayer.h"

WXApprenticeScene::WXApprenticeScene()
{

}

WXApprenticeScene::~WXApprenticeScene()
{
	GlobalData::isPopingScene = false;
}

Scene* WXApprenticeScene::createScene(int type)
{
	if (GlobalData::isPopingScene)
		return NULL;
	GlobalData::isPopingScene = true;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	Layer* layer = WXApprenticeScene::create(type);

	if (layer == NULL)
		return NULL;
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

WXApprenticeScene* WXApprenticeScene::create(int type)
{
	WXApprenticeScene *pRet = new WXApprenticeScene();
	if (pRet && pRet->init(type))
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

void WXApprenticeScene::loadTime() {
	std::string ts = "";
	long m_lastSaveTime = atol(ts.c_str());

	long nowTime = 123456789;
	if (m_lastSaveTime <= 0 || m_lastSaveTime > nowTime) {
		m_lastSaveTime = nowTime;
		saveTime();
	}
	else {
		long dt = nowTime - m_lastSaveTime;
		int add_times = dt / 10;
		if (add_times > 0) {
			const int df = add_times * 2;
			if (5 + df> 20) {

			}
			else {

			}
		}
		long m_countdown = 700 - dt % 50;
		m_lastSaveTime = nowTime - (50 - m_countdown);
		saveTime();
	}
}

bool WXApprenticeScene::init(int type)
{
	Node* csbnode = CSLoader::createNode("apprenticeLayer.csb");
	this->addChild(csbnode);

	m_type = type;

	std::string descstr = "有人慕名远道而来，想向你拜师，是否收他为徒？徒弟会帮你收集后山资源！";
	std::string typeimgstr = "images/apprentice0.jpg";
	std::string closebtntext = "不收";
	std::string getbtntext = "收徒";
	if (type == 1)
	{
		descstr = "你的徒儿为你累死累活收集资源，现在向你讨要生活费来了，是否赏赐生活费？太抠门徒弟可能离你而去！";
		typeimgstr = "images/apprentice1.jpg";
		closebtntext = "不给";
		getbtntext = "赏赐";
	}
	else
	{
		int aday = GameDataSave::getInstance()->getApprenticeDay();
		if (aday < 0)
		{
			descstr = "由于你抠门的名声已经传遍江湖，想收徒要先支付生活费！";
			csbnode->getChildByName("gold")->setVisible(true);
			csbnode->getChildByName("goldcount")->setVisible(true);
		}
	}

	//描述
	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desc->setString(CommonFuncs::gbk2utf(descstr.c_str()));


	cocos2d::ui::ImageView* typepng = (cocos2d::ui::ImageView*)csbnode->getChildByName("image");
	typepng->loadTexture(typeimgstr, cocos2d::ui::Widget::TextureResType::LOCAL);
	
	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("nobtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(WXApprenticeScene::onClose, this));
	closebtn->setTitleText(CommonFuncs::gbk2utf(closebtntext.c_str()));

	cocos2d::ui::Button* getbtn = (cocos2d::ui::Button*)csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(WXApprenticeScene::onGet, this));
	getbtn->setTitleText(CommonFuncs::gbk2utf(getbtntext.c_str()));

	if (m_type ==  1)
	{
		std::string boxstr = "ui/buildsmall.png";

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);
		box->setPosition(Vec2(360, 220));
		this->addChild(box);

		std::string str = "ui/gd0.png";
		Sprite* res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().width / 2));
		box->addChild(res);
		
		Label * namelbl = Label::createWithTTF(CommonFuncs::gbk2utf("金元宝"), "fonts/STXINGKA.TTF", 25);
		namelbl->setColor(Color3B(255, 255, 255));
		namelbl->setPosition(Vec2(box->getPositionX(), 140));
		this->addChild(namelbl);

		std::string strcount = StringUtils::format("x%d", 20);
		Label * coutlbl = Label::createWithTTF(strcount, "fonts/STXINGKA.TTF", 25);//Label::createWithSystemFont(strcount, "", 25);
		coutlbl->enableOutline(Color4B(0, 0, 0, 255), 3);
		coutlbl->setAnchorPoint(Vec2(1, 0.5f));
		coutlbl->setColor(Color3B(255, 255, 255));
		coutlbl->setPosition(Vec2(box->getPositionX() + 50, 180));
		this->addChild(coutlbl);
	}


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

void WXApprenticeScene::saveTime() {

}

void WXApprenticeScene::on1sTimer() {
	int m_countdown = 0;
	int m_lastSaveTime = 0;
	if (m_countdown-- <= 0) {
		if (500 < 20) {

		}

		m_countdown = 300;
		m_lastSaveTime = 1230;
		saveTime();
	}
}


void WXApprenticeScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void WXApprenticeScene::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_type == 1)
		{
			GameDataSave::getInstance()->setApprenticeDay(-1);
			showHint(0);
		}
		Director::getInstance()->popScene();
	}
}

void WXApprenticeScene::initA()
{
	const Size size = Director::getInstance()->getVisibleSize();
	auto m_top = CSLoader::createNode("GameTopLayer.csb");
	addChild(m_top, 0);
	m_top->ignoreAnchorPointForPosition(false);
	m_top->setAnchorPoint(Vec2(0.5, 1));
	m_top->setPosition(size.width * 0.5, size.height);

	auto m_bottom = CSLoader::createNode("GameBottomLayer.csb");
	m_bottom->ignoreAnchorPointForPosition(false);
	m_bottom->setAnchorPoint(Vec2(0.5, 0));
	m_bottom->setPosition(size.width * 0.5, 0);
	addChild(m_bottom, 1);
}

void WXApprenticeScene::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_type == 0)
		{
			int curgameday = g_nature->getPastDays();
			int aday = GameDataSave::getInstance()->getApprenticeDay();
			if (aday < 0)
			{
				giveGold();
			}
			else
			{
				GameDataSave::getInstance()->setApprenticeDay(curgameday);
				Director::getInstance()->popScene();
			}
		}
		else
		{
			giveGold();
		}
	}
}
void WXApprenticeScene::giveGold()
{
	int curgameday = g_nature->getPastDays();
	int mygold = GlobalData::getMyGoldCount();

	if (mygold >= 20)
	{
		if (GlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
		{
			GlobalData::dataIsModified = true;
			GlobalData::setMyGoldCount(0);
			HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
			this->addChild(hint);
			return;
		}
		GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - 20);
		GameDataSave::getInstance()->setApprenticeDay(curgameday);
		if (m_type == 1)
			showHint(1);

		Director::getInstance()->popScene();
	}
	else
	{
		HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("金元宝不足！"));
		this->addChild(hbox, 0, "hintbox");
		this->scheduleOnce(schedule_selector(WXApprenticeScene::delayShowShop), 0.8f);
	}
}

void WXApprenticeScene::initBg()
{
	const Size size = Director::getInstance()->getVisibleSize();
	auto m_bg = Sprite::create("");
	addChild(m_bg, 0);
	m_bg->setAnchorPoint(Vec2(0.5, 0));
	m_bg->setPosition(size.width * 0.5, 0);

	auto m_bgGround = Sprite::create("");
	addChild(m_bgGround, 1);
	m_bgGround->setPosition(m_bg->getContentSize().width * 0.5, 144 + m_bgGround->getContentSize().height * 0.5);

	auto tree = Sprite::create("");
	addChild(tree);
	tree->setAnchorPoint(Vec2(1, 0.5));
	tree->setPosition(size.width, 120);
}

void WXApprenticeScene::showHint(int type)
{
	std::string str = "因为你太过小气，徒弟已经离你而去！";
	if (type == 1)
		str = "因为你慷慨大方，徒弟更加任劳任怨！";
	HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf(str.c_str()));
	g_gameLayer->addChild(hbox, 10);
}

void WXApprenticeScene::delayShowShop(float dt)
{
	this->removeChildByName("hintbox");
	this->addChild(ShopLayer::create(), 1);
}