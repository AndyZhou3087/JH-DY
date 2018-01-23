#include "ResDetailsLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "Hero.h"
#include "StorageUILayer.h"
#include "SoundManager.h"
#include "BuyComfirmLayer.h"
#include "Const.h"
#include "GameDataSave.h"
#include "ShopLayer.h"
#include "OutDoor.h"
#include "ExchangeLayer.h"

#define WINESTRID "23"
#define GRASSRID "5"

int ResDetailsLayer::whereClick = 0;//0--仓库，1其他，2出门长按资源按钮，3长按交易中我的物品

ResDetailsLayer::ResDetailsLayer()
{
	m_expendtime = 0;
}

ResDetailsLayer::~ResDetailsLayer()
{
	whereClick = 0;
}

bool ResDetailsLayer::init(PackageData* pdata)
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 160));
	this->addChild(color);

	m_packageData = pdata;
	m_csbnode = CSLoader::createNode("resDetailsLayer.csb");
	this->addChild(m_csbnode);

	m_okbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("okbtn");
	m_okbtn->addTouchEventListener(CC_CALLBACK_2(ResDetailsLayer::onOk, this));

	cocos2d::ui::Button* usebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("usebtn");
	usebtn->addTouchEventListener(CC_CALLBACK_2(ResDetailsLayer::onUse, this));

	uselbl = (cocos2d::ui::Text*)usebtn->getChildByName("valuelbl");

	cocos2d::ui::Text* oklabel = (cocos2d::ui::Text*)m_okbtn->getChildByName("oklabel");

	if ((pdata->type == FOOD || (pdata->type == MEDICINAL && pdata->strid.compare(WINESTRID) != 0)) && whereClick == 0)
		oklabel->setString(CommonFuncs::gbk2utf("使用"));
	else if (pdata->type == RES_1 && pdata->strid.compare(GRASSRID) == 0 && whereClick == 0)
	{
		oklabel->setString(CommonFuncs::gbk2utf("喂马"));
		updateHorseData(0);
	}

	cocos2d::ui::Text* resname = (cocos2d::ui::Text*)m_csbnode->getChildByName("namelbl");

	resname->setString(GlobalData::map_allResource[pdata->strid].cname);

	lefttimelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("lefttime");

	cocos2d::ui::Text* qulbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("qulbl");

	cocos2d::ui::ImageView* resbox = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("buildsmall");
	std::string qustr = "ui/buildsmall.png";
	int qu = -1;
	if (pdata->type == WEAPON || pdata->type == PROTECT_EQU)
	{
		qu = GlobalData::map_equips[pdata->strid].qu;

		if (pdata->slv > 0)
		{
			cocos2d::ui::Text* slvlbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("slvlbl");
			std::string slvstr = StringUtils::format("+%d", pdata->slv);
			slvlbl->setPositionX(resname->getPositionX() + resname->getContentSize().width / 2 + 10);
			slvlbl->setString(slvstr);
		}
	}
	else if (pdata->type == N_GONG || pdata->type == W_GONG)
	{
		qu = GlobalData::map_wgngs[pdata->strid].qu;
	}
	if (qu > 0)
	{
		qustr = StringUtils::format("ui/qubox%d.png", qu);
		qulbl->setVisible(true);
		qulbl->setString(CommonFuncs::gbk2utf(qudesc[qu - 1].c_str()));
		qulbl->setTextColor(qucolor[qu - 1]);
	}

	resbox->loadTexture(qustr, cocos2d::ui::TextureResType::PLIST);
	resbox->setContentSize(Sprite::createWithSpriteFrameName(qustr)->getContentSize());

	std::string mymixgf = GlobalData::getMixGF();
	MixGfData mdata = GlobalData::map_MixGfData[mymixgf];
	if (mymixgf.length() > 0)
	{
		if (pdata->strid.compare(mdata.mastergf) == 0)
		{
			Sprite * mixtag = Sprite::createWithSpriteFrameName("ui/mixtag.png");
			mixtag->setPosition(Vec2(resbox->getContentSize().width - 15, resbox->getContentSize().height - 15));
			resbox->addChild(mixtag);
		}
	}

	cocos2d::ui::ImageView* resimg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("Image");

	std::string str = StringUtils::format("ui/%s.png", pdata->strid.c_str());
	resimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	resimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	cocos2d::ui::Text* atkdftext = (cocos2d::ui::Text*)m_csbnode->getChildByName("atkdftext");

	cocos2d::ui::Text* crittext = (cocos2d::ui::Text*)m_csbnode->getChildByName("crittext");

	valuelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("valuelbl");
	cocos2d::ui::Text* slvatk = (cocos2d::ui::Text*)m_csbnode->getChildByName("slvatk");

	cocos2d::ui::Text* skilltext = (cocos2d::ui::Text*)m_csbnode->getChildByName("skilltext");

	int count = StorageRoom::getCountById(pdata->strid);

	if (whereClick == 1)
	{
		for (unsigned i = 0; i < MyPackage::vec_packages.size(); i++)
		{
			if (MyPackage::vec_packages[i].strid.compare(pdata->strid) == 0)
			{
				count += MyPackage::vec_packages[i].count;
				break;
			}
		}
	}
	else if (whereClick == 2 || whereClick == 3 || whereClick == 4)
	{
		if (atoi(pdata->strid.c_str()) > 0)
		{
			Node* selectCountNode = (Node*)m_csbnode->getChildByName("selectcountnode");
			selectCountNode->setVisible(true);
			selectCountlbl = (cocos2d::ui::Text*)selectCountNode->getChildByName("rescountlbl");

			slider = (cocos2d::ui::Slider*)selectCountNode->getChildByName("slider");
			slider->setPercent(0);
			slider->addEventListener(CC_CALLBACK_2(ResDetailsLayer::sliderEvent, this));

			cocos2d::ui::Button* addone = (cocos2d::ui::Button*)selectCountNode->getChildByName("addone");
			addone->addTouchEventListener(CC_CALLBACK_2(ResDetailsLayer::onAddOne, this));

			cocos2d::ui::Button* minusone = (cocos2d::ui::Button*)selectCountNode->getChildByName("minusone");
			minusone->addTouchEventListener(CC_CALLBACK_2(ResDetailsLayer::onMinusOne, this));
		}
	}

	std::string countstr;

	if (pdata->type == FOOD || pdata->type == MEDICINAL || pdata->type == RES_1 || pdata->type == RES_2)
	{
		countstr = StringUtils::format("库存%d", count);

		if (whereClick == 3 || whereClick == 4)
		{
			ExchangeLayer* exlayer = (ExchangeLayer*)g_gameLayer->getChildByName("exchangelayer");
			if (exlayer != NULL)
			{
				countstr = StringUtils::format("库存%d", exlayer->getCountByResId(pdata->strid, whereClick - 3));
			}
		}
	}
	else if (pdata->type == WEAPON || pdata->type == PROTECT_EQU || pdata->type == TOOLS)
	{
		if (g_hero->getMeHas(pdata->strid) != NULL)
			countstr = StringUtils::format("耐久度%d%%", pdata->goodvalue);

		std::string tmpstr;
		std::string sstr;
		if (pdata->type == WEAPON)
		{
			int eatk = GlobalData::map_equips[pdata->strid].atk;
			tmpstr = StringUtils::format("攻击力增加%d", eatk);

			int slv = 0;

			PackageData* hasdata = g_hero->getMeHas(pdata->strid);
			if (hasdata != NULL)
			{
				slv = hasdata->slv;
			}
			int satk = slv * (slv + eatk / 10);

			if (satk > 0)
			{
				sstr = StringUtils::format("+%d", satk);
				slvatk->setVisible(true);
			}
		}
		else if (pdata->type == PROTECT_EQU)
		{
			int pdf = GlobalData::map_equips[pdata->strid].df;
			tmpstr = StringUtils::format("防御力增加%d", pdf);

			int slv = 0;
			PackageData* hasdata = g_hero->getMeHas(pdata->strid);
			if (hasdata != NULL)
			{
				slv = hasdata->slv;
			}
			int sdf = slv * (slv + pdf / 10);
			if (sdf > 0)
			{
				sstr = StringUtils::format("+%d", sdf);
				slvatk->setVisible(true);
			}
		}
		if (tmpstr.length() > 0)
			atkdftext->setString(CommonFuncs::gbk2utf(tmpstr.c_str()));

		slvatk->setPositionX(atkdftext->getPositionX() + atkdftext->getContentSize().width/2 + 5);
		slvatk->setString(sstr);
			
	}
	else if (pdata->type == N_GONG || pdata->type == W_GONG)
	{
		int lv = pdata->lv + 1;
		countstr = StringUtils::format("功法等级 %d/%d", lv, GlobalData::map_wgngs[pdata->strid].maxlv);

		std::string critstr;
		if (pdata->type == N_GONG)
		{
			critstr = StringUtils::format("闪避率增加%.4f%%", GlobalData::map_wgngs[pdata->strid].vec_dodge[pdata->lv]);
			 
		}
		else
		{
			critstr = StringUtils::format("暴击率增加%.4f%%", GlobalData::map_wgngs[pdata->strid].vec_cirt[pdata->lv]);
		}
		crittext->setString(CommonFuncs::gbk2utf(critstr.c_str()));

		int skilltype = GlobalData::map_wgngs[pdata->strid].skilltype;
		if (skilltype > S_SNONE)
		{
			std::string skilldesc = StringUtils::format(GlobalData::map_gfskills[skilltype].desc.c_str(), GlobalData::map_wgngs[pdata->strid].vec_skrnd[pdata->lv], GlobalData::map_wgngs[pdata->strid].skilleffect);
			std::string skillstr = StringUtils::format("%s%s(%s)", CommonFuncs::gbk2utf("技能：").c_str(), GlobalData::map_gfskills[skilltype].name.c_str(), skilldesc.c_str());
			skilltext->setString(skillstr);
		}
	}
	if (pdata->type == RES_2)
	{
		if (pdata->strid.compare("70") == 0)
		{
			usebtn->setVisible(true);

			m_okbtn->setPositionX(460);
			std::string uselblstr = StringUtils::format("经验药水 x%d", StorageRoom::getCountById("70"));
			uselbl->setString(CommonFuncs::gbk2utf(uselblstr.c_str()));
			m_expendtime = GameDataSave::getInstance()->getHeroExpEndTime();
			updataLeftTime(0);
			this->schedule(schedule_selector(ResDetailsLayer::updataLeftTime), 1);
		}
		else if (pdata->strid.compare("71") == 0)
		{
			usebtn->setVisible(true);
			m_okbtn->setPositionX(460);
			std::string uselblstr = StringUtils::format("大力丸 x%d", StorageRoom::getCountById("71"));
			uselbl->setString(CommonFuncs::gbk2utf(uselblstr.c_str()));
			m_expendtime = GameDataSave::getInstance()->getGfEndTime();
			updataLeftTime(0);
			this->schedule(schedule_selector(ResDetailsLayer::updataLeftTime), 1);
		}
		else if (pdata->strid.compare("74") == 0)
		{
			countstr = StringUtils::format("生命%d", pdata->goodvalue);
		}
		else if (pdata->extype == 4)
			countstr = "永久";
	}

	valuelbl->setString(CommonFuncs::gbk2utf(countstr.c_str()));

	cocos2d::ui::Text* resdesc = (cocos2d::ui::Text*)m_csbnode->getChildByName("desclbl");
	resdesc->setString(GlobalData::map_allResource[pdata->strid].desc);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		removSelf();
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

