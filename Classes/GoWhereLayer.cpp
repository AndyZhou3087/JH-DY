#include "GoWhereLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GlobalData.h"
#include "HomeHill.h"
#include "HomeLayer.h"
#include "MapLayer.h"
#include "GameScene.h"
#include "TempStorageLayer.h"
#include "NpcLayer.h"
#include "SoundManager.h"
#include "GameDataSave.h"
#include "NewerGuideLayer.h"
#include "ActivitScene.h"
#include "SepcialNpcLayer.h"
#include "AnalyticUtil.h"
#include "RollDiceLayer.h"
#include "TopBar.h"
GoWhereLayer::GoWhereLayer()
{

}


GoWhereLayer::~GoWhereLayer()
{
}

GoWhereLayer* GoWhereLayer::create(std::string addrname, WHERELAYER_TYPE type, float distance)
{
	GoWhereLayer *pRet = new GoWhereLayer();
	if (pRet && pRet->init(addrname, type, distance))
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

bool GoWhereLayer::init(std::string addrid, WHERELAYER_TYPE type, float distance)
{
	Node* csbnode = CSLoader::createNode("goWhereLayer.csb");
	this->addChild(csbnode);

	m_addrstr = addrid;
	m_type = type;

	MapData mdata = GlobalData::map_maps[addrid];

	//名称
	cocos2d::ui::Text* title = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	title->setString(mdata.cname);

	//描述
	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desc->setString(mdata.desc);

	//距离 ，换算成时间
	cocos2d::ui::Text* fast = (cocos2d::ui::Text*)csbnode->getChildByName("fast");

    float fmin = 0.0f;
    if (g_maplayer != NULL)
		fmin = g_maplayer->moveToDestTime(distance);

	int minute = (int)fmin;
	std::string faststr;
	if (minute / 60 > 0)
	{
		faststr = StringUtils::format("%d小时%d分钟", minute / 60, minute % 60);
		if (minute % 60 == 0)
			faststr = StringUtils::format("%d小时", minute / 60);
	}
	else
		faststr = StringUtils::format("%d分钟", minute);
	
	fast->setString(CommonFuncs::gbk2utf(faststr.c_str()));

	cocos2d::ui::Text* fasttitle = (cocos2d::ui::Text*)csbnode->getChildByName("text1");
	
	//地点的类型
	cocos2d::ui::ImageView* typeimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("typepng");
	std::string str = StringUtils::format("images/%s.jpg", mdata.tpngname);
	typeimg->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
	
	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(GoWhereLayer::onClose, this));

	m_gobtn = (cocos2d::ui::Button*)csbnode->getChildByName("gobtn");
	m_gobtn->addTouchEventListener(CC_CALLBACK_2(GoWhereLayer::onGO, this));

	//物品存放
	m_stbtn = (cocos2d::ui::Button*)csbnode->getChildByName("stbtn");
	m_stbtn->addTouchEventListener(CC_CALLBACK_2(GoWhereLayer::onST, this));

	m_stredpoint = (cocos2d::ui::Widget*)m_stbtn->getChildByName("redpoint");
	checkRedPoint(0);

	m_enterbtn = (cocos2d::ui::Button*)csbnode->getChildByName("enterbtn");
	m_enterbtn->addTouchEventListener(CC_CALLBACK_2(GoWhereLayer::onComeIn, this));

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(GoWhereLayer::onClose, this));

	if (type == GOWHERE)
	{
		closebtn->setVisible(true);
		m_gobtn->setVisible(true);
		m_stbtn->setVisible(false);
		m_enterbtn->setVisible(false);
		fast->setVisible(true);
		fasttitle->setVisible(true);
	}
	else if (type == ARRIVE)
	{
		closebtn->setVisible(false);
		m_gobtn->setVisible(false);
		m_stbtn->setVisible(true);
		m_enterbtn->setVisible(true);
		fast->setVisible(false);
		fasttitle->setVisible(false);
		if (m_addrstr.compare("m1-1") == 0)//家，没有物品存放
		{
			m_stbtn->setVisible(false);
			m_enterbtn->setPositionX(360);
		}
	}
	else if (type == ONWAY_JUMP)
	{
		closebtn->setVisible(true);
		closebtn->setTitleText(CommonFuncs::gbk2utf("不跳"));
		m_gobtn->setVisible(false);
		m_stbtn->setVisible(false);
		m_enterbtn->setVisible(true);
		m_enterbtn->setTitleText(CommonFuncs::gbk2utf("跳入"));
		fast->setVisible(false);
		fasttitle->setVisible(false);
		title->setString(CommonFuncs::gbk2utf("奇遇"));
		desc->setString(CommonFuncs::gbk2utf("发现悬崖下面紫光闪耀，好像藏着什么好东西，你想下去一探究竟，少侠还请三思，崖下白骨露野，跳下去多半会一命呜呼！"));
		std::string str = StringUtils::format("images/%s.jpg", "t008");
		typeimg->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
#ifdef ANALYTICS
		AnalyticUtil::onEvent("showjump");
#endif
	}
	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->schedule(schedule_selector(GoWhereLayer::checkRedPoint), 1.0f);
	checkNewerGuide();
	return true;
}


void GoWhereLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	if (m_type == ONWAY_JUMP && g_maplayer != NULL)
		g_maplayer->heroPauseMoving();
}

void GoWhereLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_type == ONWAY_JUMP && g_maplayer != NULL)
		{
			g_maplayer->heroResumeMoving();
		}
		this->removeFromParentAndCleanup(true);
	}
}

void GoWhereLayer::onGO(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
		if (g_maplayer != NULL)//地图上移动角色头像
			g_maplayer->showMoveToDest();
	}
}

void GoWhereLayer::onST(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		TempStorageLayer* layer = TempStorageLayer::create(m_addrstr);

		if (g_gameLayer != NULL)
			g_gameLayer->addChild(layer, 2, "TempStorageLayer");
	}
}

void GoWhereLayer::onComeIn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_addrstr.compare("m1-1") == 0)//进入家
		{
			HomeLayer* homelayer = HomeLayer::create();
			g_gameLayer->addChild(homelayer, 1, "homelayer");
			g_maplayer->removeFromParentAndCleanup(true);
			g_maplayer = NULL;
		}
		else if (m_addrstr.compare("m1-2") == 0)//进入后山
		{
			if (g_gameLayer != NULL)
				g_gameLayer->addChild(HomeHill::create(), 2, "homehill");
		}
		else if (m_addrstr.compare("m1-9") == 0)
		{
			RollDiceLayer* rlayer = RollDiceLayer::create();
			g_gameLayer->addChild(rlayer, 5);
		}
		else
		{
			if (m_type == ONWAY_JUMP)
			{
				Scene* activityScene = ActivitScene::createScene("images/findtreasure.jpg", CommonFuncs::gbk2utf(""));
				if (activityScene != NULL)
				{
					Director::getInstance()->pushScene(activityScene);
					this->removeFromParentAndCleanup(true);
				}
				return;
			}
			else//进入NPC
			{
				if (m_addrstr.compare("m13-1") == 0)
				{
					if (g_gameLayer != NULL)
						g_gameLayer->addChild(SepcialNpcLayer::create(m_addrstr), 2, "sepcialnpclayer");
				}
				else
				{
					if (g_gameLayer != NULL)
						g_gameLayer->addChild(NpcLayer::create(m_addrstr), 2, "npclayer");
					std::string mapname = GlobalData::map_maps[m_addrstr].cname;
					if (mapname.find(CommonFuncs::gbk2utf("客栈")) != std::string::npos)
					{
						g_uiScroll->addEventText(CommonFuncs::gbk2utf("小二：客官里面请，吃饭一两银子，睡觉二两银子，喝酒二两银子"), 25, Color3B(204, 4, 4));
					}
				}
			}
		}
		this->removeFromParentAndCleanup(true);
	}
}



void GoWhereLayer::checkRedPoint(float dt)
{
	if (GameDataSave::getInstance()->getTempStorage(m_addrstr).length() > 0)
		m_stredpoint->setVisible(true);
	else
		m_stredpoint->setVisible(false);
}

void GoWhereLayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if ((step == 15) && m_type == GOWHERE)
	{
		TopBar* topbar = (TopBar*)g_gameLayer->getChildByName("topbar");
		topbar->showNewerGuide(15);
	}
	else if ((step == 16) && m_type == GOWHERE)
	{
		nodes.push_back(m_gobtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}
	else if ((step == 17) && m_type == ARRIVE)
	{
		nodes.push_back(m_enterbtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}
	else if ((step == 39) && m_type == GOWHERE)
	{
		nodes.push_back(m_gobtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}
	else if (step == 40 && m_type == ARRIVE)
	{
		nodes.push_back(m_enterbtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}
	else if (step == 69 && m_type == ARRIVE)
	{
		nodes.push_back(m_stbtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}
	if (nodes.size() > 0)
		g_gameLayer->showNewerGuide(step, nodes);
}

void GoWhereLayer::checkNewerGuide()
{
	if (NewerGuideLayer::checkifNewerGuide(15))
		showNewerGuide(15);
	else if (NewerGuideLayer::checkifNewerGuide(16))
		showNewerGuide(16);
	else if (NewerGuideLayer::checkifNewerGuide(17))
		showNewerGuide(17);
	else if (NewerGuideLayer::checkifNewerGuide(39))
		showNewerGuide(39);
	else if (NewerGuideLayer::checkifNewerGuide(40))
		showNewerGuide(40);
	else if (NewerGuideLayer::checkifNewerGuide(69))
	{
		if (m_type == ARRIVE)
		{
			std::string datastr = GameDataSave::getInstance()->getTempStorage(m_addrstr);
			std::vector<std::string> vec_retstr;
			CommonFuncs::split(datastr, vec_retstr, ";");
			if (vec_retstr.size() > 0)
				showNewerGuide(69);
		}
	}
}
