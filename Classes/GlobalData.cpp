#include "GlobalData.h"
#include "Building.h"
#include "StorageRoom.h"
#include "MyPackage.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"
#include "json.h"
#include "Const.h"
#include "MD5.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iosfunc.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

std::map<std::string, std::vector<BuildActionData>> GlobalData::map_buidACData;

std::map<std::string, AllResource> GlobalData::map_allResource;

std::vector<ResData> GlobalData::vec_resData;

std::vector<std::string> GlobalData::vec_hillResid;

std::map<std::string, MapData> GlobalData::map_maps;

std::map<std::string, NpcData> GlobalData::map_npcs;

std::map<int, HeroAtrData> GlobalData::map_heroAtr;

std::map<std::string, WG_NGData> GlobalData::map_wgngs;

std::map<std::string, EquipData> GlobalData::map_equips;

std::vector<PlotMissionData> GlobalData::vec_PlotMissionData;

std::map<std::string, std::vector<PlotMissionData>> GlobalData::map_BranchPlotMissionData;
std::map<std::string, BranchPlotMissionItem> GlobalData::map_BranchPlotMissionItem;

std::map<std::string, GFTrickData> GlobalData::map_gftricks;

std::map<int, GFSkillData> GlobalData::map_gfskills;

std::vector<GoodsData> GlobalData::vec_goods;

std::vector<std::string> GlobalData::vec_buyVipIds;

std::map<std::string, int> GlobalData::map_buyVipDays;


std::map<std::string, ChallengeRewardData> GlobalData::map_challengeReward;
std::map<std::string, FriendlyData> GlobalData::map_myfriendly;

std::map<std::string, NPCFriendData> GlobalData::map_NPCFriendData;

std::map<std::string, NPCMasterData> GlobalData::map_NPCMasterData;

std::map<std::string, MixGfData> GlobalData::map_MixGfData;

std::vector<RankData> GlobalData::vec_rankData;

std::vector<FactionListData> GlobalData::vec_factionListData;

std::vector<FactionMemberData> GlobalData::vec_factionMemberData;

int GlobalData::factionExp = 0;

std::vector<AchiveData> GlobalData::vec_achiveData;
std::vector<std::string> GlobalData::vec_showAchiveNames;

bool GlobalData::unlockhero[4] = {true, false, false, false};

std::string GlobalData::uid = "";

std::vector<std::string> GlobalData::vec_saveids;

bool GlobalData::hasBuy = false;
int GlobalData::adsinterval = 10;
long GlobalData::adschoose = 11111111;

bool GlobalData::isPopingScene = false;

int GlobalData::myGlodCount = 0;

GameStatus GlobalData::g_gameStatus = GAMEOVER;
int GlobalData::wxbmapos = 0;
int GlobalData::dgqbmapos = 0;

int GlobalData::timegiftlefttime = 0;

bool GlobalData::isBuyTimeGift = false;

int GlobalData::freeReviveCount = 0;

std::map<std::string, std::vector<std::string>> GlobalData::map_tempGf_Equip;

std::string GlobalData::MD5MyGoldCount;
std::string GlobalData::MD5CostGlodCount;
std::string GlobalData::MD5FreeReviveCount;
std::string GlobalData::MD5HeroLv;
std::string GlobalData::MD5LiveDays;
bool GlobalData::dataIsModified = false;
bool GlobalData::ispunishment = false;
bool GlobalData::isFightMaster = false;
int GlobalData::servertime = 0;

int GlobalData::myrank = 0;
std::string GlobalData::noticecontent;

int GlobalData::myFaction = 0;
int GlobalData::mytitle = 0;

int GlobalData::myFactionlv = 0;

MyLotteryData GlobalData::myLotteryData;

RaffleData GlobalData::myRaffleData;

int GlobalData::continueLoginDays = 0;

bool GlobalData::isFrozen = false;

int GlobalData::myFihgtCount = 0;
int GlobalData::myTotalFihgtCount = 0;
int GlobalData::myFihgtexp = 0;

std::map<std::string, int> GlobalData::map_fighterPlayerData;

std::map<std::string, FriendlyData> GlobalData::map_fightPlayerfriendly;

std::string GlobalData::fightPlayerMixgf;

int GlobalData::fightPlayerFactionLv = 0;
bool GlobalData::isPlayerChallenging = false;

bool GlobalData::isExchangeGift = false;

int GlobalData::myLastHuafeiRank = 0;

std::string GlobalData::couponinfo;

MyMatchInfoData GlobalData::myMatchInfo;

MatchPlayerInfo GlobalData::matchPlayerInfo;
std::vector<MatchPlayerData> GlobalData::vec_matchPlayerData;

std::vector<HSLJRankData> GlobalData::vec_hsljRankData;

std::string GlobalData::updateDownLoadURL;

bool GlobalData::isPopUpdate = false;

std::string GlobalData::curBranchMissionID;

std::vector<std::string> GlobalData::vec_qq;

bool GlobalData::isOnline = false;

GlobalData::GlobalData()
{

}


GlobalData::~GlobalData()
{

}

void GlobalData::init()
{
	myGlodCount = GameDataSave::getInstance()->getGoldCount();
	MD5MyGoldCount = md5(myGlodCount);
	isBuyTimeGift = GameDataSave::getInstance()->getIsBuyTimeGift();
	myFaction = 0;
	mytitle = 0;
	myFactionlv = 0;
}

void GlobalData::loadBuildActionJSon()
{
	map_buidACData.clear();

	rapidjson::Document doc = ReadJsonFile("data/buildings.json");
	rapidjson::Value& allBuilds = doc["b"];
	for (unsigned int i = 0; i < allBuilds.Size(); i++)
	{
		rapidjson::Value& oneBuild = allBuilds[i];
		rapidjson::Value& oneitem = oneBuild["name"];
		std::string buildname = oneitem.GetString();

		std::string jsonfilename = StringUtils::format("data/%s.json", buildname.c_str());
		rapidjson::Document doc = ReadJsonFile(jsonfilename);
		rapidjson::Value& bc = doc["bc"];
		for (unsigned int i = 0; i < bc.Size(); i++)//建筑物数组
		{
			BuildActionData data;
			rapidjson::Value& jsonvalue = bc[i];
			if (jsonvalue.IsObject())
			{
				rapidjson::Value& value = jsonvalue["id"];
				strcpy(data.icon, value.GetString());

				value = jsonvalue["blv"];
				data.blv = atoi(value.GetString());

				value = jsonvalue["time"];
				data.actime = atoi(value.GetString());

				if (jsonvalue.HasMember("extime"))
				{
					value = jsonvalue["extime"];
					data.extime = atoi(value.GetString());
				}
				else
					data.extime = 0;

				if (jsonvalue.HasMember("type"))
				{
					value = jsonvalue["type"];
					data.type = atoi(value.GetString());
				}
				else
					data.type = -1;

				value = jsonvalue["actext"];
				strcpy(data.actext, value.GetString());

				if (jsonvalue.HasMember("ep"))
				{
					value = jsonvalue["ep"];

					for (unsigned int m = 0; m < value.Size(); m++)
					{
						data.ep.push_back(value[m].GetInt());
					}
				}

				if (jsonvalue.HasMember("extype"))
				{
					value = jsonvalue["extype"];
					data.extype = atoi(value.GetString());
				}
				else
					data.extype = 0;

				value = jsonvalue["res"];
				for (unsigned int i = 0; i < value.Size(); i++)
				{
					int tmp = value[i].GetInt();
					if (tmp > 0)
						data.res.push_back(value[i].GetInt());
				}
				if (jsonvalue.HasMember("name"))
				{
					value = jsonvalue["name"];
					data.cname = value.GetString();
				}
				else
				{
					data.cname = "";
				}
				if (jsonvalue.HasMember("desc"))
				{
					value = jsonvalue["desc"];
					data.desc = value.GetString();
				}
				else
				{
					data.desc = "";
				}

				map_buidACData[buildname].push_back(data);
			}
		}
	}
}

