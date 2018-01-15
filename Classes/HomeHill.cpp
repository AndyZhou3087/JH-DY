#include "HomeHill.h"
#include "json.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "ActionGetLayer.h"
#include "Const.h"
#include "GameScene.h"
#include "CommonFuncs.h"
#include "FightLayer.h"
#include "SoundManager.h"
#include "MapLayer.h"
#include "NewerGuideLayer.h"
HomeHill::HomeHill()
{
}


HomeHill::~HomeHill()
{
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_MAP);
}

bool HomeHill::init()
{
	Node* csbnode = CSLoader::createNode("homeHillLayer.csb");
	this->addChild(csbnode);

	//返回按钮
	m_backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(HomeHill::onBack, this));
	
	int ressize = GlobalData::vec_hillResid.size();
	scrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");
	scrollView->setScrollBarEnabled(false);
	scrollView->setBounceEnabled(true);

	int itemheight = 140;
	int innerheight = itemheight * ressize;
	int contentheight = scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollView->setInnerContainerSize(Size(650, innerheight));

	for (int i = 0; i < ressize; i++)
	{
		//产出资源node
		Node* resnode = CSLoader::createNode("resNode.csb");
		std::string namestr = StringUtils::format("node%d", i);
		scrollView->addChild(resnode,0, namestr);
		resnode->setPosition(Vec2(scrollView->getContentSize().width/2, innerheight - i * itemheight - itemheight / 2));

		//资源id
		cocos2d::ui::ImageView* iconimg = (cocos2d::ui::ImageView*)resnode->getChildByName("icon");
		//资源名称
		cocos2d::ui::Text* name = (cocos2d::ui::Text*)resnode->getChildByName("name");
		//产出个数
		cocos2d::ui::Text* count = (cocos2d::ui::Text*)resnode->getChildByName("count");
		//产出速度
		cocos2d::ui::Text* speed = (cocos2d::ui::Text*)resnode->getChildByName("speed");
		//等待时间，采完之后显示，恢复满
		cocos2d::ui::Text* waittime = (cocos2d::ui::Text*)resnode->getChildByName("waittime");
		//资源描述
		cocos2d::ui::Text* desc = (cocos2d::ui::Text*)resnode->getChildByName("desc");
		cocos2d::ui::Text* waittext = (cocos2d::ui::Text*)resnode->getChildByName("waittext");
		
		//操作按钮
		cocos2d::ui::Button* actionbtn = (cocos2d::ui::Button*)resnode->getChildByName("actionbtn");
		actionbtn->addTouchEventListener(CC_CALLBACK_2(HomeHill::onclick, this));

		for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
		{
			ResData data = GlobalData::vec_resData[m];
			if (GlobalData::vec_hillResid[i].compare(data.strid) == 0)
			{
				actionbtn->setUserData((void*)GlobalData::vec_resData[m].strid.c_str());
				std::string str = StringUtils::format("ui/%s.png", data.strid.c_str());
				iconimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
				iconimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

				name->setString(GlobalData::map_allResource[data.strid].cname);
				desc->setString(GlobalData::map_allResource[data.strid].desc);

				str = StringUtils::format("%d", data.count);
				count->setString(str);

				float fs = (data.speed[g_nature->getReason()]) / 60.0f;
				str = StringUtils::format("%.1fh", fs);
				speed->setString(str);

				float wfs = fs * data.max;
				str = StringUtils::format("%.1fh", wfs);
				waittime->setString(str);

				if (data.count <= 0)
				{
					waittext->setVisible(true);
					waittime->setVisible(true);
					actionbtn->setEnabled(false);
				}
				else
				{
					waittext->setVisible(false);
					waittime->setVisible(false);
					actionbtn->setEnabled(true);
				}

				actionbtn->setTitleText(CommonFuncs::gbk2utf(acname[data.actype].c_str()));
				
			}
		}

	}
	this->schedule(schedule_selector(HomeHill::updateUI),0.2f);


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_ENTER_MAPADDR);

	return true;
}