ResDetailsLayer* ResDetailsLayer::create(PackageData* pdata)
{
	ResDetailsLayer *pRet = new ResDetailsLayer();
	if (pRet && pRet->init(pdata))
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

ResDetailsLayer* ResDetailsLayer::createByResId(std::string resid)
{
	PackageData sdata;
	bool isInRes = false;
	for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
	{
		if (resid.compare(GlobalData::vec_resData[i].strid) == 0)
		{
			isInRes = true;
			sdata.strid = resid;
			sdata.type = GlobalData::vec_resData[i].type - 1;
			break;
		}
	}
	if (!isInRes)
	{
		PackageData* pdata = g_hero->getMeHas(resid);
		if (pdata != NULL)
		{
			isInRes = true;
			sdata = *pdata;
		}
		else
		{
			std::map<std::string, std::vector<BuildActionData>>::iterator it;
			for (it = GlobalData::map_buidACData.begin(); it != GlobalData::map_buidACData.end(); ++it)
			{
				for (unsigned int i = 0; i < GlobalData::map_buidACData[it->first].size(); i++)
				{
					if (resid.compare(GlobalData::map_buidACData[it->first][i].icon) == 0)
					{
						isInRes = true;
						sdata.strid = resid;
						sdata.type = GlobalData::map_buidACData[it->first][i].type - 1;
						break;
					}
				}
			}
		}
	}

	if (isInRes)
	{
		return create(&sdata);
	}
	return NULL;
}

void ResDetailsLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		StorageUILayer* storageUI = (StorageUILayer*)this->getParent();

		if (whereClick == 0)
		{
			if (GlobalData::isExercising() && !GlobalData::isHasFSF() && m_okbtn->getTitleText().compare(CommonFuncs::gbk2utf("使用")) == 0)
			{
				int index = -1;
				for (unsigned int i = 0; i < GlobalData::vec_goods.size(); i++)
				{
					if (GlobalData::vec_goods[i].icon.compare("72") == 0)
					{
						index = i;
						break;
					}
				}
				if (index >= 0)
				{
					BuyComfirmLayer* layer = BuyComfirmLayer::create(&GlobalData::vec_goods[index]);
					Director::getInstance()->getRunningScene()->addChild(layer, 1000, "buycomfirmlayer");
				}
				return;
			}

			if (m_packageData->type == FOOD)
			{
				bool isInres = false;
				for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
				{
					if (m_packageData->strid.compare(GlobalData::vec_resData[i].strid) == 0)
					{
						isInres = true;
						int addvalue = GlobalData::vec_resData[i].ep[0];
						recoveHungerValue(addvalue);
						int r = 0;
						if (m_packageData->strid.compare("6") == 0 || m_packageData->strid.compare("9") == 0)
						{
							r = GlobalData::createRandomNum(100);
						}
						
						if (r >= 50)
						{
							g_hero->setInnerinjuryValue(g_hero->getInnerinjuryValue() - 10.0f);
						}

						StorageRoom::use(m_packageData->strid);
						break;
					}
				}

				if (!isInres)
				{
					for (unsigned int i = 0; i < GlobalData::map_buidACData["cooktable"].size(); i++)
					{
						if (m_packageData->strid.compare(GlobalData::map_buidACData["cooktable"][i].icon) == 0)
						{
							int addvalue = GlobalData::map_buidACData["cooktable"][i].ep[0];
							recoveHungerValue(addvalue);

							StorageRoom::use(m_packageData->strid);
							break;
						}
					}
				}
				storageUI->updateResContent();
			}
			else if (m_packageData->type == MEDICINAL)
			{
				bool isInRes = false;
				for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
				{
					if (m_packageData->strid.compare(GlobalData::vec_resData[i].strid) == 0)
					{
						isInRes = true;
						int wvalue = GlobalData::vec_resData[i].ep[0];
						int nvalue = GlobalData::vec_resData[i].ep[1];
						recoveInjuryValue(wvalue, nvalue);
						StorageRoom::use(m_packageData->strid);
						break;
					}
				}
				if (!isInRes)
				{
					for (unsigned int i = 0; i < GlobalData::map_buidACData["medicinekit"].size(); i++)
					{
						if (m_packageData->strid.compare(GlobalData::map_buidACData["medicinekit"][i].icon) == 0)
						{
							int wvalue = GlobalData::map_buidACData["medicinekit"][i].ep[0];
							int nvalue = GlobalData::map_buidACData["medicinekit"][i].ep[1];
							recoveInjuryValue(wvalue, nvalue);
							StorageRoom::use(m_packageData->strid);
							break;
						}
					}
				}
				storageUI->updateResContent();
			}
			else if (m_packageData->type == RES_1 && m_packageData->strid.compare(GRASSRID) == 0)
			{
				StorageRoom::use(m_packageData->strid);

				int count = StorageRoom::getCountById(GRASSRID);
				if (count <= 0)
				{
					m_okbtn->setEnabled(false);
				}

				cocos2d::ui::Text* valuelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("valuelbl");
				std::string countstr = StringUtils::format("库存%d", count);
				valuelbl->setString(CommonFuncs::gbk2utf(countstr.c_str()));

				updateHorseData(1);
				storageUI->updateResContent();
				return;
			}

		}
		else if (whereClick == 2)
		{
			if (atoi(m_packageData->strid.c_str()) > 0)
			{
				OutDoor* Olayer = (OutDoor*)g_gameLayer->getChildByName("OutDoor");
				int usecount = atoi(selectCountlbl->getString().c_str());
				if (Olayer != NULL)
				{
					for (int i = 0; i < usecount; i++)
					{
						Olayer->takeout(m_packageData);
					}
					Olayer->updata();
				}
			}
		}
		else if (whereClick == 3 || whereClick == 4)
		{
			std::string resstrid = m_packageData->strid;
			if (atoi(resstrid.c_str()) > 0)
			{
				ExchangeLayer* exlayer = (ExchangeLayer*)g_gameLayer->getChildByName("exchangelayer");
				int usecount = atoi(selectCountlbl->getString().c_str());
				if (exlayer != NULL)
				{
					for (int i = 0; i < usecount; i++)
					{
						if (whereClick == 3)
							exlayer->giveNpc(resstrid);
						else
							exlayer->giveHero(resstrid);
					}
					exlayer->updata();
				}
			}
		}
		
		removSelf();
	}
}