void GlobalData::loadResJsonData()
{
	vec_resData.clear();
	rapidjson::Document doc = ReadJsonFile("data/res.json");
	rapidjson::Value& values = doc["r"];
	for (unsigned int i = 0; i < values.Size(); i++)//一级资源数组
	{
		ResData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.strid = v.GetString();
		v = item["max"];
		data.max = atoi(v.GetString());

		data.count = data.max;
		data.pastmin = 0.0f;
		data.waittime = 0.0f;
		v = item["perhour"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			data.speed.push_back(v[m].GetDouble() * 60);
		}

		v = item["type"];
		data.type = atoi(v.GetString());
		v = item["actype"];
		data.actype = atoi(v.GetString());
		v = item["ep"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			data.ep.push_back(v[m].GetInt());
		}
		v = item["res"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			data.res.push_back(v[m].GetInt());
		}

		v = item["unit"];
		data.unitname = v.GetString();
		vec_resData.push_back(data);

	}
}

void GlobalData::loadAllResourceJsonData()
{
	map_allResource.clear();
	rapidjson::Document doc = ReadJsonFile("data/allresouces.json");
	rapidjson::Value& values = doc["rd"];
	for (unsigned int i = 0; i < values.Size(); i++)//一级资源数组
	{
		AllResource data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.strid = v.GetString();

		v = item["cname"];
		data.cname = v.GetString();
		v = item["desc"];
		data.desc = v.GetString();

		v = item["val"];
		data.val = atoi(v.GetString());

		v = item["fval"];
		data.fval = atoi(v.GetString());
		
		v = item["npcs"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& npsv = v[m];
			std::string npcid = npsv.GetString();
			if (npcid.length() > 1)
				data.npc.push_back(npcid);
		}

		v = item["npcval"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& npsv = v[m];
			std::string npcval = npsv.GetString();
			if (npcval.length() > 0)
				data.npcval.push_back(atof(npcval.c_str()));
		}

		map_allResource[data.strid]= data;

	}
}

void GlobalData::loadHillResJsonData()
{
	vec_hillResid.clear();
	rapidjson::Document doc = ReadJsonFile("data/homehill.json");
	rapidjson::Value& values = doc["sh"];
	for (unsigned int i = 0; i < values.Size(); i++)//后山显示的资源列表
	{
		vec_hillResid.push_back(values[i].GetString());
	}
}

void GlobalData::loadMapJsonData()
{
	map_maps.clear();
	rapidjson::Document doc = ReadJsonFile("data/map.json");
	rapidjson::Value& values = doc["m"];
	for (unsigned int i = 0; i < values.Size(); i++)//地图地点数组
	{
		MapData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		strcpy(data.strid, v.GetString());
		v = item["npc"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& npsv = v[m];
			std::string npcid = npsv.GetString();
			if (npcid.compare("0") != 0)
				data.npcs.push_back(npcid);
		}
		v = item["cname"];
		strcpy(data.cname, v.GetString());

		v = item["t"];
		strcpy(data.tpngname, v.GetString());

		v = item["desc"];
		data.desc = v.GetString();

		v = item["qy"];
		int iscliff = atoi(v.GetString());
		data.isCliff = iscliff == 1 ? true : false;

		map_maps[data.strid] = data;

	}
}

void GlobalData::loadNpcJsonData()
{
	map_npcs.clear();
	rapidjson::Document doc = ReadJsonFile("data/npc.json");
	rapidjson::Value& values = doc["n"];
	for (unsigned int i = 0; i < values.Size(); i++)//npc数组
	{
		NpcData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		strcpy(data.id, v.GetString());
		v = item["name"];
		strcpy(data.name, v.GetString());
		v = item["type"];
		data.type = atoi(v.GetString());
		v = item["life"];
		data.life = atoi(v.GetString());
		v = item["atk"];
		data.atk = atoi(v.GetString());
		v = item["df"];
		data.df = atoi(v.GetString());
		v = item["exp"];
		data.exp = atoi(v.GetString());

		v = item["lv"];
		data.lv = atoi(v.GetString());

		v = item["es"];
		data.escapernd = atoi(v.GetString());
		

		v = item["winres"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			if (str.length() > 1)
				data.winres.push_back(str);
		}
		v = item["winresrnd"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			int rnd = atoi(str.c_str());
			if (rnd > 0)
			{
				data.winresrnd.push_back(rnd);
				data.winrescount.push_back(-1);
			}
		}
		v = item["exchg"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			if (str.length() > 1)
				data.exchgres.push_back(str);
		}
		v = item["word"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			if (str.length() > 0)
				data.words.push_back(str);
		}
		v = item["avoid"];
		data.dodge = atof(v.GetString());

		v = item["crit"];
		data.crit = atof(v.GetString());

		map_npcs[data.id] = data;
	}
}

void GlobalData::saveResData()
{
	//保存资源数据每项分号（;）分割
	std::string str;
	for (unsigned int i = 0; i < vec_resData.size(); i++)
	{
		std::string onestr = StringUtils::format("%d-%.1f-%.1f;", vec_resData[i].count, vec_resData[i].pastmin, vec_resData[i].waittime);
		str.append(onestr);
	}
	GameDataSave::getInstance()->setResData(str.substr(0, str.length() - 1));
}

void GlobalData::loadResData()
{
	//解析保存的资源数据
	std::string datastr = GameDataSave::getInstance()->getResData();
	if (datastr.length() > 0)
	{
		std::vector<std::string> vec_retstr;
		CommonFuncs::split(datastr, vec_retstr, ";");
		for (unsigned int i = 0; i < vec_retstr.size(); i++)
		{
			std::vector<std::string> tmp;
			CommonFuncs::split(vec_retstr[i], tmp, "-");
			vec_resData[i].count = atoi(tmp[0].c_str());
			vec_resData[i].pastmin = atof(tmp[1].c_str());
			vec_resData[i].waittime = atof(tmp[2].c_str());
		}
	}
	else
	{
		for (unsigned int i = 0; i < vec_resData.size(); i++)
		{
			vec_resData[i].count = vec_resData[i].max;
			vec_resData[i].pastmin = 0.0f;
			vec_resData[i].waittime = 0.0f;
		}
	}
}

void GlobalData::loadHeroAtrJsonData()
{
	map_heroAtr.clear();
	rapidjson::Document doc = ReadJsonFile("data/heroatr.json");
	rapidjson::Value& values = doc["h"];
	for (unsigned int i = 0; i < values.Size(); i++)//角色数组4个角色
	{
		rapidjson::Value& vitem = values[i];
		HeroAtrData data;
		rapidjson::Value& v = vitem["id"];
		data.id = atoi(v.GetString());

		v = vitem["name"];
		strcpy(data.name, v.GetString());

		v = vitem["atk"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_atk.push_back(v[j].GetInt());
		}

		v = vitem["df"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_df.push_back(v[j].GetInt());
		}

		v = vitem["exp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_exp.push_back(v[j].GetInt());
		}

		v = vitem["maxhp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_maxhp.push_back(v[j].GetInt());
		}
		v = vitem["crit"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_crit.push_back(v[j].GetDouble());
		}
		v = vitem["avoid"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_dodge.push_back(v[j].GetDouble());
		}
		map_heroAtr[data.id] = data;
	}
}

