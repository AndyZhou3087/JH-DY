#include "MatchFightLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "Winlayer.h"
#include "SoundManager.h"
#include "AnalyticUtil.h"
#include "MapLayer.h"
#include "Shake.h"
#include "MyActionProgressTimer.h"
#include "MatchFightResultLayer.h"
#include "GameDataSave.h"
#include"GameScene.h"

MatchFightLayer::MatchFightLayer()
{
	isUseWg = false;
	win = 0;
	nextmyhero = 1;
	nextplayerhero = 1;
	GlobalData::isPlayerChallenging = true;
}


MatchFightLayer::~MatchFightLayer()
{
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_ENTER_MAPADDR);
}

MatchFightLayer* MatchFightLayer::create(std::string addrid)
{
	MatchFightLayer *pRet = new MatchFightLayer();
	if (pRet && pRet->init(addrid))
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

bool MatchFightLayer::init(std::string addrid)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("fightLayer.csb");
	csbnode->setPositionY(100);
	this->addChild(csbnode);

	//地点名称
	cocos2d::ui::Text* addrnametxt = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	addrnametxt->setString(GlobalData::map_maps[addrid].cname);

	myheroicon = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroicon");

	// player 图标
	playerhead = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcicon");

	//player名称
	playernametxt = (cocos2d::ui::Text*)csbnode->getChildByName("npcname");
	playernametxt->setString(GlobalData::matchPlayerInfo.nickname);
	if (GlobalData::matchPlayerInfo.nickname.length() >= 12)
		playernametxt->setFontSize(25);
	//角色名
	cocos2d::ui::Text* heronametxt = (cocos2d::ui::Text*)csbnode->getChildByName("heroname");
	heronametxt->setString(GlobalData::getMyNickName());

	//角色血量显示
	myhpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("herohpvaluetext");

	//角色血量进度
	myhpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("herohpbar");

	myhpbar2 = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("herohpbar2");

	updateMyInfo();

	playermaxhp = getPlayerMaxLife(nextplayerhero - 1);
	playerlife = playermaxhp;

	playeratk = getPlayerAtk(nextplayerhero - 1);
	playerdf = getPlayerDf(nextplayerhero - 1);

	//palyer血量显示
	playerhpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("npchpvaluetext");

	//palyer血量进度
	playerhpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("npchpbar");

	playerhpbar2 = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("npchpbar2");

	updatePlayerInfo();
	// 滚动文字
	m_fihgtScorll = UIScroll::create(610.0f, 435.0f);
	m_fihgtScorll->setPosition(Vec2(360, 350));
	csbnode->addChild(m_fihgtScorll);

	myactimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroactimg");
	playeractimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcactimg");

	mycritfnt = (cocos2d::ui::TextBMFont*)myactimg->getChildByName("herocritfnt");
	playercritfnt = (cocos2d::ui::TextBMFont*)playeractimg->getChildByName("npccritfnt");

	cocos2d::ui::Button* escapebtn = (cocos2d::ui::Button*)csbnode->getChildByName("escapebtn");
	escapebtn->setVisible(false);

	GameDataSave::getInstance()->setHsljMatchPlayer(GlobalData::matchPlayerInfo.playerid);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	int r = GlobalData::createRandomNum(4);
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_FIGHT_0 + r);
	return true;
}

void MatchFightLayer::updateMyInfo()
{
	std::string heroiconstr = StringUtils::format("ui/fhero%d.png", nextmyhero);
	myheroicon->loadTexture(heroiconstr, cocos2d::ui::TextureResType::PLIST);
	myheroicon->setScale(0.6f);

	curmyherolocalid = loadMyData(nextmyhero);

	int maxlife = getMyMaxLife(curmyherolocalid);

	mylife = maxlife;
	//角色血量显示
	std::string hpstr = StringUtils::format("%d/%d", mylife, maxlife);
	myhpvaluetext->setString(hpstr);

	//角色血量进度
	float herohppercent = 100 * mylife / maxlife;

	myhpbar->setPercent(herohppercent);

	myhpbar2->setPercent(herohppercent);

	resetSkills();

	this->scheduleOnce(schedule_selector(MatchFightLayer::delayMyFight), 1.5f);//1.0s，hero->player
}

void MatchFightLayer::updatePlayerInfo()
{
	std::string playerheadstr = StringUtils::format("ui/fhero%d.png", GlobalData::vec_matchPlayerData[nextplayerhero - 1].type);
	playerhead->loadTexture(playerheadstr, cocos2d::ui::TextureResType::PLIST);
	playerhead->setScale(0.6f);
	playerhead->setFlippedX(true);

	playermaxhp = getPlayerMaxLife(nextplayerhero - 1);
	playerlife = playermaxhp;

	playeratk = getPlayerAtk(nextplayerhero - 1);
	playerdf = getPlayerDf(nextplayerhero - 1);

	//palyer血量显示
	std::string hpstr = StringUtils::format("%d/%d", playerlife, playermaxhp);
	playerhpvaluetext->setString(hpstr);

	//palyer血量进度
	int playerhppercent = 100 * playerlife / playermaxhp;
	playerhpbar->setPercent(playerhppercent);

	playerhpbar2->setPercent(playerhppercent);
	resetSkills();
	this->scheduleOnce(schedule_selector(MatchFightLayer::delayPlayerFight), 1.5f);
}

std::string MatchFightLayer::loadMyData(int herotype)
{
	std::vector<std::string> vec_ids = GlobalData::getSaveListId();

	std::vector<std::string> vec_userid;
	for (unsigned int i = 0; i < vec_ids.size(); i++)
	{
		std::string saveuid = vec_ids[i];
		vec_userid.push_back(saveuid);
	}
	GlobalData::myMatchInfo.map_myfriendly.clear();
	std::string localid = vec_userid[herotype - 1];
	if (localid.length() > 0)
	{
		std::string datastr = GameDataSave::getInstance()->getFriendly(localid);
		std::vector<std::string> vec_retstr;
		CommonFuncs::split(datastr, vec_retstr, ";");
		for (unsigned int i = 0; i < vec_retstr.size(); i++)
		{
			std::vector<std::string> tmp;
			CommonFuncs::split(vec_retstr[i], tmp, ",");
			if (tmp.size() >= 3)
			{
				int friendly = atoi(tmp[1].c_str());
				if (friendly < -100000 || friendly > 100000)
					friendly = 0;
				GlobalData::myMatchInfo.map_myfriendly[tmp[0]].friendly = friendly;
				GlobalData::myMatchInfo.map_myfriendly[tmp[0]].relation = atoi(tmp[2].c_str());
			}
		}

		std::string strval = GameDataSave::getInstance()->getHeroProperData(localid);
		std::vector<std::string> tmp;
		CommonFuncs::split(strval, tmp, ";");
		//解析
		for (unsigned int i = 0; i < tmp.size(); i++)
		{
			std::vector<std::string> tmp2;
			CommonFuncs::split(tmp[i], tmp2, "-");

			PackageData sdata;
			sdata.strid = tmp2[0];
			sdata.type = atoi(tmp2[1].c_str());
			sdata.count = atoi(tmp2[2].c_str());
			sdata.extype = GlobalData::getResExType(sdata.strid);//atoi(tmp2[3].c_str());
			sdata.lv = atoi(tmp2[4].c_str());
			sdata.exp = atoi(tmp2[5].c_str());
			sdata.goodvalue = atoi(tmp2[6].c_str());//耐久度
			if (tmp2.size() >= 9)
			{
				sdata.slv = atoi(tmp2[7].c_str());//
				sdata.tqu = atoi(tmp2[8].c_str());//
			}

			if (sdata.type == WEAPON || sdata.type == PROTECT_EQU)
			{
				GlobalData::myMatchInfo.map_EquipData[sdata.strid] = sdata.slv * 1000 + sdata.goodvalue;
			}
			else if (sdata.type == N_GONG || sdata.type == W_GONG)
			{
				GlobalData::myMatchInfo.map_EquipData[sdata.strid] = sdata.lv;
			}
		}
	}

	return localid;
}

