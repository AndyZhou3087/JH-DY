#include "WXGoldGoodsItem.h"
#include "CommonFuncs.h"
#include "ShopLayer.h"
#include "SoundManager.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "GameDataSave.h"
#include "BuyComfirmLayer.h"
#include "MapLayer.h"

WXGoldGoodsItem::WXGoldGoodsItem()
{
	isDraging = false;
}


WXGoldGoodsItem::~WXGoldGoodsItem()
{
}
void WXGoldGoodsItem::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec * 1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}
bool WXGoldGoodsItem::init(GoodsData* gdata)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_goodData = gdata;

	Node* csbnode = CSLoader::createNode("goldGoodsNode.csb");//物品节点
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	cocos2d::ui::ImageView* bgnode = (cocos2d::ui::ImageView*)csbnode->getChildByName("itembg");

	if (gdata->icon.compare("gp5") == 0 || gdata->icon.compare("gp6") == 0)
		bgnode->loadTexture("ui/specialshopitem.png", cocos2d::ui::Widget::TextureResType::PLIST);

	icon = (cocos2d::ui::ImageView*)bgnode->getChildByName("icon");
	nameTxt = (cocos2d::ui::Text*)bgnode->getChildByName("name");
	descTxt = (cocos2d::ui::Text*)bgnode->getChildByName("desc");
	priceTxt = (cocos2d::ui::Text*)bgnode->getChildByName("price");

	//图标
	std::string imagepath = StringUtils::format("ui/%s.png", gdata->icon.c_str());
	icon->loadTexture(imagepath, cocos2d::ui::TextureResType::PLIST);
	icon->setContentSize(Sprite::createWithSpriteFrameName(imagepath)->getContentSize());

	nameTxt->setString(gdata->name);
	descTxt->setString(gdata->desc);
	std::string pricestr = StringUtils::format("%d", gdata->price);
	priceTxt->setString(pricestr);

	cocos2d::ui::Button* bgbtn = (cocos2d::ui::Button*)csbnode->getChildByName("itembg");//整块节点击
	bgbtn->addTouchEventListener(CC_CALLBACK_2(WXGoldGoodsItem::onItem, this));
	bgbtn->setSwallowTouches(false);

	cocos2d::ui::Button* buybtn = (cocos2d::ui::Button*)csbnode->getChildByName("buybtn");
	buybtn->addTouchEventListener(CC_CALLBACK_2(WXGoldGoodsItem::onBuyBtn, this));
	buybtn->setSwallowTouches(false);
	return true;
}
time_t WXGoldGoodsItem::getNowTime()
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

WXGoldGoodsItem* WXGoldGoodsItem::create(GoodsData* gdata)
{
	WXGoldGoodsItem *pRet = new WXGoldGoodsItem();
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
void WXGoldGoodsItem::onBuyBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
}

long long WXGoldGoodsItem::getNowTimeMs() {
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

void WXGoldGoodsItem::onItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* node = (Node*)pSender;
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		startPos = node->convertToWorldSpace(this->getParent()->getPosition());
	}
	else if (type == ui::Widget::TouchEventType::MOVED)
	{
		Vec2 pos = node->convertToWorldSpace(this->getParent()->getPosition());
		if (fabsf(pos.y - startPos.y) > 20)
			isDraging = true;
	}
	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (!isDraging)
		{
			//购买
			//addBuyGoods(m_goodData);
			SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
			BuyComfirmLayer* layer = BuyComfirmLayer::create(m_goodData);
			Director::getInstance()->getRunningScene()->addChild(layer, 1000, "buycomfirmlayer");

		}
		isDraging = false;
	}
}


