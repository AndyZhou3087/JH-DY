#include "NpcLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "FightLayer.h"
#include "GameScene.h"
#include "MapLayer.h"
#include "SoundManager.h"
#include "NewerGuideLayer.h"
#include "GameDataSave.h"
#include "ExchangeLayer.h"
#include "HintBox.h"
#include "CannotTouchLayer.h"
#include "MyActionProgressTimer.h"
#include "RepairLayer.h"
#include "GiveLayer.h"
#include "NewerGuide2Layer.h"
#include "NpcTalkLayer.h"
#include "Winlayer.h"

std::string replacestr[] = {"少侠","小子","小兄弟","小伙子", "兄台"};
std::string areplacestr[] = {"女侠","小娘子","小姑娘","小姑娘","姑娘"};

int silvercost[] = { 1, 2, 2 };
NpcLayer::NpcLayer()
{

}


NpcLayer::~NpcLayer()
{
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_MAP);

}

NpcLayer* NpcLayer::create(std::string addrname)
{
	NpcLayer *pRet = new NpcLayer();
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

bool NpcLayer::init(std::string addrid)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("npcLayer.csb");
	this->addChild(m_csbnode);

	m_npctalkbg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("npctalkbg");
	m_npctalkbg->setOpacity(0);

	m_addrstr = addrid;

	MapData mdata = GlobalData::map_maps[addrid];

	cocos2d::ui::Text* title = (cocos2d::ui::Text*)m_csbnode->getChildByName("title");
	title->setString(mdata.cname);

	cocos2d::ui::ImageView* typeimg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("typepng");
	std::string str = StringUtils::format("images/%s.jpg", mdata.tpngname);
	typeimg->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
	
	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onBack, this));

	m_scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");

	m_scrollview->setScrollBarEnabled(false);
	m_scrollview->setBounceEnabled(true);

	m_lastWxpPos = GlobalData::getWxbMapPos();
	m_lastDgqbPos = GlobalData::getDgqbMapPos();

	refreshNpcNode();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_ENTER_MAPADDR);

	this->schedule(schedule_selector(NpcLayer::checkUpateNpc), 1.0f);
	return true;
}

void NpcLayer::refreshNpcNode()
{
	MapData mdata = GlobalData::map_maps[m_addrstr];
	int ncpsize = mdata.npcs.size();

	int itemheight = 200;
	int innerheight = itemheight * ncpsize;
	int contentheight = m_scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_scrollview->setInnerContainerSize(Size(m_scrollview->getContentSize().width, innerheight));

	cocos2d::ui::Widget* hintdown = (cocos2d::ui::Widget*)m_csbnode->getChildByName("hintdown");
	if (ncpsize > 2)
	{
		hintdown->runAction(RepeatForever::create(Blink::create(2, 2)));
	}
	else
	{
		hintdown->stopAllActions();
		hintdown->setVisible(false);
	}

	int scrollviewChildSize = m_scrollview->getChildrenCount();

	int lesscount = scrollviewChildSize - ncpsize;
	if (lesscount > 0)
	{
		for (int i = 0; i < lesscount; i++)
		{
			std::string removechildname = StringUtils::format("npcnode%d", ncpsize + i);
			m_scrollview->removeChildByName(removechildname);
			std::string keepchildname = StringUtils::format("npcnode%d", i);
			m_scrollview->getChildByName(keepchildname)->setPosition(Vec2(m_scrollview->getContentSize().width / 2, innerheight - i * itemheight - itemheight / 2));
		}
	}
	else
	{
		for (int i = 0; i < scrollviewChildSize; i++)
		{
			std::string keepchildname = StringUtils::format("npcnode%d", i);
			m_scrollview->getChildByName(keepchildname)->setPosition(Vec2(m_scrollview->getContentSize().width / 2, innerheight - i * itemheight - itemheight / 2));
		}
		for (int i = scrollviewChildSize; i < ncpsize; i++)
		{
			Node* npcitem = CSLoader::createNode("npcNode.csb");
			npcitem->setPosition(Vec2(m_scrollview->getContentSize().width / 2, innerheight - i * itemheight - itemheight / 2));
			std::string childname = StringUtils::format("npcnode%d", i);
			npcitem->setTag(i);
			m_scrollview->addChild(npcitem, 0, childname);

			cocos2d::ui::ImageView* npchead = (cocos2d::ui::ImageView*)npcitem->getChildByName("npcicon");
			std::string npcheadstr = StringUtils::format("ui/%s.png", mdata.npcs[i].c_str());
			npchead->loadTexture(npcheadstr, cocos2d::ui::TextureResType::PLIST);
			npchead->setScale(0.6f);

			cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)npcitem->getChildByName("npcname");
			npcname->setString(GlobalData::map_npcs[mdata.npcs[i]].name);

			cocos2d::ui::ImageView* npcrsi = (cocos2d::ui::ImageView*)npcitem->getChildByName("npcrsi");

			int count = checkFightCount(mdata.npcs[i]);

			std::string rsistr;
			if (count > 0)
			{
				if (count <= 10)
					rsistr = "ui/npcrsi0.png";
				else
					rsistr = "ui/npcrsi1.png";
			}
			else
			{
				if (count >= -10)
					rsistr = "ui/npcrsi2.png";
				else
					rsistr = "ui/npcrsi1.png";
			}

			npcrsi->loadTexture(rsistr, cocos2d::ui::TextureResType::PLIST);

			cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
			talkbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemTalk, this));
			talkbtn->setTag(i);
			cocos2d::ui::Text* talkbtnlabel = (cocos2d::ui::Text*)talkbtn->getChildByName("text");

			cocos2d::ui::Button* fightbtn = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");
			fightbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemFight, this));
			cocos2d::ui::Text* fightbtnlabel = (cocos2d::ui::Text*)fightbtn->getChildByName("text");

			cocos2d::ui::Button* masterbtn = (cocos2d::ui::Button*)npcitem->getChildByName("msterbtn");
			masterbtn->setTag(i);
			masterbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemMaster, this));
			cocos2d::ui::Text* msterbtnlabel = (cocos2d::ui::Text*)masterbtn->getChildByName("text");

			cocos2d::ui::Button* friendbtn = (cocos2d::ui::Button*)npcitem->getChildByName("friendbtn");
			friendbtn->setTag(i);
			friendbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemFriend, this));
			cocos2d::ui::Text* friendbtnlabel = (cocos2d::ui::Text*)friendbtn->getChildByName("text");

			int relation = F_NOMAR;
			if (GlobalData::map_myfriendly.find(mdata.npcs[i]) != GlobalData::map_myfriendly.end())
				relation = GlobalData::map_myfriendly[mdata.npcs[i]].relation;

			if (relation == F_MASTEROUT)
			{
				msterbtnlabel->setString(CommonFuncs::gbk2utf("出师"));
				masterbtn->setEnabled(false);
			}
			else if (relation == F_FRIEND)
			{
				friendbtnlabel->setString(CommonFuncs::gbk2utf("绝交"));
			}
			else if (relation == F_MASTER)
			{
				msterbtnlabel->setString(CommonFuncs::gbk2utf("出师"));
			}

			cocos2d::ui::Button* givebtn = (cocos2d::ui::Button*)npcitem->getChildByName("givebtn");
			givebtn->setTag(i);
			givebtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemGive, this));

			cocos2d::ui::Button* exchangebtn = (cocos2d::ui::Button*)npcitem->getChildByName("exchgbtn");
			exchangebtn->setTag(i);
			exchangebtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemExchange, this));
			if (GlobalData::map_npcs[mdata.npcs[i]].exchgres.size() <= 0)
				exchangebtn->setVisible(false);

			if (mdata.npcs[i].compare("n009") == 0)
			{
				talkbtnlabel->setString(CommonFuncs::gbk2utf("吃饭"));
				talkbtn->setTag(10 * i);
				talkbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onHostelAction, this));
				fightbtnlabel->setString(CommonFuncs::gbk2utf("睡觉"));
				fightbtn->setTag(10 * i + 1);
				fightbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onHostelAction, this));
				masterbtn->setVisible(true);
				msterbtnlabel->setString(CommonFuncs::gbk2utf("喝酒"));
				masterbtn->setTag(10 * i + 2);
				masterbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onHostelAction, this));
				friendbtn->setVisible(false);
				givebtn->setVisible(false);
				exchangebtn->setVisible(false);
			}
			else if (mdata.npcs[i].compare("n092") == 0)
			{
				fightbtnlabel->setString(CommonFuncs::gbk2utf("修理"));
				fightbtn->setTag(1);
				fightbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onRepair, this));
				masterbtn->setVisible(true);
				msterbtnlabel->setString(CommonFuncs::gbk2utf("强化"));
				masterbtn->setTag(2);
				masterbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onRepair, this));
				friendbtn->setVisible(false);
				givebtn->setVisible(false);
				exchangebtn->setVisible(false);
			}
		}
	}

	reFreshFriendlyUI();

	updatePlotUI(0);
	updatePlotUI(1);
}

