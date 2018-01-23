#include "BuildingUILayer.h"
#include "json.h"
#include "Bed.h"
#include "MyActionProgressTimer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "Const.h"
#include "StorageRoom.h"
#include "HintBox.h"
#include "SoundManager.h"
#include "ResDetailsLayer.h"
#include "BuildingDetailsLayer.h"
#include "HomeLayer.h"
#include "NewerGuideLayer.h"
#include "GameDataSave.h"
#include "ExerciseCancelLayer.h"
#include "ExerciseDoneLayer.h"
#include "BuyComfirmLayer.h"
#include "BookShelfLayer.h"
#include "DivideLayer.h"
#include "WaitingProgress.h"

BuildingUILayer::BuildingUILayer()
{
	selectActionIndex = -1;
	estarttime = 0;
	lastCategoryindex = 0;
	exersiceTag = 0;
}


BuildingUILayer::~BuildingUILayer()
{

}


BuildingUILayer* BuildingUILayer::create(Building* build)
{
	BuildingUILayer *pRet = new BuildingUILayer();
	if (pRet && pRet->init(build))
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

bool BuildingUILayer::init(Building* build)
{
	m_build = build;

	// ui
	m_csbnode = CSLoader::createNode("buidingUiLayer.csb");
	m_csbnode->setPosition(Vec2(0, -90));
	this->addChild(m_csbnode);

	//建筑物名称
	cocos2d::ui::Text* title = (cocos2d::ui::Text*)m_csbnode->getChildByName("title");
	title->setString(m_build->data.cname);

	//返回按钮
	m_backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onBack, this));

	scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");
	scrollview->setScrollBarEnabled(false);
	scrollview->setBounceEnabled(true);
	if (m_build->data.level <= 0)
		scrollview->setVisible(false);

	//建筑物每个操作 node
	buildnode = CSLoader::createNode("actionNode.csb");
	buildnode->setPosition(Vec2(360, 905));
	m_csbnode->addChild(buildnode);

	//建造按钮
	buildbtn = (cocos2d::ui::Button*)buildnode->getChildByName("item")->getChildByName("actionbtn");
	buildbtn->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onAction, this));
	buildbtn->setTag(BUILD);

	//建造label
	actionbtnlabel = (cocos2d::ui::Text*)buildbtn->getChildByName("text");

	//建筑物图标
	cocos2d::ui::ImageView* buildicon = (cocos2d::ui::ImageView*)buildnode->getChildByName("item")->getChildByName("box")->getChildByName("icon");
	std::string iconstr = StringUtils::format("ui/s%s.png", m_build->data.name);
	buildicon->loadTexture(iconstr, cocos2d::ui::TextureResType::PLIST);
	buildicon->setContentSize(Sprite::createWithSpriteFrameName(iconstr)->getContentSize());
	buildicon->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onBuidingDetails, this));


	//建筑升级进度条
	buildbar = (cocos2d::ui::LoadingBar*)buildnode->getChildByName("item")->getChildByName("loadingbar");

	cocos2d::ui::Text* needtimelbl = (cocos2d::ui::Text*)buildnode->getChildByName("item")->getChildByName("needtime");
	int blv = m_build->data.level;
	if (blv >= m_build->data.maxlevel)
		blv = m_build->data.maxlevel - 1;
	std::string needtimestr = StringUtils::format("%d分钟", m_build->data.needtime[blv]);
	needtimelbl->setString(CommonFuncs::gbk2utf(needtimestr.c_str()));

	updataBuildRes();


	if (strcmp(m_build->data.name, "forgingtable") == 0)
	{
		Node* categoryBtnNode = m_csbnode->getChildByName("tagnode");
		for (int i = 0; i < categoryBtnNode->getChildrenCount(); i++)
		{
			std::string btnstr = StringUtils::format("btn%d", i);
			cocos2d::ui::Button* btn = (cocos2d::ui::Button*)categoryBtnNode->getChildByName(btnstr);
			btn->setTag(i);
			btn->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onCategory, this));
			vec_categoryBtn.push_back(btn);
		}
	}

	if (m_build->data.level > 0)
	{
		setActionScrollViewUI();
	}

	if (strcmp(m_build->data.name, "exerciseroom") == 0 && m_build->data.level >= 1)
	{
		updateExerciseDesc();
	}


	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (strcmp(m_build->data.name, "forgingtable") == 0 && m_build->data.level <= 0)
		checkNewerGuide();
	return true;
}

void BuildingUILayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void BuildingUILayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (NewerGuideLayer::checkifNewerGuide(7))
		{
			TopBar* topbar = (TopBar*)g_gameLayer->getChildByName("topbar");
			if (topbar != NULL)
				topbar->showNewerGuide(7);
		}
		else if (NewerGuideLayer::checkifNewerGuide(46))
		{
			HomeLayer* homlayer = (HomeLayer*)g_gameLayer->getChildByName("homelayer");
			if (homlayer != NULL)
				homlayer->showNewerGuide(46);
		}
		this->removeFromParentAndCleanup(true);
	}
}

