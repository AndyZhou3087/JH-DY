#include "OutDoor.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "MapLayer.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "ActivitScene.h"
#include "MyMenu.h"
#include "ResDetailsLayer.h"
#include "NewerGuideLayer.h"

OutDoor::OutDoor()
{
	lastSrollViewHeight = -1;
	lastrows = 0;
	m_isLongPress = false;
	m_longTouchNode = NULL;
}


OutDoor::~OutDoor()
{
	lastSrollViewHeight = -1;
}

bool OutDoor::init()
{
	m_csbnode = CSLoader::createNode("outDoorLayer.csb");
	this->addChild(m_csbnode, 0, "csbnode");

	m_heroproper = HeroProperNode::create();
	m_heroproper->setPosition(Vec2(360, 775));
	m_csbnode->addChild(m_heroproper, 1, "HeroProperNode");

	scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");
	scrollview->setScrollBarEnabled(false);
	scrollview->setBounceEnabled(true);

	updata();

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(OutDoor::onBack, this));


	m_outbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("outbtn");
	m_outbtn->addTouchEventListener(CC_CALLBACK_2(OutDoor::onOut, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	checkNewerGuide();

	return true;
}


void OutDoor::checkNewerGuide()
{
	if (NewerGuideLayer::checkifNewerGuide(13))
	{
		showNewerGuide(13);
	}
	else if (!NewerGuideLayer::checkifNewerGuide(46) && NewerGuideLayer::checkifNewerGuide(47))
	{
		showNewerGuide(47);
	}
	else if (!NewerGuideLayer::checkifNewerGuide(56) && NewerGuideLayer::checkifNewerGuide(57))
	{
		showNewerGuide(57);
	}
}

void OutDoor::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		for (int i = 0; i < MyPackage::getSize(); i++)
		{
			StorageRoom::add(MyPackage::vec_packages[i]);
		}
		MyPackage::takeoff();
		this->removeFromParentAndCleanup(true);
	}
}

void OutDoor::onOut(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_outbtn->setEnabled(false);
		this->removeFromParentAndCleanup(true);
		g_gameLayer->scheduleOnce(schedule_selector(GameScene::delayShowOutScence), 0.02f);
	}
}

void OutDoor::updata()
{
	//更新仓库栏
	updataStorageUI();
	//更新背包栏
	updataMyPackageUI();
}

void OutDoor::updataMyPackageUI()
{
	for (int i = 0; i < MyPackage::getMax(); i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		m_csbnode->removeChildByName(name);
	}

	int countindex = -1;
	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = MyPackage::vec_packages[i];

		bool isin = false;
		std::map<std::string, AllResource>::iterator resit;
		for (resit = GlobalData::map_allResource.begin(); resit != GlobalData::map_allResource.end(); resit++)
		{
			if (tmpdata.strid.compare(GlobalData::map_allResource[resit->first].strid) == 0)
			{
				isin = true;
				countindex++;
				break;
			}
		}
		if (!isin)
			continue;

		std::string countorlvstr = StringUtils::format("%d", tmpdata.count);
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata.strid].qu);
			countorlvstr = StringUtils::format("Lv%d", tmpdata.slv+1);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata.strid].qu);
			countorlvstr = StringUtils::format("Lv%d", tmpdata.lv+1);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(OutDoor::onPackageItem, this));
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(110 + countindex * 125, 530));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", countindex);
		m_csbnode->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", MyPackage::vec_packages[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		Label * reslbl = Label::createWithTTF(countorlvstr, "fonts/STXINGKA.TTF", 18);//Label::createWithSystemFont(str, "", 18);
		reslbl->setAnchorPoint(Vec2(1, 0.5));
		reslbl->setPosition(Vec2(box->getContentSize().width - 10, 20));
		box->addChild(reslbl);

		std::string mymixgf = GlobalData::getMixGF();
		MixGfData mdata = GlobalData::map_MixGfData[mymixgf];
		if (mymixgf.length() > 0)
		{
			if (MyPackage::vec_packages[i].strid.compare(mdata.mastergf) == 0)
			{
				Sprite * mixtag = Sprite::createWithSpriteFrameName("ui/mixtag.png");
				mixtag->setPosition(Vec2(box->getContentSize().width - 15, box->getContentSize().height - 15));
				box->addChild(mixtag);
			}
		}
	}
}