void GlobalData::loadWG_NGJsonData()
{
	map_wgngs.clear();
	rapidjson::Document doc = ReadJsonFile("data/wg.json");
	rapidjson::Value& values = doc["w"];
	for (unsigned int i = 0; i < values.Size(); i++)//外功数组
	{
		rapidjson::Value& vitem = values[i];
		WG_NGData data;
		rapidjson::Value& v = vitem["id"];
		strcpy(data.id, v.GetString());

		v = vitem["maxlv"];
		data.maxlv = atoi(v.GetString());

		v = vitem["bns"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_bns.push_back(v[j].GetInt());
		}

		v = vitem["exp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_exp.push_back(v[j].GetInt());
		}

		v = vitem["crit"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_cirt.push_back(v[j].GetDouble());
		}

		v = vitem["skrnd"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_skrnd.push_back(v[j].GetDouble());
		}

		v = vitem["sk"];
		data.skilltype = atoi(v.GetString());

		v = vitem["skef"];
		data.skilleffect = atoi(v.GetString());

		data.type = W_GONG + 1;
		v = vitem["qu"];
		data.qu = atoi(v.GetString());

		v = vitem["type"];
		data.extype = atoi(v.GetString());

		map_wgngs[data.id] = data;
	}

	doc = ReadJsonFile("data/ng.json");
	rapidjson::Value& nvalues = doc["n"];
	int a = nvalues.Size();
	for (unsigned int i = 0; i < nvalues.Size(); i++)//内功数组
	{
		rapidjson::Value& vitem = nvalues[i];
		WG_NGData data;
		rapidjson::Value& v = vitem["id"];
		strcpy(data.id, v.GetString());

		v = vitem["maxlv"];
		data.maxlv = atoi(v.GetString());

		v = vitem["bns"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_bns.push_back(v[j].GetInt());
		}

		v = vitem["exp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_exp.push_back(v[j].GetInt());
		}
		v = vitem["avoid"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_dodge.push_back(v[j].GetDouble());
		}

		v = vitem["skrnd"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_skrnd.push_back(v[j].GetDouble());
		}

		v = vitem["sk"];
		data.skilltype = atoi(v.GetString());

		v = vitem["skef"];
		data.skilleffect = atoi(v.GetString());

		data.type = N_GONG + 1;
		data.extype = 0;
		v = vitem["qu"];
		data.qu = atoi(v.GetString());

		map_wgngs[data.id] = data;
	}
}

void GlobalData::loadEquipJsonData()
{
	map_equips.clear();
	rapidjson::Document doc = ReadJsonFile("data/equip.json");
	rapidjson::Value& values = doc["ae"];
	for (unsigned int i = 0; i < values.Size(); i++)//武器防具数组
	{
		EquipData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.id = v.GetString();
		v = item["type"];
		data.type = atoi(v.GetString());
		v = item["atk"];
		data.atk = atoi(v.GetString());
		v = item["df"];
		data.df = atoi(v.GetString());
		v = item["extype"];
		data.extype = atoi(v.GetString());
		v = item["qu"];
		data.qu = atoi(v.GetString());

		v = item["res"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			int val = v[m].GetInt();
			if (val > 0)
			{
				std::string str = StringUtils::format("%d", val);
				data.repair_res.push_back(str);
			}
		}
		map_equips[data.id] = data;
	}
}

void GlobalData::loadUnlockHeroData()
{
	//解析角色是否解锁"-"分割
	std::string str = GameDataSave::getInstance()->getHeroUnlockData();
	std::vector<std::string> tmp;
	CommonFuncs::split(str, tmp, "-");

	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		unlockhero[i] = atoi(tmp[i].c_str()) == 1 ? true : false;	
	}
	unlockhero[0] = true;
}

void GlobalData::setUnlockHero(int index, bool val)
{
	//保存角色是否解锁"-"分割
	unlockhero[index] = val;
	std::string str;
	for (int i = 0; i < 4; i++)
	{
		std::string tmp = StringUtils::format("%d-", unlockhero[i]?1:0);
		str.append(tmp);
	}
	GameDataSave::getInstance()->setHeroUnlockData(str.substr(0, str.length() - 1));
}

bool GlobalData::getUnlocHero(int index)
{
	return unlockhero[index];
}

int GlobalData::getSysSecTime()
{
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	return timep;
}

int GlobalData::getDayOfYear()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	tm = localtime(&timep);
	int day = tm->tm_yday;
	return day;
}

int GlobalData::getMonth_Days()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	tm = localtime(&timep);
	int month = tm->tm_mon;
	int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int day = days[month];
	int year = tm->tm_year;
	if ((year % 4 == 0 && year % 100 != 0 || year % 400 == 0) && month == 2)
		day += 1;
	return day;
}

int GlobalData::getTomorrowZeroTimeDif()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	tm = localtime(&timep);
	int hour = tm->tm_hour;
	int min = tm->tm_min;
	int sec = tm->tm_sec;

	int dif = (23 - hour) * 3600 + (59 - min) * 60 + (60 - sec);
	return dif;
}

std::string GlobalData::getUId()
{
	uid = GameDataSave::getInstance()->getUserId();
	return uid;
}
void GlobalData::setUId(std::string struid)
{
	uid = struid;
	GameDataSave::getInstance()->saveUserId(struid);
}

std::string GlobalData::getDefaultStorage(int heroindex)
{
	//默认仓库的数据
	
	std::string defaultdata = GameDataSave::getInstance()->getModifyDefaultStorage(heroindex);
	if (defaultdata.length() > 0)
		return defaultdata;
	else
	{
		return getOriginLocalStorage(heroindex);
	}
	return "";
}

std::string GlobalData::getOriginLocalStorage(int heroindex)
{
	rapidjson::Document doc = ReadJsonFile("data/defaultstorage.json");
	rapidjson::Value& values = doc["ds"];

	int size = values.Size();
	if (size > 0 && heroindex <= size)
	{
		rapidjson::Value& item = values[heroindex - 1];
		rapidjson::Value& v = item["val"];
		return v.GetString();
	}
	return "";
}

void GlobalData::setPlotMissionIndex(int val)
{
	GameDataSave::getInstance()->setPlotMissionIndex(val);
}

int GlobalData::getPlotMissionIndex()
{
	//兼容上一版，已经到通关了，最大的INDEX溢出
	int plotindex = GameDataSave::getInstance()->getPlotMissionIndex();
	int max = vec_PlotMissionData.size();

	if (plotindex >= max)
		plotindex = max - 1;
	//迭代2个版本后删除 start
	if (vec_PlotMissionData[plotindex].dnpc.compare("n089") == 0 && vec_PlotMissionData[plotindex].status == M_DONE)
	{
		plotindex++;
	}
	if (plotindex >= max)
		plotindex = max - 1;
	//迭代2个版本后删除 end
	return plotindex;
}

void GlobalData::loadPlotMissionJsonData(int herotype)
{
	vec_PlotMissionData.clear();
	std::string plotfilename = StringUtils::format("data/plotmission%d.json", herotype);

	rapidjson::Document doc = ReadJsonFile(plotfilename);
	rapidjson::Value& values = doc["m"];
	for (unsigned int i = 0; i < values.Size(); i++)//剧情数组
	{
		rapidjson::Value& vitem = values[i];
		PlotMissionData data;
		rapidjson::Value& v = vitem["id"];
		data.id = v.GetString();

		v = vitem["snpc"];
		data.snpc = v.GetString();

		v = vitem["dnpc"];
		data.dnpc = v.GetString();

		if (vitem.HasMember("map"))
		{
			v = vitem["map"];
			data.mapid = v.GetString();
		}
		else
		{
			data.mapid = "";
		}

		v = vitem["unlock"];
		data.unlockchapter = atoi(v.GetString());

		if (data.dnpc.compare("n089") == 0)
		{
			data.unlockchapter = 13;
		}
		v = vitem["t"];
		data.type = atoi(v.GetString());

		data.status = M_NONE;

		v = vitem["word"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.words.push_back(str);
		}

		v = vitem["myword"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.mywords.push_back(str);
		}

		v = vitem["bossword"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.bossword.push_back(str);
		}
		v = vitem["rwds"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 1)
				data.rewords.push_back(str);
		}
		vec_PlotMissionData.push_back(data);
	}

	GlobalData::updatePlotMissionStatus();
}