void ResDetailsLayer::onUse(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Text* valuelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("valuelbl");
		if (m_packageData->strid.compare("71") == 0)
		{
			int count = StorageRoom::getCountById("71");
			if (count > 0)
			{
				StorageRoom::use("71");
				std::string uselblstr = StringUtils::format("大力丸 x%d", count - 1);
				uselbl->setString(CommonFuncs::gbk2utf(uselblstr.c_str()));
				std::string leftstr = StringUtils::format("库存%d", count - 1);
				valuelbl->setString(CommonFuncs::gbk2utf(leftstr.c_str()));
				int gfetime = GameDataSave::getInstance()->getGfEndTime();
				int cursectime = GlobalData::getSysSecTime();
				if (cursectime <= gfetime)
				{
					GameDataSave::getInstance()->setGfEndTime(gfetime + 24 * 3600);
					m_expendtime = gfetime + 24 * 3600;
				}
				else
				{
					GameDataSave::getInstance()->setGfEndTime(cursectime + 24 * 3600);
					m_expendtime = cursectime + 24 * 3600;
				}
				updateUI();
			}
			if (count - 1 <= 0)
			{
				this->removeFromParentAndCleanup(true);
			}
		}
		else if (m_packageData->strid.compare("70") == 0)
		{
			int count = StorageRoom::getCountById("70");
			if (count > 0)
			{
				StorageRoom::use("70");
				std::string uselblstr = StringUtils::format("经验药水 x%d", count - 1);
				uselbl->setString(CommonFuncs::gbk2utf(uselblstr.c_str()));

				std::string leftstr = StringUtils::format("库存%d", count - 1);
				valuelbl->setString(CommonFuncs::gbk2utf(leftstr.c_str()));

				int heorexpetime = GameDataSave::getInstance()->getHeroExpEndTime();
				int cursectime = GlobalData::getSysSecTime();
				if (GlobalData::getSysSecTime() <= heorexpetime)
				{
					GameDataSave::getInstance()->setHeroExpEndTime(heorexpetime + 24 * 3600);
					m_expendtime = heorexpetime + 24 * 3600;
				}
				else
				{
					GameDataSave::getInstance()->setHeroExpEndTime(cursectime + 24 * 3600);
					m_expendtime = cursectime + 24 * 3600;
				}
				updateUI();
			}
			if (count - 1 <= 0)
			{
				this->removeFromParentAndCleanup(true);
			}
		}
	}
}