void NpcLayer::checkUpateNpc(float dt)
{
	if (m_lastWxpPos != GlobalData::getWxbMapPos())
	{
		refreshNpcNode();
		m_lastWxpPos = GlobalData::getWxbMapPos();
	}
	if (m_lastDgqbPos != GlobalData::getDgqbMapPos())
	{
		refreshNpcNode();
		m_lastDgqbPos = GlobalData::getDgqbMapPos();
	}
}

void NpcLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void NpcLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void NpcLayer::onItemTalk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		NpcData npc = GlobalData::map_npcs[GlobalData::map_maps[m_addrstr].npcs[node->getTag()]];

		if (GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].snpc.compare(npc.id) == 0 && GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].status == M_NONE && GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].words.size() <= 0)
			return;

		vec_wordstr.clear();
		std::string wordstr;
		PlotMissionData pdata = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()];
		int plottype = 1;
		if ((pdata.snpc.compare(npc.id) == 0 && pdata.status == 0) || (pdata.dnpc.compare(npc.id) == 0 && pdata.status == 1))
			plottype = 0;

		bool ismissing = doCheckPlotMisson(plottype, npc);

		if (!ismissing)
		{
			//多个任务时，一起的任务对话还是算任务中的对话
			std::string curmid = GlobalData::getCurBranchPlotMissison();
			if (curmid.length() > 0 && GlobalData::map_BranchPlotMissionData[curmid].size() > 0)
			{
				int subindex = GlobalData::map_BranchPlotMissionItem[curmid].subindex;

				if (subindex > 0 && subindex < GlobalData::map_BranchPlotMissionData[curmid].size() - 1)
				{
					for (int i = 0; i <= subindex; i++)
					{
						PlotMissionData* pd = &GlobalData::map_BranchPlotMissionData[curmid][i];
						if (pd->snpc.compare(npc.id) == 0)
						{
							vec_wordstr.clear();
							for (unsigned int m = 0; m < pd->words.size(); m++)
							{
								wordstr = StringUtils::format("%s%s%s", npc.name, CommonFuncs::gbk2utf("：").c_str(), pd->words[m].c_str());
								if (g_hero->getHeadID() == 4)
									wordstr = replaceSexWord(wordstr);
								vec_wordstr.push_back(wordstr);

								wordstr = StringUtils::format("%s%s%s", g_hero->getMyName().c_str(), CommonFuncs::gbk2utf("：").c_str(), pd->mywords[m].c_str());
								vec_wordstr.push_back(wordstr);
							}
							ismissing = true;
						}
					}
				}
			}
			if (!ismissing)
			{
				wordstr = StringUtils::format("%s%s%s", npc.name, CommonFuncs::gbk2utf("：").c_str(), npc.words[0].c_str());
				if (g_hero->getHeadID() == 4)
					wordstr = replaceSexWord(wordstr);
				vec_wordstr.push_back(wordstr);
			}
		}

		if (vec_wordstr.size() > 0)
		{
			NpcTalkLayer* nlayer = NpcTalkLayer::create(vec_wordstr, npc.id);
			g_gameLayer->addChild(nlayer, 5);
		}
	}
}