void BuildingUILayer::setActionScrollViewUI()
{
	m_loadlbl = Label::createWithTTF(CommonFuncs::gbk2utf("加载中..."), "fonts/STXINGKA.TTF", 28);
	m_loadlbl->setColor(Color3B(0, 0, 0));
	m_loadlbl->setPosition(Vec2(320, 540));
	this->addChild(m_loadlbl);
	this->scheduleOnce(schedule_selector(BuildingUILayer::delayLoadActionUi), 0.1f);

	if (strcmp(m_build->data.name, "forgingtable") == 0)
	{
		Node* categoryBtnNode = m_csbnode->getChildByName("tagnode");
		vec_categoryBtn[0]->setBright(false);
		categoryBtnNode->setVisible(true);
		scrollview->setContentSize(Size(scrollview->getContentSize().width, 530));
	}
}

void BuildingUILayer::loadActionUi()
{
	int itemheight = 135;
	int acsize = vec_buildAcitonData.size();

	int scrollinnerheight = acsize * itemheight;

	int scorellheight = scrollview->getContentSize().height;
	if (scrollinnerheight < scorellheight)
		scrollinnerheight = scorellheight;
	scrollview->setInnerContainerSize(Size(scrollview->getContentSize().width, scrollinnerheight));

	vec_actionItem.clear();
	vec_actionbtn.clear();
	vec_actionbar.clear();
	vec_progresstext.clear();

	for (int i = 0; i < acsize; i++)
	{
		Node *acnode = CSLoader::createNode("actionNode.csb");
		acnode->setPosition(Vec2(scrollview->getContentSize().width / 2 - 4, scrollinnerheight - itemheight / 2 - i * itemheight - 15));

		scrollview->addChild(acnode);
		vec_actionItem.push_back(acnode);
	}

	for (unsigned int i = 0; i < vec_actionItem.size(); i++)
	{
		cocos2d::ui::Widget* item = (cocos2d::ui::Widget*)vec_actionItem[i]->getChildByName("item");
		cocos2d::ui::ImageView* icon = (cocos2d::ui::ImageView*)item->getChildByName("box")->getChildByName("icon");

		std::string iconstr = StringUtils::format("ui/%s.png", vec_buildAcitonData.at(i).icon);
		icon->loadTexture(iconstr, cocos2d::ui::TextureResType::PLIST);
		icon->setContentSize(Sprite::createWithSpriteFrameName(iconstr)->getContentSize());
		/*建筑物操作的显示ICON*/
		icon->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onResDetails, this));
		icon->setTag(10000 * (i + 1));//点击按钮TAG来区分10000以上 
		icon->setSwallowTouches(false);

		cocos2d::ui::Button* actbtn = (cocos2d::ui::Button*)item->getChildByName("actionbtn");
		actbtn->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onAction, this));
		actbtn->setTag(ACTION + i);
		cocos2d::ui::Text* actbtnlabel = (cocos2d::ui::Text*)actbtn->getChildByName("text");
		actbtnlabel->setString(vec_buildAcitonData.at(i).actext);
		vec_actionbtn.push_back(actbtn);
		int type = vec_buildAcitonData.at(i).type - 1;
		if (type == WEAPON || type == PROTECT_EQU)
		{
			if (g_hero->checkifHasGF_Equip(vec_buildAcitonData.at(i).icon))
				actbtn->setTitleText(CommonFuncs::gbk2utf("分解"));
			else if (GlobalData::tempHasGf_Equip(vec_buildAcitonData.at(i).icon).length() > 0)
			{
				actbtn->setTitleText(CommonFuncs::gbk2utf("已有"));
			}
		}

		vec_progresstext.push_back((cocos2d::ui::Text*)item->getChildByName("progresstext"));

		cocos2d::ui::LoadingBar* actloadbar = (cocos2d::ui::LoadingBar*)item->getChildByName("loadingbar");
		vec_actionbar.push_back(actloadbar);

		cocos2d::ui::Text* needtimelbl = (cocos2d::ui::Text*)item->getChildByName("needtime");
		std::string needtimestr = StringUtils::format("%d分钟", vec_buildAcitonData.at(i).actime);
		needtimelbl->setString(CommonFuncs::gbk2utf(needtimestr.c_str()));
		if (strcmp(m_build->data.name, "bed") == 0)
			needtimelbl->setVisible(false);
	}
	updataActionRes();

	checkNewerGuide();
}

void BuildingUILayer::delayLoadActionUi(float dt)
{
	vec_buildAcitonData.clear();

	std::string name = m_build->data.name;

	int size = GlobalData::map_buidACData[name].size();

	for (int i = 0; i < size; i++)
		vec_buildAcitonData.push_back(GlobalData::map_buidACData[name][i]);

	loadActionUi();

	m_loadlbl->removeFromParentAndCleanup(true);
	getServerTime();
}