int MatchFightLayer::getMyMaxLife(std::string localid)
{
	float friendhppercent = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::myMatchInfo.map_myfriendly.begin(); it != GlobalData::myMatchInfo.map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::myMatchInfo.map_myfriendly[nid].relation == F_FRIEND)
		{
			friendhppercent += GlobalData::map_NPCFriendData[nid].hppercent / 100;
		}
		else if (GlobalData::myMatchInfo.map_myfriendly[nid].relation == F_MASTER)
		{
			friendhppercent += GlobalData::map_NPCMasterData[nid].hppercent / 100;
		}
	}

	float mixfglifepercent = 0.0f;

	std::string mymixgf = GameDataSave::getInstance()->getMixGF(localid);
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mymixgf];

		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::myMatchInfo.map_EquipData[nid];

			if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
				mixfglifepercent += mdata.hppercent / 100;
		}
	}
	int lv = GameDataSave::getInstance()->getHeroLV(localid);
	float flife = GlobalData::map_heroAtr[nextmyhero].vec_maxhp[lv] * 1.0f;
	flife = flife + flife*friendhppercent + flife * mixfglifepercent;

	if (GlobalData::myMatchInfo.vec_factionlv[nextmyhero - 1] == 3)
		flife += flife*0.02f;
	return flife;
}

int MatchFightLayer::getMyAtk(std::string localid)
{
	int weaponAtk = 0;
	int wgAtk = 0;
	float slvAtk = 0.0f;//强化等级加攻

	std::string myweaponstr;
	std::string mywgstr;
	int slv = 0;
	int lv = 0;
	int gv = 0;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
	{
		std::string nid = fite->first;
		int value = GlobalData::myMatchInfo.map_EquipData[nid];

		if (nid.compare(0, 1, "a") == 0 && value > 0)
		{
			myweaponstr = nid;
			slv = value / 1000;
			gv = value % 1000;
		}
		else if (nid.compare(0, 1, "w") == 0 && value > 0)
		{
			mywgstr = nid;
			lv = value;
		}
	}

	if (myweaponstr.length() > 0 && gv > 0)
	{
		std::string strid = myweaponstr;
		weaponAtk = GlobalData::map_equips[strid].atk;
		slvAtk = slv * (slv + weaponAtk / 10);
	}

	float friendatkpercent = 0.0f;
	std::string masternpc;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::myMatchInfo.map_myfriendly.begin(); it != GlobalData::myMatchInfo.map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::myMatchInfo.map_myfriendly[nid].relation == F_FRIEND)
		{
			friendatkpercent += GlobalData::map_NPCFriendData[nid].atkpercent / 100;
		}
		else if (GlobalData::myMatchInfo.map_myfriendly[nid].relation == F_MASTER)
		{
			friendatkpercent += GlobalData::map_NPCMasterData[nid].atkpercent / 100;
			masternpc = nid;
		}
	}

	if (mywgstr.length() > 0)
	{
		std::string strid = mywgstr;
		int gflv = lv;
		if (gflv > GlobalData::map_wgngs[strid].maxlv - 1)
			gflv = GlobalData::map_wgngs[strid].maxlv - 1;
		wgAtk = GlobalData::map_wgngs[strid].vec_bns[gflv];

		int masteratk = 0;
		if (masternpc.length() > 0)
		{
			for (unsigned int i = 0; i < GlobalData::map_NPCMasterData[masternpc].vec_gfid.size(); i++)
			{
				if (strid.compare(GlobalData::map_NPCMasterData[masternpc].vec_gfid[i]) == 0)
				{
					int msbatk = wgAtk * GlobalData::map_NPCMasterData[masternpc].vec_gfbonus[i] / 100;
					masteratk += msbatk;
					break;
				}
			}
		}

		int mixatk = 0;
		std::string mymixgf = GameDataSave::getInstance()->getMixGF(localid);
		if (mymixgf.length() > 0)
		{
			MixGfData mdata = GlobalData::map_MixGfData[mymixgf];

			std::map<std::string, int>::iterator fite;
			for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
			{
				std::string nid = fite->first;
				int value = GlobalData::myMatchInfo.map_EquipData[nid];

				if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
					mixatk += wgAtk * mdata.atkpercent / 100;
			}
		}
		wgAtk = wgAtk + masteratk + mixatk;
	}
	int hlv = GameDataSave::getInstance()->getHeroLV(localid);
	//攻击属性
	float fack = GlobalData::map_heroAtr[nextmyhero].vec_atk[hlv] + weaponAtk + wgAtk;

	if (mywgstr.length() > 0 && myweaponstr.length() > 0)
	{
		if (GlobalData::map_wgngs[mywgstr].extype == GlobalData::map_equips[myweaponstr].extype)
		{
			float back = fack * 0.05f;
			fack += back;
		}
	}

	fack += slvAtk;

	fack += fack* friendatkpercent;

	if (GlobalData::myMatchInfo.vec_factionlv[nextmyhero - 1] == 5)
		fack += fack*0.02f;

	int tatk = int(fack + 0.5f);
	return tatk;
}


int MatchFightLayer::getMyDf(std::string localid)
{
	int adf = 0;
	int ngdf = 0;
	float slvdf = 0.0f;//强化等级加防

	float frienddfpercent = 0.0f;
	std::string masternpc;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::myMatchInfo.map_myfriendly.begin(); it != GlobalData::myMatchInfo.map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::myMatchInfo.map_myfriendly[nid].relation == F_FRIEND)
		{
			frienddfpercent += GlobalData::map_NPCFriendData[nid].dfpercent / 100;
		}
		else if (GlobalData::myMatchInfo.map_myfriendly[nid].relation == F_MASTER)
		{
			frienddfpercent += GlobalData::map_NPCMasterData[nid].dfpercent / 100;
			masternpc = nid;
		}
	}

	std::string myarmorstr;
	std::string myngstr;
	int slv = 0;
	int lv = 0;
	int gv = 0;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
	{
		std::string nid = fite->first;
		int value = GlobalData::myMatchInfo.map_EquipData[nid];

		if (nid.compare(0, 1, "e") == 0 && value > 0)
		{
			myarmorstr = nid;
			slv = value / 1000;
			gv = value % 1000;
		}
		else if (nid.compare(0, 1, "x") == 0 && value > 0)
		{
			myngstr = nid;
			lv = value;
		}
	}

	if (myngstr.length() > 0)
	{
		std::string gfname = myngstr;
		int gflv = lv;

		if (gflv > GlobalData::map_wgngs[gfname].maxlv - 1)
			gflv = GlobalData::map_wgngs[gfname].maxlv - 1;
		ngdf = GlobalData::map_wgngs[gfname].vec_bns[gflv];

		int masterdf = 0;
		if (masternpc.length() > 0)
		{
			for (unsigned int i = 0; i < GlobalData::map_NPCMasterData[masternpc].vec_gfid.size(); i++)
			{
				if (gfname.compare(GlobalData::map_NPCMasterData[masternpc].vec_gfid[i]) == 0)
				{
					int msbdf = ngdf * GlobalData::map_NPCMasterData[masternpc].vec_gfbonus[i] / 100;
					masterdf += msbdf;

					break;
				}
			}
		}

		int mixdf = 0;
		std::string mymixgf = GameDataSave::getInstance()->getMixGF(localid);
		if (mymixgf.length() > 0)
		{
			MixGfData mdata = GlobalData::map_MixGfData[mymixgf];

			std::map<std::string, int>::iterator fite;
			for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
			{
				std::string nid = fite->first;
				int value = GlobalData::myMatchInfo.map_EquipData[nid];

				if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
					mixdf += ngdf * mdata.dfpercent / 100;
			}
		}
		ngdf = ngdf + masterdf + mixdf;
	}

	if (myarmorstr.length() > 0 && gv > 0)
	{
		std::string aname = myarmorstr;
		adf = GlobalData::map_equips[aname].df;
		slvdf = slv * (slv + adf / 10);
	}
	//防御属性
	int hlv = GameDataSave::getInstance()->getHeroLV(localid);
	
	float fdf = GlobalData::map_heroAtr[nextmyhero].vec_df[hlv] + ngdf + adf;
	fdf += slvdf;

	fdf += fdf*frienddfpercent;

	if (GlobalData::myMatchInfo.vec_factionlv[nextmyhero - 1] == 4)
		fdf += fdf*0.02f;

	adf = int(fdf + 0.5f);
	return adf;
}


