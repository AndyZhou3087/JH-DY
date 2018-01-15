#include "FriendNpcScene.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "GameScene.h"
#include "FightLayer.h"

FriendNpcScene::FriendNpcScene()
{

}

FriendNpcScene::~FriendNpcScene()
{
	GlobalData::isPopingScene = false;
}

Scene* FriendNpcScene::createScene()
{
	if (GlobalData::isPopingScene)
		return NULL;
	GlobalData::isPopingScene = true;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	Layer* layer = FriendNpcScene::create();

	if (layer == NULL)
		return NULL;
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

FriendNpcScene* FriendNpcScene::create()
{
	FriendNpcScene *pRet = new FriendNpcScene();
	if (pRet && pRet->init())
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

bool FriendNpcScene::init()
{
	std::vector<std::string> vec_friends;
	std::map<std::string, FriendlyData>::iterator mit;
	for (mit = GlobalData::map_myfriendly.begin(); mit != GlobalData::map_myfriendly.end(); ++mit)
	{
		if (GlobalData::map_myfriendly[mit->first].relation == F_FRIEND)
		{
			if (GlobalData::map_NPCFriendData[mit->first].vec_fightnpc.size() > 0)
			vec_friends.push_back(mit->first);
		}
	}

	if (vec_friends.size() <= 0)
		return false;

	Node* csbnode = CSLoader::createNode("friendNpcChallengeLayer.csb");
	this->addChild(csbnode);

	int rnd = GlobalData::createRandomNum(vec_friends.size());
	std::string npcid = vec_friends[rnd];

	vec_fightnpc = GlobalData::map_NPCFriendData[npcid].vec_fightnpc;
	std::string enemynpcname;

	GlobalData::map_maps["m1-6"].npcs.clear();
	for (unsigned int i = 0; i < vec_fightnpc.size(); i++)
	{
		enemynpcname.append(GlobalData::map_npcs[vec_fightnpc[i]].name);
		enemynpcname.append(CommonFuncs::gbk2utf("、"));
		GlobalData::map_maps["m1-6"].npcs.push_back(vec_fightnpc[i]);
	}
	enemynpcname = enemynpcname.substr(0, enemynpcname.length() - 3);

	//描述
	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");

	desc1 = (cocos2d::ui::Text*)csbnode->getChildByName("desc1");
	desc1->setVisible(false);

	std::string descstr = StringUtils::format("%s%s%s%s%s", CommonFuncs::gbk2utf("你与").c_str(), GlobalData::map_npcs[npcid].name, CommonFuncs::gbk2utf("成为好友，").c_str(), enemynpcname.c_str(), CommonFuncs::gbk2utf("向你发起挑战！").c_str());
	desc->setString(descstr);

	cocos2d::ui::ImageView* typepng = (cocos2d::ui::ImageView*)csbnode->getChildByName("image");
	typepng->loadTexture("images/cchallenge.jpg", cocos2d::ui::Widget::TextureResType::LOCAL);
	
	escapebtn = (cocos2d::ui::Button*)csbnode->getChildByName("escapebtn");
	escapebtn->addTouchEventListener(CC_CALLBACK_2(FriendNpcScene::onEscape, this));

	getbtn = (cocos2d::ui::Button*)csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(FriendNpcScene::onGet, this));


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


void FriendNpcScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void FriendNpcScene::onEscape(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		int r = GlobalData::createRandomNum(100);
		if (r < 50)
		{
			desc1->setVisible(true);
			escapebtn->setEnabled(false);
			getbtn->setEnabled(false);
			this->scheduleOnce(schedule_selector(FriendNpcScene::delayFight), 1.5f);
		}
		else
		{
			Director::getInstance()->popScene();
		}
	}
}

void FriendNpcScene::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		escapebtn->setEnabled(false);
		getbtn->setEnabled(false);
		this->scheduleOnce(schedule_selector(FriendNpcScene::delayFight), 0.5f);
	}
}

void FriendNpcScene::delayFight(float dt)
{
	Director::getInstance()->popScene();

	if (g_gameLayer != NULL)
	{
		g_gameLayer->addChild(FightLayer::create("m1-6", vec_fightnpc[0]), 4, "fightlayer");
	}
}