void GlobalData::loadBranchPlotMissionJsonData()
{
	map_BranchPlotMissionData.clear();
	map_BranchPlotMissionItem.clear();
	std::string plotfilename = "data/branchplotmission.json";

	rapidjson::Document doc = ReadJsonFile(plotfilename);
	rapidjson::Value& values = doc["m"];
	for (unsigned int i = 0; i < values.Size(); i++)//剧情数组
	{
		rapidjson::Value& vitem = values[i];
		PlotMissionData data;
		rapidjson::Value& v = vitem["id"];
		std::string mid = v.GetString();
		data.id = mid;
		v = vitem["snpc"];
		data.snpc = v.GetString();

		v = vitem["dnpc"];
		data.dnpc = v.GetString();

		v = vitem["unlock"];
		data.unlockchapter = atoi(v.GetString());

		v = vitem["t"];
		data.type = atoi(v.GetString());

		data.status = M_NONE;

		v = vitem["word"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.words.push_back(str);
		}

		v = vitem["myword"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.mywords.push_back(str);
		}

		v = vitem["bossword"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.bossword.push_back(str);
		}
		v = vitem["rwds"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 1)
				data.rewords.push_back(str);
		}

		v = vitem["para"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 1)
				data.needgoods.push_back(str);
		}

		map_BranchPlotMissionData[mid].push_back(data);
		BranchPlotMissionItem bitme;
		int time = -1;
		int count = -1;
		v = vitem["time"];
		time = atoi(v.GetString());
		bitme.time = time*60;
		bitme.maxtime = time*60;
		v = vitem["count"];
		count = atoi(v.GetString());
		bitme.count = count<0?99:count;
		bitme.maxcount = bitme.count;
		bitme.subindex = 0;
		map_BranchPlotMissionItem[mid] = bitme;
	}

	GlobalData::updateBranchPlotMissionStatus();
}

void GlobalData::savePlotMissionStatus()
{
	//保存剧情状态"-"分割
	std::string str;
	for (unsigned int i = 0; i < GlobalData::vec_PlotMissionData.size(); i++)
	{
		std::string tmpstr = StringUtils::format("%d-", GlobalData::vec_PlotMissionData[i].status);
		str.append(tmpstr);
	}
	GameDataSave::getInstance()->setPlotMissionStatus(str.substr(0, str.length() - 1));
}

void GlobalData::updatePlotMissionStatus()
{
	//解析剧情状态"-"分割
	std::string str = GameDataSave::getInstance()->getPlotMissionStatus();
	if (str.length() > 0)
	{
		std::vector<std::string> tmp;
		CommonFuncs::split(str, tmp, "-");

		int msize = GlobalData::vec_PlotMissionData.size();
		unsigned int i = 0;
		for (i = 0; i < tmp.size(); i++)
		{
			GlobalData::vec_PlotMissionData[i].status = atoi(tmp[i].c_str());
		}
		int localsavesize = tmp.size();
		if (msize > localsavesize)
		{
			int addcount = msize - tmp.size();
			for (int m = 0; m < addcount; m++)
			{
				GlobalData::vec_PlotMissionData[i+m].status = M_NONE;
			}
		}

	}
}

void GlobalData::saveBranchPlotMissionStatus(std::string mid, int status)
{
	//保存支线剧情状态"-"分割
	std::string str;
	std::map<std::string, BranchPlotMissionItem>::iterator it;

	int count = 0;

	if (mid.length() > 0)
	{
		std::string curstr = StringUtils::format("%s,%d,%d,%d", mid.c_str(), GlobalData::map_BranchPlotMissionItem[mid].subindex, status, GlobalData::map_BranchPlotMissionItem[mid].time);
		str.append(curstr);
	}
	str.append(";");

	for (it = GlobalData::map_BranchPlotMissionItem.begin(); it != GlobalData::map_BranchPlotMissionItem.end(); it++)
	{
		std::string countstr = StringUtils::format("%d,", GlobalData::map_BranchPlotMissionItem[it->first].count);
		str.append(countstr);
	}

	curBranchMissionID = mid;
	GameDataSave::getInstance()->setBranchPlotMissionStatus(str.substr(0, str.length() - 1));
}

void GlobalData::updateBranchPlotMissionStatus()
{
	//解析支线剧情状态"-"分割
	std::string str = GameDataSave::getInstance()->getBranchPlotMissionStatus();
	if (str.length() > 0)
	{
		std::vector<std::string> savestr;
		CommonFuncs::split(str, savestr, ";");
		if (savestr.size() >= 2)
		{
			std::vector<std::string> tmp;
			CommonFuncs::split(savestr[0], tmp, ",");
			if (tmp.size() >= 4)
			{
				std::string mid = tmp[0];
				curBranchMissionID = mid;
				int subindex = atoi(tmp[1].c_str());
				int status = atoi(tmp[2].c_str());
				int time = atoi(tmp[3].c_str());

				GlobalData::map_BranchPlotMissionData[mid][subindex].status = status;
				GlobalData::map_BranchPlotMissionItem[mid].subindex = subindex;
				GlobalData::map_BranchPlotMissionItem[mid].time = time;
			}
			std::vector<std::string> tmp1;
			CommonFuncs::split(savestr[1], tmp1, ",");
			int i = 0;
			if (tmp1.size() >= GlobalData::map_BranchPlotMissionItem.size())
			{
				std::map<std::string, BranchPlotMissionItem>::iterator it;
				for (it = GlobalData::map_BranchPlotMissionItem.begin(); it != GlobalData::map_BranchPlotMissionItem.end(); it++)
				{
					GlobalData::map_BranchPlotMissionItem[it->first].count = atoi(tmp1[i].c_str());
					i++;
				}

			}
		}
	}
}

std::string GlobalData::getCurBranchPlotMissison()
{
	//std::string str = GameDataSave::getInstance()->getBranchPlotMissionStatus();
	//if (str.length() > 0)
	//{
	//	std::vector<std::string> tmp;
	//	CommonFuncs::split(str, tmp, ",");
	//	if (tmp.size() >= 3)
	//	{
	//		std::string mid = tmp[0];
	//		return mid;
	//	}
	//}
	//return "";
	return curBranchMissionID;
}

bool GlobalData::isDoingBranchPlotMisson()
{
	std::map<std::string, std::vector<PlotMissionData>>::iterator it;
	for (it = map_BranchPlotMissionData.begin(); it != map_BranchPlotMissionData.end(); it++)
	{
		std::vector<PlotMissionData>::iterator sit;
		for (sit = map_BranchPlotMissionData[it->first].begin(); sit != map_BranchPlotMissionData[it->first].end(); sit++)
		{
			if (sit->status == M_DOING)
				return true;
		}
	}

	std::string mid = getCurBranchPlotMissison();

	if (mid.length() > 0)
	{
		int subindex = GlobalData::map_BranchPlotMissionItem[mid].subindex;

		if (subindex < GlobalData::map_BranchPlotMissionData[mid].size() - 1 && GlobalData::map_BranchPlotMissionData[mid][subindex].status == M_DONE)
		{
			return true;
		}
	}
	return false;
}