float MatchFightLayer::getMyCrit(std::string localid)
{
	int hlv = GameDataSave::getInstance()->getHeroLV(localid);
	int critrnd = GlobalData::map_heroAtr[nextmyhero].vec_crit[hlv];

	std::string mywgstr;
	int gflv = 0;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
	{
		std::string nid = fite->first;
		int value = GlobalData::myMatchInfo.map_EquipData[nid];

		if (nid.compare(0, 1, "w") == 0 && value > 0)
		{
			mywgstr = nid;
			gflv = value;
		}
	}

	if (mywgstr.length() > 0)
		critrnd += GlobalData::map_wgngs[mywgstr].vec_cirt[gflv];

	float friendcritrnd = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::myMatchInfo.map_myfriendly.begin(); it != GlobalData::myMatchInfo.map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::myMatchInfo.map_myfriendly[nid].relation == F_FRIEND)
		{
			friendcritrnd += GlobalData::map_NPCFriendData[nid].critpercent;
		}
		else if (GlobalData::myMatchInfo.map_myfriendly[nid].relation == F_MASTER)
		{
			friendcritrnd += GlobalData::map_NPCMasterData[nid].critpercent;
		}
	}
	critrnd += friendcritrnd;

	float mixcrit = 0.0f;
	std::string mymixgf = GameDataSave::getInstance()->getMixGF(localid);
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mymixgf];
		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::myMatchInfo.map_EquipData[nid];

			if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
				mixcrit += mdata.critpercent;
		}
	}
	critrnd += mixcrit;
	if (GlobalData::myMatchInfo.vec_factionlv[nextmyhero - 1] == 2)
		critrnd += 2;
	return critrnd;
}

float MatchFightLayer::getMyDodge(std::string localid)
{
	int hlv = GameDataSave::getInstance()->getHeroLV(localid);
	int dodgernd = GlobalData::map_heroAtr[nextmyhero].vec_dodge[hlv];

	std::string myngstr;
	int gflv = 0;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
	{
		std::string nid = fite->first;
		int value = GlobalData::myMatchInfo.map_EquipData[nid];

		if (nid.compare(0, 1, "x") == 0 && value > 0)
		{
			myngstr = nid;
			gflv = value;
		}
	}
	if (myngstr.length() > 0)
		dodgernd += GlobalData::map_wgngs[myngstr].vec_dodge[gflv];

	float frienddogdernd = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::myMatchInfo.map_myfriendly.begin(); it != GlobalData::myMatchInfo.map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::myMatchInfo.map_myfriendly[nid].relation == F_FRIEND)
		{
			frienddogdernd += GlobalData::map_NPCFriendData[nid].dodgepercent;
		}
		else if (GlobalData::myMatchInfo.map_myfriendly[nid].relation == F_MASTER)
		{
			frienddogdernd += GlobalData::map_NPCMasterData[nid].dodgepercent;
		}
	}
	dodgernd += frienddogdernd;

	float mixdodge = 0.0f;
	std::string mymixgf = GameDataSave::getInstance()->getMixGF(localid);
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mymixgf];
		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::myMatchInfo.map_EquipData[nid];

			if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
				mixdodge += mdata.dodgepercent;
		}
	}
	dodgernd += mixdodge;
	if (GlobalData::myMatchInfo.vec_factionlv[nextmyhero - 1] == 1)
		dodgernd += 2;

	return dodgernd;
}

int MatchFightLayer::getPlayerHurt()
{
	int heroCurAck = getMyAtk(curmyherolocalid);

	int playerhurt = heroCurAck - playerdf;
	float fminack = 0.1f * heroCurAck;
	int intminack = int(fminack + 0.5f);
	if (playerhurt < intminack)
		playerhurt = intminack;

	return playerhurt;
}

void MatchFightLayer::mySkillComboAtk(float dt)
{
	int count = GlobalData::map_gfskills[S_SKILL_3].leftval;
	int c = getPlayerHurt();
	playerlife -= c * count / 10;
	GlobalData::map_gfskills[S_SKILL_3].leftval--;

	if (playerlife <= 0)
	{
		playerlife = 0;
		this->unschedule(schedule_selector(MatchFightLayer::mySkillComboAtk));
	}
	updatePlayerLife();
}

void MatchFightLayer::playerSkillComboAtk(float dt)
{
	int count = GlobalData::map_gfskills[S_SKILL_3].fightPlayerleftval;
	int c = getMyHurt(curmyherolocalid);
	mylife -= c * count / 10;
	GlobalData::map_gfskills[S_SKILL_3].fightPlayerleftval--;

	if (mylife <= 0)
	{
		mylife = 0;
		this->unschedule(schedule_selector(MatchFightLayer::playerSkillComboAtk));
	}
	updateMyLife();
}

void MatchFightLayer::delayMyFight(float dt)
{
	int playerhurt = getPlayerHurt();

	int skilltype = checkPlayerSkill(H_WG);

	if (skilltype == S_SKILL_1 || skilltype == S_SKILL_2 || skilltype == S_SKILL_5)
	{
		GlobalData::map_gfskills[skilltype].fightPlayerleftval--;
	}

	if (ismyAct != 1)
	{
		if (mylife <= 0)
		{
			showResultLayer(0);
			return;
		}

		if (skilltype == S_SKILL_1 || skilltype == S_SKILL_5)
		{
			showPlayerSkill(skilltype);
			mylife -= getMyHurt(curmyherolocalid) * 3 / 10;
			if (mylife < 0)
				mylife = 0;
			updateMyLife();
			if (mylife <= 0)
			{
				showResultLayer(0);
				return;
			}
		}
		else if (skilltype == S_SKILL_2)
		{
			showPlayerSkill(skilltype);
			this->scheduleOnce(schedule_selector(MatchFightLayer::delayPlayerFight), 1.5f);
			return;
		}
		skilltype = checkPlayerSkill(H_NG);
		if (skilltype == S_SKILL_6)
		{
			showPlayerSkill(skilltype);

			std::map<std::string, int>::iterator fite;
			for (fite = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.end(); ++fite)
			{
				std::string nid = fite->first;
				int value = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData[nid];

				if (nid.compare(0, 1, "x") == 0)
				{
					playerhurt = playerhurt * (100 - GlobalData::map_wgngs[nid].skilleffect) / 100;
				}
			}
		}
		else if (skilltype == S_SKILL_7)
		{
			showPlayerSkill(skilltype);
			int mylosthp = 0;

			std::map<std::string, int>::iterator fite;
			for (fite = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.end(); ++fite)
			{
				std::string nid = fite->first;
				int value = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData[nid];

				if (nid.compare(0, 1, "x") == 0)
				{
					mylosthp = mylife * GlobalData::map_wgngs[nid].skilleffect / 100;
				}
			}

			mylife -= mylosthp;
			if (mylife < 0)
				mylife = 0;
			updateMyLife();
			playerlife = +mylosthp;
			updatePlayerLife();
			if (mylosthp <= 0)
			{
				showResultLayer(0);
				return;
			}
		}
	}
	ismyAct = -1;
	isplayerAct = - 1;
	std::string mywgstr;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
	{
		std::string nid = fite->first;
		int value = GlobalData::myMatchInfo.map_EquipData[nid];

		if (nid.compare(0, 1, "w") == 0)
			mywgstr = nid;
	}

	int critrnd = getMyCrit(curmyherolocalid) * 100;

	if (checkMySkill(H_WG) == S_SKILL_4)
	{
		showMySkill(S_SKILL_4);
		if (mywgstr.length() > 0)
		{
			critrnd += GlobalData::map_wgngs[mywgstr].skilleffect * 100;
		}
	}

	int npcdodgernd = getPlayerDodde(nextplayerhero - 1) * 100;
	int r = GlobalData::createRandomNum(10000);

	if (checkPlayerSkill(H_NG) == S_SKILL_8)
	{
		showPlayerSkill(S_SKILL_8);

		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData[nid];
			if (nid.compare(0, 1, "x") == 0)
			{
				npcdodgernd += GlobalData::map_wgngs[nid].skilleffect * 100;
			}
		}
	}

	if (r < critrnd)
	{
		ismyAct = 0;
		playerhurt *= 2;
		std::string tmpstr = "ui/crit.png";
		playeractimg->loadTexture(tmpstr, cocos2d::ui::TextureResType::PLIST);
		playeractimg->setContentSize(Sprite::createWithSpriteFrameName(tmpstr)->getContentSize());
		playeractimg->setVisible(true);
		playeractimg->setOpacity(200);
		playeractimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		playeractimg->runAction(Sequence::create(ac1, DelayTime::create(1.0f), Hide::create(), NULL));
		tmpstr = StringUtils::format("%d", playerhurt);
		playercritfnt->setString(tmpstr);
		playercritfnt->setVisible(true);
	}
	else if (r < npcdodgernd)
	{
		isplayerAct = 1;
		std::string imgstr = "ui/dodge.png";
		playeractimg->loadTexture(imgstr, cocos2d::ui::TextureResType::PLIST);
		playeractimg->setContentSize(Sprite::createWithSpriteFrameName(imgstr)->getContentSize());
		playeractimg->setVisible(true);
		playercritfnt->setVisible(false);
		playeractimg->setOpacity(200);
		playeractimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		playeractimg->runAction(Sequence::create(ac1, Shake::create(0.2f, 20, 1), DelayTime::create(0.8f), Hide::create(), NULL));

	}

	if (isplayerAct != 1)
	{
		playerlife -= playerhurt;
		if (playerlife < 0)
			playerlife = 0;
	}

	updatePlayerLife();

	showFightWord(0, playerhurt);

	if (playerhurt <= 0)//胜利
	{
		showResultLayer(1);
	}
	else
	{
		if (checkMySkill(H_WG) == S_SKILL_3)
		{
			int count = GlobalData::map_gfskills[S_SKILL_3].leftval;
			if (count > 0)
			{
				showMySkill(S_SKILL_3);
				this->schedule(schedule_selector(MatchFightLayer::mySkillComboAtk), 0.3f, count - 1, 0.2f);
			}
		}
		this->scheduleOnce(schedule_selector(MatchFightLayer::delayPlayerFight), 1.5f);//延迟显示NPC 攻击，主要文字显示，需要看一下，所以延迟下
	}
}