void BuildingUILayer::onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* node = (cocos2d::ui::Button*)pSender;
		int tag = node->getTag();
		if (GlobalData::isExercising() && !GlobalData::isHasFSF() && strcmp(m_build->data.name, "exerciseroom") != 0)
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

		if (tag == BUILD)//建造或者升级
		{

			for (unsigned int i = 0; i < m_build->data.Res[m_build->data.level].size(); i++)
			{
				int restypecount = m_build->data.Res[m_build->data.level].at(i);
				std::string strid = StringUtils::format("%d", restypecount / 1000);
				if (StorageRoom::getCountById(strid) < restypecount%1000)
				{
					HintBox* layer = HintBox::create(CommonFuncs::gbk2utf("没有足够的资源!"));
					addChild(layer);
					return;
				}
			}
			buildbtn->setEnabled(false);
			m_backbtn->setEnabled(false);
			for (unsigned int i = 0; i < vec_actionbtn.size(); i++)
				vec_actionbtn[i]->setEnabled(false);

			buildbar->runAction(Sequence::create(MyProgressTo::create(ACTION_BAR_TIME, 100), CallFuncN::create(CC_CALLBACK_1(BuildingUILayer::onfinish, this, BUILD)), NULL));
			m_build->build();
		}
		else//操作
		{
			int actime = vec_buildAcitonData.at(tag - ACTION).actime;
			int extime = vec_buildAcitonData.at(tag - ACTION).extime;

			if (actime < TIMESCALE * ACTION_BAR_TIME)
			{
				m_build->setActionBarTime(actime / TIMESCALE);
			}
			else
			{
				m_build->setActionBarTime(ACTION_BAR_TIME);
			}

			if (node->getTitleText().compare(CommonFuncs::gbk2utf("已有")) == 0)
			{
				std::string mapid = GlobalData::tempHasGf_Equip(vec_buildAcitonData.at(tag - ACTION).icon);
				std::string hasstr = StringUtils::format("%s%s%s", GlobalData::map_maps[mapid].cname, CommonFuncs::gbk2utf("临时存放点存有").c_str(), vec_buildAcitonData.at(tag - ACTION).cname.c_str());
				HintBox* layer = HintBox::create(hasstr);
				this->addChild(layer);
				return;
			}

			if (node->getTitleText().compare(CommonFuncs::gbk2utf("分解")) == 0)
			{
				DivideLayer* layer = DivideLayer::create(&vec_buildAcitonData.at(tag - ACTION));
				g_gameLayer->addChild(layer, 4);
				vec_actionbar[tag - ACTION]->runAction(Sequence::create(MyProgressTo::create(m_build->getActionBarTime(), 100), CallFuncN::create(CC_CALLBACK_1(BuildingUILayer::ondivideSucc, this, (BACTIONTYPE)tag, layer)), NULL));
				m_build->action(actime, extime);
				return;
			}
			for (unsigned int m = 0; m < vec_buildAcitonData.at(tag - ACTION).res.size(); m++)
			{
				int restypecount = vec_buildAcitonData.at(tag - ACTION).res.at(m);
				std::string strid = StringUtils::format("%d", restypecount / 1000);
				if (StorageRoom::getCountById(strid) < restypecount % 1000)
				{
					HintBox* layer = HintBox::create(CommonFuncs::gbk2utf("没有足够的资源!"));
					this->addChild(layer);
					return;
				}
			}

			if (strcmp(m_build->data.name, "furnace") == 0 && g_nature->getIsMaKeWarm())
			{
				HintBox* hintbox = HintBox::create(CommonFuncs::gbk2utf("暖炉中正在生火！请稍后再操作"));
				Director::getInstance()->getRunningScene()->addChild(hintbox);
				return;
			}

			buildbtn->setEnabled(false);

			for (unsigned int i = 0; i < vec_actionbtn.size(); i++)
				vec_actionbtn[i]->setEnabled(false);

			if (strcmp(m_build->data.name, "exerciseroom") == 0)
			{
				selectActionIndex = tag - ACTION;
				for (unsigned int i = 0; i < vec_actionbtn.size(); i++)
				{
					if (i != selectActionIndex)
						vec_actionbtn[i]->setEnabled(false);
					else
						vec_actionbtn[i]->setEnabled(true);
				}

				if (vec_actionbtn[tag - ACTION]->getTitleText().compare(CommonFuncs::gbk2utf("闭关")) == 0)
				{
					exersiceTag = tag;
					WaitingProgress* waitbox = WaitingProgress::create("准备中...");
					Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
					ServerDataSwap::init(this)->getServerTime();
				}
				else if (vec_actionbtn[tag - ACTION]->getTitleText().compare(CommonFuncs::gbk2utf("取消")) == 0)
				{
					ExerciseCancelLayer* layer = ExerciseCancelLayer::create();

					this->addChild(layer, 4);
					//resetExercise();
				}

				else if (vec_actionbtn[tag - ACTION]->getTitleText().compare(CommonFuncs::gbk2utf("出关")) == 0)
				{
					exersiceTag = tag;
					WaitingProgress* waitbox = WaitingProgress::create("准备中...");
					Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
					ServerDataSwap::init(this)->getServerTime();
				}

			}
			else
			{
				m_backbtn->setEnabled(false);
				vec_actionbar[tag - ACTION]->runAction(Sequence::create(MyProgressTo::create(m_build->getActionBarTime(), 100), CallFuncN::create(CC_CALLBACK_1(BuildingUILayer::onfinish, this, (BACTIONTYPE)tag)), NULL));
			}
			m_build->action(actime, extime);
		}
	}
}

