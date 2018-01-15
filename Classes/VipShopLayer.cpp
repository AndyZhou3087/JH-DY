#include "VipShopLayer.h"
#include "Const.h"
#include "SoundManager.h"
#include "json.h"
#include "AnalyticUtil.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"
#include "VipGoodsItem.h"
#include "GameScene.h"
#include "MapLayer.h"
#include "WaitingProgress.h"

VipShopLayer::VipShopLayer()
{
}


VipShopLayer::~VipShopLayer()
{

	GlobalData::g_gameStatus = GAMESTART;
	if (g_hero != NULL && g_hero->getIsMoving())
	{
		g_maplayer->heroResumeMoving();
	}
}

VipShopLayer* VipShopLayer::create()
{
	VipShopLayer *pRet = new VipShopLayer();
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

bool VipShopLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("vipShopLayer.csb");
	this->addChild(m_csbnode);

	std::vector<GoodsData*> vec_vipGoods;

	int goodsize = GlobalData::vec_goods.size();

	for (int i = 0; i < goodsize; i++)
	{
		GoodsData* vipgdata = &GlobalData::vec_goods[i];
		if (vipgdata->type == 2)
			vec_vipGoods.push_back(vipgdata);
	}

	m_vipScrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("vipgoodsscroll");
	m_vipScrollview->setScrollBarEnabled(false);
	m_vipScrollview->setBounceEnabled(true);

	int itemheight = 230;
	int innerheight = itemheight * vec_vipGoods.size();
	int contentheight = m_vipScrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_vipScrollview->setInnerContainerSize(Size(m_vipScrollview->getContentSize().width, innerheight));

	//去掉第一个6月月卡
	int starti = 1;
	std::map<std::string, int>::iterator it;

	for (it = GlobalData::map_buyVipDays.begin(); it != GlobalData::map_buyVipDays.end(); ++it)
	{
		if (it->first.find("vip1") != std::string::npos)
		{
			starti = 0;
		}
	}

	for (unsigned int i = starti; i < vec_vipGoods.size(); i++)
	{
		VipGoodsItem* node = VipGoodsItem::create(vec_vipGoods[i]);
		node->setTag(sizeof(heroprice) / sizeof(heroprice[0]) + sizeof(goldcount) / sizeof(goldcount[0]) + i);
		m_vipScrollview->addChild(node);
		node->setPosition(Vec2(360, innerheight - itemheight / 2 - (i - starti) * itemheight));
	}

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(VipShopLayer::onBack, this));

	if (GlobalData::g_gameStatus == GAMESTART)
		GlobalData::g_gameStatus = GAMEPAUSE;

	if (g_hero != NULL && g_hero->getIsMoving())
	{
		g_maplayer->heroPauseMoving();
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

void VipShopLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void VipShopLayer::onSuccess()
{
	this->removeChildByName("waitbox");
}

void VipShopLayer::onErr(int errcode)
{
	this->removeChildByName("waitbox");
}

void VipShopLayer::getLeftDays()
{
	WaitingProgress* waitbox = WaitingProgress::create("处理中...");
	this->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->vipIsOn(g_hero->getHeadID());
}
