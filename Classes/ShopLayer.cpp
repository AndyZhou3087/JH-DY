#include "ShopLayer.h"
#include "Const.h"
#include "BuildingUILayer.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "SelectHeroScene.h"
#include "SoundManager.h"
#include "ReviveLayer.h"
#include "json.h"
#include "AnalyticUtil.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"
#include "RmbGoodsItem.h"
#include "GoldGoodsItem.h"
#include "GetVipRewardLayer.h"
#include "MapLayer.h"
#include "ServerDataSwap.h"
#include "VipShopLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSPurchaseWrap.h"
#include "iosfunc.h"
#endif

int ShopLayer::payindex = -1;
bool ShopLayer::isPaying = false;
ShopLayer::ShopLayer()
{
	ischangePause = false;
}


ShopLayer::~ShopLayer()
{
	if (GlobalData::g_gameStatus == GAMEPAUSE && ischangePause)
		GlobalData::g_gameStatus = GAMESTART;
	if (g_hero != NULL && g_hero->getIsMoving())
	{
		g_maplayer->heroResumeMoving();
	}
}

ShopLayer* ShopLayer::create()
{
	ShopLayer *pRet = new ShopLayer();
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

bool ShopLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("shopLayer.csb");
	this->addChild(m_csbnode);

	refreshGoldCount(0);

	std::vector<GoodsData*> vec_rmbGoods;
	std::vector<GoodsData*> vec_goldGoods;

	int goodsize = GlobalData::vec_goods.size();

	for (int i = 0; i < goodsize; i++)
	{
		GoodsData* gdata = &GlobalData::vec_goods[i];
		if (gdata->type == 0)
			vec_rmbGoods.push_back(gdata);
		else if (gdata->type == 1)
			vec_goldGoods.push_back(gdata);
	}


	m_rmbScrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("rmbgoodsscroll");
	m_rmbScrollview->setScrollBarEnabled(false);
	m_rmbScrollview->setBounceEnabled(true);

	int itemwidth = 160;
	int innerwidth = itemwidth * vec_rmbGoods.size();
	int contentwidth = m_rmbScrollview->getContentSize().width;
	if (innerwidth < contentwidth)
		innerwidth = contentwidth;
	m_rmbScrollview->setInnerContainerSize(Size(innerwidth, m_rmbScrollview->getContentSize().height));

	for (unsigned int i = 0; i < vec_rmbGoods.size(); i++)
	{
		RmbGoodsItem* node = RmbGoodsItem::create(vec_rmbGoods[i]);

		node->setTag(sizeof(heroprice) / sizeof(heroprice[0]) + i);
		m_rmbScrollview->addChild(node);
		node->setPosition(Vec2(itemwidth / 2  + 10 + i * itemwidth, m_rmbScrollview->getContentSize().height/2));
	}

	m_goldScrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("goldgoodsscroll");
	m_goldScrollview->setScrollBarEnabled(false);
	m_goldScrollview->setBounceEnabled(true);

	int itemheight = 205;
	int innerheight = itemheight * vec_goldGoods.size();
	int contentheight = m_goldScrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_goldScrollview->setInnerContainerSize(Size(m_goldScrollview->getContentSize().width, innerheight));

	for (unsigned int i = 0; i < vec_goldGoods.size(); i++)
	{
		GoldGoodsItem* node = GoldGoodsItem::create(vec_goldGoods[i]);
		m_goldScrollview->addChild(node);
		node->setPosition(Vec2(360, innerheight - itemheight / 2 - i * itemheight));
	}

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(ShopLayer::onBack, this));


	qqtitle = (cocos2d::ui::Text*)m_csbnode->getChildByName("qqtext");
	qqtitle->setVisible(false);

	qq1 = (cocos2d::ui::Text*)m_csbnode->getChildByName("qq");
	qq1->setVisible(false);
	qq1->addTouchEventListener(CC_CALLBACK_2(ShopLayer::onQQ, this));

	qq2 = (cocos2d::ui::Text*)m_csbnode->getChildByName("qq_1");
	qq2->addTouchEventListener(CC_CALLBACK_2(ShopLayer::onQQ, this));
	qq2->setVisible(false);

	int qqsize = GlobalData::vec_qq.size();
	if (qqsize > 0)
	{
		qqtitle->setVisible(true);
		int rqq = GlobalData::createRandomNum(qqsize);
		qq1->setString(GlobalData::vec_qq[rqq]);
		qq1->setVisible(true);
		if (qqsize > 1)
		{
			qq2->setVisible(true);
			qq2->setString(GlobalData::vec_qq[1 - rqq]);
		}
		else
		{
			qq2->setVisible(false);
			qq1->setPositionX(qq2->getPositionX());
			m_csbnode->getChildByName("qqtext")->setPositionX(510);
		}
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (GlobalData::g_gameStatus == GAMESTART)
	{
		GlobalData::g_gameStatus = GAMEPAUSE;
		ischangePause = true;
	}

	if (g_hero != NULL && g_hero->getIsMoving())
	{
		g_maplayer->heroPauseMoving();
	}

	this->schedule(schedule_selector(ShopLayer::refreshGoldCount), 1);
	return true;
}

void ShopLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}



void ShopLayer::beginPay(int index)
{
	if (isPaying)
		return;

	isPaying = true;
	payindex = index;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	setMessage(PAY_SUCC);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//PaySelectLayer* layer = PaySelectLayer::create(payindex);
	//Director::getInstance()->getRunningScene()->addChild(layer, 1);
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };
	sprintf(p_str, "%s", payCode[index].c_str());
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/PayAction", "pay", "(Ljava/lang/String;I)V"))
	{
		jstring str1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, str1, payindex);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//payCode
	buy((char*)payCode[payindex].c_str());
#endif
}
void ShopLayer::setMessage(PYARET ret)
{
	if (ret == PAY_SUCC)
	{
		int herocount = sizeof(heroprice) / sizeof(heroprice[0]);
		int golditemcount = sizeof(goldcount) / sizeof(goldcount[0]);
		int vipcount = sizeof(vipgoldcount) / sizeof(vipgoldcount[0]);
		if (payindex < herocount) // 人物解锁
		{
			if (g_SelectHeroScene != NULL)
				g_SelectHeroScene->unlockSucc(payindex);
#ifdef ANALYTICS
			std::string heroname[] = { "bxym", "bssy", "bjxb", "baq" };
			AnalyticUtil::onEvent(heroname[payindex].c_str());
#endif
		}
		else if (payindex < herocount + golditemcount)//买元宝
		{
			//addBuyGoods();
			GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() + goldcount[payindex - herocount]);
			SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUYOK);
#ifdef ANALYTICS
			std::string name[] = { "b6", "b12", "b30", "b68"};
			AnalyticUtil::onEvent(name[payindex - herocount].c_str());
#endif
		}
		else if (payindex < herocount + golditemcount + vipcount)//买VIP
		{
			std::string vipid = GlobalData::vec_goods[payindex - herocount].icon;
			GlobalData::vec_buyVipIds.push_back(vipid);
			//int monthdays = GlobalData::getMonth_Days();
			//if (GlobalData::map_buyVipDays.find(vipid) != GlobalData::map_buyVipDays.end())
			//	GlobalData::map_buyVipDays[vipid] += monthdays;
			//else
			//	GlobalData::map_buyVipDays[vipid] = monthdays;

			ServerDataSwap::init()->vipSuccNotice(vipid);
			if (g_gameLayer != NULL)
			{
				g_gameLayer->scheduleOnce(schedule_selector(ShopLayer::showVipReward), 0.1f);
			}
#ifdef ANALYTICS
			std::string name[] = { "byk6", "byk30", "byk68"};
			AnalyticUtil::onEvent(name[payindex - herocount - golditemcount].c_str());
#endif
		}
		else if (payindex == TIMEGIFT)
		{
			if (g_gameLayer != NULL)
			{
				g_gameLayer->removeChildByName("gift");
			}
			GlobalData::setIsBuyTimeGift(true);
			GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() + 100);
			GoldGoodsItem::addBuyGoods(&GlobalData::vec_goods[payindex - herocount]);

#ifdef ANALYTICS
			AnalyticUtil::onEvent("timegift");
#endif
		}
#ifdef ANALYTICS
		AnalyticUtil::pay("pay", buyprice[payindex], 1);
#endif
	}
	isPaying = false;
}

void ShopLayer::showVipReward(float dt)
{
	GetVipRewardLayer* layer = GetVipRewardLayer::create();
	if (g_gameLayer != NULL)
	{
		g_gameLayer->addChild(layer, 10, "viprewardlayer");
		VipShopLayer* vipshop = (VipShopLayer*)g_gameLayer->getChildByName("vipshoplayer");
		if (vipshop != NULL)
			vipshop->getLeftDays();
	}
}

void ShopLayer::refreshGoldCount(float dt)
{
	mygoldlbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("mygoldlbl");
	std::string countstr = StringUtils::format("%d", GlobalData::getMyGoldCount());
	mygoldlbl->setString(countstr);
}

void ShopLayer::onQQ(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		cocos2d::ui::Text* qq = (cocos2d::ui::Text*)pSender;
		GlobalData::copyToClipBoard(qq->getString());
	}

}