void BuildingUILayer::ondivideSucc(Ref* pSender, BACTIONTYPE type, Node* divideLayer)
{
	g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);
	vec_actionbar[type - ACTION]->setPercent(0);
	vec_actionbtn[type - ACTION]->setTitleText(CommonFuncs::gbk2utf("锻造"));
	DivideLayer* layer = (DivideLayer*)divideLayer;
	layer->showContent();

	
	for (unsigned int m = 0; m < vec_buildAcitonData.at(type - ACTION).res.size(); m++)
	{
		int restypecount = vec_buildAcitonData.at(type - ACTION).res.at(m);
		std::string strid = StringUtils::format("%d", restypecount / 1000);

		PackageData data;
		data.type = GlobalData::getResType(strid);
		data.strid = strid;
		data.count = restypecount%1000/2;
		StorageRoom::add(data);
	}
	std::string strid = vec_buildAcitonData.at(type - ACTION).icon;
	if (g_hero->getAtrByType(H_WEAPON)->count > 0 && g_hero->getAtrByType(H_WEAPON)->strid.compare(strid) == 0)
	{
		PackageData data;
		data.count = 0;
		g_hero->setAtrByType(H_WEAPON, data);
	}
	if (g_hero->getAtrByType(H_ARMOR)->count > 0 && g_hero->getAtrByType(H_ARMOR)->strid.compare(strid) == 0)
	{
		PackageData data;
		data.count = 0;
		g_hero->setAtrByType(H_ARMOR, data);
	}
	StorageRoom::use(strid);

	updataActionRes();
	updataBuildRes();

}

void BuildingUILayer::onfinish(Ref* pSender, BACTIONTYPE type)
{
	g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);
	if (type == BUILD)//建造或升级完成
	{
		for (unsigned int i = 0; i < m_build->data.Res[m_build->data.level - 1].size(); i++)
		{
			int restypecount = m_build->data.Res[m_build->data.level - 1].at(i);
			std::string strid = StringUtils::format("%d", restypecount / 1000);
			StorageRoom::use(strid, restypecount % 1000);
		}
		updataBuildRes();
		if (strcmp(m_build->data.name, "exerciseroom") == 0 && m_build->data.level >= 1)
		{
			updateExerciseDesc();
		}
		buildbar->setPercent(0);
		if (m_build->data.level <= 1)
		{
			setActionScrollViewUI();
		}
		else
		{
			updataBuildRes();
			updataActionRes();
		}

		m_backbtn->setEnabled(true);

		std::string franmename = "images/buildtext0.png";
		if (m_build->data.level > 1)
		{
			franmename = "images/buildtext1.png";
		}
		showFinishHintText(franmename);
		HomeLayer* homelayer = (HomeLayer*)g_gameLayer->getChildByName("homelayer");
		if (homelayer != NULL)
			homelayer->updateBuilding();

		if (strcmp(m_build->data.name, "bookshelf") == 0)
		{
			Director::getInstance()->getRunningScene()->addChild(BookShelfLayer::create(), 4);
			this->removeFromParentAndCleanup(true);
		}
	}
	else//操作完成
	{
		m_backbtn->setEnabled(true);

		if (m_build->data.level < m_build->data.maxlevel)
			buildbtn->setEnabled(true);

		int stype = vec_buildAcitonData.at(type - ACTION).type - 1;
		if (stype == WEAPON || stype == PROTECT_EQU)
		{
			vec_actionbtn[type - ACTION]->setTitleText(CommonFuncs::gbk2utf("分解"));
		}

		vec_actionbar[type - ACTION]->setPercent(0);
		std::string strid = vec_buildAcitonData.at(type - ACTION).icon;
		//是否是产出新的物品，（睡觉和暖炉不会产出新的物品 icon以“0-”开头，其他建筑物的操作或产出新的物品，eg:制作烤肉）
		if (strid.length() > 0 && strid.compare(0,1, "0") != 0)
		{
			PackageData data;
			data.type = vec_buildAcitonData.at(type - ACTION).type - 1;
			std::string idstr = StringUtils::format("%s", strid.c_str());
			data.strid = idstr;
			data.count = 1;
			data.extype = vec_buildAcitonData.at(type - ACTION).extype;
			StorageRoom::add(data);
			//HintBox* layer = HintBox::create(CommonFuncs::gbk2utf("制作成功"));
			//this->addChild(layer);
			showFinishHintText("images/buildtext2.png");
		}

		for (unsigned int m = 0; m < vec_buildAcitonData.at(type - ACTION).res.size(); m++)
		{
			int restypecount = vec_buildAcitonData.at(type - ACTION).res.at(m);
			std::string strid = StringUtils::format("%d", restypecount / 1000);
			StorageRoom::use(strid, restypecount % 1000);
		}

		updataActionRes();
		updataBuildRes();

		if (strcmp(m_build->data.name, "forgingtable") == 0)
		{
			checkNewerGuide();
		}
	}
}