void ResDetailsLayer::updateUI()
{
	updataLeftTime(0);
	this->schedule(schedule_selector(ResDetailsLayer::updataLeftTime), 1);

	StorageUILayer* storageUI = (StorageUILayer*)this->getParent();
	storageUI->updateResContent();
}

void ResDetailsLayer::recoveInjuryValue(int addwvalue, int addnvalue)
{
	float outvalue = g_hero->getOutinjuryValue();
	if (addwvalue + outvalue > g_hero->getMaxOutinjuryValue())
		g_hero->setOutinjuryValue(g_hero->getMaxOutinjuryValue());
	else
		g_hero->recoverOutjury(addwvalue);
	float invalue = g_hero->getInnerinjuryValue();
	if (invalue + addnvalue > g_hero->getMaxInnerinjuryValue())
		g_hero->setInnerinjuryValue(g_hero->getMaxInnerinjuryValue());
	else
		g_hero->recoverInjury(addnvalue);
}

void ResDetailsLayer::recoveHungerValue(int addvalue)
{
	float hungervale = g_hero->getHungerValue();
	if (addvalue + hungervale > g_hero->getMaxHungerValue())
		g_hero->setHungerValue(g_hero->getMaxHungerValue());
	else
		g_hero->recoverHunger(addvalue);
}

