#include "TempStorageLayer.h"
#include "json.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "MyPackage.h"
#include "Const.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "MyMenu.h"

TempStorageLayer::TempStorageLayer()
{
}


TempStorageLayer::~TempStorageLayer()
{
}

TempStorageLayer* TempStorageLayer::create(std::string addrname)
{
	TempStorageLayer *pRet = new TempStorageLayer();
	if (pRet && pRet->init(addrname))
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

bool TempStorageLayer::init(std::string addrname)
{
	Node* csbnode = CSLoader::createNode("tempStorageLayer.csb");
	this->addChild(csbnode);
	m_addrname = addrname;

	MapData mdata = GlobalData::map_maps[addrname];

	cocos2d::ui::Text* title = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	title->setString(mdata.cname);

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(TempStorageLayer::onBack, this));

	cocos2d::ui::Button* getallbtn = (cocos2d::ui::Button*)csbnode->getChildByName("allgetbtn");
	getallbtn->addTouchEventListener(CC_CALLBACK_2(TempStorageLayer::onAllGet, this));

	m_scrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");
	m_scrollView->setScrollBarEnabled(false);
	m_scrollView->setBounceEnabled(true);

	loadTempData();

	updata();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void TempStorageLayer::onRewardItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();

	int size = tempResData.size();

	int count = data->count - 1;
	if (count <= 0)
	{
		std::vector<PackageData>::iterator it;
		for (it = tempResData.begin(); it != tempResData.end(); ++it)
		{
			if (it->strid.compare(data->strid) == 0)
			{
				PackageData pdata = *data;
				pdata.count = 1;
				if (MyPackage::add(pdata) == 0)
				{
					data->count--;
					tempResData.erase(it);
				}
				break;
			}
		}
	}
	else
	{
		PackageData pdata = *data;
		pdata.count = 1;
		if (MyPackage::add(pdata) == 0)
		{
			data->count--;
		}
	}

	for (int i = 0; i < size; i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		m_scrollView->removeChildByName(name);
	}

	saveTempData();
	updata();
}

void TempStorageLayer::onPackageItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	int index = node->getTag();
	PackageData data = MyPackage::vec_packages[index];
	unsigned int i = 0;
	for (i = 0; i < tempResData.size(); i++)
	{
		if (data.strid == tempResData[i].strid && (tempResData[i].type == FOOD || tempResData[i].type == MEDICINAL || tempResData[i].type == RES_1 || tempResData[i].type == RES_2))
		{
			tempResData[i].count++;
			break;
		}
	}

	if (i == tempResData.size())
	{
		data.count = 1;
		tempResData.push_back(data);
	}
	saveTempData();
	MyPackage::cutone(data.strid);

	for (unsigned int i = 0; i < tempResData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		m_scrollView->removeChildByName(name);
	}

	updata();
}


void TempStorageLayer::updataTempUI()
{
	int tempsize = tempResData.size();
	int itemheight = 125;
	int row = tempsize % 5 == 0 ? tempsize / 5 : (tempsize / 5 + 1);
	int innerheight = itemheight * row;
	int contentheight = m_scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_scrollView->setInnerContainerSize(Size(650, innerheight));

	for (int i = 0; i < tempsize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = tempResData[i];
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata.strid].qu);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata.strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(TempStorageLayer::onRewardItem, this));
		boxItem->setTag(i);
		boxItem->setUserData(&tempResData[i]);
		boxItem->setPosition(Vec2(70 + i % 5 * 128, innerheight - i / 5 * itemheight - itemheight / 2));
		MyMenu* menu = MyMenu::create();
		menu->addChild(boxItem);
		menu->setTouchlimit(m_scrollView);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		m_scrollView->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", tempResData[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", tempResData[i].count);
		Label * reslbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 18);//Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}

void TempStorageLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}


void TempStorageLayer::loadTempData()
{
	tempResData.clear();
	std::string datastr = GameDataSave::getInstance()->getTempStorage(m_addrname);
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
		tempResData.push_back(data);
	}
}

void TempStorageLayer::saveTempData()
{
	GlobalData::map_tempGf_Equip[m_addrname].clear();
	std::string str;
	for (unsigned int i = 0; i < tempResData.size(); i++)
	{
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", tempResData[i].strid.c_str(), tempResData[i].type, tempResData[i].count, tempResData[i].extype, tempResData[i].lv, tempResData[i].exp, tempResData[i].goodvalue, tempResData[i].slv, tempResData[i].tqu);
		str.append(onestr);

		int tmptype = tempResData[i].type;
		std::string tmpstrid = tempResData[i].strid;
		if (tmptype == W_GONG || tmptype == N_GONG || tmptype == WEAPON || tmptype == PROTECT_EQU)
		{
			GlobalData::map_tempGf_Equip[m_addrname].push_back(tmpstrid);
		}
	}
	GameDataSave::getInstance()->setTempStorage(m_addrname, str.substr(0, str.length() - 1));
}

void TempStorageLayer::onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		for (unsigned int i = 0; i < tempResData.size(); i++)
		{
			std::string name = StringUtils::format("resitem%d", i);
			m_scrollView->removeChildByName(name);
		}
		std::vector<PackageData>::iterator it;

		bool isfull = false;
		for (it = tempResData.begin(); it != tempResData.end();)
		{
			bool isOver = false;
			int count = it->count;
			for (int m = 0; m < count; m++)
			{
				PackageData data = *it;
				data.count = 1;
				if (MyPackage::add(data) == 0)
				{
					if (--it->count <= 0)
					{
						it = tempResData.erase(it);
						isOver = true;
						break;
					}
				}
				else
				{
					isfull = true;
					break;
				}
			}
			if (!isOver)
				it++;
			if (isfull)
				break;
		}
		saveTempData();
		updata();
	}
}

void TempStorageLayer::updata()
{
	//更新临时资源栏
	updataTempUI();
	//更新背包栏
	updataMyPackageUI();

}

void TempStorageLayer::updataMyPackageUI()
{
	for (int i = 0; i < MyPackage::getMax(); i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		this->removeChildByName(name);
	}

	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = MyPackage::vec_packages[i];
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata.strid].qu);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata.strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(TempStorageLayer::onPackageItem, this));
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(110 + i * 125, 225));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", MyPackage::vec_packages[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		str = StringUtils::format("%d", MyPackage::vec_packages[i].count);
		Label * reslbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 18);//Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}

void TempStorageLayer::onExit()
{
	saveTempData();
	Layer::onExit();
}