int GlobalData::getUnlockChapter()
{
	int c = GameDataSave::getInstance()->getPlotUnlockChapter();

	int plotindex = GameDataSave::getInstance()->getPlotMissionIndex();
	int max = vec_PlotMissionData.size();

	if (plotindex >= max)
		plotindex = max - 1;

    if (plotindex >= max - 1)
    {
        c = MAXCHAPTER;
    }
	if (vec_PlotMissionData[plotindex].dnpc.compare("n089") == 0 && vec_PlotMissionData[plotindex].status == M_DONE)
	{
		c = MAXCHAPTER;
	}

	if (c > MAXCHAPTER)
		c = MAXCHAPTER;
	return c;
}

void GlobalData::setUnlockChapter(int val)
{
	int lastChapter = GlobalData::getUnlockChapter();

	if (val >= lastChapter)
		GameDataSave::getInstance()->setPlotUnlockChapter(val);
}

void GlobalData::loadShopData()
{
	vec_goods.clear();
	rapidjson::Document doc = ReadJsonFile("data/shop.json");
	rapidjson::Value& values = doc["gs"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		GoodsData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["icon"];
		data.icon = v.GetString();

		v = item["res"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			data.vec_res.push_back(v[m].GetString());
		}
		v = item["name"];
		data.name = v.GetString();

		v = item["type"];
		data.type = atoi(v.GetString());

		v = item["desc"];
		data.desc = v.GetString();

		v = item["price"];
		data.price = atoi(v.GetString());
		vec_goods.push_back(data);
	}
}

void GlobalData::loadAchiveJsonData()
{
	vec_achiveData.clear();
	rapidjson::Document doc = ReadJsonFile("data/achive.json");
	rapidjson::Value& values = doc["a"];
	for (unsigned int i = 0; i < values.Size(); i++)//武器防具数组
	{
		AchiveData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.id = v.GetString();
		v = item["type"];
		data.type = atoi(v.GetString());
		v = item["name"];
		data.name = v.GetString();
		v = item["desc"];
		data.desc = v.GetString();
		v = item["para1"];
		data.vec_para.push_back(v.GetString());
		v = item["para2"];
		std::string para2str = v.GetString();
		if (para2str.length() > 0 && para2str.compare("0") != 0)
			data.vec_para.push_back(v.GetString());

		v = item["rwd"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string val = v[m].GetString();
			if (val.length() > 0 && val.compare("0") != 0)
			{
				data.vec_rwd.push_back(val);
			}
		}
		data.finish = 0;
		data.isshowanim = 0;
		vec_achiveData.push_back(data);
	}
}
int GlobalData::createRandomNum(int val)
{
	int syssec = GlobalData::getSysSecTime();
	int static randNum = 0;
	randNum += 3600 * 24;
	syssec += randNum;
	if (syssec < 0 || randNum < 0)
	{
		syssec = 0;
		randNum = 0;
	}
	srand(syssec);
	int r = rand() % val;
	return r;
}

std::vector<std::string> GlobalData::getSaveListId()
{
	std::string str = GameDataSave::getInstance()->getSaveListId();

	std::vector<std::string> tmp;
	CommonFuncs::split(str, tmp, ";");
	vec_saveids.clear();
	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		vec_saveids.push_back(tmp[i]);
	}

	return vec_saveids;
}

void GlobalData::setSaveListId(std::vector<std::string> vec_val)
{
	std::string str;
	for (unsigned int i = 0; i < vec_val.size(); i++)
	{
		str.append(vec_val[i]);
		str.append(";");
	}

	GameDataSave::getInstance()->setSaveListId(str.substr(0, str.length() - 1));
}

void GlobalData::setCurHeroIdToSaveList()
{
	std::string uid = GameDataSave::getInstance()->getUserId();

	int heroid = GameDataSave::getInstance()->getHeroIdByUid(uid);
	std::vector<std::string> vec_ids = GlobalData::getSaveListId();

	vec_ids[heroid - 1] = uid;

	GlobalData::setSaveListId(vec_ids);
}

bool GlobalData::isExercising()
{
	std::string str = GameDataSave::getInstance()->getExersiceCfg();
	return str.length() > 0 ? true : false;
}

bool GlobalData::isHasFSF()
{
	for (unsigned int m = 0; m < StorageRoom::map_storageData[RES_2].size(); m++)
	{
		PackageData* pData = &StorageRoom::map_storageData[RES_2][m];
		if (pData->strid.compare("72") == 0 && pData->count > 0)
		{
			return true;
		}
	}

	for (unsigned int m = 0; m < MyPackage::vec_packages.size(); m++)
	{
		PackageData* pData = &MyPackage::vec_packages[m];
		if (pData->strid.compare("72") == 0 && pData->count > 0)
		{
			return true;
		}
	}
	return false;
}

bool GlobalData::isHasVulture()
{
	for (unsigned int m = 0; m < StorageRoom::map_storageData[RES_2].size(); m++)
	{
		PackageData* pData = &StorageRoom::map_storageData[RES_2][m];
		if (pData->strid.compare("83") == 0 && pData->count > 0)
		{
			return true;
		}
	}
	
	return false;
}

void GlobalData::loadGftrickData()
{
	map_gftricks.clear();
	rapidjson::Document doc = ReadJsonFile("data/gftrick.json");
	rapidjson::Value& values = doc["s"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		rapidjson::Value& vitem = values[i];
		GFTrickData data;
		rapidjson::Value& v = vitem["id"];
		data.id = v.GetString();

		v = vitem["name"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			data.snames.push_back(str);
		}

		map_gftricks[data.id] = data;
	}
}

void GlobalData::loadGfskillData()
{
	map_gfskills.clear();
	rapidjson::Document doc = ReadJsonFile("data/skill.json");
	rapidjson::Value& values = doc["sk"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		rapidjson::Value& vitem = values[i];
		GFSkillData data;

		rapidjson::Value& v = vitem["name"];
		data.name = v.GetString();
		v = vitem["id"];
		data.id = v.GetString();

		v = vitem["desc"];
		data.desc = v.GetString();

		v = vitem["desc1"];
		data.desc1 = v.GetString();

		data.leftval = 0;
		int intid = atoi(data.id.c_str());
		map_gfskills[intid] = data;
	}
}

std::string GlobalData::tempHasGf_Equip(std::string strid)
{
	//std::map<std::string, MapData>::iterator it;

	//for (it = GlobalData::map_maps.begin(); it != GlobalData::map_maps.end(); ++it)
	//{
	//	std::string mapid = GlobalData::map_maps[it->first].strid;

	//	std::string datastr = GameDataSave::getInstance()->getTempStorage(mapid);
	//	if (datastr.length() > 0)
	//	{
	//		std::vector<std::string> vec_retstr;
	//		CommonFuncs::split(datastr, vec_retstr, ";");
	//		for (unsigned int i = 0; i < vec_retstr.size(); i++)
	//		{
	//			std::vector<std::string> tmp;
	//			CommonFuncs::split(vec_retstr[i], tmp, "-");
	//			std::string tmpstrid = tmp[0];
	//			int tmptype = atoi(tmp[1].c_str());
	//			if ((tmptype == W_GONG || tmptype == N_GONG || tmptype == WEAPON || tmptype == PROTECT_EQU) && strid.compare(tmpstrid) == 0)
	//				return true;
	//		}
	//	}
	//}

	std::map<std::string, std::vector<std::string>>::iterator it;

	for (it = GlobalData::map_tempGf_Equip.begin(); it != GlobalData::map_tempGf_Equip.end(); ++it)
	{
		for (unsigned int i = 0; i < GlobalData::map_tempGf_Equip[it->first].size(); i++)
		{
			if (GlobalData::map_tempGf_Equip[it->first][i].compare(strid) == 0)
				return it->first;
		}
	}

	return "";
}