void MatchFightLayer::delayPlayerFight(float dt)
{

	float curmyHp = mylife;

	int myhurt = getMyHurt(curmyherolocalid);

	std::string myngstr;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
	{
		std::string nid = fite->first;
		int value = GlobalData::myMatchInfo.map_EquipData[nid];

		if (nid.compare(0, 1, "x") == 0)
			myngstr = nid;
	}

    int skilltype = checkMySkill(H_WG);
    
    if (skilltype == S_SKILL_1 || skilltype == S_SKILL_2 || skilltype == S_SKILL_5)
    {
       GlobalData::map_gfskills[skilltype].leftval--;
    }
    
	if (isplayerAct != 1)
	{
		if (playerlife <= 0)
		{
			showResultLayer(1);
			return;
		}
		
		if (skilltype == S_SKILL_1 || skilltype == S_SKILL_5)
		{
			showMySkill(skilltype);
			playerlife -= getPlayerHurt() * 3 / 10;
			if (playerlife < 0)
				playerlife = 0;
			updatePlayerLife();
			if (playerlife <= 0)
			{
				showResultLayer(1);
				return;
			}
		}
		else if (skilltype == S_SKILL_2)
		{
			showMySkill(skilltype);
			this->scheduleOnce(schedule_selector(MatchFightLayer::delayMyFight), 1.5f);
			return;
		}

		skilltype = checkMySkill(H_NG);
		if (skilltype == S_SKILL_6)
		{
			showMySkill(skilltype);
			if (myngstr.length() > 0)
			{
				myhurt = myhurt * (100 - GlobalData::map_wgngs[myngstr].skilleffect) / 100;
			}
		}
		else if (skilltype == S_SKILL_7)
		{
			showMySkill(skilltype);
			int playerlosthp = 0;
			if (myngstr.length() > 0)
			{
				playerlosthp = playerlife * GlobalData::map_wgngs[myngstr].skilleffect / 100;
			}
			playerlife -= playerlosthp;
			if (playerlife < 0)
				playerlife = 0;
			updatePlayerLife();
			mylife = curmyHp + playerlosthp;
			updateMyLife();
			if (playerlife <= 0)
			{
				showResultLayer(1);
				return;
			}
		}
		
	}


	ismyAct = -1;
	isplayerAct = -1;

	int dodgernd = getMyDodge(curmyherolocalid) * 100;
	if (checkMySkill(H_NG) == S_SKILL_8)
	{
		showMySkill(S_SKILL_8);
		if (myngstr.length() > 0)
		{
			dodgernd += GlobalData::map_wgngs[myngstr].skilleffect * 100;
		}
		
	}

	int playercritrnd = getPlayerCrit(nextplayerhero - 1) * 100;
	int r = GlobalData::createRandomNum(10000);

	if (checkPlayerSkill(H_WG) == S_SKILL_4)
	{
		showPlayerSkill(S_SKILL_4);
		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData[nid];
			if (nid.compare(0, 1, "w") == 0)
			{
				playercritrnd += GlobalData::map_wgngs[nid].skilleffect * 100;
			}
		}
		
	}

	if (r < playercritrnd)
	{
		isplayerAct = 0;
		myhurt *= 2;
		std::string tmpstr = "ui/crit.png";
		myactimg->loadTexture(tmpstr, cocos2d::ui::TextureResType::PLIST);
		myactimg->setContentSize(Sprite::createWithSpriteFrameName(tmpstr)->getContentSize());
		myactimg->setVisible(true);
		mycritfnt->setVisible(true);
		myactimg->setOpacity(200);
		myactimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		myactimg->runAction(Sequence::create(ac1, DelayTime::create(1.0f), Hide::create(), NULL));
		tmpstr = StringUtils::format("%d", myhurt);
		mycritfnt->setString(tmpstr);
	}
	else if (r < dodgernd)
	{
		ismyAct = 1;
		std::string imgstr = "ui/dodge.png";
		myactimg->loadTexture(imgstr, cocos2d::ui::TextureResType::PLIST);
		myactimg->setContentSize(Sprite::createWithSpriteFrameName(imgstr)->getContentSize());
		myactimg->setVisible(true);
		mycritfnt->setVisible(false);
		myactimg->setOpacity(200);
		myactimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		myactimg->runAction(Sequence::create(ac1, Shake::create(0.2f, 20, 1), DelayTime::create(0.8f), Hide::create(), NULL));

	}

	if (ismyAct != 1)
	{
		curmyHp -= myhurt;
	}

	showFightWord(1, myhurt);

	if (curmyHp < 0.0f)
		curmyHp = 0.0f;

	mylife = curmyHp;
	updateMyLife();

	if (mylife > 0.0f)
	{
		this->scheduleOnce(schedule_selector(MatchFightLayer::delayMyFight), 1.5f);

		if (checkPlayerSkill(H_WG) == S_SKILL_3)
		{
			int count = GlobalData::map_gfskills[S_SKILL_3].fightPlayerleftval;
			if (count > 0)
			{
				showPlayerSkill(S_SKILL_3);
				this->schedule(schedule_selector(MatchFightLayer::playerSkillComboAtk), 0.3f, count - 1, 0.2f);
			}
		}
	}
	else
	{
		showResultLayer(0);
	}
	
}

int MatchFightLayer::getMyHurt(std::string localid)
{
	int curheroDf =getMyDf(localid);
	int herohurt = playeratk - curheroDf;

	float fminack = 0.1f * playeratk;
	int intminack = int(fminack + 0.5f);

	if (herohurt < intminack)
		herohurt = intminack;
	return herohurt;
}

void MatchFightLayer::resetSkills()
{
	for (int i = S_SKILL_1; i <= S_SKILL_8; i++)
	{
		GlobalData::map_gfskills[i].leftval = 0;
		GlobalData::map_gfskills[i].fightPlayerleftval = 0;
	}
}

void MatchFightLayer::delayShowResultLayer(float dt)
{
	MatchFightResultLayer* layer = MatchFightResultLayer::create(nextmyhero, win);
	g_gameLayer->addChild(layer, 5);
	this->removeFromParentAndCleanup(true);
}

