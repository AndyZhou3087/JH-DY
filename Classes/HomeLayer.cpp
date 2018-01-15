#include "HomeLayer.h"
#include "json.h"
#include "Bed.h"
#include "Furnace.h"
#include "Forgingtable.h"
#include "CookTable.h"
#include "MedicineKit.h"
#include "WineTable.h"
#include "WineMaker.h"
#include "StorageRoom.h"
#include "Fence.h"
#include "ExerciseRoom.h"
#include "BuildingUILayer.h"
#include "StorageUILayer.h"
#include "OutDoor.h"
#include "GameScene.h"
#include "SoundManager.h"
#include "NewerGuideLayer.h"
#include "HintBox.h"
#include "CommonFuncs.h"
#include "BuyComfirmLayer.h"
#include "BookShelf.h"
#include "BookShelfLayer.h"

HomeLayer::HomeLayer()
{
}


HomeLayer::~HomeLayer()
{
}

bool HomeLayer::init()
{
	Node* csbnode = CSLoader::createNode("homeLayer.csb");
	this->addChild(csbnode);

	Building* bed = Bed::create();
	Vec_Buildings.push_back(bed);

	m_badfurnace = (cocos2d::ui::Widget*)csbnode->getChildByName("badfurnace");
	m_badfurnace->setLocalZOrder(1);

	m_badbookshelf = (cocos2d::ui::Widget*)csbnode->getChildByName("badbookshelf");
	m_badbookshelf->setLocalZOrder(1);

	MenuItemSprite* bedItem = MenuItemSprite::create(
		bed,
		bed,
		bed,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	bedItem->setName("bed");
	bedItem->setTag(0);
	bedItem->setPosition(Vec2(160, 700));
	bedItem->setOpacity(0);
	Menu* menu = Menu::create();
	menu->addChild(bedItem);
	menu->setPosition(Vec2(0, 0));
	csbnode->addChild(menu);

	Building* cooktable = CookTable::create();
	Vec_Buildings.push_back(cooktable);
	MenuItemSprite* cooktableItem = MenuItemSprite::create(
		cooktable,
		cooktable,
		cooktable,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	cooktableItem->setName("cooktable");
	cooktableItem->setTag(1);
	cooktableItem->setOpacity(0);
	cooktableItem->setPosition(Vec2(580, 786));
	menu->addChild(cooktableItem);

	Building* forgingtable = Forgingtable::create();
	Vec_Buildings.push_back(forgingtable);
	MenuItemSprite* forgingtableItem = MenuItemSprite::create(
		forgingtable,
		forgingtable,
		forgingtable,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	forgingtableItem->setName("forgingtable");
	forgingtableItem->setOpacity(0);
	forgingtableItem->setTag(2);
	forgingtableItem->setPosition(Vec2(195, 410));
	menu->addChild(forgingtableItem);

	Building* winemaker = WineMaker::create();
	Vec_Buildings.push_back(winemaker);
	MenuItemSprite* winemakerItem = MenuItemSprite::create(
		winemaker,
		winemaker,
		winemaker,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	winemakerItem->setName("winemaker");
	winemakerItem->setOpacity(0);
	winemakerItem->setTag(3);
	winemakerItem->setPosition(Vec2(445, 800));
	menu->addChild(winemakerItem);

	Building* medicinekit = MedicineKit::create();
	Vec_Buildings.push_back(medicinekit);
	MenuItemSprite* medicinekitItem = MenuItemSprite::create(
		medicinekit,
		medicinekit,
		medicinekit,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	medicinekitItem->setName("medicinekit");
	medicinekitItem->setOpacity(0);
	medicinekitItem->setTag(4);
	medicinekitItem->setPosition(Vec2(310, 650));
	menu->addChild(medicinekitItem);


	Building* furnace = Furnace::create();
	Vec_Buildings.push_back(furnace);
	MenuItemSprite* furnaceItem = MenuItemSprite::create(
		furnace,
		furnace,
		furnace,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	furnaceItem->setName("furnace");
	furnaceItem->setTag(5);
	furnaceItem->setOpacity(0);
	furnaceItem->setPosition(Vec2(125, 613));
	menu->addChild(furnaceItem);

	Building* winetable = WineTable::create();
	Vec_Buildings.push_back(winetable);
	MenuItemSprite* winetableItem = MenuItemSprite::create(
		winetable,
		winetable,
		winetable,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	winetableItem->setName("winetable");
	winetableItem->setOpacity(0);
	winetableItem->setTag(6);
	winetableItem->setPosition(Vec2(540, 470));
	menu->addChild(winetableItem);

	Building* exersiceroom = ExerciseRoom::create();
	Vec_Buildings.push_back(exersiceroom);
	MenuItemSprite* exersiceroomItem = MenuItemSprite::create(
		exersiceroom,
		exersiceroom,
		exersiceroom,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	exersiceroomItem->setName("exersiceroom");
	exersiceroomItem->setOpacity(0);
	exersiceroomItem->setTag(7);
	exersiceroomItem->setPosition(Vec2(257, 93));
	menu->addChild(exersiceroomItem);

	Building* bookshelf = BookShelf::create();
	Vec_Buildings.push_back(bookshelf);
	MenuItemSprite* bookshelfItem = MenuItemSprite::create(
		bookshelf,
		bookshelf,
		bookshelf,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	bookshelfItem->setName("bookshelf");
	bookshelfItem->setOpacity(0);
	bookshelfItem->setTag(8);
	bookshelfItem->setPosition(Vec2(665, 751));
	menu->addChild(bookshelfItem);

	loadJsonData();

	m_storageroom = StorageRoom::create();
	MenuItemSprite* storageroomItem = MenuItemSprite::create(
		m_storageroom,
		m_storageroom,
		m_storageroom,
		CC_CALLBACK_1(HomeLayer::onStorageRoom, this));
	storageroomItem->setPosition(Vec2(105, 188));
	menu->addChild(storageroomItem);

	m_fence = Fence::create();
	MenuItemSprite* fenceItem = MenuItemSprite::create(
		m_fence,
		m_fence,
		m_fence,
		CC_CALLBACK_1(HomeLayer::onFence, this));
	fenceItem->setPosition(Vec2(630, 48));
	menu->addChild(fenceItem);

	updateBuilding();

	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		StorageRoom::add(MyPackage::vec_packages[i]);
	}
	MyPackage::takeoff();

	g_hero->setIsOut(false);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_HOME);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ServerDataSwap::init(NULL)->postOneData(GlobalData::getUId());
#endif

	this->scheduleOnce(schedule_selector(HomeLayer::delayShowNewerGuide), 0.1f);

	return true;
}

void HomeLayer::onclick(Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	std::string nodename = node->getName();
	int nodetag = node->getTag();

	if (nodename.compare("bookshelf") == 0 && Vec_Buildings[nodetag]->data.level >= Vec_Buildings[nodetag]->data.maxlevel)
	{
		Layer* layer = BookShelfLayer::create();
		g_gameLayer->addChild(layer, 4);
	}
	else
	{
		Layer* layer = BuildingUILayer::create(Vec_Buildings[nodetag]);
		g_gameLayer->addChild(layer, 4, "builduilayer");
	}

}

void HomeLayer::loadJsonData()
{
	//建筑的JSON数据
	rapidjson::Document doc = ReadJsonFile("data/buildings.json");
	rapidjson::Value& allBuilds = doc["b"];
	for (unsigned int i = 0; i < Vec_Buildings.size(); i++)
	{
		rapidjson::Value& oneBuild = allBuilds[i];
		Vec_Buildings[i]->parseData(oneBuild);
	}
}

void HomeLayer::onStorageRoom(Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	Layer* layer = StorageUILayer::create();
	g_gameLayer->addChild(layer, 10, "storageuilayer");
}

void HomeLayer::onFence(Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

	if (GlobalData::isExercising() && !GlobalData::isHasFSF())
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

	Layer* layer = OutDoor::create();
	if (g_gameLayer != NULL)
		g_gameLayer->addChild(layer, 10, "OutDoor");
}

void HomeLayer::updateBuilding()
{
	for (unsigned int i = 0; i < Vec_Buildings.size(); i++)
	{
		MenuItemSprite* item = (MenuItemSprite*)Vec_Buildings[i]->getParent();
		if (Vec_Buildings[i]->data.level >= 1)
		{
			item->setOpacity(255);
			if (i == 5)
				m_badfurnace->setVisible(false);
			else if (i == 8)
				m_badbookshelf->setVisible(false);
		}
	}
}

void HomeLayer::showNewerGuide(int step)
{
	bool showguide = false;
	std::vector<Node*> nodes;
	if (step == 0)
	{
		showguide = true;
	}
	else if (step == 1)
	{
		if (Vec_Buildings.size() > 2)
		{
			nodes.push_back(Vec_Buildings[2]->getParent());
			NewerGuideLayer::pushUserData("buildingguide");
			showguide = true;
		}
	}
	else if (step == 12 || step == 46)
	{
		nodes.push_back(m_fence->getParent());
		NewerGuideLayer::pushUserData("fence");
		showguide = true;
	}
	else if (step == 41)
	{
		if (Vec_Buildings.size() > 2)
		{
			nodes.push_back(Vec_Buildings[2]->getParent());
			if (Vec_Buildings[2]->data.level > 0)
				NewerGuideLayer::pushUserData("forgingtable");
			else
				NewerGuideLayer::pushUserData("buildingguide");
			showguide = true;
		}
	}
	else if (step == 65)
	{
		showguide = true;
	}
	else if (step == 66)
	{
		nodes.push_back(m_storageroom->getParent());
		NewerGuideLayer::pushUserData("storageroom");
		showguide = true;
	}
	else if (step >= 70)
	{
		nodes.push_back(Vec_Buildings[step - 70]->getParent());
		NewerGuideLayer::pushUserData("buildingguide");
		showguide = true;
	}
	if (showguide)
		g_gameLayer->showNewerGuide(step, nodes);
}

void HomeLayer::checkNewerGuide()
{
	if (NewerGuideLayer::checkifNewerGuide(0))
		showNewerGuide(0);
	else if (NewerGuideLayer::checkifNewerGuide(1))
		showNewerGuide(1);
	else if (NewerGuideLayer::checkifNewerGuide(41))
		showNewerGuide(41);
	else if (!NewerGuideLayer::checkifNewerGuide(64) && NewerGuideLayer::checkifNewerGuide(65))
		showNewerGuide(65);
	else if (!NewerGuideLayer::checkifNewerGuide(65) && NewerGuideLayer::checkifNewerGuide(66))
		showNewerGuide(66);
	else
	{
		std::vector<PackageData>::iterator it;
		//有足够资源引导建造
		for (unsigned int i = 0; i < Vec_Buildings.size(); i++)
		{
			int findcount = 0;
			int bulidressize = Vec_Buildings[i]->data.Res[0].size();
			for (int n = 0; n < bulidressize; n++)
			{
				int buildresid = Vec_Buildings[i]->data.Res[0][n] / 1000;
				int buildrescount = Vec_Buildings[i]->data.Res[0][n] % 1000;
				std::string strResid = StringUtils::format("%d", buildresid);
				for (int m = 0; m < RES_MAX; m++)
				{
					for (unsigned int k = 0; k < StorageRoom::map_storageData[m].size(); k++)
					{
						if (StorageRoom::map_storageData[m][k].strid.compare(strResid) == 0)
						{
							if (StorageRoom::map_storageData[m][k].count >= buildrescount)
							{
								findcount++;
							}
						}
					}
				}
			}
			if (findcount == bulidressize)
			{
				if (NewerGuideLayer::checkifNewerGuide(70 + i) && Vec_Buildings[i]->data.level <= 0 && g_hero->getLifeValue() > 0.0f)
				{
					showNewerGuide(70 + i);
					break;
				}
			}
		}
	}
}

void HomeLayer::delayShowNewerGuide(float dt)
{
	checkNewerGuide();
}