void BuildingUILayer::updataBuildRes()
{
	cocos2d::ui::Widget* mainitem = (cocos2d::ui::Widget*)buildnode->getChildByName("item");

	int level = m_build->data.level;

	if (level > 0)
	{
		scrollview->setVisible(true);
		if (level == m_build->data.maxlevel)
		{
			buildbar->setVisible(false);
			buildnode->getChildByName("item")->getChildByName("progressbg")->setVisible(false);
			buildbtn->setEnabled(false);
			actionbtnlabel->setString(CommonFuncs::gbk2utf("最高级"));
			buildnode->getChildByName("item")->getChildByName("needtime")->setVisible(false);
		}
		else
		{
			actionbtnlabel->setString(CommonFuncs::gbk2utf("升级"));
		}
	}

	if (level >= m_build->data.maxlevel)
		level = m_build->data.maxlevel - 1;

	int ressize = m_build->data.Res[level].size();

	if (ressize > 0)
	{
		//更新升级需要的资源
		for (unsigned int i = 0; i < m_build->data.Res[level].size(); i++)
		{
			//建筑物需要的资源图标
			std::string str = StringUtils::format("res%d", i);
			cocos2d::ui::Widget* resitem = (cocos2d::ui::Widget*)mainitem->getChildByName(str);

			resitem->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onResDetails, this));
			resitem->setTag(i); 

			str = StringUtils::format("count%d", i);
			cocos2d::ui::Text* rescount = (cocos2d::ui::Text*)mainitem->getChildByName(str);

			if (m_build->data.level < m_build->data.maxlevel)
			{
				int restypecount = m_build->data.Res[level].at(i);
				if (restypecount > 0)
				{
					resitem->setVisible(true);

					str = StringUtils::format("ui/%d.png", restypecount / 1000);
					Sprite* res = Sprite::createWithSpriteFrameName(str);
					res->setPosition(Vec2(resitem->getContentSize().width / 2, resitem->getContentSize().height / 2));
					res->setScale(0.38f);
					resitem->addChild(res);
					std::string strid = StringUtils::format("%d", restypecount / 1000);
					int hascount = StorageRoom::getCountById(strid);
					int needcount = restypecount % 1000;
					str = StringUtils::format("%d/%d", hascount, needcount);
					rescount->setVisible(true);
					rescount->setString(str);
					if (hascount < needcount)
						rescount->setTextColor(Color4B::RED);
					else
						rescount->setTextColor(Color4B::BLACK);
				}
				buildbtn->setEnabled(true);
			}
			else
			{
				resitem->setVisible(false);
				rescount->setVisible(false);
			}
		}
	}
}

void BuildingUILayer::updataActionRes()
{
	//更新操作需要的资源

	for (unsigned int i = 0; i < vec_actionItem.size(); i++)
	{
		cocos2d::ui::Widget* item = (cocos2d::ui::Widget*)vec_actionItem[i]->getChildByName("item");
		cocos2d::ui::Text* desc = (cocos2d::ui::Text*)item->getChildByName("desc");
		cocos2d::ui::Button* actbtn = (cocos2d::ui::Button*)item->getChildByName("actionbtn");
		if (m_build->data.level >= vec_buildAcitonData.at(i).blv)
		{
			int ressize = vec_buildAcitonData.at(i).res.size();
			if (ressize > 0)
			{
				for (unsigned int m = 0; m < vec_buildAcitonData.at(i).res.size(); m++)
				{
					int restypecount = vec_buildAcitonData.at(i).res.at(m);
					if (restypecount > 0)
					{
						//合成需要的资源
						std::string str = StringUtils::format("res%d", m);
						cocos2d::ui::Widget* resitem = (cocos2d::ui::Widget*)item->getChildByName(str);
						resitem->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onResDetails, this));
						resitem->setTag((i + 1) * 100 + m); //点击按钮TAG来区分100以上
						resitem->setVisible(true);
						resitem->setSwallowTouches(false);

						str = StringUtils::format("ui/%d.png", restypecount / 1000);//资源图标
						Sprite* res = Sprite::createWithSpriteFrameName(str);
						res->setPosition(Vec2(resitem->getContentSize().width / 2, resitem->getContentSize().height / 2));
						res->setScale(0.38f);
						resitem->addChild(res);

						str = StringUtils::format("count%d", m);
						cocos2d::ui::Text* rescount = (cocos2d::ui::Text*)item->getChildByName(str);
						rescount->setVisible(true);

						std::string strid = StringUtils::format("%d", restypecount / 1000);
						int hascount = StorageRoom::getCountById(strid);
						int needcount = restypecount % 1000;
						str = StringUtils::format("%d/%d", hascount, needcount);//拥有的资源个数/需要资源个数
						rescount->setString(str);
						if (hascount < needcount)
							rescount->setTextColor(Color4B::RED);
						else
							rescount->setTextColor(Color4B::BLACK);
					}
				}
				actbtn->setEnabled(true);
				desc->setVisible(false);
			}
			else
			{
				desc->setString(vec_buildAcitonData.at(i).desc);
				desc->setColor(Color3B(0, 0, 0));
				desc->setVisible(true);
				actbtn->setEnabled(true);
			}

		}
		else
		{
			actbtn->setEnabled(false);
			desc->setString(CommonFuncs::gbk2utf("需要更高等级的建筑！"));
			desc->setColor(Color3B(204, 4, 4));
			desc->setVisible(true);
		}
	}
}