void NpcLayer::onItemFight(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int tag = node->getParent()->getTag();
		std::string npcid = GlobalData::map_maps[m_addrstr].npcs[tag];
		std::string protectword;
		
		if (npcid.compare("n004") == 0 && checkFightCount("n004") < 0)
		{
			protectword = StringUtils::format("%s%s", GlobalData::map_npcs[npcid].name, CommonFuncs::gbk2utf("：就凭你？还不够我一刀的，我不杀无名小卒，再去练几年吧！先去打打野狼和兔子，升升等级再来吧！以后的人可没我这么好心！").c_str());
			
		}
		else if (npcid.compare("n005") == 0 && checkFightCount("n005") < 0)
		{
			protectword = StringUtils::format("%s%s", GlobalData::map_npcs[npcid].name, CommonFuncs::gbk2utf("：少侠，你现在还太弱，还是在修炼修炼吧！先去打打野狼和兔子，升升等级再来吧！以后的人可没我这么好心！").c_str());
			if (g_hero->getHeadID() == 4)
				protectword = replaceSexWord(protectword);	
		}
		else
		{
			int count = checkFightCount(npcid);
			if (NewerGuide2Layer::checkifNewerGuide(101))
			{
				if (count < 0 && count >= -10)
				{
					std::vector<Node*> vec_node;
					int npcitemcount = m_scrollview->getChildrenCount();
					if (npcitemcount > 0)
					{
						std::string childname = StringUtils::format("npcnode%d", tag);
						Node* npcitemnode = m_scrollview->getChildByName(childname);

						cocos2d::ui::Widget* npcrsi = (cocos2d::ui::Widget*)npcitemnode->getChildByName("npcrsi");
						npcrsi->setUserData((void*)"npcrsi2");
						vec_node.push_back(npcrsi);
					}
					NewerGuide2Layer *layer = NewerGuide2Layer::create(101, vec_node);
					g_gameLayer->addChild(layer, NEWERLAYERZOER);
					return;
				}
			}
		}
		if (protectword.length() > 0)
		{
			vec_wordstr.clear();
			vec_wordstr.push_back(protectword);
			NpcTalkLayer* nlayer = NpcTalkLayer::create(vec_wordstr, npcid);
			g_gameLayer->addChild(nlayer, 5);

			return;
		}

		if (g_gameLayer != NULL)
			g_gameLayer->addChild(FightLayer::create(m_addrstr, npcid), 4, "fightlayer");

		for (unsigned int i = 0; i < GlobalData::vec_achiveData.size(); i++)
		{
			if (GlobalData::vec_achiveData[i].type == A_12)
			{
				if (GlobalData::vec_achiveData[i].vec_para[0].compare(npcid) == 0)
				{
					if (GlobalData::vec_achiveData[i].finish != -1)
					{
						GlobalData::vec_achiveData[i].finish += 1;
						GlobalData::saveAchiveData();
					}
				}
			}
		}

		PlotMissionData pdata = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()];
		int plottype = 1;
		if (pdata.snpc.compare(npcid) == 0 || pdata.dnpc.compare(npcid) == 0)
			plottype = 0;

		if (!checkIsMissing(plottype, npcid))
			updateFriendly(npcid);
	}
}


void NpcLayer::onItemMaster(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btn = (cocos2d::ui::Button*)pSender;
		std::string npcid = GlobalData::map_maps[m_addrstr].npcs[btn->getTag()];
		if (btn->getTitleText().compare(CommonFuncs::gbk2utf("拜师")) == 0)
		{
			int friendly = 0;
			int relation = F_NOMAR;
			
			if (GlobalData::map_myfriendly.find(npcid) != GlobalData::map_myfriendly.end())
			{
				friendly = GlobalData::map_myfriendly[npcid].friendly;
				relation = GlobalData::map_myfriendly[npcid].relation;
			}
			if (relation == F_FRIEND)
			{
				g_uiScroll->addEventText(CommonFuncs::gbk2utf("你我关系这么好，我怎么能当你的师傅呢！"), 25, Color3B(204, 4, 4));
				return;
			}

			std::map<std::string, FriendlyData>::iterator mit;
			for (mit = GlobalData::map_myfriendly.begin(); mit != GlobalData::map_myfriendly.end(); ++mit)
			{
				if (GlobalData::map_myfriendly[mit->first].relation == F_MASTER)
				{
					std::string desc = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("你已拜入").c_str(), GlobalData::map_npcs[mit->first].name, CommonFuncs::gbk2utf("门下！请回吧！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
					return;
				}
			}

			int needfriendly = GlobalData::map_NPCMasterData[npcid].needfriendly;
			if (friendly >= needfriendly)
			{
				bool ishasenemy = false;
				std::string e_npcnamstr;
				std::vector<std::string> e_npc = GlobalData::map_NPCFriendData[npcid].vec_enemynpc;
				std::map<std::string, FriendlyData>::iterator it;
				for (it = GlobalData::map_myfriendly.begin(); it != GlobalData::map_myfriendly.end(); ++it)
				{
					std::string nid = it->first;

					for (unsigned int m = 0; m < e_npc.size(); m++)
					{
						if (nid.compare(e_npc[m]) == 0 && GlobalData::map_myfriendly[nid].relation == F_FRIEND)
						{
							GlobalData::map_myfriendly[nid].friendly -= GlobalData::map_NPCFriendData[nid].maxfriendly/5;
							GlobalData::map_myfriendly[nid].relation = F_NOMAR;
							e_npcnamstr.append(GlobalData::map_npcs[nid].name);
							e_npcnamstr.append(CommonFuncs::gbk2utf("、"));
							ishasenemy = true;
						}
					}
				}

				if (ishasenemy)
				{

					GlobalData::map_myfriendly[npcid].friendly = GlobalData::map_NPCFriendData[npcid].maxfriendly / 5;
					GlobalData::map_myfriendly[npcid].relation = F_NOMAR;
					e_npcnamstr = e_npcnamstr.substr(0, e_npcnamstr.length() - 3);
					std::string desc = StringUtils::format("%s%s%s%s", GlobalData::map_npcs[npcid].name, CommonFuncs::gbk2utf("发现你与").c_str(), e_npcnamstr.c_str(), CommonFuncs::gbk2utf("的关系，他们发现你是这种三头两面的人，纷纷离你而去！").c_str());
					HintBox* hbox = HintBox::create(desc);
					this->addChild(hbox);
					GlobalData::saveFriendly();
					reFreshFriendlyUI();
					reFreshRelationUI();

					return;
				}

				GlobalData::map_myfriendly[npcid].relation = F_MASTER;
				GlobalData::saveFriendly();
				std::string desc = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("拜入").c_str(), GlobalData::map_npcs[npcid].name, CommonFuncs::gbk2utf("门下！一日为师，终生为父！").c_str());
				g_uiScroll->addEventText(desc, 25, Color3B(27, 141, 0));
				cocos2d::ui::Text* msterbtnlabel = (cocos2d::ui::Text*)btn->getChildByName("text");
				msterbtnlabel->setString(CommonFuncs::gbk2utf("出师"));
			}
			else
			{
				g_uiScroll->addEventText(CommonFuncs::gbk2utf("师徒缘分未到！"), 25, Color3B(204, 4, 4));
			}

		}
		else
		{
			GlobalData::isFightMaster = true;
			FightLayer* fightlayer = FightLayer::create(m_addrstr, npcid);
			this->addChild(fightlayer);
		}

	}
}