void MatchFightLayer::showFightWord(int type, int value)
{
	std::string wordstr;
	int size = 0;
	int r = 0;
	isUseWg = false;

	std::string myweaponstr;
	std::string mywgstr;
	std::string myarmorstr;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
	{
		std::string nid = fite->first;
		int value = GlobalData::myMatchInfo.map_EquipData[nid];

		if (nid.compare(0, 1, "a") == 0 && value % 1000 > 0)
			myweaponstr = nid;
		else if (nid.compare(0, 1, "w") == 0)
			mywgstr = nid;
		else if (nid.compare(0, 1, "e") == 0 && value % 1000 > 0)
			myarmorstr = nid;
	}

	if (type == 0)//
	{
		std::string herowordstr;

		if (myweaponstr.length() > 0)//是否有武器
		{
			int showr = GlobalData::createRandomNum(2);

			std::string wstrid = myweaponstr;
			if (mywgstr.length() > 0)
			{
				if (showr == 0)
					isUseWg = true;
			}
			if (isUseWg)
			{
				herowordstr = getMyGfFightStr(mywgstr);
			}
			else
			{
				int extype = GlobalData::getResExType(myweaponstr);
				size = sizeof(herofightdesc1[extype - 1]) / sizeof(herofightdesc1[extype - 1][0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc1[extype - 1][r];
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), GlobalData::map_allResource[wstrid].cname.c_str(), GlobalData::matchPlayerInfo.nickname.c_str());
			}

			int r = GlobalData::createRandomNum(100);
			if (r < 40)
			{
				int gv = GlobalData::myMatchInfo.map_EquipData[myweaponstr]--;
				if (gv <= 0)
				{
					std::string desc = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("你的").c_str(), GlobalData::map_allResource[wstrid].cname.c_str(), CommonFuncs::gbk2utf("已毁坏！！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
				}
				else if (gv == 20 || gv == 10)
				{
					std::string descstr = StringUtils::format("%s%s%d", GlobalData::map_allResource[wstrid].cname.c_str(), CommonFuncs::gbk2utf("耐久度仅剩").c_str(), gv);

					g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
				}
			}
		}
		else//没有武器
		{

			if (mywgstr.length() > 0)
			{
				isUseWg = true;
			}
			if (isUseWg)
			{
				herowordstr = getMyGfFightStr(mywgstr);
			}
			else
			{
				size = sizeof(herofightdesc) / sizeof(herofightdesc[0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc[r];
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), GlobalData::matchPlayerInfo.nickname.c_str());
			}
		}
		
		checkWordLblColor(herowordstr);

		if (isUseWg)
		{
			int r1 = GlobalData::createRandomNum(4);
			wordstr = bossgetherowg[r1];
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), value);
		}
		else
		{
			if (myweaponstr.length() > 0)
			{
				int extype = GlobalData::getResExType(myweaponstr);
				size = sizeof(herofightdesc2[extype - 1]) / sizeof(herofightdesc2[extype - 1][0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc2[extype - 1][r];
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), value);
			}
			else
			{
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightdesc3[0].c_str()).c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), value);
			}
		}

		std::string playerarmorstr;
		std::map<std::string, int>::iterator eite;
		for (eite = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.begin(); eite != GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.end(); ++eite)
		{
			std::string nid = eite->first;
			int value = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData[nid];

			int goodvalue = value % 1000 - 1;
			if (nid.compare(0, 1, "e") == 0 && goodvalue > 0)
			{
				playerarmorstr = nid;
			}
		}

		if (playerarmorstr.length() > 0)//player是有有防具
		{
			int r = GlobalData::createRandomNum(100);
			std::string astrid = playerarmorstr;
			if (r < 40)
			{
				int gv = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData[playerarmorstr]--;

				if (gv <= 0)
				{
					std::string desc = StringUtils::format("%s%s%s", GlobalData::matchPlayerInfo.nickname.c_str(), GlobalData::map_allResource[astrid].cname.c_str(), CommonFuncs::gbk2utf("已毁坏！！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
				}
			}

			size = sizeof(bossfight1) / sizeof(bossfight1[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = bossfight1[r];
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), GlobalData::map_allResource[astrid].cname.c_str(), value);
		}
		else//没有防具
		{
			size = sizeof(bossfight) / sizeof(bossfight[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = bossfight[r];
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), value);
		}

		if (isplayerAct == 1)//npc闪避
		{
			int dcount = sizeof(npcdodgedesc) / sizeof(npcdodgedesc[0]);
			int dodgernd = GlobalData::createRandomNum(dcount);
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(npcdodgedesc[dodgernd].c_str()).c_str(), GlobalData::matchPlayerInfo.nickname.c_str());
		}
		checkWordLblColor(herowordstr);

		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_ATTACK);
	}
	else//
	{
		std::string bosswordstr;

		std::string playerweaponstr;
		std::string wstrid;
		std::map<std::string, int>::iterator wite;
		for (wite = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.begin(); wite != GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.end(); ++wite)
		{
			std::string nid = wite->first;
			int value = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData[nid];

			int goodvalue = value % 1000 - 1;
			if (nid.compare(0, 1, "a") == 0 && goodvalue > 0)
			{
				playerweaponstr = nid;
			}
			else if (nid.compare(0, 1, "w") == 0)
			{
				wstrid = nid;
			}
		}
		if (playerweaponstr.length() > 0)//是否有武器
		{
			int showr = GlobalData::createRandomNum(2);

			if (wstrid.length() > 0)
			{
				if (showr == 0)
					isUseWg = true;
			}
			if (isUseWg)
			{
				bosswordstr = getPlayerGfFightStr(wstrid);
			}
			else
			{
				int extype = GlobalData::getResExType(playerweaponstr);
				size = sizeof(herofightdesc1[extype - 1]) / sizeof(herofightdesc1[extype - 1][0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc1[extype - 1][r];
				bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), GlobalData::map_allResource[playerweaponstr].cname.c_str(), CommonFuncs::gbk2utf("你").c_str());
			}


			int r = GlobalData::createRandomNum(100);
			if (r < 40)
			{
				int gv = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData[playerweaponstr]--;
				if (gv <= 0)
				{
					std::string desc = StringUtils::format("%s%s%s", GlobalData::matchPlayerInfo.nickname.c_str(), GlobalData::map_allResource[wstrid].cname.c_str(), CommonFuncs::gbk2utf("已毁坏！！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
				}
			}
		}
		else//没有武器
		{

			if (wstrid.length() > 0)
			{
				isUseWg = true;
			}
			if (isUseWg)
			{
				bosswordstr = getPlayerGfFightStr(wstrid);
			}
			else
			{
				size = sizeof(herofightdesc) / sizeof(herofightdesc[0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc[r];
				bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), CommonFuncs::gbk2utf("你").c_str());
			}
		}

		checkWordLblColor(bosswordstr);

		if (isUseWg)
		{
			int r1 = GlobalData::createRandomNum(4);
			wordstr = bossgetherowg[r1];
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), value);
		}
		else
		{
			if (playerweaponstr.length() > 0)
			{
				int extype = GlobalData::getResExType(playerweaponstr);
				size = sizeof(herofightdesc2[extype - 1]) / sizeof(herofightdesc2[extype - 1][0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc2[extype - 1][r];
				bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), value);
			}
			else
			{
				bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightdesc3[0].c_str()).c_str(), CommonFuncs::gbk2utf("你").c_str(), value);
			}
		}

		if (myarmorstr.length() > 0)//是有有防具
		{
			int r = GlobalData::createRandomNum(100);
			std::string astrid = myarmorstr;
			if (r < 40)
			{
				int gv = GlobalData::myMatchInfo.map_EquipData[myarmorstr]--;

				if (gv <= 0)
				{
					std::string desc = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("你的").c_str(), GlobalData::map_allResource[astrid].cname.c_str(), CommonFuncs::gbk2utf("已毁坏！！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
				}
				else if (gv == 20 || gv == 10)
				{
					std::string descstr = StringUtils::format("%s%s%d", GlobalData::map_allResource[astrid].cname.c_str(), CommonFuncs::gbk2utf("耐久度仅剩").c_str(), gv);

					g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
				}
			}

			size = sizeof(bossfight1) / sizeof(bossfight1[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = bossfight1[r];
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), CommonFuncs::gbk2utf("你").c_str(), GlobalData::map_allResource[astrid].cname.c_str(), value);
		}
		else//没有防具
		{
			size = sizeof(bossfight) / sizeof(bossfight[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = bossfight[r];
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), CommonFuncs::gbk2utf("你").c_str(), value);
		}

		if (ismyAct == 1)
		{
			int dcount = sizeof(herododgedesc) / sizeof(herododgedesc[0]);
			int dodgernd = GlobalData::createRandomNum(dcount);
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(herododgedesc[dodgernd].c_str()).c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), CommonFuncs::gbk2utf("你").c_str());
		}
		checkWordLblColor(bosswordstr);

		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_HURT);
	}
}