void BuildingUILayer::onResDetails(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* node = (Node*)pSender;
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		isDraging = false;
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
			SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

			Node* node = (Node*)pSender;
			int tag = node->getTag();
			std::string strid;
			int intresid = 0;
			if (tag >= 100 && tag < 10000)//小图标资源
			{
				intresid = vec_buildAcitonData[tag / 100 - 1].res[tag % 100];
				strid = StringUtils::format("%d", intresid / 1000);

			}
			else if (tag >= 10000)//需要合成的
			{
				strid = vec_buildAcitonData[tag / 10000 - 1].icon;
			}
			else//建筑物需要的资源
			{
				intresid = m_build->data.Res[m_build->data.level][tag];
				strid = StringUtils::format("%d", intresid / 1000);
			}
			ResDetailsLayer::whereClick = 1;
			this->addChild(ResDetailsLayer::createByResId(strid));
		}
	}
}

void BuildingUILayer::onBuidingDetails(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		this->addChild(BuildingDetailsLayer::create(m_build));
	}
}


void BuildingUILayer::showFinishHintText(std::string path)
{
	Sprite* bsprite = Sprite::create(path);
	bsprite->setPosition(Vec2(360, 600));
	this->addChild(bsprite);
	bsprite->runAction(Sequence::create(FadeOut::create(1.5f), CallFuncN::create(CC_CALLBACK_1(BuildingUILayer::finishAnim, this, bsprite)), NULL));
}

void BuildingUILayer::finishAnim(Ref* pSender, Node* node)
{
	node->removeFromParentAndCleanup(true);
}

void BuildingUILayer::showNewerGuide(int step)
{
    bool isshowguide = false;
	std::vector<Node*> nodes;
	if (step == 2)
	{
		nodes.push_back(buildnode->getChildByName("item")->getChildByName("res0"));
		nodes.push_back(buildnode->getChildByName("item")->getChildByName("res1"));
		nodes.push_back(buildnode->getChildByName("item")->getChildByName("res2"));
		NewerGuideLayer::pushUserData("resbox");
		NewerGuideLayer::pushUserData("resbox");
		NewerGuideLayer::pushUserData("resbox");
        isshowguide = true;
	}
	else if (step == 3)
	{
		nodes.push_back(buildnode->getChildByName("item")->getChildByName("actionbtn"));
		NewerGuideLayer::pushUserData("normalbtn");
        isshowguide = true;
	}
	else if (step == 4)
	{
        isshowguide = true;
	}
	else if (step == 5)
	{
		if (vec_categoryBtn.size() > 2)
		{
			nodes.push_back(vec_categoryBtn[2]);
			NewerGuideLayer::pushUserData("buildtagbtn1");
			isshowguide = true;
		}
	}
	else if (step == 6)
	{
		
		if (vec_actionItem.size() > 1 && vec_categoryBtn.size() > 2 && !vec_categoryBtn[2]->isBright())
		{
			cocos2d::ui::Widget* item = (cocos2d::ui::Widget*)vec_actionItem[1]->getChildByName("item");
			cocos2d::ui::Button* actbtn = (cocos2d::ui::Button*)item->getChildByName("actionbtn");
			nodes.push_back(actbtn);
			NewerGuideLayer::pushUserData("normalbtn");
            isshowguide = true;
		}
	}

	else if (step == 42)
	{
		if (vec_categoryBtn.size() > 2)
		{
			nodes.push_back(vec_categoryBtn[2]);
			NewerGuideLayer::pushUserData("buildtagbtn1");
			isshowguide = true;
		}
	}
	else if (step == 43)
	{
		if (vec_actionItem.size() > 0 && vec_categoryBtn.size() > 2 && !vec_categoryBtn[2]->isBright())
		{
			cocos2d::ui::Widget* item = (cocos2d::ui::Widget*)vec_actionItem[0]->getChildByName("item");
			cocos2d::ui::Button* actbtn = (cocos2d::ui::Button*)item->getChildByName("actionbtn");
			nodes.push_back(actbtn);
			NewerGuideLayer::pushUserData("normalbtn");
            isshowguide = true;
		}
	}
	else if (step == 44)
	{
		if (vec_actionItem.size() > 2 && vec_categoryBtn.size() > 2 && !vec_categoryBtn[2]->isBright())
		{
			cocos2d::ui::Widget* item = (cocos2d::ui::Widget*)vec_actionItem[2]->getChildByName("item");
			cocos2d::ui::Button* actbtn = (cocos2d::ui::Button*)item->getChildByName("actionbtn");
			nodes.push_back(actbtn);
			NewerGuideLayer::pushUserData("normalbtn");
            isshowguide = true;
		}
	}
	else if (step == 45)
	{
        isshowguide = true;
	}
	if (isshowguide)
		g_gameLayer->showNewerGuide(step, nodes);
}