void OutDoor::updataStorageUI()
{
	for (unsigned int i = 0; i < allStorageData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		scrollview->removeChildByName(name);
	}

	int typecount = 0;
	for (int i = 0; i < RES_MAX; i++)
	{
		typecount += StorageRoom::map_storageData[i].size();
	}

	int row = typecount % 5 == 0 ? typecount / 5 : (typecount / 5 + 1);

	int innerheight = scrollview->getInnerContainerSize().height;
	if (lastSrollViewHeight < 0 || lastrows < row)
	{
		innerheight = row * 130;
		int contentheight = scrollview->getContentSize().height;
		if (innerheight < contentheight)
			innerheight = contentheight;
		lastSrollViewHeight = innerheight;
		scrollview->setInnerContainerSize(Size(650, innerheight));
	}
	lastrows = row;

	allStorageData.clear();
	for (int i = 0; i < RES_MAX; i++)
	{
		for (unsigned int m = 0; m < StorageRoom::map_storageData[i].size(); m++)
		{
			allStorageData.push_back(&StorageRoom::map_storageData[i][m]);
		}
	}

	int countindex = -1;
	for (unsigned int i = 0; i < allStorageData.size(); i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = *allStorageData[i];

		bool isin = false;
		std::map<std::string, AllResource>::iterator resit;
		for (resit = GlobalData::map_allResource.begin(); resit != GlobalData::map_allResource.end(); resit++)
		{
			if (tmpdata.strid.compare(GlobalData::map_allResource[resit->first].strid) == 0)
			{
				isin = true;
				countindex++;
				break;
			}
		}
		if (!isin)
			continue;

		std::string countorlvstr = StringUtils::format("%d", tmpdata.count);

		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata.strid].qu);
			countorlvstr = StringUtils::format("Lv%d", tmpdata.slv + 1);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata.strid].qu);
			countorlvstr = StringUtils::format("Lv%d", tmpdata.lv + 1);
		}
		std::string name = StringUtils::format("resitem%d", countindex);
		cocos2d::ui::ImageView* boxItem = cocos2d::ui::ImageView::create(boxstr, cocos2d::ui::Widget::TextureResType::PLIST);
		boxItem->addTouchEventListener(CC_CALLBACK_2(OutDoor::onStorageItem, this));
		boxItem->setTouchEnabled(true);
		scrollview->addChild(boxItem, 0, name);
		boxItem->setUserData(allStorageData[i]);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 10 + countindex % 5 * 125, innerheight - boxItem->getContentSize().height / 2 - countindex / 5 * 130));

		std::string str = StringUtils::format("ui/%s.png", allStorageData[i]->strid.c_str());

		cocos2d::ui::ImageView* res = cocos2d::ui::ImageView::create(str, cocos2d::ui::Widget::TextureResType::PLIST);
		boxItem->addChild(res, 0, "res");
		res->setPosition(Vec2(boxItem->getContentSize().width / 2, boxItem->getContentSize().height / 2));

		Label * reslbl = Label::createWithTTF(countorlvstr, "fonts/STXINGKA.TTF", 18);
		boxItem->addChild(reslbl, 0, "lbl");
		reslbl->setAnchorPoint(Vec2(1, 0.5f));

		reslbl->setPosition(Vec2(boxItem->getContentSize().width - 10, 20));

		std::string mymixgf = GlobalData::getMixGF();
		MixGfData mdata = GlobalData::map_MixGfData[mymixgf];
		if (mymixgf.length() > 0)
		{
			if (allStorageData[i]->strid.compare(mdata.mastergf) == 0)
			{
				Sprite * mixtag = Sprite::createWithSpriteFrameName("ui/mixtag.png");
				mixtag->setPosition(Vec2(boxItem->getContentSize().width - 15, boxItem->getContentSize().height - 15));
				boxItem->addChild(mixtag, 0, "mixtag");
			}
		}
	}
}

void OutDoor::longTouchUpdate(float delay){
	m_isLongPress = true;
	if (m_longTouchNode != NULL){
		std::string name = m_longTouchNode->getName();
		//if (name.find("resitem") != std::string::npos)
		{
			unschedule(schedule_selector(OutDoor::longTouchUpdate));
			ResDetailsLayer::whereClick = 2;
			PackageData* data = (PackageData*)m_longTouchNode->getUserData();
			this->addChild(ResDetailsLayer::create(data));
		}
	}
}

void OutDoor::onStorageItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* node = (Node*)pSender;
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		m_longTouchNode = node;
		schedule(schedule_selector(OutDoor::longTouchUpdate), 1.0f);

	}

	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		unschedule(schedule_selector(OutDoor::longTouchUpdate));
		if (!m_isLongPress)
		{
			takeout((PackageData*)node->getUserData());
			updata();
		}
		m_isLongPress = false;
	}
	else if (type == ui::Widget::TouchEventType::CANCELED)
	{
		unschedule(schedule_selector(OutDoor::longTouchUpdate));
		m_isLongPress = false;
	}
}

void OutDoor::takeout(PackageData* pdata)
{
	PackageData* data = pdata;

	int count = data->count - 1;
	if (count <= 0)
	{
		std::vector<PackageData>::iterator it;
		for (it = StorageRoom::map_storageData[data->type].begin(); it != StorageRoom::map_storageData[data->type].end(); ++it)
		{
			if (it->strid.compare(data->strid) == 0 && it->goodvalue == data->goodvalue)
			{
				PackageData pdata = *data;
				pdata.count = 1;
				if (MyPackage::add(pdata) == 0)
				{
					StorageRoom::use(pdata);
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
			StorageRoom::use(data->strid);
		}
	}
	m_heroproper->refreshCarryData();
}

void OutDoor::onPackageItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	int index = node->getTag();
	PackageData data = MyPackage::vec_packages[index];
	data.count = 1;
	StorageRoom::add(data);
	MyPackage::cutone(data);
	m_heroproper->refreshCarryData();
	updata();
}


void OutDoor::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	nodes.push_back(m_outbtn);
	NewerGuideLayer::pushUserData("normalbtn");
	g_gameLayer->showNewerGuide(step, nodes);
}