void MatchFightLayer::checkWordLblColor(std::string wordstr)
{
	Label* wordlbl = Label::createWithTTF(wordstr, "fonts/STXINGKA.TTF", 28);
	wordlbl->setLineBreakWithoutSpace(true);
	wordlbl->setMaxLineWidth(610);
	int index = 0;
	//getletter --是每个字，中文，英文都算一个字
	while (wordlbl->getLetter(index) != NULL)
	{
		wordlbl->getLetter(index)->setColor(Color3B::BLACK);
		index++;
	}

	std::u16string utf16word;
	StringUtils::UTF8ToUTF16(wordstr, utf16word);

	std::u16string wordtmp = utf16word;
	std::u16string utf16playername;
	StringUtils::UTF8ToUTF16(GlobalData::matchPlayerInfo.nickname, utf16playername);
	std::size_t findpos;
	//player名称颜色 红色
	while (1){
		findpos = wordtmp.find(utf16playername);
		if (findpos != std::string::npos){
			wordtmp = wordtmp.substr(findpos + utf16playername.length(), wordtmp.length() - utf16playername.length());
			int len = utf16playername.length();
			int sindex = utf16word.length() - wordtmp.length() - utf16playername.length();
			for (int i = sindex; i < sindex + len; i++)
			{
				wordlbl->getLetter(i)->setColor(Color3B(230, 35, 35));
			}
		}
		else
			break;
	}
	//角色名称颜色 绿色
	wordtmp = utf16word;
	std::u16string utf16heroname;
	StringUtils::UTF8ToUTF16(CommonFuncs::gbk2utf("你"), utf16heroname);
	while (1){
		findpos = wordtmp.find(utf16heroname);
		if (findpos != std::string::npos){
			wordtmp = wordtmp.substr(findpos + utf16heroname.length(), wordtmp.length() - utf16heroname.length());
			int len = utf16heroname.length();
			int sindex = utf16word.length() - wordtmp.length() - utf16heroname.length();
			for (int i = sindex; i < sindex + len; i++)
			{
				wordlbl->getLetter(i)->setColor(Color3B(27, 141, 0));
			}
		}
		else
			break;
	}
	wordtmp = utf16word;
	//武器，功法名称颜色 紫色
	std::map<std::string, EquipData>::iterator ite;
	for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
	{
		std::string ename = GlobalData::map_allResource[ite->first].cname;
		std::u16string utf16ename;
		StringUtils::UTF8ToUTF16(ename, utf16ename);
		std::size_t findpos;

		while (1){
			findpos = wordtmp.find(utf16ename);
			if (findpos != std::string::npos){
				wordtmp = wordtmp.substr(findpos + utf16ename.length(), wordtmp.length() - utf16ename.length());
				int len = utf16ename.length();
				int sindex = utf16word.length() - wordtmp.length() - utf16ename.length();
				for (int i = sindex; i < sindex + len; i++)
				{
					wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
				}
			}
			else
				break;
		}
	}
	wordtmp = utf16word;

	std::string mywgstr;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
	{
		std::string nid = fite->first;
		int value = GlobalData::myMatchInfo.map_EquipData[nid];
		if (nid.compare(0, 1, "w") == 0)
			mywgstr = nid;
	}

	if (mywgstr.length() > 0)
	{
		GFTrickData gftrickdata = GlobalData::map_gftricks[mywgstr];
		int mygftricksize = gftrickdata.snames.size();
		
		for (int n = 0; n < mygftricksize; n++)
		{
			std::string gfname = gftrickdata.snames[n];

			std::u16string utf16gfname;
			StringUtils::UTF8ToUTF16(gfname, utf16gfname);
			std::size_t findpos;

			while (1){
				findpos = wordtmp.find(utf16gfname);
				if (findpos != std::string::npos){
					wordtmp = wordtmp.substr(findpos + utf16gfname.length(), wordtmp.length() - utf16gfname.length());
					int len = utf16gfname.length();
					int sindex = utf16word.length() - wordtmp.length() - utf16gfname.length();
					for (int i = sindex; i < sindex + len; i++)
					{
						wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
					}
				}
				else
					break;
			}
		}
	}

	wordtmp = utf16word;

	////数字颜色 红色

	for (unsigned int i = wordtmp.length() - 8; i < wordtmp.length(); i++)
	{
		if (wordtmp[i] >= '0' && wordtmp[i] <= '9')
		{
			wordlbl->getLetter(i)->setColor(Color3B(230, 35, 35));
		}
	}

	m_fihgtScorll->addEventLabel(wordlbl);
}

std::string MatchFightLayer::getMyGfFightStr(std::string wgstr)
{
	std::string retstr;

	int r1 = GlobalData::createRandomNum(2);
	int r2 = GlobalData::createRandomNum(4);

	std::string gfstr = wgstr;
	GFTrickData ssdata = GlobalData::map_gftricks[gfstr];
	int r3 = GlobalData::createRandomNum(ssdata.snames.size());
	std::string gfsname = ssdata.snames[r3];
	std::string	wordstr = CommonFuncs::gbk2utf(herousewgfight[r1][r2].c_str());
	if (r1 == 0)
		retstr = StringUtils::format(wordstr.c_str(), CommonFuncs::gbk2utf("你").c_str(), gfsname.c_str(), GlobalData::matchPlayerInfo.nickname.c_str());
	else
		retstr = StringUtils::format(wordstr.c_str(), CommonFuncs::gbk2utf("你").c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), gfsname.c_str(), gfsname.c_str(), GlobalData::matchPlayerInfo.nickname.c_str());

	return retstr;
}

std::string MatchFightLayer::getPlayerGfFightStr(std::string wgstr)
{
	std::string retstr;

	int r1 = GlobalData::createRandomNum(2);
	int r2 = GlobalData::createRandomNum(4);

	std::string gfstr = wgstr;
	GFTrickData ssdata = GlobalData::map_gftricks[gfstr];
	int r3 = GlobalData::createRandomNum(ssdata.snames.size());
	std::string gfsname = ssdata.snames[r3];
	std::string	wordstr = CommonFuncs::gbk2utf(herousewgfight[r1][r2].c_str());
	if (r1 == 0)
		retstr = StringUtils::format(wordstr.c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), gfsname.c_str(), CommonFuncs::gbk2utf("你").c_str());
	else
		retstr = StringUtils::format(wordstr.c_str(), GlobalData::matchPlayerInfo.nickname.c_str(), CommonFuncs::gbk2utf("你").c_str(), gfsname.c_str(), gfsname.c_str(), CommonFuncs::gbk2utf("你").c_str());

	return retstr;
}

void MatchFightLayer::updateMyLife()
{
	int mymaxlife = getMyMaxLife(curmyherolocalid);
	std::string hpstr = StringUtils::format("%d/%d", mylife, mymaxlife);
	myhpvaluetext->setString(hpstr);
	float herohppercent = 100 * mylife / mymaxlife;
	myhpbar->setPercent(herohppercent);

	MyProgressTo * fromto = MyProgressTo::create(0.5f, herohppercent);
	myhpbar2->runAction(fromto);
}

void MatchFightLayer::updatePlayerLife()
{
	//NPC血量显示
	std::string hpstr = StringUtils::format("%d/%d", playerlife, playermaxhp);
	playerhpvaluetext->setString(hpstr);

	//NCP血量进度
	int playerhppercent = 100 * playerlife / playermaxhp;
	playerhpbar->setPercent(playerhppercent);
	MyProgressTo * to = MyProgressTo::create(0.5f, playerhppercent);
	playerhpbar2->runAction(to);
}


int MatchFightLayer::checkMySkill(HeroAtrType gftype)
{
	int ret = S_SNONE;

	std::string gfstr;
	int lv = 0;

	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::myMatchInfo.map_EquipData.begin(); fite != GlobalData::myMatchInfo.map_EquipData.end(); ++fite)
	{
		std::string nid = fite->first;
		lv = GlobalData::myMatchInfo.map_EquipData[nid];

		if (nid.compare(0, 1, "w") == 0 && gftype == H_WG)
		{
			gfstr = nid;
		}
		else if (nid.compare(0, 1, "x") == 0 && gftype == H_NG)
		{
			gfstr = nid;
		}
	}

	if (gfstr.length() > 0)
	{
		int stype = GlobalData::map_wgngs[gfstr].skilltype;
		if (GlobalData::map_gfskills[stype].leftval > 0)
		{
			ret =  stype;
		}
		else
		{
			if (stype > S_SNONE)
			{
				int rand = GlobalData::map_wgngs[gfstr].vec_skrnd[lv] * 100;
				int r = GlobalData::createRandomNum(10000);
				if (r < rand)
				{
					if (stype != S_SKILL_4 && stype != S_SKILL_6 && stype != S_SKILL_7 && stype != S_SKILL_8)
						GlobalData::map_gfskills[stype].leftval = GlobalData::map_wgngs[gfstr].skilleffect;
					ret = stype;
				}
			}
		}
	}

	return ret;

}