void ResDetailsLayer::removSelf()
{
	StorageUILayer* storagelayer = (StorageUILayer*)g_gameLayer->getChildByName("storageuilayer");
	if (storagelayer != NULL)
		storagelayer->showNewerGuide(68);
	this->removeFromParentAndCleanup(true);
}

void ResDetailsLayer::updateHorseData(int addvalue)
{
	PackageData* horseData = NULL;
	if (g_hero->getAtrByType(H_MOUNT)->count > 0)
		horseData = g_hero->getAtrByType(H_MOUNT);
	else
	{
		bool isfind = false;
		std::map<int, std::vector<PackageData>>::iterator it;
		for (it = StorageRoom::map_storageData.begin(); it != StorageRoom::map_storageData.end(); ++it)
		{
			for (unsigned int i = 0; i < StorageRoom::map_storageData[it->first].size(); i++)
			{
				PackageData *sdata = &StorageRoom::map_storageData[it->first][i];
				if (sdata->strid.compare("74") == 0)
				{
					isfind = true;
					horseData = sdata;
					break;
				}
			}
		}
		if (!isfind)
		{
			for (int i = 0; i < MyPackage::getSize(); i++)
			{
				if (MyPackage::vec_packages[i].strid.compare("74") == 0)
				{
					horseData =  &MyPackage::vec_packages[i];
					break;
				}
			}
		}
	}
	if (horseData != NULL)
	{
		cocos2d::ui::Text* horselbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("horselbl");
		horselbl->setVisible(true);
		horseData->goodvalue += addvalue;
		if (horseData->goodvalue > 100)
			horseData->goodvalue = 100;
		std::string temp = horselbl->getString().c_str();
		std::string horesstr = StringUtils::format(CommonFuncs::gbk2utf("白马（生命%d）").c_str(), horseData->goodvalue);
		horselbl->setString(horesstr);
	}
}