void NpcLayer::onItemFriend(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btn = (cocos2d::ui::Button*)pSender;
		cocos2d::ui::Text* friendbtnlabel = (cocos2d::ui::Text*)btn->getChildByName("text");
		std::string npcid = GlobalData::map_maps[m_addrstr].npcs[btn->getTag()];
		if (btn->getTitleText().compare(CommonFuncs::gbk2utf("结交")) == 0)
		{
			int friendly = 0;
			int relation = F_NOMAR;

			if (GlobalData::map_myfriendly.find(npcid) != GlobalData::map_myfriendly.end())
			{
				friendly = GlobalData::map_myfriendly[npcid].friendly;
				relation = GlobalData::map_myfriendly[npcid].relation;
			}

			if (relation == F_MASTER)
			{
				g_uiScroll->addEventText(CommonFuncs::gbk2utf("逆徒！还想跟为师称兄道弟？！"), 25, Color3B(204, 4, 4));
				return;
			}

			int needfriendly = GlobalData::map_NPCFriendData[npcid].needfriendly;
			if (friendly >= needfriendly)
			{
				bool ishasenemy = false;
				std::string e_npcnamstr;
				std::vector<std::string> e_npc = GlobalData::map_NPCFriendData[npcid].vec_enemynpc;
				std::map<std::string, FriendlyData>::iterator it;
				for (it = GlobalData::map_myfriendly.begin(); it != GlobalData::map_myfriendly.end(); ++it)
				{
					std::string nid = it->first;
					
					for (unsigned int m = 0; m < e_npc.size(); m++)
					{
						if (nid.compare(e_npc[m]) == 0 && (GlobalData::map_myfriendly[nid].relation == F_FRIEND || GlobalData::map_myfriendly[nid].relation == F_MASTER))
						{
							GlobalData::map_myfriendly[nid].friendly -= GlobalData::map_NPCFriendData[nid].maxfriendly / 5;
							GlobalData::map_myfriendly[nid].relation = F_NOMAR;
							e_npcnamstr.append(GlobalData::map_npcs[nid].name);
							e_npcnamstr.append(CommonFuncs::gbk2utf("、"));
							ishasenemy = true;
						}
					}
				}

				if (ishasenemy)
				{

					GlobalData::map_myfriendly[npcid].friendly -= GlobalData::map_NPCFriendData[npcid].maxfriendly / 5;
					GlobalData::map_myfriendly[npcid].relation = F_NOMAR;
					e_npcnamstr = e_npcnamstr.substr(0, e_npcnamstr.length() - 3);
					std::string desc = StringUtils::format("%s%s%s%s", GlobalData::map_npcs[npcid].name, CommonFuncs::gbk2utf("发现你与").c_str(), e_npcnamstr.c_str(), CommonFuncs::gbk2utf("的关系，他们发现你是这种三头两面的人，纷纷离你而去！").c_str());
					HintBox* hbox = HintBox::create(desc);
					this->addChild(hbox);
					GlobalData::saveFriendly();
					reFreshFriendlyUI();
					reFreshRelationUI();

					return;
				}

				GlobalData::map_myfriendly[npcid].relation = F_FRIEND;
				GlobalData::saveFriendly();
				std::string desc = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("与").c_str(), GlobalData::map_npcs[npcid].name, CommonFuncs::gbk2utf("成为好友！").c_str());
				g_uiScroll->addEventText(desc, 25, Color3B(27, 141, 0));

				friendbtnlabel->setString(CommonFuncs::gbk2utf("绝交"));
			}
			else
			{
				g_uiScroll->addEventText(CommonFuncs::gbk2utf("咱们两个不是很熟，就不要走的太近了！"), 25, Color3B(204, 4, 4));
			}

		}
		else
		{
			GlobalData::map_myfriendly[npcid].relation = F_NOMAR;
			GlobalData::saveFriendly();
			friendbtnlabel->setString(CommonFuncs::gbk2utf("结交"));
			GlobalData::map_myfriendly[npcid].friendly -= GlobalData::map_NPCFriendData[npcid].maxfriendly / 5;
			updateFriendly(npcid);
		}
	}
}

void NpcLayer::onItemGive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		std::string npcid = GlobalData::map_maps[m_addrstr].npcs[node->getTag()];
		Layer* layer = GiveLayer::create(npcid, m_addrstr);
		g_gameLayer->addChild(layer, 4);
	}
}


void NpcLayer::onItemExchange(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		std::string npcid = GlobalData::map_maps[m_addrstr].npcs[node->getTag()];
		g_gameLayer->addChild(ExchangeLayer::create(npcid), 4, "exchangelayer");
	}
}

void NpcLayer::onHostelAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int itemindex = node->getTag() / 10;
		int actionIndex = node->getTag()%10;
		int silver = 0;

		for (int i = 0; i < MyPackage::getSize(); i++)
		{
			if (MyPackage::vec_packages[i].strid.compare("80") == 0)
			{
				silver = MyPackage::vec_packages[i].count;
				break;
			}
		}

		if (silver < silvercost[actionIndex])
		{
			HintBox* hintBox = HintBox::create(CommonFuncs::gbk2utf("我靠。。。银子没带够！！"));
			addChild(hintBox);
		}
		else
		{
			CannotTouchLayer* layer = CannotTouchLayer::create();
			g_gameLayer->addChild(layer, 5, "notouchlayer");

			std::string childname = StringUtils::format("npcnode%d", itemindex);
			Node* npcitem = m_scrollview->getChildByName(childname);
			cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
			talkbtn->setEnabled(false);

			cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");
			onFight->setEnabled(false);

			cocos2d::ui::Button* onmsterbtn = (cocos2d::ui::Button*)npcitem->getChildByName("msterbtn");
			onmsterbtn->setEnabled(false);

			cocos2d::ui::Widget* pbg = (cocos2d::ui::Widget*)npcitem->getChildByName("progressbg");
			pbg->setVisible(true);
			cocos2d::ui::LoadingBar* pbar = (cocos2d::ui::LoadingBar*)npcitem->getChildByName("loadingbar");
			pbar->setVisible(true);

			cocos2d::ui::Text* ptext = (cocos2d::ui::Text*)npcitem->getChildByName("actiontext");
			ptext->setVisible(true);

			if (actionIndex == 0)
			{
				ptext->setString(CommonFuncs::gbk2utf("吃饭中..."));
			}
			else if (actionIndex == 1)
			{
				ptext->setString(CommonFuncs::gbk2utf("睡觉八小时"));
				int exminute = 480;
				g_nature->setTimeInterval(exminute * NORMAL_TIMEINTERVAL * 1.0f / (ACTION_BAR_TIME * TIMESCALE));
				g_hero->sleep(TIMESCALE * ACTION_BAR_TIME, exminute / 60);
			}

			else if (actionIndex == 2)
			{
				ptext->setString(CommonFuncs::gbk2utf("喝酒中..."));
			}
			pbar->runAction(Sequence::create(MyProgressTo::create(ACTION_BAR_TIME, 100), CallFuncN::create(CC_CALLBACK_1(NpcLayer::actionOver, this, npcitem, actionIndex)), NULL));
		}
	}
}

void NpcLayer::onRepair(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int tag = node->getTag();
		RepairLayer* layer = RepairLayer::create(tag);
		g_gameLayer->addChild(layer, 4);
	}
}