bool WXGoldGoodsItem::isBeforeToday(time_t sec) {
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
void WXGoldGoodsItem::addBuyGoods(GoodsData* gdata) 
{
	std::vector<std::string> payRes = gdata->vec_res;
	for (unsigned int i = 0; i < payRes.size(); i++)
	{
		int intRes = atoi(payRes[i].c_str());
		if (intRes != 0)
		{
			bool isfind = false;
			std::map<std::string, std::vector<BuildActionData>>::iterator it;
			for (it = GlobalData::map_buidACData.begin(); it != GlobalData::map_buidACData.end(); ++it)
			{
				std::vector<BuildActionData> vec_bactData = GlobalData::map_buidACData[it->first];

				for (unsigned int m = 0; m < vec_bactData.size(); m++)
				{
					BuildActionData bdata = vec_bactData[m];
					if (atoi(bdata.icon) == intRes / 1000)
					{
						isfind = true;
						PackageData pdata;
						pdata.strid = bdata.icon;
						pdata.count = intRes % 1000;
						pdata.type = bdata.type - 1;
						pdata.extype = bdata.extype;
						StorageRoom::add(pdata);
						break;
					}
				}
				if (isfind)
					break;
			}
			if (!isfind)
			{
				for (unsigned int n = 0; n < GlobalData::vec_resData.size(); n++)
				{
					ResData rdata = GlobalData::vec_resData[n];
					int rint = atoi(rdata.strid.c_str());
					if (rint == intRes / 1000)
					{
						PackageData pdata;
						pdata.strid = rdata.strid;
						pdata.count = intRes % 1000;
						pdata.type = rdata.type - 1;
						pdata.extype = rdata.actype;

						if (rint >= 75 && rint <= 78)
							updateDefaultStorage(pdata, g_hero->getHeadID());

						if (rint == 83)
                        {
                            if (g_maplayer != NULL)
                                g_maplayer->vultureAnim();
                        }

						StorageRoom::add(pdata);
						break;
					}
				}
			}
		}
		else
		{
			bool isfind = false;
			std::map<std::string, WG_NGData>::iterator it;
			for (it = GlobalData::map_wgngs.begin(); it != GlobalData::map_wgngs.end(); ++it)
			{
				WG_NGData gfdata = GlobalData::map_wgngs[it->first];
				if (payRes[i].compare(gfdata.id) == 0)
				{
					isfind = true;
					PackageData pdata;
					pdata.strid = gfdata.id;
					pdata.count = 1;
					pdata.lv = 0;
					pdata.type = gfdata.type - 1;
					pdata.extype = gfdata.extype;
					updateDefaultStorage(pdata,g_hero->getHeadID());
					if (!g_hero->checkifHasGF_Equip(payRes[i]) && GlobalData::tempHasGf_Equip(payRes[i]).length() <= 0)
					{
						StorageRoom::add(pdata);
						break;
					}
				}
			}

			if (!isfind)
			{
				std::map<std::string, EquipData>::iterator ite;
				for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
				{
					EquipData edata = GlobalData::map_equips[ite->first];
					if (payRes[i].compare(edata.id) == 0)
					{
						PackageData pdata;
						pdata.strid = edata.id;
						pdata.count = 1;
						pdata.type = edata.type - 1;
						pdata.goodvalue = 100;
						pdata.extype = edata.extype;
						updateDefaultStorage(pdata, g_hero->getHeadID());
						if (!g_hero->checkifHasGF_Equip(payRes[i]) && GlobalData::tempHasGf_Equip(payRes[i]).length() <= 0)
						{
							StorageRoom::add(pdata);
							break;
						}
					}
				}
			}
		}
	}
}


long long WXGoldGoodsItem::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec % 86400);
}


void WXGoldGoodsItem::updateDefaultStorage(PackageData pdata, int heroindex)
{
	std::vector<PackageData> vec_defaultStorage;
	std::string datastr = GlobalData::getDefaultStorage(heroindex);

	std::vector<std::string> vec_retstr;
	CommonFuncs::split(datastr, vec_retstr, ";");
	for (unsigned int i = 0; i < vec_retstr.size(); i++)
	{
		std::vector<std::string> tmp;
		CommonFuncs::split(vec_retstr[i], tmp, "-");
		PackageData data;
		data.strid = tmp[0];
		data.type = atoi(tmp[1].c_str());
		data.count = atoi(tmp[2].c_str());
		data.extype = GlobalData::getResExType(data.strid);//atoi(tmp[3].c_str());
		data.lv = atoi(tmp[4].c_str());
		data.exp = atoi(tmp[5].c_str());
		data.goodvalue = atoi(tmp[6].c_str());
		if (tmp.size() >= 9)
		{
			data.slv = atoi(tmp[7].c_str());
			data.tqu = atoi(tmp[8].c_str());
		}
		vec_defaultStorage.push_back(data);
	}

	bool ishas = false;
	for (unsigned int i = 0; i < vec_defaultStorage.size(); i++)
	{
		if (vec_defaultStorage[i].strid.compare(pdata.strid) == 0)
		{
			ishas = true;
			break;
		}
	}

	if (!ishas)
		vec_defaultStorage.push_back(pdata);

	std::string str;
	for (unsigned int i = 0; i < vec_defaultStorage.size(); i++)
	{
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", vec_defaultStorage[i].strid.c_str(), vec_defaultStorage[i].type, vec_defaultStorage[i].count, vec_defaultStorage[i].extype, vec_defaultStorage[i].lv, vec_defaultStorage[i].exp, vec_defaultStorage[i].goodvalue, vec_defaultStorage[i].slv, vec_defaultStorage[i].tqu);
		str.append(onestr);
	}
	GameDataSave::getInstance()->setModifyDefaultStorage(g_hero->getHeadID(), str.substr(0, str.length() - 1));

}