void ResDetailsLayer::updataLeftTime(float dt)
{
	int letftime = m_expendtime - GlobalData::getSysSecTime();
	if (letftime > 0)
	{
		std::string str;
		int day = letftime / 86400;
		int sectime = letftime % 86400;
		if (day > 0)
			str = StringUtils::format("剩%d天%02d:%02d:%02d", day, sectime / 3600, sectime % 3600 / 60, sectime % 3600 % 60);
		else
			str = StringUtils::format("剩%02d:%02d:%02d", sectime / 3600, sectime % 3600 / 60, sectime % 3600 % 60);
		lefttimelbl->setString(CommonFuncs::gbk2utf(str.c_str()));
		lefttimelbl->setVisible(true);
	}
	else
	{
		lefttimelbl->setVisible(false);
	}
}

void ResDetailsLayer::sliderEvent(Ref * pSender, cocos2d::ui::Slider::EventType type)
{
	if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED)            //进度条的值发生变化
	{
		cocos2d::ui::Slider* slider = (cocos2d::ui::Slider*)pSender;
		int percent = slider->getPercent();
		int max = MyPackage::canTakeCount(m_packageData);
		int tcount = m_packageData->count;
		if (whereClick == 3 || whereClick == 4)
		{
			ExchangeLayer* exlayer = (ExchangeLayer*)g_gameLayer->getChildByName("exchangelayer");
			if (exlayer != NULL)
			{
				max = exlayer->getCountByResId(m_packageData->strid, whereClick - 3);
				tcount = max;
			}
		}
		int selectcount = max * percent/100;
		std::string str = StringUtils::format("%d", selectcount);
		selectCountlbl->setString(str);
		str = StringUtils::format("库存%d", tcount - selectcount);
		valuelbl->setString(CommonFuncs::gbk2utf(str.c_str()));
	}
}