void NpcLayer::actionOver(Ref* pSender, Node* item, int actionindex)
{
	MyPackage::cutone("80", silvercost[actionindex]);
	if (actionindex == 0)
	{
		float hungervale = g_hero->getHungerValue();
		int addvalue = 40;
		if (addvalue + hungervale > g_hero->getMaxHungerValue())
			g_hero->setHungerValue(g_hero->getMaxHungerValue());
		else
			g_hero->recoverHunger(addvalue);
	}
	else if (actionindex == 2)
	{
		float value = g_hero->getSpiritValue();
		float addvalue = 40;
		if (addvalue + value > g_hero->getMaxSpiritValue())
			g_hero->setSpiritValue(g_hero->getMaxSpiritValue());
		else
			g_hero->setSpiritValue(addvalue + value);
	}

	g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);

	cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)item->getChildByName("talkbtn");
	talkbtn->setEnabled(true);

	cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)item->getChildByName("fightbtn");
	onFight->setEnabled(true);

	cocos2d::ui::Button* onmsterbtn = (cocos2d::ui::Button*)item->getChildByName("msterbtn");
	onmsterbtn->setEnabled(true);

	cocos2d::ui::Widget* pbg = (cocos2d::ui::Widget*)item->getChildByName("progressbg");
	pbg->setVisible(false);
	cocos2d::ui::LoadingBar* pbar = (cocos2d::ui::LoadingBar*)item->getChildByName("loadingbar");
	pbar->setPercent(0);
	pbar->setVisible(false);

	cocos2d::ui::Widget* ptext = (cocos2d::ui::Widget*)item->getChildByName("actiontext");
	ptext->setVisible(false);

	g_gameLayer->removeChildByName("notouchlayer");
}

bool NpcLayer::doCheckPlotMisson(int type, NpcData npcdata)
{
	bool isplotMissioning = false;
	std::string wordstr;
	int curplot = 0;
	PlotMissionData* plotData = NULL;

	if (type == 0)
	{
		curplot = GlobalData::getPlotMissionIndex();
		plotData = &GlobalData::vec_PlotMissionData[curplot];
	}
	else
	{
		std::string curmid = GlobalData::getCurBranchPlotMissison();
		if (curmid.length() > 0)
		{
			int subindex = GlobalData::map_BranchPlotMissionItem[curmid].subindex;
			PlotMissionData pd = GlobalData::map_BranchPlotMissionData[curmid][subindex];
			if (pd.unlockchapter <= GlobalData::getUnlockChapter() && GlobalData::map_BranchPlotMissionItem[curmid].count > 0)
			{
				plotData = &GlobalData::map_BranchPlotMissionData[curmid][subindex];
			}
		}
		else
		{
			std::map<std::string, std::vector<PlotMissionData>>::iterator it;
			for (it = GlobalData::map_BranchPlotMissionData.begin(); it != GlobalData::map_BranchPlotMissionData.end(); it++)
			{
				if (GlobalData::map_BranchPlotMissionData[it->first].size() > 0)
				{
					PlotMissionData* pmdata = &GlobalData::map_BranchPlotMissionData[it->first][0];
					if (pmdata->snpc.compare(npcdata.id) == 0 && pmdata->unlockchapter <= GlobalData::getUnlockChapter() && GlobalData::map_BranchPlotMissionItem[pmdata->id].count > 0)
					{
						plotData = pmdata;
						break;
					}
				}
			}
			
		}
	}
	
	if (plotData != NULL)
	{
		if (plotData->snpc.compare(npcdata.id) == 0 && plotData->status != M_DONE)
		{
			isplotMissioning = true;
			plotData->status = M_DOING;
			if (type == 1)
				GlobalData::saveBranchPlotMissionStatus(plotData->id, plotData->status);
			updatePlotUI(type);
			for (unsigned int m = 0; m < plotData->words.size(); m++)
			{
				wordstr = StringUtils::format("%s%s%s", npcdata.name, CommonFuncs::gbk2utf("：").c_str(), plotData->words[m].c_str());
				if (g_hero->getHeadID() == 4)
					wordstr = replaceSexWord(wordstr);
				vec_wordstr.push_back(wordstr);

				wordstr = StringUtils::format("%s%s%s", g_hero->getMyName().c_str(), CommonFuncs::gbk2utf("：").c_str(), plotData->mywords[m].c_str());
				vec_wordstr.push_back(wordstr);
			}
			if (g_maplayer != NULL)
				g_maplayer->updataPlotMissionIcon(type);
		}

		if (plotData->status == M_DOING)
		{
			if (plotData->dnpc.compare(npcdata.id) == 0)
			{
				isplotMissioning = true;

				if (plotData->type == 0)
				{
					plotData->status = M_DONE;
					std::vector<std::string> vec_rwdres = plotData->rewords;
					getWinRes(vec_rwdres, m_addrstr);

					int unlockchapter = 0;
					if (type == 0)
					{
						unlockchapter = plotData->unlockchapter;
						GlobalData::setUnlockChapter(unlockchapter);

						GlobalData::setPlotMissionIndex(curplot + 1);
					}
					else
					{
						int subindex = GlobalData::map_BranchPlotMissionItem[plotData->id].subindex;
						GlobalData::map_BranchPlotMissionData[plotData->id][subindex].status = M_NONE;
						if (subindex + 1 >= GlobalData::map_BranchPlotMissionData[plotData->id].size())
						{
							GlobalData::map_BranchPlotMissionItem[plotData->id].subindex = 0;
							GlobalData::map_BranchPlotMissionItem[plotData->id].count--;
							GlobalData::map_BranchPlotMissionItem[plotData->id].time = GlobalData::map_BranchPlotMissionItem[plotData->id].maxtime;
							GlobalData::saveBranchPlotMissionStatus("", 0);
						}
						else
						{
							GlobalData::map_BranchPlotMissionItem[plotData->id].subindex++;
							GlobalData::saveBranchPlotMissionStatus(plotData->id, M_NONE);
						}
						
						Winlayer::showMissionAnim(g_gameLayer, "任务完成", vec_rwdres);
					}

					updatePlotUI(type);

					if (g_maplayer != NULL)
					{
						g_maplayer->updataPlotMissionIcon(type);
						if (unlockchapter > 0 && type == 0 && unlockchapter <= MAXCHAPTER)
						{
							g_maplayer->scheduleOnce(schedule_selector(MapLayer::showUnlockLayer), 1.0f);
						}

						if (plotData->dnpc.compare("n089") == 0)
						{
							g_maplayer->scheduleOnce(schedule_selector(MapLayer::showEndAnim), 1.5f);
						}
					}

				}
				//else
				//{
				//	GlobalData::saveBranchPlotMissionStatus(plotData->id, plotData->status);
				//}

				for (unsigned int m = 0; m < plotData->bossword.size(); m++)
				{
					wordstr = StringUtils::format("%s%s%s", npcdata.name, CommonFuncs::gbk2utf("：").c_str(), plotData->bossword[m].c_str());
					if (g_hero->getHeadID() == 4)
						wordstr = replaceSexWord(wordstr);
					vec_wordstr.push_back(wordstr);

				}
			}
		}
		if (isplotMissioning)
		{
			if (type == 0)
			{
				GlobalData::savePlotMissionStatus();
				for (unsigned int i = 0; i < GlobalData::vec_achiveData.size(); i++)
				{
					if (GlobalData::vec_achiveData[i].type == A_6)
					{
						if (plotData->id.compare(GlobalData::vec_achiveData[i].vec_para[0]) == 0 && GlobalData::vec_achiveData[i].finish != -1)
						{
							GlobalData::vec_achiveData[i].finish = 1;
							GlobalData::saveAchiveData();
						}
					}
				}
			}
			else if (plotData->type != 0)
			{
				GlobalData::saveBranchPlotMissionStatus(plotData->id, plotData->status);
			}
		}
	}

	return isplotMissioning;
}