void BuildingUILayer::checkNewerGuide()
{
	if (NewerGuideLayer::checkifNewerGuide(2))
		showNewerGuide(2);
	else if (NewerGuideLayer::checkifNewerGuide(3))
		showNewerGuide(3);
	else if (NewerGuideLayer::checkifNewerGuide(4))
		showNewerGuide(4);
	else if (NewerGuideLayer::checkifNewerGuide(5))
		showNewerGuide(5);
	else if (NewerGuideLayer::checkifNewerGuide(6))
		showNewerGuide(6);
	else if (!NewerGuideLayer::checkifNewerGuide(41) && NewerGuideLayer::checkifNewerGuide(42))
		showNewerGuide(42);
	else if (!NewerGuideLayer::checkifNewerGuide(42) && NewerGuideLayer::checkifNewerGuide(43))
		showNewerGuide(43);
	else if (!NewerGuideLayer::checkifNewerGuide(43) && NewerGuideLayer::checkifNewerGuide(44))
		showNewerGuide(44);
	else if (!NewerGuideLayer::checkifNewerGuide(44) && NewerGuideLayer::checkifNewerGuide(45))
		showNewerGuide(45);
}

void BuildingUILayer::onExercisefinish(int index)
{
	vec_actionbtn[index]->setTitleText(CommonFuncs::gbk2utf("出关"));
	this->unschedule(schedule_selector(BuildingUILayer::updateExerciseLeftTime));
	vec_progresstext[index]->setVisible(false);
}

void BuildingUILayer::updateExerciseDesc()
{
	cocos2d::ui::Text* importdesc = (cocos2d::ui::Text*)buildnode->getChildByName("item")->getChildByName("desc");
	importdesc->setVisible(true);
	importdesc->setString(CommonFuncs::gbk2utf(exersiceDesc.c_str()));
}

void BuildingUILayer::updateExerciseLeftTime(float dt)
{
	if (selectActionIndex >= 0)
	{
		int tatoltime = vec_buildAcitonData.at(selectActionIndex).actime * 60;
		int lefttime = tatoltime - (curtime - estarttime);
		std::string str = StringUtils::format("%02d:%02d:%02d", lefttime / 3600, lefttime % 3600/60, lefttime%3600%60);
		vec_progresstext[selectActionIndex]->setVisible(true);
		vec_progresstext[selectActionIndex]->setString(str);

		float pecert = 100.0f*(curtime - estarttime) / tatoltime;

		vec_actionbar[selectActionIndex]->runAction(MyProgressFromTo::create(1.0f, vec_actionbar[selectActionIndex]->getPercent(), pecert));
		if (lefttime <= 0)
		{
			onExercisefinish(selectActionIndex);
		}
	}
	curtime++;
}

void BuildingUILayer::resetExercise()
{
	this->unschedule(schedule_selector(BuildingUILayer::updateExerciseLeftTime));
	GameDataSave::getInstance()->setExersiceCfg("");
	vec_progresstext[selectActionIndex]->setVisible(false);
	vec_actionbar[selectActionIndex]->setPercent(0);
	vec_actionbar[selectActionIndex]->stopAllActions();
	vec_actionbtn[selectActionIndex]->setTitleText(CommonFuncs::gbk2utf("闭关"));
	for (unsigned int i = 0; i < vec_actionbtn.size(); i++)
	{
		vec_actionbtn[i]->setEnabled(true);
	}
}

void BuildingUILayer::onCategory(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		if (lastCategoryindex == node->getTag())
			return;

		lastCategoryindex = node->getTag();

		for (unsigned int i = 0; i < vec_categoryBtn.size(); i++)
		{
			vec_categoryBtn[i]->setEnabled(false);
			if (node->getTag() != i)
			{
				vec_categoryBtn[i]->setBright(true);
			}
			else
			{
				vec_categoryBtn[i]->setBright(false);
			}
		}

		m_loadlbl = Label::createWithTTF(CommonFuncs::gbk2utf("加载中..."), "fonts/STXINGKA.TTF", 28);
		m_loadlbl->setColor(Color3B(0, 0, 0));
		m_loadlbl->setPosition(Vec2(320, 600));
		this->addChild(m_loadlbl, 100);

		scrollview->removeAllChildrenWithCleanup(true);

		this->scheduleOnce(schedule_selector(BuildingUILayer::delayShowCategoryActionUI), 0.1f);
	}
}