void ResDetailsLayer::onAddOne(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		int max = MyPackage::canTakeCount(m_packageData);
		int tcount = m_packageData->count;
		if (whereClick == 3 || whereClick == 4)
		{
			ExchangeLayer* exlayer = (ExchangeLayer*)g_gameLayer->getChildByName("exchangelayer");
			if (exlayer != NULL)
			{
				max = exlayer->getCountByResId(m_packageData->strid, whereClick - 3);
				tcount = max;
			}
		}
		int curcount = atoi(selectCountlbl->getString().c_str());
		curcount++;
		if (curcount > max)
			curcount = max;
		std::string str = StringUtils::format("%d", curcount);
		selectCountlbl->setString(str);
		str = StringUtils::format("库存%d", tcount - curcount);
		valuelbl->setString(CommonFuncs::gbk2utf(str.c_str()));
		int percent = curcount * 100 / max;
		slider->setPercent(percent);

	}
}
void ResDetailsLayer::onMinusOne(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		int curcount = atoi(selectCountlbl->getString().c_str());
		int max = MyPackage::canTakeCount(m_packageData);
		int tcount = m_packageData->count;
		if (whereClick == 3 || whereClick == 4)
		{
			ExchangeLayer* exlayer = (ExchangeLayer*)g_gameLayer->getChildByName("exchangelayer");
			if (exlayer != NULL)
			{
				max = exlayer->getCountByResId(m_packageData->strid, whereClick - 3);
				tcount = max;
			}
		}

		curcount--;
		if (curcount < 0)
			curcount = 0;
		std::string str = StringUtils::format("%d", curcount);
		selectCountlbl->setString(str);
		str = StringUtils::format("库存%d", tcount - curcount);
		valuelbl->setString(CommonFuncs::gbk2utf(str.c_str()));
		int percent = curcount * 100 / max;
		slider->setPercent(percent);
	}
}