int NpcLayer::checkIsMissing(int type, std::string npcid)
{
	bool isplotMissioning = false;
	int curplot = 0;
	PlotMissionData* plotData = NULL;
	if (type == 0)
	{
		curplot = GlobalData::getPlotMissionIndex();
		plotData = &GlobalData::vec_PlotMissionData[curplot];

		if (plotData->snpc.compare(npcid) == 0 && plotData->status != M_DONE)
		{
			isplotMissioning = true;
		}

		if (plotData->dnpc.compare(npcid) == 0 && plotData->status == M_DOING)
		{
			isplotMissioning = true;
		}
	}
	else
	{
		isplotMissioning = GlobalData::isDoingBranchPlotMisson();
	}

	return isplotMissioning;
}

void NpcLayer::updatePlotUI(int type)
{
	int plotIndex = 0;
	PlotMissionData* plotData = NULL;
	int ncpsize = GlobalData::map_maps[m_addrstr].npcs.size();
	if (type == 0)
	{
		plotIndex = GlobalData::getPlotMissionIndex();
		plotData = &GlobalData::vec_PlotMissionData[plotIndex];
	}
	else
	{
		std::string curmid = GlobalData::getCurBranchPlotMissison();
		if (curmid.length() > 0)
		{
			int subindex = GlobalData::map_BranchPlotMissionItem[curmid].subindex;
			PlotMissionData pd = GlobalData::map_BranchPlotMissionData[curmid][subindex];
			if (pd.unlockchapter <= GlobalData::getUnlockChapter() && GlobalData::map_BranchPlotMissionItem[curmid].count > 0)
			{
				plotData = &GlobalData::map_BranchPlotMissionData[curmid][subindex];
			}
		}
		else
		{

			std::map<std::string, std::vector<PlotMissionData>>::iterator it;
			for (it = GlobalData::map_BranchPlotMissionData.begin(); it != GlobalData::map_BranchPlotMissionData.end(); it++)
			{
				if (GlobalData::map_BranchPlotMissionData[it->first].size() > 0)
				{
					PlotMissionData* pmdata = &GlobalData::map_BranchPlotMissionData[it->first][0];
					for (int i = 0; i < ncpsize; i++)
					{
						if (pmdata->snpc.compare(GlobalData::map_maps[m_addrstr].npcs[i]) == 0 && pmdata->unlockchapter <= GlobalData::getUnlockChapter() && GlobalData::map_BranchPlotMissionItem[pmdata->id].count > 0)
						{
							//PlotMissionData pd = *sit;
							plotData = pmdata;
							break;
						}
					}
				}
			}
		}
	}
	if (plotData != NULL)
	{
		std::string snpc = plotData->snpc;
		std::string dnpc = plotData->dnpc;
		for (int i = 0; i < ncpsize; i++)
		{
			std::string childname = StringUtils::format("npcnode%d", i);
			Node* npcitem = m_scrollview->getChildByName(childname);
			cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
			cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");

			std::string smissionname = StringUtils::format("m%d_0", type);
			if (talkbtn->getChildByName(smissionname) != NULL)
				talkbtn->removeChildByName(smissionname);

			if (snpc.compare(GlobalData::map_maps[m_addrstr].npcs[i]) == 0)
			{
				if (plotData->status == M_NONE)
				{
					if (plotData->words.size() <= 0)
					{
						plotData->status = M_DOING;
					}
					else
					{
						std::string miconstr = StringUtils::format("ui/mapmission%d_0.png", type);
						Sprite* micon = Sprite::createWithSpriteFrameName(miconstr);
						micon->setScale(0.6f);
						micon->setPosition(Vec2(talkbtn->getContentSize().width - 10, talkbtn->getContentSize().height - 10));
						talkbtn->addChild(micon, 0, smissionname);
					}
					//支线任务不现实提示标签
					if (type == 1)
					{
						if (onFight->getChildByName(smissionname) != NULL)
							onFight->removeChildByName(smissionname);

						if (talkbtn->getChildByName(smissionname) != NULL)
							talkbtn->removeChildByName(smissionname);
					}
				}
			}
		}

		for (int i = 0; i < ncpsize; i++)
		{
			std::string childname = StringUtils::format("npcnode%d", i);
			Node* npcitem = m_scrollview->getChildByName(childname);
			cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
			cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");

			std::string dmissionname = StringUtils::format("m%d_1", type);

			if (onFight->getChildByName(dmissionname) != NULL)
				onFight->removeChildByName(dmissionname);

			if (talkbtn->getChildByName(dmissionname) != NULL)
				talkbtn->removeChildByName(dmissionname);

			bool ret = false;
			if (plotData->mapid.length() > 0)
			{
				if (plotData->mapid.compare(m_addrstr) == 0)
					ret = true;
			}
			else
			{
				ret = true;
			}

			if (dnpc.compare(GlobalData::map_maps[m_addrstr].npcs[i]) == 0 && ret)
			{

				if (plotData->status == M_DOING)
				{
					std::string diconstr = StringUtils::format("ui/mapmission%d_1.png", type);
					Sprite* dicon = Sprite::createWithSpriteFrameName(diconstr);
					dicon->setScale(0.6f);
					if (plotData->type == 1)
					{
						dicon->setPosition(Vec2(onFight->getContentSize().width - 10, onFight->getContentSize().height - 10));
						onFight->addChild(dicon, 0, dmissionname);
					}
					else
					{
						dicon->setPosition(Vec2(talkbtn->getContentSize().width - 10, talkbtn->getContentSize().height - 10));
						talkbtn->addChild(dicon, 0, dmissionname);
					}
				}
			}
			//支线任务不现实提示标签
			if (type == 1)
			{
				if (onFight->getChildByName(dmissionname) != NULL)
					onFight->removeChildByName(dmissionname);

				if (talkbtn->getChildByName(dmissionname) != NULL)
					talkbtn->removeChildByName(dmissionname);
			}
		}
	}
}