void HomeHill::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	showNewerGuide(18);
}

void HomeHill::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (g_maplayer != NULL)
			g_maplayer->checkNewerGuide();
		this->removeFromParentAndCleanup(true);
	}
}

void HomeHill::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		unsigned int i = 0;
		ResData* data = NULL;
		for (i = 0; i<GlobalData::vec_resData.size(); i++)
		{
			data = &GlobalData::vec_resData[i];
			std::string strid = (char*)node->getUserData();
			if (data->strid.compare(strid) == 0)
				break;
		}

		if (GlobalData::vec_resData[i].count > 0)
		{
			GlobalData::vec_resData[i].count--;

			if (data->strid.compare("67") == 0 || data->strid.compare("68") == 0)//兔子，狼战斗界面
			{
				std::string npcid = "n002";
				if (data->strid.compare("68") == 0)
					npcid = "n003";
				FightLayer* layer = FightLayer::create("m1-2", npcid);
				if (g_gameLayer != NULL)
					g_gameLayer->addChild(layer, 4, "fightlayer");
			}
			else
			{
				ActionGetLayer* layer = ActionGetLayer::create(i, data->res, data->type, data->actype);
				if (g_gameLayer != NULL)
					g_gameLayer->addChild(layer, 2, "ActionGetLayer");
			}


		}
	}
}

void HomeHill::updateUI(float dt)
{
	for (unsigned int i = 0; i < GlobalData::vec_hillResid.size(); i++)
	{
		std::string namestr = StringUtils::format("node%d", i);
		Node* resnode = scrollView->getChildByName(namestr);

		cocos2d::ui::Text* count = (cocos2d::ui::Text*)resnode->getChildByName("count");
		cocos2d::ui::Text* speed = (cocos2d::ui::Text*)resnode->getChildByName("speed");
		cocos2d::ui::Text* waittime = (cocos2d::ui::Text*)resnode->getChildByName("waittime");
		cocos2d::ui::Text* waittext = (cocos2d::ui::Text*)resnode->getChildByName("waittext");
		cocos2d::ui::Button* actionbtn = (cocos2d::ui::Button*)resnode->getChildByName("actionbtn");
		for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
		{
			ResData* data = &GlobalData::vec_resData[m];
			if (GlobalData::vec_hillResid[i].compare(data->strid) == 0)
			{
				std::string str = StringUtils::format("%d", data->count);
				count->setString(str);
				str = StringUtils::format("%.1fh", data->speed[g_nature->getReason()] / 60.f);
				speed->setString(str);

				str = StringUtils::format("%.1fh", (data->speed[g_nature->getReason()] * data->max - data->waittime) / 60.f);
				waittime->setString(str);
	
				if (data->count <= 0)
				{
					waittext->setVisible(true);
					waittime->setVisible(true);
					count->setColor(Color3B::RED);
					actionbtn->setEnabled(false);
				}
				else
				{
					waittext->setVisible(false);
					waittime->setVisible(false);
					count->setColor(Color3B::BLACK);
					actionbtn->setEnabled(true);
				}
			}
		}
	}
}

void HomeHill::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 18)
	{
		Node* resnode = scrollView->getChildByName("node0");
		NewerGuideLayer::pushUserData("normalbtn");
		nodes.push_back(resnode->getChildByName("actionbtn"));
	}
	else if (step == 28)
	{
		scrollView->jumpToPercentVertical(20);
		Node* resnode = scrollView->getChildByName("node4");
		NewerGuideLayer::pushUserData("normalbtn");
		nodes.push_back(resnode->getChildByName("actionbtn"));
	}
	else if (step == 37)
	{
		nodes.push_back(m_backbtn);
		NewerGuideLayer::pushUserData("backbtn");
	}
	if (step == 18 || step == 28 || step == 37)
		g_gameLayer->showNewerGuide(step, nodes);
}