int MatchFightLayer::checkPlayerSkill(HeroAtrType gftype)
{
	int ret = S_SNONE;
	std::string gfstr = "";
	int lv = 0;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData.end(); ++fite)
	{
		std::string nid = fite->first;
		int value = GlobalData::vec_matchPlayerData[nextplayerhero - 1].map_playerData[nid];

		if (nid.compare(0, 1, "w") == 0 && gftype == H_WG)
		{
			gfstr = nid;
			lv = value - 1;
		}
		else if (nid.compare(0, 1, "x") == 0 && gftype == H_NG)
		{
			gfstr = nid;
			lv = value - 1;
		}
	}

	if (gfstr.length() > 0)
	{
		int stype = GlobalData::map_wgngs[gfstr].skilltype;
		if (GlobalData::map_gfskills[stype].fightPlayerleftval > 0)
		{
			ret = stype;
		}
		else
		{
			if (stype > S_SNONE)
			{
				int rand = GlobalData::map_wgngs[gfstr].vec_skrnd[lv] * 100;
				int r = GlobalData::createRandomNum(10000);
				if (r < rand)
				{
					if (stype != S_SKILL_4 && stype != S_SKILL_6 && stype != S_SKILL_7 && stype != S_SKILL_8)
						GlobalData::map_gfskills[stype].fightPlayerleftval = GlobalData::map_wgngs[gfstr].skilleffect;
					ret = stype;
				}
			}
		}
	}

	return ret;
}

void MatchFightLayer::showMySkill(int skilltype)
{
	if (skilltype >= S_SKILL_1 && skilltype <= S_SKILL_5)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showPlayerTextAmin(str);
	}
	else if (skilltype >= S_SKILL_6 && skilltype <= S_SKILL_8)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showMyTextAmin(str);
	}
}

void MatchFightLayer::showPlayerSkill(int skilltype)
{
	if (skilltype >= S_SKILL_1 && skilltype <= S_SKILL_5)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showMyTextAmin(str);
	}
	else if (skilltype >= S_SKILL_6 && skilltype <= S_SKILL_8)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showPlayerTextAmin(str);
	}
}

void MatchFightLayer::showMyTextAmin(std::string filename)
{
	myactimg->loadTexture(filename, cocos2d::ui::TextureResType::PLIST);
	myactimg->setContentSize(Sprite::createWithSpriteFrameName(filename)->getContentSize());
	myactimg->setVisible(true);
	mycritfnt->setVisible(false);
	myactimg->setOpacity(200);
	myactimg->setScale(3);
	ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
	myactimg->runAction(Sequence::create(ac1, Shake::create(0.2f, 20, 1), DelayTime::create(0.6f), Hide::create(), NULL));
	mycritfnt->setVisible(false);
}


void MatchFightLayer::showPlayerTextAmin(std::string filename)
{
	playeractimg->loadTexture(filename, cocos2d::ui::TextureResType::PLIST);
	playeractimg->setContentSize(Sprite::createWithSpriteFrameName(filename)->getContentSize());
	playeractimg->setVisible(true);
	playeractimg->setOpacity(200);
	playeractimg->setScale(3);
	ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
	playeractimg->runAction(Sequence::create(ac1, DelayTime::create(0.8f), Hide::create(), NULL));
	playercritfnt->setVisible(false);
}

int MatchFightLayer::getPlayerMaxLife(int hindex)
{
	int heroindex = hindex;
	float friendhppercent = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly.begin(); it != GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly[nid].relation == F_FRIEND)
		{
			friendhppercent += GlobalData::map_NPCFriendData[nid].hppercent / 100;
		}
		else if (GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly[nid].relation == F_MASTER)
		{
			friendhppercent += GlobalData::map_NPCMasterData[nid].hppercent / 100;
		}
	}

	float mixfglifepercent = 0.0f;

	std::string mixgf = GlobalData::vec_matchPlayerData[heroindex].mixgf;
	if (mixgf.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mixgf];

		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::vec_matchPlayerData[heroindex].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[heroindex].map_playerData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::vec_matchPlayerData[heroindex].map_playerData[nid];

			if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
				mixfglifepercent += mdata.hppercent / 100;
		}

	}

	float flife = GlobalData::map_heroAtr[nextplayerhero].vec_maxhp[GlobalData::vec_matchPlayerData[heroindex].herolv] * 1.0f;
	flife = flife + flife*friendhppercent + flife * mixfglifepercent;

	if (GlobalData::vec_matchPlayerData[heroindex].factionlv == 3)
		flife += flife*0.02f;
	return flife;
}

int MatchFightLayer::getPlayerDf(int hindex)
{
	int adf = 0;
	int ngdf = 0;
	float slvdf = 0.0f;//强化等级加防

	int heroindex = hindex;
	float frienddfpercent = 0.0f;
	std::string masternpc;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly.begin(); it != GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly[nid].relation == F_FRIEND)
		{
			frienddfpercent += GlobalData::map_NPCFriendData[nid].dfpercent / 100;
		}
		else if (GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly[nid].relation == F_MASTER)
		{
			frienddfpercent += GlobalData::map_NPCMasterData[nid].dfpercent / 100;
			masternpc = nid;
		}
	}

	std::string ngstr;
	int ngvalue = 0;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::vec_matchPlayerData[heroindex].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[heroindex].map_playerData.end(); ++fite)
	{
		std::string nid = fite->first;
		ngvalue = GlobalData::vec_matchPlayerData[heroindex].map_playerData[nid] - 1;

		if (nid.compare(0, 1, "w") == 0)
			ngstr = nid;
	}
	if (ngstr.length() > 0)
	{
		int gflv = ngvalue;

		if (gflv > GlobalData::map_wgngs[ngstr].maxlv - 1)
			gflv = GlobalData::map_wgngs[ngstr].maxlv - 1;
		ngdf = GlobalData::map_wgngs[ngstr].vec_bns[gflv];

		int masterdf = 0;
		if (masternpc.length() > 0)
		{
			for (unsigned int i = 0; i < GlobalData::map_NPCMasterData[masternpc].vec_gfid.size(); i++)
			{
				if (ngstr.compare(GlobalData::map_NPCMasterData[masternpc].vec_gfid[i]) == 0)
				{
					int msbdf = ngdf * GlobalData::map_NPCMasterData[masternpc].vec_gfbonus[i] / 100;
					masterdf += msbdf;

					break;
				}
			}
		}

		int mixdf = 0;
		std::string mixgf = GlobalData::vec_matchPlayerData[heroindex].mixgf;
		if (mixgf.length() > 0)
		{
			MixGfData mdata = GlobalData::map_MixGfData[mixgf];

			std::map<std::string, int>::iterator fite;
			for (fite = GlobalData::vec_matchPlayerData[heroindex].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[heroindex].map_playerData.end(); ++fite)
			{
				std::string nid = fite->first;
				int value = GlobalData::vec_matchPlayerData[heroindex].map_playerData[nid];

				if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
					mixdf += ngdf * mdata.dfpercent / 100;
			}
		}
		ngdf = ngdf + masterdf + mixdf;
	}

	std::map<std::string, int>::iterator eite;
	for (eite = GlobalData::vec_matchPlayerData[heroindex].map_playerData.begin(); eite != GlobalData::vec_matchPlayerData[heroindex].map_playerData.end(); ++eite)
	{
		std::string nid = eite->first;
		int value = GlobalData::vec_matchPlayerData[heroindex].map_playerData[nid];

		int goodvalue = value % 1000 - 1;
		if (nid.compare(0, 1, "a") == 0 && goodvalue > 0)
		{
			std::string aname = nid;
			adf = GlobalData::map_equips[aname].df;
			int slv = value/1000;
			slvdf = slv * (slv + adf / 10);
		}
	}
	//防御属性
	float fdf = GlobalData::map_heroAtr[nextplayerhero].vec_df[GlobalData::vec_matchPlayerData[heroindex].herolv] + ngdf + adf;
	fdf += slvdf;

	fdf += fdf*frienddfpercent;

	if (GlobalData::vec_matchPlayerData[heroindex].factionlv == 4)
		fdf += fdf*0.02f;

	adf = int(fdf + 0.5f);
	return adf;
}

