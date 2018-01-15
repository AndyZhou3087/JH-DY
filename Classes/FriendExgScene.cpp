#include "FriendExgScene.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "GameScene.h"
#include "NpcLayer.h"
#include "StorageRoom.h"
#include "MapLayer.h"
#include "HintBox.h"

FriendExgScene::FriendExgScene()
{

}

FriendExgScene::~FriendExgScene()
{
	GlobalData::isPopingScene = false;
}

Scene* FriendExgScene::createScene(int type)
{
	if (GlobalData::isPopingScene)
		return NULL;
	GlobalData::isPopingScene = true;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	Layer* layer = FriendExgScene::create(type);

	if (layer == NULL)
		return NULL;
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

FriendExgScene* FriendExgScene::create(int type)
{
	FriendExgScene *pRet = new FriendExgScene();
	if (pRet && pRet->init(type))
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

bool FriendExgScene::init(int type)
{
	std::vector<std::string> vec_friends;
	std::map<std::string, FriendlyData>::iterator mit;
	for (mit = GlobalData::map_myfriendly.begin(); mit != GlobalData::map_myfriendly.end(); ++mit)
	{
		if (GlobalData::map_myfriendly[mit->first].relation == F_FRIEND)
		{
			if (type == 0 && GlobalData::map_NPCFriendData[mit->first].vec_giveres.size() > 0)
			vec_friends.push_back(mit->first);
			else if (type == 1 && GlobalData::map_NPCFriendData[mit->first].vec_askres.size() > 0)
				vec_friends.push_back(mit->first);
		}
	}

	if (vec_friends.size() <= 0)
		return false;

	Node* csbnode = CSLoader::createNode("friendExgLayer.csb");
	this->addChild(csbnode);

	m_type = type;

	int rnd = GlobalData::createRandomNum(vec_friends.size());
	m_npcid = vec_friends[rnd];

	vec_res = GlobalData::map_NPCFriendData[m_npcid].vec_giveres;

	std::string titlestr = "赠送礼物";
	std::string descstr = "赠送以下物品";
	std::string typeimgstr = "images/friendgive.jpg";
	std::string closebtntext = "不收";
	std::string getbtntext = "收下";
	if (type == 1)
	{
		titlestr = "索要礼物";
		descstr = "索要以下物品";
		typeimgstr = "images/friendask.jpg";
		closebtntext = "不给";
		getbtntext = "给予";
		vec_res = GlobalData::map_NPCFriendData[m_npcid].vec_askres;
	}
	//名称
	cocos2d::ui::Text* title = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	title->setString(CommonFuncs::gbk2utf(titlestr.c_str()));

	//描述
	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desc->setString(CommonFuncs::gbk2utf(descstr.c_str()));

	//NPC
	cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)csbnode->getChildByName("npcname");
	npcname->setString(GlobalData::map_npcs[m_npcid].name);

	cocos2d::ui::ImageView* typepng = (cocos2d::ui::ImageView*)csbnode->getChildByName("image");
	typepng->loadTexture(typeimgstr, cocos2d::ui::Widget::TextureResType::LOCAL);
	
	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(FriendExgScene::onClose, this));
	closebtn->setTitleText(CommonFuncs::gbk2utf(closebtntext.c_str()));

	cocos2d::ui::Button* getbtn = (cocos2d::ui::Button*)csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(FriendExgScene::onGet, this));
	getbtn->setTitleText(CommonFuncs::gbk2utf(getbtntext.c_str()));

	int startx[] = { 360, 260, 180, 130, 90 };
	int spacex[] = { 190, 190, 170, 150, 130 };
	int ressize = vec_res.size();
	for (int i = 0; i < ressize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);
		box->setPosition(Vec2(startx[ressize - 1] + i*spacex[ressize - 1], 220));
		this->addChild(box);

		std::string residstr = StringUtils::format("%d", vec_res[i] / 1000);
		std::string str = StringUtils::format("ui/%s.png", residstr.c_str());
		Sprite* res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().width / 2));
		box->addChild(res);
		
		Label * namelbl = Label::createWithTTF(GlobalData::map_allResource[residstr].cname, "fonts/STXINGKA.TTF", 25);
		namelbl->setColor(Color3B(255, 255, 255));
		namelbl->setPosition(Vec2(box->getPositionX(), 140));
		this->addChild(namelbl);

		std::string strcount = StringUtils::format("x%d", vec_res[i]%1000);
		Label * coutlbl = Label::createWithTTF(strcount, "fonts/STXINGKA.TTF", 25);//Label::createWithSystemFont(strcount, "", 25);
		coutlbl->setAnchorPoint(Vec2(1, 0.5f));
		coutlbl->setColor(Color3B(255, 255, 255));
		coutlbl->setPosition(Vec2(box->getPositionX() + 50, 180));
		this->addChild(coutlbl);
	}


	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void FriendExgScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void FriendExgScene::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_type == 1)
		{
			NpcLayer* npclayer = (NpcLayer*)g_gameLayer->getChildByName("npclayer");
			if (npclayer != NULL)
			{
				npclayer->updateFriendly(m_npcid);
			}
			else
			{
				GlobalData::map_myfriendly[m_npcid].friendly -= 5;
				int friendly = GlobalData::map_myfriendly[m_npcid].friendly;
				int needfriendly = GlobalData::map_NPCMasterData[m_npcid].needfriendly;
				if (friendly < needfriendly)
				{
					if (GlobalData::map_myfriendly[m_npcid].relation == F_FRIEND)
					{
						GlobalData::map_myfriendly[m_npcid].relation = F_NOMAR;
						std::string desc = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("与").c_str(), GlobalData::map_npcs[m_npcid].name, CommonFuncs::gbk2utf("关系恶化，不再是朋友！").c_str());
						g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
						//GlobalData::map_myfriendly[m_npcid].friendly = 0;
					}
				}
				GlobalData::saveFriendly();
			}
		}
		Director::getInstance()->popScene();
	}
}

void FriendExgScene::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_type == 0)
		{
			for (unsigned int i = 0; i < vec_res.size(); i++)
			{
				std::string strid = StringUtils::format("%d", vec_res[i] / 1000);
				int count = vec_res[i] % 1000;
				PackageData data;
				data.type = GlobalData::getResType(strid);
				data.strid = strid;
				data.count = count;
				data.extype = GlobalData::getResType(strid);
				StorageRoom::add(data);
			}
			Director::getInstance()->popScene();
		}
		else
		{
			bool ishas = true;
			for (unsigned int i = 0; i < vec_res.size(); i++)
			{
				std::string strid = StringUtils::format("%d", vec_res[i] / 1000);
				int count = vec_res[i] % 1000;
				if (StorageRoom::getCountById(strid) < count)
				{
					ishas = false;
					break;
				}
			}
			if (ishas)
			{
				for (unsigned int i = 0; i < vec_res.size(); i++)
				{
					std::string strid = StringUtils::format("%d", vec_res[i] / 1000);
					int count = vec_res[i] % 1000;
					StorageRoom::use(strid, count);
				}
				Director::getInstance()->popScene();
			}
			else
			{
				HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("没有多余的物品了。。。"));
				this->addChild(hint);
			}
		}
	}
}