void NpcLayer::getWinRes(std::vector<std::string> vec_res, std::string addrid)
{
	std::vector<PackageData> tempResData;//背包满了，保存到临时存放点
	std::vector<std::string> winres = vec_res;
	int addret = 0;
	PackageData data;
	for (unsigned int i = 0; i < winres.size(); i++)
	{
		int res = atoi(winres[i].c_str());

		if (res != 0)
		{
			bool isfind = false;
			std::string strid = StringUtils::format("%d", res / 1000);
			data.strid = strid;
			data.count = res % 1000;

			for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
			{
				ResData rdata = GlobalData::vec_resData[i];
				if (atoi(rdata.strid.c_str()) == res / 1000)
				{
					isfind = true;
					data.type = rdata.type - 1;
					break;
				}
			}

			if (!isfind)
			{
				std::map<std::string, std::vector<BuildActionData>>::iterator it;
				for (it = GlobalData::map_buidACData.begin(); it != GlobalData::map_buidACData.end(); ++it)
				{
					std::vector<BuildActionData> vec_bactData = GlobalData::map_buidACData[it->first];

					for (unsigned int m = 0; m < vec_bactData.size(); m++)
					{
						BuildActionData bdata = vec_bactData[m];
						if (atoi(bdata.icon) == res / 1000)
						{
							isfind = true;
							data.strid = bdata.icon;
							data.count = res % 1000;
							data.type = bdata.type - 1;
							data.extype = bdata.extype;
							break;
						}
					}
					if (isfind)
						break;
				}
			}
			int count = data.count;
			for (int i = 0; i < count; i++)
			{
				PackageData onedata = data;
				onedata.count = 1;
				addret = MyPackage::add(onedata);
				if (addret < 0)
					break;
				else
				{
					data.count--;
				}
			}
		}
		else
		{
			std::string strid = winres[i];
			data.strid = strid;
			data.count = 1;

			bool isfind = false;
			std::map<std::string, WG_NGData>::iterator it;
			for (it = GlobalData::map_wgngs.begin(); it != GlobalData::map_wgngs.end(); ++it)
			{
				WG_NGData gfdata = GlobalData::map_wgngs[it->first];
				if (winres[i].compare(gfdata.id) == 0 && !g_hero->checkifHasGF_Equip(winres[i]) && GlobalData::tempHasGf_Equip(winres[i]).length() <= 0)
				{
					isfind = true;
					data.strid = gfdata.id;
					data.count = 1;
					data.type = gfdata.type - 1;
					data.extype = gfdata.extype;
					addret = MyPackage::add(data);
					break;
				}
			}

			if (!isfind)
			{
				std::map<std::string, EquipData>::iterator ite;
				for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
				{
					EquipData edata = GlobalData::map_equips[ite->first];
					if (winres[i].compare(edata.id) == 0 && !g_hero->checkifHasGF_Equip(winres[i]) && GlobalData::tempHasGf_Equip(winres[i]).length() <= 0)
					{
						data.strid = edata.id;
						data.count = 1;
						data.type = edata.type - 1;
						data.extype = edata.extype;
						addret = MyPackage::add(data);
						break;
					}
				}
			}
		}
		if (addret < 0 && data.count > 0)
		{
			tempResData.push_back(data);
		}
	}
	if (tempResData.size() > 0)
	{
		std::string datastr = GameDataSave::getInstance()->getTempStorage(addrid);

		for (unsigned int i = 0; i < tempResData.size(); i++)
		{
			std::string sepstr;
			if (datastr.size() <= 0)
				sepstr = "";
			else
				sepstr = ";";
			std::string onestr = StringUtils::format("%s%s-%d-%d-%d-%d-%d-%d-%d-%d", sepstr.c_str(), tempResData[i].strid.c_str(), tempResData[i].type, tempResData[i].count, tempResData[i].extype, tempResData[i].lv, tempResData[i].exp, tempResData[i].goodvalue, tempResData[i].slv, tempResData[i].tqu);
			datastr.append(onestr);

			int tmptype = tempResData[i].type;
			std::string tmpstrid = tempResData[i].strid;
			if (tmptype == W_GONG || tmptype == N_GONG || tmptype == WEAPON || tmptype == PROTECT_EQU)
			{
				GlobalData::map_tempGf_Equip[addrid].clear();
				GlobalData::map_tempGf_Equip[addrid].push_back(tmpstrid);
			}
		}
		GameDataSave::getInstance()->setTempStorage(addrid, datastr);
	}

}

int NpcLayer::checkFightCount(std::string npcid)
{

	int npcdf = GlobalData::map_npcs[npcid].df;
	int npcatk = GlobalData::map_npcs[npcid].atk;

	int heroCurAck = g_hero->getTotalAtck();

	int npchurt = heroCurAck - npcdf;
	float fminack = 0.1f * heroCurAck;
	int intminack = int(fminack + 0.5f);
	if (npchurt < intminack)
		npchurt = intminack;

	if (npchurt <= 0)
		npchurt = 1;

	int npclife = GlobalData::map_npcs[npcid].life;
	
	int heroVsBossCount = npclife % npchurt == 0 ? npclife / npchurt : (npclife / npchurt + 1);

	int curheroDf = g_hero->getTotalDf();

	int herohurt = npcatk - curheroDf;

	fminack = 0.1f * npcatk;
	intminack = int(fminack + 0.5f);

	if (herohurt < intminack)
		herohurt = intminack;

	if (herohurt <= 0)
		herohurt = 1;

	int intlife = int(g_hero->getLifeValue() + 1);
	int bossVsHeroCount = intlife % herohurt == 0 ? intlife / herohurt : (intlife / herohurt + 1);

	if (heroVsBossCount <= bossVsHeroCount)
		return heroVsBossCount;
	else
		return -bossVsHeroCount;
}

std::string NpcLayer::replaceSexWord(std::string dstr)
{
	std::string retstr = dstr;
	int size = sizeof(replacestr) / sizeof(replacestr[0]);
	for (int i = 0; i < size; i++)
	{
		retstr = CommonFuncs::replace_all(dstr, CommonFuncs::gbk2utf(replacestr[i].c_str()), CommonFuncs::gbk2utf(areplacestr[i].c_str()));
	}
	return retstr;
}