int MatchFightLayer::getPlayerAtk(int hindex)
{
	int weaponAtk = 0;
	int wgAtk = 0;
	float slvAtk = 0.0f;//强化等级加攻
	int heroindex = hindex;
	std::map<std::string, int>::iterator eite;
	for (eite = GlobalData::vec_matchPlayerData[heroindex].map_playerData.begin(); eite != GlobalData::vec_matchPlayerData[heroindex].map_playerData.end(); ++eite)
	{
		std::string nid = eite->first;
		int value = GlobalData::vec_matchPlayerData[heroindex].map_playerData[nid];
		int goodvalue = value % 1000 - 1;
		if (nid.compare(0, 1, "e") == 0 && goodvalue > 0)
		{
			std::string aname = nid;
			weaponAtk = GlobalData::map_equips[aname].atk;
			int slv = value / 1000;
			slvAtk = slv * (slv + weaponAtk / 10);
		}
	}

	float friendatkpercent = 0.0f;
	std::string masternpc;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly.begin(); it != GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly[nid].relation == F_FRIEND)
		{
			friendatkpercent += GlobalData::map_NPCFriendData[nid].atkpercent / 100;
		}
		else if (GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly[nid].relation == F_MASTER)
		{
			friendatkpercent += GlobalData::map_NPCMasterData[nid].atkpercent / 100;
			masternpc = nid;
		}
	}

	std::string wgstr;
	int wgvalue = 0;
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::vec_matchPlayerData[heroindex].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[heroindex].map_playerData.end(); ++fite)
	{
		std::string nid = fite->first;
		wgvalue = GlobalData::vec_matchPlayerData[heroindex].map_playerData[nid] - 1;

		if (nid.compare(0, 1, "w") == 0)
			wgstr = nid;
	}
	if (wgstr.length() > 0)
	{
		int gflv = wgvalue;
		if (gflv > GlobalData::map_wgngs[wgstr].maxlv - 1)
			gflv = GlobalData::map_wgngs[wgstr].maxlv - 1;
		wgAtk = GlobalData::map_wgngs[wgstr].vec_bns[gflv];

		int masteratk = 0;
		if (masternpc.length() > 0)
		{
			for (unsigned int i = 0; i < GlobalData::map_NPCMasterData[masternpc].vec_gfid.size(); i++)
			{
				if (wgstr.compare(GlobalData::map_NPCMasterData[masternpc].vec_gfid[i]) == 0)
				{
					int msbatk = wgAtk * GlobalData::map_NPCMasterData[masternpc].vec_gfbonus[i] / 100;
					masteratk += msbatk;
					break;
				}
			}
		}

		int mixatk = 0;
		std::string mixgf = GlobalData::vec_matchPlayerData[heroindex].mixgf;
		if (mixgf.length() > 0)
		{
			MixGfData mdata = GlobalData::map_MixGfData[mixgf];

			std::map<std::string, int>::iterator fite;
			for (fite = GlobalData::vec_matchPlayerData[heroindex].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[heroindex].map_playerData.end(); ++fite)
			{
				std::string nid = fite->first;
				int value = GlobalData::vec_matchPlayerData[heroindex].map_playerData[nid];

				if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
					mixatk += wgAtk * mdata.atkpercent / 100;
			}
		}
		wgAtk = wgAtk + masteratk + mixatk;
	}

	//攻击属性
	float fack = GlobalData::map_heroAtr[nextplayerhero].vec_atk[GlobalData::vec_matchPlayerData[heroindex].herolv] + weaponAtk + wgAtk;

	if (wgstr.length() > 0)
	{
		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::vec_matchPlayerData[heroindex].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[heroindex].map_playerData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::vec_matchPlayerData[heroindex].map_playerData[nid] - 1;

			if (nid.compare(0, 1, "e") == 0 && value > 0)
			{
				float back = fack * 0.05f;
				fack += back;
			}
		}
	}
	fack += slvAtk;

	fack += fack* friendatkpercent;

	if (GlobalData::vec_matchPlayerData[heroindex].factionlv == 5)
		fack += fack*0.02f;

	int tatk = int(fack + 0.5f);
	return tatk;
}

int MatchFightLayer::getPlayerDodde(int hindex)
{
	int heroindex = hindex;
	int dodgernd = GlobalData::map_heroAtr[nextplayerhero].vec_dodge[GlobalData::vec_matchPlayerData[heroindex].herolv];

	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::vec_matchPlayerData[heroindex].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[heroindex].map_playerData.end(); ++fite)
	{
		std::string nid = fite->first;
		int ngvalue = GlobalData::vec_matchPlayerData[heroindex].map_playerData[nid] - 1;

		if (nid.compare(0, 1, "x") == 0)
			dodgernd += GlobalData::map_wgngs[nid].vec_dodge[ngvalue];
	}



	float frienddogdernd = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly.begin(); it != GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly[nid].relation == F_FRIEND)
		{
			frienddogdernd += GlobalData::map_NPCFriendData[nid].dodgepercent;
		}
		else if (GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly[nid].relation == F_MASTER)
		{
			frienddogdernd += GlobalData::map_NPCMasterData[nid].dodgepercent;
		}
	}
	dodgernd += frienddogdernd;

float mixdodge = 0.0f;
std::string mymixgf = GlobalData::vec_matchPlayerData[heroindex].mixgf;
if (mymixgf.length() > 0)
{
	MixGfData mdata = GlobalData::map_MixGfData[mymixgf];

	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::vec_matchPlayerData[heroindex].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[heroindex].map_playerData.end(); ++fite)
	{
		std::string nid = fite->first;
		int value = GlobalData::vec_matchPlayerData[heroindex].map_playerData[nid];

		if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
			mixdodge += mdata.dodgepercent;
	}
}
dodgernd += mixdodge;
if (GlobalData::vec_matchPlayerData[heroindex].factionlv == 1)
dodgernd += 2;

return dodgernd;
}

int MatchFightLayer::getPlayerCrit(int hindex)
{
	int heroindex = hindex;
	int critrnd = GlobalData::map_heroAtr[nextplayerhero].vec_crit[GlobalData::vec_matchPlayerData[heroindex].herolv];
	std::map<std::string, int>::iterator fite;
	for (fite = GlobalData::vec_matchPlayerData[heroindex].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[heroindex].map_playerData.end(); ++fite)
	{
		std::string nid = fite->first;
		int wgvalue = GlobalData::vec_matchPlayerData[heroindex].map_playerData[nid] - 1;

		if (nid.compare(0, 1, "w") == 0)
			critrnd += GlobalData::map_wgngs[nid].vec_cirt[wgvalue];
	}


	float friendcritrnd = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly.begin(); it != GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly[nid].relation == F_FRIEND)
		{
			friendcritrnd += GlobalData::map_NPCFriendData[nid].critpercent;
		}
		else if (GlobalData::vec_matchPlayerData[heroindex].map_playerfriendly[nid].relation == F_MASTER)
		{
			friendcritrnd += GlobalData::map_NPCMasterData[nid].critpercent;
		}
	}
	critrnd += friendcritrnd;

	float mixcrit = 0.0f;
	std::string mixgf = GlobalData::vec_matchPlayerData[heroindex].mixgf;
	if (mixgf.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mixgf];
		std::map<std::string, int>::iterator fite;
		for (fite = GlobalData::vec_matchPlayerData[heroindex].map_playerData.begin(); fite != GlobalData::vec_matchPlayerData[heroindex].map_playerData.end(); ++fite)
		{
			std::string nid = fite->first;
			int value = GlobalData::vec_matchPlayerData[heroindex].map_playerData[nid];

			if ((nid.compare(0, 1, "w") == 0 && nid.compare(mdata.mastergf) == 0) || (nid.compare(0, 1, "x") == 0 && nid.compare(mdata.mastergf) == 0))
				mixcrit += mdata.critpercent;
		}
	}
	critrnd += mixcrit;
	if (GlobalData::vec_matchPlayerData[heroindex].factionlv == 2)
		critrnd += 2;
	return critrnd;
}

void MatchFightLayer::showResultLayer(int result)
{
	win = result;
	if (result == 1)
	{
		int playerherocount = GlobalData::vec_matchPlayerData.size();
		if (nextplayerhero >= playerherocount)
		{
			this->scheduleOnce(schedule_selector(MatchFightLayer::delayShowResultLayer), 1.0f);
		}
		else
		{
			nextplayerhero++;
			updatePlayerInfo();
		}
	}
	else
	{
		int myherocount = 0;
		std::vector<std::string> vec_ids = GlobalData::getSaveListId();
		std::vector<std::string> vec_userid;
		int starti = nextmyhero;
		for (unsigned int i = starti; i < vec_ids.size(); i++)
		{
			nextmyhero++;
			if (vec_ids[nextmyhero - 1].length() <= 0)
			{
				continue;
			}
			else
			{
				updateMyInfo();
				return;
			}
		}
		this->scheduleOnce(schedule_selector(MatchFightLayer::delayShowResultLayer), 1.0f);
	}
}