void BuildingUILayer::loadActionUIByCategory(int category)
{
	vec_buildAcitonData.clear();
	int size = GlobalData::map_buidACData["forgingtable"].size();
	if (category == 0)
	{
		for (int i = 0; i < size; i++)
			vec_buildAcitonData.push_back(GlobalData::map_buidACData["forgingtable"][i]);
	}
	else if (category == 1)
	{
		for (int i = 0; i < size; i++)
		{
			int stype = GlobalData::map_buidACData["forgingtable"][i].type - 1;
			if (stype == RES_1)
				vec_buildAcitonData.push_back(GlobalData::map_buidACData["forgingtable"][i]);
		}
	}
	else if (category == 2)
	{
		for (int i = 0; i < size; i++)
		{
			int stype = GlobalData::map_buidACData["forgingtable"][i].type - 1;
			if (stype == TOOLS)
				vec_buildAcitonData.push_back(GlobalData::map_buidACData["forgingtable"][i]);
		}
	}
	else if (category == 3)
	{
		for (int i = 0; i < size; i++)
		{
			int stype = GlobalData::map_buidACData["forgingtable"][i].type - 1;
			if (stype == WEAPON)
				vec_buildAcitonData.push_back(GlobalData::map_buidACData["forgingtable"][i]);
		}
	}
	else if (category == 4)
	{
		for (int i = 0; i < size; i++)
		{
			int stype = GlobalData::map_buidACData["forgingtable"][i].type - 1;
			if (stype == PROTECT_EQU)
				vec_buildAcitonData.push_back(GlobalData::map_buidACData["forgingtable"][i]);
		}
	}
	
	loadActionUi();
	m_loadlbl->removeFromParentAndCleanup(true);
	for (unsigned int i = 0; i < vec_categoryBtn.size(); i++)
	{
		vec_categoryBtn[i]->setEnabled(true);
		if (lastCategoryindex != i)
		{
			vec_categoryBtn[i]->setBright(true);
		}
		else
		{
			vec_categoryBtn[i]->setBright(false);
		}
	}

}

void BuildingUILayer::delayShowCategoryActionUI(float dt)
{
	loadActionUIByCategory(lastCategoryindex);
}

void BuildingUILayer::getServerTime()
{
	if (strcmp(m_build->data.name, "exerciseroom") == 0)
	{
		WaitingProgress* waitbox = WaitingProgress::create("加载中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->getServerTime();
	}
}

void BuildingUILayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	if (exersiceTag > 0)
	{
		if (vec_actionbtn[exersiceTag - ACTION]->getTitleText().compare(CommonFuncs::gbk2utf("闭关")) == 0)
		{
			estarttime = GlobalData::servertime;
			vec_actionbtn[exersiceTag - ACTION]->setTitleText(CommonFuncs::gbk2utf("取消"));
			curtime = GlobalData::servertime;
			updateExerciseLeftTime(0);
			this->schedule(schedule_selector(BuildingUILayer::updateExerciseLeftTime), 1);

			if (g_hero->getAtrByType(H_WG)->count > 0)
			{
				ex_wgstrid = g_hero->getAtrByType(H_WG)->strid;
			}
			if (g_hero->getAtrByType(H_NG)->count > 0)
			{
				ex_ngstrid = g_hero->getAtrByType(H_NG)->strid;
			}

			std::string estr = StringUtils::format("%d-%d-%s-%s", exersiceTag - ACTION, estarttime, ex_wgstrid.c_str(), ex_ngstrid.c_str());
			GameDataSave::getInstance()->setExersiceCfg(estr);
		}
		else if (vec_actionbtn[exersiceTag - ACTION]->getTitleText().compare(CommonFuncs::gbk2utf("出关")) == 0)
		{
			int extime = vec_buildAcitonData.at(exersiceTag - ACTION).extime;
			if (GlobalData::servertime + 2 - estarttime >= extime * 60)
			{
				ExerciseDoneLayer* layer = ExerciseDoneLayer::create(ex_wgstrid, ex_ngstrid, extime / 60);
				this->addChild(layer, 4);
				resetExercise();
			}
			else
			{
				HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("出关失败"));
				this->addChild(hbox, 4);
				vec_actionbtn[exersiceTag - ACTION]->setTitleText(CommonFuncs::gbk2utf("取消"));
				curtime = GlobalData::servertime;

				updateExerciseLeftTime(0);
				this->schedule(schedule_selector(BuildingUILayer::updateExerciseLeftTime), 1);
			}
		}
	}
	else
	{
		if (strcmp(m_build->data.name, "exerciseroom") == 0)
		{
			std::string estr = GameDataSave::getInstance()->getExersiceCfg();
			int index = -1;

			if (estr.length() > 0)
			{
				std::vector<std::string> tmp;
				CommonFuncs::split(estr, tmp, "-");
				if (tmp.size() >= 4)
				{
					index = atoi(tmp[0].c_str());
					estarttime = atoi(tmp[1].c_str());
					ex_wgstrid = tmp[2];
					ex_ngstrid = tmp[3];
				}
			}
			selectActionIndex = index;
			if (index >= 0)
			{
				curtime = GlobalData::servertime;
				int pasttime = curtime - estarttime;

				if (pasttime >= vec_buildAcitonData.at(index).actime * 60)
				{
					vec_actionbar[index]->setPercent(100);
					onExercisefinish(selectActionIndex);
				}
				else
				{
					vec_actionbtn[index]->setTitleText(CommonFuncs::gbk2utf("取消"));
					updateExerciseLeftTime(0);
					this->schedule(schedule_selector(BuildingUILayer::updateExerciseLeftTime), 1);
				}

				for (unsigned int i = 0; i < vec_actionbtn.size(); i++)
				{
					if (i != index)
						vec_actionbtn[i]->setEnabled(false);
					else
						vec_actionbtn[i]->setEnabled(true);
				}

			}

		}
	}
}

void BuildingUILayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("失败!!!"));
	this->addChild(hbox, 4);
}