int GlobalData::getResType(std::string strid)
{
	std::map<std::string, std::vector<BuildActionData>>::iterator it;
	for (it = GlobalData::map_buidACData.begin(); it != GlobalData::map_buidACData.end(); ++it)
	{
		std::vector<BuildActionData> vec_bactData = GlobalData::map_buidACData[it->first];

		for (unsigned int m = 0; m < vec_bactData.size(); m++)
		{
			BuildActionData bdata = vec_bactData[m];
			if (strid.compare(bdata.icon) == 0)
			{
				return bdata.type - 1;
			}
		}
	}

	for (unsigned int n = 0; n < GlobalData::vec_resData.size(); n++)
	{
		ResData rdata = GlobalData::vec_resData[n];
		if (strid.compare(rdata.strid) == 0)
		{
			return rdata.type - 1;
		}
	}

	std::map<std::string, WG_NGData>::iterator itf;
	for (itf = GlobalData::map_wgngs.begin(); itf != GlobalData::map_wgngs.end(); ++itf)
	{
		WG_NGData gfdata = GlobalData::map_wgngs[itf->first];
		if (strid.compare(gfdata.id) == 0)
		{
			return gfdata.type - 1;
		}
	}

	std::map<std::string, EquipData>::iterator ite;
	for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
	{
		EquipData edata = GlobalData::map_equips[ite->first];
		if (strid.compare(edata.id) == 0)
		{
			return edata.type - 1;
		}
	}

	return 0;
}

int GlobalData::getResExType(std::string strid)
{
	std::map<std::string, std::vector<BuildActionData>>::iterator it;
	for (it = GlobalData::map_buidACData.begin(); it != GlobalData::map_buidACData.end(); ++it)
	{
		std::vector<BuildActionData> vec_bactData = GlobalData::map_buidACData[it->first];

		for (unsigned int m = 0; m < vec_bactData.size(); m++)
		{
			BuildActionData bdata = vec_bactData[m];
			if (strid.compare(bdata.icon) == 0)
			{
				return bdata.extype;
			}
		}
	}

	for (unsigned int n = 0; n < GlobalData::vec_resData.size(); n++)
	{
		ResData rdata = GlobalData::vec_resData[n];
		if (strid.compare(rdata.strid) == 0)
		{
			return rdata.actype;
		}
	}

	std::map<std::string, WG_NGData>::iterator itf;
	for (itf = GlobalData::map_wgngs.begin(); itf != GlobalData::map_wgngs.end(); ++itf)
	{
		WG_NGData gfdata = GlobalData::map_wgngs[itf->first];
		if (strid.compare(gfdata.id) == 0)
		{
			return gfdata.extype;
		}
	}

	std::map<std::string, EquipData>::iterator ite;
	for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
	{
		EquipData edata = GlobalData::map_equips[ite->first];
		if (strid.compare(edata.id) == 0)
		{
			return edata.extype;
		}
	}

	return 0;
}

void GlobalData::loadChallengeRewardData()
{
	map_challengeReward.clear();
	rapidjson::Document doc = ReadJsonFile("data/challengereward.json");
	rapidjson::Value& values = doc["rs"];
	for (unsigned int i = 0; i < values.Size(); i++)//npc数组
	{
		ChallengeRewardData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["npc"];
		std::string npcid = v.GetString();

		v = item["winres"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			if (str.length() > 1)
				data.vec_winres.push_back(str);
		}
		v = item["winresrnd"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			float rnd = atof(str.c_str());
			if (str.length() > 0 && rnd > 0.0f)
			{
				data.vec_winrnd.push_back(rnd);
			}
		}
		map_challengeReward[npcid] = data;
	}
}

void GlobalData::loadTempGF_EquipData()
{
	map_tempGf_Equip.clear();
	std::map<std::string, MapData>::iterator it;

	for (it = GlobalData::map_maps.begin(); it != GlobalData::map_maps.end(); ++it)
	{
		std::string mapid = GlobalData::map_maps[it->first].strid;

		std::string datastr = GameDataSave::getInstance()->getTempStorage(mapid);
		if (datastr.length() > 0)
		{
			std::vector<std::string> vec_retstr;
			CommonFuncs::split(datastr, vec_retstr, ";");
			for (unsigned int i = 0; i < vec_retstr.size(); i++)
			{
				std::vector<std::string> tmp;
				CommonFuncs::split(vec_retstr[i], tmp, "-");
				std::string tmpstrid = tmp[0];
				int tmptype = atoi(tmp[1].c_str());
				if (tmptype == W_GONG || tmptype == N_GONG || tmptype == WEAPON || tmptype == PROTECT_EQU)
				{
					map_tempGf_Equip[mapid].push_back(tmpstrid);
				}
			}
		}
	}
}

int GlobalData::getShareDay()
{
	return GameDataSave::getInstance()->getShareDayOfYear();
}

void GlobalData::setShareDay(int day)
{
	GameDataSave::getInstance()->setShareDayOfYear(day);
}


std::string GlobalData::getMakeWarmConfig()
{
	return GameDataSave::getInstance()->getWarmConfig();
}

void GlobalData::setMakeWarmConfig(std::string strval)
{
	GameDataSave::getInstance()->setWarmConfig(strval);
}

std::string GlobalData::getExgCfgData()
{
	return GameDataSave::getInstance()->getExgCfgData();
}

void GlobalData::setExgCfgData(std::string strval)
{
	GameDataSave::getInstance()->setExgCfgData(strval);
}

void GlobalData::setWxbMapPos(int pos)
{
	wxbmapos = pos;
	GameDataSave::getInstance()->setWxbMapPos(pos);
}


int GlobalData::getWxbMapPos()
{
	return wxbmapos;
}

void GlobalData::setDgqbMapPos(int pos)
{
	dgqbmapos = pos;
	GameDataSave::getInstance()->setDgqbMapPos(pos);
}

int GlobalData::getDgqbMapPos()
{
	return dgqbmapos;
}

void GlobalData::setReviveCount(int val)
{
	GameDataSave::getInstance()->setReviveCount(val);
}

int GlobalData::getReviveCount()
{
	return GameDataSave::getInstance()->getReviveCount();
}

int GlobalData::getMyGoldCount()
{
	return myGlodCount;
}

void GlobalData::setMyGoldCount(int count)
{
	myGlodCount = count;
	GlobalData::setMD5MyGoldCount(md5(myGlodCount));
	GameDataSave::getInstance()->setGoldCount(myGlodCount);
	GlobalData::doAchive(A_1, count);
}

std::string GlobalData::getMyID()
{
	return GameDataSave::getInstance()->getMyID();
}

void GlobalData::setMyID(std::string str)
{
	GameDataSave::getInstance()->setMyID(str);
}

std::string GlobalData::getMyNickName()
{
	return GameDataSave::getInstance()->getMyNickName();
}

void GlobalData::setMyNickName(std::string str)
{
	GameDataSave::getInstance()->setMyNickName(str);
}

int GlobalData::getTimeGiftLeftTime()
{
	return timegiftlefttime;
}

void GlobalData::setTimeGiftLeftTime(int val)
{
	timegiftlefttime = val;
}


void GlobalData::setIsBuyTimeGift(bool val)
{
	isBuyTimeGift = val;
	GameDataSave::getInstance()->setIsBuyTimeGift(val);
	
}