void NpcLayer::reFreshFriendlyUI()
{
	int npcitemcount = m_scrollview->getChildrenCount();
	for (int i = 0; i < npcitemcount; i++)
	{
		std::string childname = StringUtils::format("npcnode%d", i);
		Node* npcitemnode = m_scrollview->getChildByName(childname);
		std::string npcid = GlobalData::map_maps[m_addrstr].npcs[i];

		int friendly = 0;
		if (GlobalData::map_myfriendly.find(npcid) != GlobalData::map_myfriendly.end())
		{
			friendly = GlobalData::map_myfriendly[npcid].friendly;
		}

		int maxfriendly = GlobalData::map_NPCFriendData[npcid].maxfriendly;
		if (friendly > maxfriendly)
			friendly = maxfriendly;
		else if (friendly < -maxfriendly)
			friendly = -maxfriendly;

		int per = maxfriendly / 5;
		int count = abs(friendly/per);

		for (int m = 0; m < 5; m++)
		{
			std::string barstr = StringUtils::format("fbar_%d", m);
			cocos2d::ui::LoadingBar* friendbar = (cocos2d::ui::LoadingBar*)npcitemnode->getChildByName(barstr);
			friendbar->setPercent(0);
			if (friendly < 0)
			{
				friendbar->loadTexture("ui/fheart0.png", cocos2d::ui::TextureResType::PLIST);
			}
			else
			{
				friendbar->loadTexture("ui/fheart1.png", cocos2d::ui::TextureResType::PLIST);
			}
			if (m < count)
			{
				friendbar->setPercent(100);
			}
			else if (m == count)
			{
				float percent = fabs(friendly % per * 100.0f / per);
				friendbar->setPercent(percent);
			}
		}

	}
}

void NpcLayer::reFreshRelationUI()
{
	int npcitemcount = m_scrollview->getChildrenCount();
	for (int i = 0; i < npcitemcount; i++)
	{
		std::string childname = StringUtils::format("npcnode%d", i);
		Node* npcitemnode = m_scrollview->getChildByName(childname);
		std::string npcid = GlobalData::map_maps[m_addrstr].npcs[i];

		cocos2d::ui::Button* masterbtn = (cocos2d::ui::Button*)npcitemnode->getChildByName("msterbtn");
		cocos2d::ui::Button* friendbtn = (cocos2d::ui::Button*)npcitemnode->getChildByName("friendbtn");
		cocos2d::ui::Text* msterbtnlabel = (cocos2d::ui::Text*)masterbtn->getChildByName("text");
		cocos2d::ui::Text* friendbtnlabel = (cocos2d::ui::Text*)friendbtn->getChildByName("text");

		int relation = F_NOMAR;

		if (GlobalData::map_myfriendly.find(npcid) != GlobalData::map_myfriendly.end())
		{
			relation = GlobalData::map_myfriendly[npcid].relation;
		}

		if (relation == F_MASTEROUT)
		{
			masterbtn->setEnabled(false);
			msterbtnlabel->setString(CommonFuncs::gbk2utf("出师"));
		}
		else if (relation == F_NOMAR)
		{
			friendbtnlabel->setString(CommonFuncs::gbk2utf("结交"));
			msterbtnlabel->setString(CommonFuncs::gbk2utf("拜师"));
		}
		else if (relation == F_FRIEND)
		{
			friendbtnlabel->setString(CommonFuncs::gbk2utf("绝交"));
		}
		else if (relation == F_MASTER)
		{
			msterbtnlabel->setString(CommonFuncs::gbk2utf("出师"));
		}

	}
}

void NpcLayer::updateFriendly(std::string npcid)
{
	GlobalData::map_myfriendly[npcid].friendly -= 5;
	int friendly = GlobalData::map_myfriendly[npcid].friendly;
	int needfriendly = GlobalData::map_NPCMasterData[npcid].needfriendly;
	int masterfriendly = GlobalData::map_NPCMasterData[npcid].needfriendly;
	if (friendly < needfriendly)
	{
		if (GlobalData::map_myfriendly[npcid].relation == F_FRIEND)
		{
			GlobalData::map_myfriendly[npcid].relation = F_NOMAR;
			std::string desc = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("与").c_str(), GlobalData::map_npcs[npcid].name, CommonFuncs::gbk2utf("关系恶化，不再是朋友！").c_str());
			g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
			GlobalData::map_myfriendly[npcid].friendly -= GlobalData::map_NPCFriendData[npcid].maxfriendly / 5;
		}
	}
	if (friendly < masterfriendly)
	{
		if (GlobalData::map_myfriendly[npcid].relation == F_MASTER)
		{
			GlobalData::map_myfriendly[npcid].relation = F_NOMAR;
			std::string desc = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("不懂尊师重道，被").c_str(), GlobalData::map_npcs[npcid].name, CommonFuncs::gbk2utf("逐出师门！").c_str());
			g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
			GlobalData::map_myfriendly[npcid].friendly -= GlobalData::map_NPCFriendData[npcid].maxfriendly / 5;
		}
	}
	GlobalData::saveFriendly();
	reFreshFriendlyUI();
	reFreshRelationUI();
}

void NpcLayer::showTalkGuider()
{
	if (NewerGuide2Layer::checkifNewerGuide(100) && GlobalData::getUnlockChapter() == 2)
	{
		std::vector<Node*> vec_node;
		int npcitemcount = m_scrollview->getChildrenCount();
		if (npcitemcount > 0)
		{
			std::string childname = StringUtils::format("npcnode%d", 0);
			Node* npcitemnode = m_scrollview->getChildByName(childname);

			cocos2d::ui::Widget* fbtn = (cocos2d::ui::Widget*)npcitemnode->getChildByName("friendbtn");
			fbtn->setUserData((void*)"npcbtn1");
			vec_node.push_back(fbtn);

			cocos2d::ui::Widget* gbtn = (cocos2d::ui::Widget*)npcitemnode->getChildByName("givebtn");
			gbtn->setUserData((void*)"npcbtn1");
			vec_node.push_back(gbtn);
			
			for (int i = 0; i < 5; i++)
			{
				std::string str = StringUtils::format("fheartbg_%d", i);
				cocos2d::ui::Widget* npcheartbg = (cocos2d::ui::Widget*)npcitemnode->getChildByName(str);
				npcheartbg->setUserData((void*)"fheartbg");
				vec_node.push_back(npcheartbg);

				//str = StringUtils::format("fbar_%d", i);
				//cocos2d::ui::Widget* npcheart = (cocos2d::ui::Widget*)npcitemnode->getChildByName(str);
				//npcheart->setUserData("fheart1");
				//vec_node.push_back(npcheart);
			}
		}
		NewerGuide2Layer *layer = NewerGuide2Layer::create(100, vec_node);
		g_gameLayer->addChild(layer, NEWERLAYERZOER);
	}
}
