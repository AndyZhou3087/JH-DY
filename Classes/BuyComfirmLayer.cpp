﻿#include "BuyComfirmLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "SoundManager.h"
#include "WXGoldGoodsItem.h"
#include "ShopLayer.h"
#include "HintBox.h"
#include "StorageUILayer.h"
#include "Const.h"
#include "AnalyticUtil.h"
#include "MD5.h"
#include "WXBuyOrangeGFLayer.h"
#include "GameDataSave.h"

BuyComfirmLayer::BuyComfirmLayer()
{
}


BuyComfirmLayer::~BuyComfirmLayer()
{

}

void BuyComfirmLayer::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}

bool BuyComfirmLayer::init(GoodsData* gdata)
{
	m_gdata = gdata;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("buyComfirmLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cacelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(BuyComfirmLayer::onCancel, this));

	cocos2d::ui::Button* revivebtn = (cocos2d::ui::Button*)csbnode->getChildByName("buybtn");
	revivebtn->addTouchEventListener(CC_CALLBACK_2(BuyComfirmLayer::onBuy, this));

	std::string desc;

	cocos2d::ui::Text* desctext = (cocos2d::ui::Text*)csbnode->getChildByName("desc");

	if (GlobalData::g_gameStatus == GAMESTART)
		desc = "闭关中！没有分身符无法进行此操作。";
	desc.append(StringUtils::format("%s%d%s%s%s", CommonFuncs::gbk2utf("是否立即花费").c_str(), m_gdata->price, CommonFuncs::gbk2utf("金元宝购买").c_str(), m_gdata->name.c_str(), CommonFuncs::gbk2utf("？").c_str()));
	desctext->setString(desc);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

time_t BuyComfirmLayer::getNowTime()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	return nowTimeval.tv_sec;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	struct tm* tm;
	time_t timep;
	time(&timep);
	return timep;
#endif
}


BuyComfirmLayer* BuyComfirmLayer::create(GoodsData* gdata)
{
	BuyComfirmLayer *pRet = new BuyComfirmLayer();
	if (pRet && pRet->init(gdata))
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

long long BuyComfirmLayer::getNowTimeMs() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	return ((long long)(nowTimeval.tv_sec)) * 1000 + nowTimeval.tv_usec / 1000;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);

	return (double)tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

void BuyComfirmLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

bool BuyComfirmLayer::isBeforeToday(time_t sec) {
	struct tm *tm;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	//win32平台
	time_t timep;
	time(&timep);
	tm = localtime(&timep);
#else  
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	tm = localtime(&nowTimeval.tv_sec);
#endif  

	struct tm * otherDay = gmtime(&sec);

	if (otherDay->tm_year < tm->tm_year) {
		return true;
	}
	else if (otherDay->tm_year > tm->tm_hour) {
		return false;
	}

	if (otherDay->tm_mon < tm->tm_mon) {
		return true;
	}
	else if (otherDay->tm_mon > tm->tm_mon) {
		return false;
	}

	if (otherDay->tm_mday < tm->tm_mday) {
		return true;
	}
	else if (otherDay->tm_mday > tm->tm_mday) {
		return false;
	}

	return false;
}

void BuyComfirmLayer::onBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		int mygold = GlobalData::getMyGoldCount();
		if (mygold >= m_gdata->price)
		{
			if (GlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
			{
				GlobalData::dataIsModified = true;
				GlobalData::setMyGoldCount(0);
				HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
				this->addChild(hint);
				return;
			}

			SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUYOK);
			GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - m_gdata->price);

			int usegold = GlobalData::getUseGold() + m_gdata->price;
			GlobalData::setUseGold(usegold);

			if (m_gdata->icon.compare("gp6") == 0)
			{
				WXBuyOrangeGFLayer* buygflayer = WXBuyOrangeGFLayer::create();
				Director::getInstance()->getRunningScene()->addChild(buygflayer);
				m_gdata->vec_res = buygflayer->getRandQu5Gf();
			}

			if (m_gdata->icon.compare("csex") == 0)
			{
				changeSex();
			}
			else
				WXGoldGoodsItem::addBuyGoods(m_gdata);

			StorageUILayer* storageUI = (StorageUILayer*)g_gameLayer->getChildByName("storageuilayer");
			if (storageUI != NULL)
				storageUI->updateResContent();
#ifdef ANALYTICS
			std::string name = StringUtils::format("b%s", m_gdata->icon.c_str());
			AnalyticUtil::onEvent(name.c_str());
#endif
		}
		else
		{
			if (GlobalData::g_gameStatus == GAMESTART)
			{
				Director::getInstance()->getRunningScene()->addChild(ShopLayer::create(), 1000);
			}
			else
			{
				Director::getInstance()->getRunningScene()->addChild(HintBox::create(CommonFuncs::gbk2utf("金元宝不足！！")), 1000);
			}
		}
		this->removeFromParentAndCleanup(true);
	}
}

long long BuyComfirmLayer::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}

void BuyComfirmLayer::changeSex()
{
	if (g_hero->getSex() == S_NONE)
	{
		g_hero->setSex(S_MAN);
		GameDataSave::getInstance()->setHeroSex(S_MAN);
		bool istakeoff = false;

		PackageData* carryData = NULL;
		if (g_hero->getAtrByType(H_WG)->count > 0 && (g_hero->getAtrByType(H_WG)->strid.compare("w040") == 0 || g_hero->getAtrByType(H_WG)->strid.compare("w022") == 0))
		{
			carryData = g_hero->getAtrByType(H_WG);
		}
		else
		{
			std::string mymixgf = GlobalData::getMixGF();
			if (mymixgf.length() > 0)
			{
				MixGfData mdata = GlobalData::map_MixGfData[mymixgf];
				if (g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WG)->strid.compare(mdata.mastergf) == 0 && mdata.sex == 0)
				{
					carryData = g_hero->getAtrByType(H_WG);
				}
				else if (g_hero->getAtrByType(H_NG)->count > 0 && g_hero->getAtrByType(H_NG)->strid.compare(mdata.mastergf) == 0 && mdata.sex == 0)
				{
					carryData = g_hero->getAtrByType(H_NG);
				}
			}
		}
		std::string str = CommonFuncs::gbk2utf("已恢复男儿身！！");
		if (carryData != NULL)
		{
			if (StorageRoom::getCountById(carryData->strid) <= 0)
			{
				StorageRoom::add(*carryData);
				carryData->count = 0;
				str = CommonFuncs::gbk2utf("恢复男儿身！！【%s】无法使用，已放置到仓库中。");
				str = StringUtils::format(str.c_str(), GlobalData::map_allResource[carryData->strid].cname.c_str());
			}
		}

		HintBox* box = HintBox::create(str);
		g_gameLayer->addChild(box, 5);
	}
}