bool GlobalData::getIsBuyTimeGift()
{
	return isBuyTimeGift;
}

void GlobalData::setFreeReviveCount(int val)
{
	freeReviveCount = val;
}

int GlobalData::getFreeReviveCount()
{
	return freeReviveCount;
}

void GlobalData::setUseGold(int val)
{
	GameDataSave::getInstance()->setUseGold(val);
	GlobalData::doAchive(A_0, val);
}

int GlobalData::getUseGold()
{
	return GameDataSave::getInstance()->getUseGold();
}

void GlobalData::loadFriendly()
{
	map_myfriendly.clear();
	std::string datastr = GameDataSave::getInstance()->getFriendly();

	if (datastr.length() > 0)
	{
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
				map_myfriendly[tmp[0]].friendly = friendly;
				map_myfriendly[tmp[0]].relation = atoi(tmp[2].c_str());
			}
		}
	}
}


void GlobalData::saveFriendly()
{
	std::string str;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::map_myfriendly.begin(); it != GlobalData::map_myfriendly.end(); ++it)
	{
		int friendly = GlobalData::map_myfriendly[it->first].friendly;
		int relation = GlobalData::map_myfriendly[it->first].relation;
		bool issave = true;
		if (friendly == 0 && relation == 0)
			issave = false;

		if (issave)
		{
			std::string onestr = StringUtils::format("%s,%d,%d", it->first.c_str(), friendly, relation);
			str.append(onestr);
			str.append(";");
		}
	}
	GameDataSave::getInstance()->setFriendly(str.substr(0, str.length() - 1));
}

void GlobalData::loadNpcFriendJsonData()
{
	map_NPCFriendData.clear();
	rapidjson::Document doc = ReadJsonFile("data/friend.json");
	rapidjson::Value& values = doc["f"];
	for (unsigned int i = 0; i < values.Size(); i++)//一级资源数组
	{
		NPCFriendData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["npcid"];
		std::string npcid = v.GetString();

		v = item["max"];
		data.maxfriendly = atoi(v.GetString());

		v = item["need"];
		data.needfriendly = atoi(v.GetString());

		v = item["atk"];
		data.atkpercent = atof(v.GetString());

		v = item["df"];
		data.dfpercent = atof(v.GetString());

		v = item["avoid"];
		data.dodgepercent = atof(v.GetString());

		v = item["crit"];
		data.critpercent = atof(v.GetString());

		v = item["hp"];
		data.hppercent = atof(v.GetString());

		v = item["enemynpc"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& npsv = v[m];
			std::string npcid = npsv.GetString();
			if (npcid.length() > 1)
				data.vec_enemynpc.push_back(npcid);
		}

		v = item["fightnpc"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& npsv = v[m];
			std::string npcid = npsv.GetString();
			if (npcid.length() > 1)
				data.vec_fightnpc.push_back(npcid);
		}

		v = item["res"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& resv = v[m];
			std::string resval = resv.GetString();
			if (resval.length() > 1)
				data.vec_giveres.push_back(atoi(resval.c_str()));
		}

		v = item["nres"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& resv = v[m];
			std::string resval = resv.GetString();
			if (resval.length() > 1)
				data.vec_askres.push_back(atoi(resval.c_str()));
		}

		map_NPCFriendData[npcid] = data;

	}
}
void GlobalData::loadNpcMasterJsonData()
{
	map_NPCMasterData.clear();
	rapidjson::Document doc = ReadJsonFile("data/master.json");
	rapidjson::Value& values = doc["ms"];
	for (unsigned int i = 0; i < values.Size(); i++)//一级资源数组
	{
		NPCMasterData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["npcid"];
		std::string npcid = v.GetString();

		v = item["need"];
		data.needfriendly = atoi(v.GetString());

		v = item["atk"];
		data.atkpercent = atof(v.GetString());

		v = item["df"];
		data.dfpercent = atof(v.GetString());

		v = item["avoid"];
		data.dodgepercent = atof(v.GetString());

		v = item["crit"];
		data.critpercent = atof(v.GetString());

		v = item["hp"];
		data.hppercent = atof(v.GetString());

		v = item["gfid"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& resv = v[m];
			std::string resval = resv.GetString();
			if (resval.length() > 1)
				data.vec_gfid.push_back(resval);
		}

		v = item["bonus"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& resv = v[m];
			std::string resval = resv.GetString();
			if (resval.length() > 0 && resval.compare("0") != 0)
				data.vec_gfbonus.push_back(atof(resval.c_str()));
		}

		map_NPCMasterData[npcid] = data;

	}
}

void GlobalData::loadMixGfJsonData()
{
	map_MixGfData.clear();
	rapidjson::Document doc = ReadJsonFile("data/zhgf.json");
	rapidjson::Value& values = doc["m"];
	for (unsigned int i = 0; i < values.Size(); i++)//一级资源数组
	{
		MixGfData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.id = v.GetString();

		v = item["name"];
		data.name = v.GetString();

		v = item["lv"];
		data.lv = atoi(v.GetString());

		v = item["atk"];
		data.atkpercent = atof(v.GetString());

		v = item["df"];
		data.dfpercent = atof(v.GetString());

		v = item["avoid"];
		data.dodgepercent = atof(v.GetString());

		v = item["crit"];
		data.critpercent = atof(v.GetString());

		v = item["hp"];
		data.hppercent = atof(v.GetString());

		v = item["sex"];
		data.sex = atoi(v.GetString());

		v = item["mgf"];
		data.mastergf = v.GetString();

		v = item["cgf"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& resv = v[m];
			std::string resval = resv.GetString();
			if (resval.length() > 1)
				data.vec_secgf.push_back(resval);
		}

		v = item["egf"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& resv = v[m];
			std::string resval = resv.GetString();
			if (resval.length() > 1)
				data.vec_mutexgf.push_back(resval);
		}

		map_MixGfData[data.id] = data;

	}
}

std::string GlobalData::getMixGF()
{
	return GameDataSave::getInstance()->getMixGF();
}

void GlobalData::setMixGF(std::string str)
{
	GameDataSave::getInstance()->setMixGF(str);
}

std::string GlobalData::getHeroProperData()
{
	return GameDataSave::getInstance()->getHeroProperData();
}
void GlobalData::setHeroProperData(std::string strval)
{
	GameDataSave::getInstance()->setHeroProperData(strval);
}

void GlobalData::getAchiveData()
{
	std::string datastr = GameDataSave::getInstance()->getAchiveData();
	if (datastr.length() > 0)
	{
		std::vector<std::string> vec_retstr;
		CommonFuncs::split(datastr, vec_retstr, ";");
		for (unsigned int i = 0; i < vec_retstr.size(); i++)
		{
			GlobalData::vec_achiveData[i].finish = atoi(vec_retstr[i].c_str());
		}
	}
}

void GlobalData::saveAchiveData()
{
	std::string str;
    std::vector<AchiveData> tmp_saveAchives;
    for (unsigned int i = 0; i < GlobalData::vec_achiveData.size(); i++)
    {
        tmp_saveAchives.push_back(GlobalData::vec_achiveData[i]);
    }
    
    int size = tmp_saveAchives.size();
    for(int i = 0; i < size; i++)
    {
        for(int j = 1; j < size - i; j++)
        {
            if(atoi(tmp_saveAchives[j].id.c_str())< atoi(tmp_saveAchives[j-1].id.c_str()))
            {
                AchiveData tempdata = tmp_saveAchives[j-1];
                tmp_saveAchives[j-1] = tmp_saveAchives[j];
                tmp_saveAchives[j] = tempdata;
            }
        }
    }

	for (unsigned int i = 0; i < tmp_saveAchives.size(); i++)
	{
		int finish = tmp_saveAchives[i].finish;
		int maxcount = GlobalData::getAchiveFinishCount(tmp_saveAchives[i]);
		if (finish > maxcount)
			finish = maxcount;
		std::string onestr = StringUtils::format("%d;", finish);
		str.append(onestr);
	}

	GameDataSave::getInstance()->setAchiveData(str.substr(0, str.length() - 1));
}

void GlobalData::doAchive(int atype, int count)
{
	bool issave = false;
	for (unsigned int i = 0; i < GlobalData::vec_achiveData.size(); i++)
	{
		if (GlobalData::vec_achiveData[i].type == atype && GlobalData::vec_achiveData[i].finish != -1)
		{
			issave = true;
			GlobalData::vec_achiveData[i].finish = count;
		}
	}
	if (issave)
		saveAchiveData();
}

int GlobalData::getAchiveFinishCount(AchiveData adata)
{
	int needcount = 1;

	if (adata.type == A_0 || adata.type == A_1 || adata.type == A_2 || adata.type == A_3 || adata.type == A_5 || adata.type == A_11)
	{
		needcount = atoi(adata.vec_para[0].c_str());
	}

	else if (adata.type == A_7 || adata.type == A_9)
	{
		needcount = atoi(adata.vec_para[1].c_str());

	}
	else if (adata.type == A_10)
	{
		needcount = atoi(adata.vec_para[1].c_str());
	}
	else if (adata.type == A_12)
	{
		needcount = atoi(adata.vec_para[1].c_str());
	}
	return needcount;
}

void GlobalData::getAchiveAnimData()
{
	std::string str = GameDataSave::getInstance()->getAchiveAnimData();
	if (str.length() > 0)
	{
		std::vector<std::string> vec_retstr;
		CommonFuncs::split(str, vec_retstr, ";");
		for (unsigned int i = 0; i < vec_retstr.size(); i++)
		{
			GlobalData::vec_achiveData[i].isshowanim = atoi(vec_retstr[i].c_str());
		}
	}
}

void GlobalData::saveAchiveAnimData()
{
	std::string str;
	std::vector<AchiveData> tmp_saveAchives;
	for (unsigned int i = 0; i < GlobalData::vec_achiveData.size(); i++)
	{
		tmp_saveAchives.push_back(GlobalData::vec_achiveData[i]);
	}

	int size = tmp_saveAchives.size();
	for (int i = 0; i < size; i++)
	{
		for (int j = 1; j < size - i; j++)
		{
			if (atoi(tmp_saveAchives[j].id.c_str())< atoi(tmp_saveAchives[j - 1].id.c_str()))
			{
				AchiveData tempdata = tmp_saveAchives[j - 1];
				tmp_saveAchives[j - 1] = tmp_saveAchives[j];
				tmp_saveAchives[j] = tempdata;
			}
		}
	}

	for (unsigned int i = 0; i < tmp_saveAchives.size(); i++)
	{
		int isshow = tmp_saveAchives[i].isshowanim;
		std::string onestr = StringUtils::format("%d;", isshow);
		str.append(onestr);
	}

	GameDataSave::getInstance()->setAchiveAnimData(str.substr(0, str.length() - 1));
}

std::string GlobalData::addUidString(std::string val)
{
    return uid + val;
}

std::string GlobalData::UUID()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return getDeviceIDInKeychain();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return "292ECD05-4F18-46C6-7754-A9C81FFEB7CA";//"";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string ret;
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/Utils", "UUID", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		ret = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
	return ret;
#endif
}

std::string GlobalData::getVersion()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return getvercode();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return "1.2.4";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string ret;
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/Utils", "getVersion", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		ret = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
	return ret;
#endif
}

std::string GlobalData::getPackageName()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return getbundleid();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return "com.kuxx.jh";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string ret;
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/Utils", "getPkgName", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		ret = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
	return ret;
#endif
}

std::string GlobalData::getChannelId()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return getbundleid();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return "win32";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string ret;
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/Utils", "getChannelID", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		ret = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
	return ret;
#endif
}

void GlobalData::copyToClipBoard(std::string text)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	copytoclipboard((char*)text.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	char p_str[256] = { 0 };
	sprintf(p_str, "%s", text.c_str());
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/AppActivity", "copyToClipboard", "(Ljava/lang/String;)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
}
#endif
}

std::string GlobalData::getMD5MyGoldCount()
{
	return MD5MyGoldCount;
}

void GlobalData::setMD5MyGoldCount(std::string val)
{
	MD5MyGoldCount = val;
}

std::string GlobalData::getMD5CostGlodCount()
{
	return MD5CostGlodCount;
}

void GlobalData::setMD5CostGlodCount(std::string val)
{
	MD5CostGlodCount = val;
}

std::string GlobalData::getMD5FreeReviveCount()
{
	return MD5FreeReviveCount;
}

void GlobalData::setMD5FreeReviveCount(std::string val)
{
	MD5FreeReviveCount = val;
}

std::string GlobalData::getMD5HeroLv()
{
	return MD5HeroLv;
}

void GlobalData::setMD5HeroLv(std::string val)
{
	MD5HeroLv = val;
}

std::string GlobalData::getMD5LiveDays()
{
	return MD5LiveDays;
}

void GlobalData::setMD5LiveDays(std::string val)
{
	MD5LiveDays = val;
}

int GlobalData::getNoPopNoticeDay()
{
	return GameDataSave::getInstance()->getNoPopNoticeDay();
}

void GlobalData::setNoPopNoticeDay(int day)
{
	GameDataSave::getInstance()->setNoPopNoticeDay(day);
}

void GlobalData::setNoAds(bool val)
{
    UserDefault::getInstance()->setBoolForKey(addUidString("noads").c_str(), val);
    UserDefault::getInstance()->flush();
}

bool GlobalData::getNoAds()
{
    return UserDefault::getInstance()->getBoolForKey(addUidString("noads").c_str(), false);
}

void GlobalData::setNoComments(bool val)
{
    UserDefault::getInstance()->setBoolForKey(addUidString("nocomments").c_str(), val);
    UserDefault::getInstance()->flush();
}

bool GlobalData::getNoComments()
{
    return UserDefault::getInstance()->getBoolForKey(addUidString("nocomments").c_str(), false);
}

void GlobalData::setDailyAdsCount(int val)
{
    UserDefault::getInstance()->setIntegerForKey(addUidString("dailyadscount").c_str(), val);
    UserDefault::getInstance()->flush();
}

int GlobalData::getDailyAdsCount()
{
    return UserDefault::getInstance()->getIntegerForKey(addUidString("dailyadscount").c_str(), 0);
}

void GlobalData::setDailyAdsDay(int val)
{
    UserDefault::getInstance()->setIntegerForKey(addUidString("dailyadsday").c_str(), val);
    UserDefault::getInstance()->flush();
}

int GlobalData::getDailyAdsDay()
{
    return UserDefault::getInstance()->getIntegerForKey(addUidString("dailyadsday").c_str(), 0);
}

void GlobalData::setAdsInterval(int val)
{
    adsinterval = val;
}

int GlobalData::getAdsInterval()
{
    return adsinterval;
}

void GlobalData::setAdsChoose(long val)
{
    adschoose = val;
}

int GlobalData::getAdsChoose(int position)
{
    /*long m = adschoose;
     int n =pow(10,position);
     float x = m/n;
     long i = (long)(m/n);
     int j = i % 10;
     return j;*/
    return (int)(adschoose/pow(10, position)) % 10;
}

void GlobalData::setHasBuy(bool val)
{
    hasBuy = val;
}

bool GlobalData::getHasBuy()
{
    return hasBuy;
}

