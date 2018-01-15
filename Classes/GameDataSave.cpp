#include "GameDataSave.h"
#include "cocos2d.h"
#include "Utility.h"
#include "Hero.h"
#include "Nature.h"
#include "GlobalData.h"
GameDataSave* GameDataSave::_Context = NULL;
std::string GameDataSave::userid = "";
GameDataSave* GameDataSave::getInstance() {
	if (_Context == NULL) {
		_Context = new GameDataSave;
	}
	return _Context;
}

void GameDataSave::purgeGameSave() {
	CC_SAFE_DELETE(_Context);
}

int GameDataSave::getLiveDays()
{
	return loadIntDataByKey(addUIDString("livedays"));
}

void GameDataSave::setLiveDays(int val)
{
	saveIntDataByKey(addUIDString("livedays"), val);
}

float GameDataSave::getHeroOutinjury()
{
	return loadFloatDataByKey(addUIDString("outinjury"), Hero::MAXOutinjuryValue);
}
void GameDataSave::setHeroOutinjury(float val)
{
	saveFloatDataByKey(addUIDString("outinjury"), val);
}

float GameDataSave::getHeroInnerinjury()
{
	return loadFloatDataByKey(addUIDString("innerinjury"), Hero::MAXInnerinjuryValue);
}
void GameDataSave::setHeroInnerinjury(float val)
{
	saveFloatDataByKey(addUIDString("innerinjury"), val);
}

float GameDataSave::getHeroHunger()
{
	return loadFloatDataByKey(addUIDString("hunger"), Hero::MAXHungerValue);
}
void GameDataSave::setHeroHunger(float val)
{
	saveFloatDataByKey(addUIDString("hunger"), val);
}

float GameDataSave::getHeroSpirit()
{
	return loadFloatDataByKey(addUIDString("spirit"), Hero::MAXSpiritValue);
}
void GameDataSave::setHeroSpirit(float val)
{
	saveFloatDataByKey(addUIDString("spirit"), val);
}

float GameDataSave::getHeroLife()
{
	return loadFloatDataByKey(addUIDString("life"), -100.0f);
}
void GameDataSave::setHeroLife(float val)
{
	saveFloatDataByKey(addUIDString("life"), val);
}

int GameDataSave::getNatureTime()
{
	return loadIntDataByKey(addUIDString("ndaytime"));
}
void GameDataSave::setNatureTime(int val)
{
	saveIntDataByKey(addUIDString("ndaytime"), val);
}

int GameDataSave::getNatureWeather()
{
	return loadIntDataByKey(addUIDString("nwhather"), EWeather::Suny);
}
void GameDataSave::setNatureWeather(int val)
{
	saveIntDataByKey(addUIDString("nwhather"), val);
}

int GameDataSave::getNatureWeatherChangeCount()
{
	return loadIntDataByKey(addUIDString("nwhatherchgcount"), 0);
}

void GameDataSave::setNatureWeatherChangeCount(int val)
{
	saveIntDataByKey(addUIDString("nwhatherchgcount"), val);
}

int GameDataSave::getNatureReason()
{
	return loadIntDataByKey(addUIDString("nreason"), EReason::Spring);
}
void GameDataSave::setNatureReason(int val)
{
	saveIntDataByKey(addUIDString("nreason"), val);
}

int GameDataSave::getNatureTemperature()
{
	//默认温度取随机
	int maxr = tempeRange[getNatureReason()][1] - tempeRange[getNatureReason()][0] + 1;
	int r = tempeRange[getNatureReason()][0] + GlobalData::createRandomNum(maxr);
	return loadIntDataByKey(addUIDString("ntempe"), r);
}
void GameDataSave::setNatureTemperature(int val)
{
	saveIntDataByKey(addUIDString("ntempe"), val);
}

std::string GameDataSave::getStorageData()
{
	return loadStringDataByKey(addUIDString("storage"), "");
}
void GameDataSave::setStorageData(std::string valstr)
{
	saveStringDataByKey(addUIDString("storage"), valstr);
}

int GameDataSave::getBuildLV(std::string buildname)
{
	std::string rname = StringUtils::format("%slv", buildname.c_str());
	return loadIntDataByKey(addUIDString(rname));
}
void GameDataSave::setBuildLV(std::string buildname, int val)
{
	std::string rname = StringUtils::format("%slv", buildname.c_str());
	saveIntDataByKey(addUIDString(rname), val);
}

std::string GameDataSave::getHeroAddr()
{
	return loadStringDataByKey(addUIDString("heroaddr"), "m1-1");
}

void GameDataSave::setHeroAddr(std::string addr)
{
	saveStringDataByKey(addUIDString("heroaddr"), addr);
}

std::string GameDataSave::getTempStorage(std::string addrname)
{
	std::string str = StringUtils::format("%s-temps", addrname.c_str());
	return loadStringDataByKey(addUIDString(str));
}

void GameDataSave::setTempStorage(std::string addrname, std::string vstr)
{
	std::string str = StringUtils::format("%s-temps", addrname.c_str());
	saveStringDataByKey(addUIDString(str), vstr);
}

std::string GameDataSave::getPackage()
{
	return loadStringDataByKey(addUIDString("packages"));
}

void GameDataSave::setPackage(std::string vstr)
{
	saveStringDataByKey(addUIDString("packages"), vstr);
}

std::string GameDataSave::getResData()
{
	return loadStringDataByKey(addUIDString("resdata"));
}
void GameDataSave::setResData(std::string vstr)
{
	saveStringDataByKey(addUIDString("resdata"), vstr);
}

int GameDataSave::getHeroId()
{
	return loadIntDataByKey(addUIDString("heroid"), 1);
}

int GameDataSave::getHeroIdByUid(std::string uid)
{
	std::string key = StringUtils::format("%sheroid", uid.c_str());
	return loadIntDataByKey(key, 1);
}

void GameDataSave::setHeroId(int id)
{
	saveIntDataByKey(addUIDString("heroid"), id);
}

int GameDataSave::getHeroLV()
{
	return loadIntDataByKey(addUIDString("herolv"), 0);
}

int GameDataSave::getHeroLV(std::string localid)
{
	std::string key = localid + "herolv";
	return loadIntDataByKey(key, 0);
}

void GameDataSave::setHeroLV(int lv)
{
	saveIntDataByKey(addUIDString("herolv"), lv);
}


int GameDataSave::getHeroExp()
{
	return loadIntDataByKey(addUIDString("heroexp"), 0);
}
void GameDataSave::setHeroExp(int val)
{
	saveIntDataByKey(addUIDString("heroexp"), val);
}

bool GameDataSave::getHeroIsOut()
{
	int isout = loadIntDataByKey(addUIDString("heroisout"), 0);
	return isout == 0?false:true;
}
void GameDataSave::setHeroIsOut(bool val)
{
	saveIntDataByKey(addUIDString("heroisout"), val == true ? 1 : 0);
}

std::string GameDataSave::getHeroProperData()
{
	return loadStringDataByKey(addUIDString("heroproper"));
}

std::string GameDataSave::getHeroProperData(std::string localid)
{
	std::string key = localid + "heroproper";
	return loadStringDataByKey(key);
}

void GameDataSave::setHeroProperData(std::string strval)
{
	saveStringDataByKey(addUIDString("heroproper"), strval);
}

std::string GameDataSave::getHeroUnlockData()
{
	return loadStringDataByKey("herounlock", "1-0-0-0");
}
void GameDataSave::setHeroUnlockData(std::string strval)
{
	saveStringDataByKey("herounlock", strval);
}

std::string GameDataSave::getUserId()
{
	return loadStringDataByKey("uid");
}

void GameDataSave::setUserId(std::string val)
{
	userid = val;
}

void GameDataSave::saveUserId(std::string val)
{
	setUserId(val);
	saveStringDataByKey("uid", val);
}

std::string GameDataSave::addUIDString(std::string val)
{
	return userid + val;
}

void GameDataSave::setPlotMissionIndex(int val)
{
	saveIntDataByKey(addUIDString("plot"), val);
}

int GameDataSave::getPlotMissionIndex()
{
	return loadIntDataByKey(addUIDString("plot"), 0);
}

void GameDataSave::setPlotMissionStatus(std::string strval)
{
	saveStringDataByKey(addUIDString("plotstatus"), strval);
}

std::string GameDataSave::getPlotMissionStatus()
{
	return loadStringDataByKey(addUIDString("plotstatus"));
}

void GameDataSave::setPlotUnlockChapter(int val)
{
	saveIntDataByKey(addUIDString("unlockchapter"), val);
}

int GameDataSave::getPlotUnlockChapter()
{
	return loadIntDataByKey(addUIDString("unlockchapter"), 1);
}

void GameDataSave::setBranchPlotMissionStatus(std::string strval)
{
	saveStringDataByKey(addUIDString("newbplotstatus"), strval);
}

std::string GameDataSave::getBranchPlotMissionStatus()
{
	return loadStringDataByKey(addUIDString("newbplotstatus"));
}

std::string GameDataSave::getSaveListId()
{
	return loadStringDataByKey("savelistid", ";;;");
}

void GameDataSave::setBranchPlotMissionGiveGoods(std::string strval)
{
	saveStringDataByKey(addUIDString("bmggs"), strval);
}

std::string GameDataSave::getBranchPlotMissionGiveGoods()
{
	return loadStringDataByKey(addUIDString("bmggs"), "");
}

void GameDataSave::setSaveListId(std::string val)
{
	saveStringDataByKey("savelistid", val);
}

bool GameDataSave::getIsNewerGuide(int index)
{
	std::string key = StringUtils::format("guide%d", index);
	int val = loadIntDataByKey(key, 1);
	return val == 1 ? true : false;
}

void GameDataSave::setIsNewerGuide(int index, bool val)
{
	std::string key = StringUtils::format("guide%d", index);
	saveIntDataByKey(key, (int)val);
}


void GameDataSave::setExersiceCfg(std::string strval)
{
	saveStringDataByKey(addUIDString("exsercise"), strval);
}

std::string GameDataSave::getExersiceCfg()
{
	return loadStringDataByKey(addUIDString("exsercise"), "");
}


void GameDataSave::setModifyDefaultStorage(int heroindex, std::string strval)
{
	std::string key = StringUtils::format("hero%ddefstorge", heroindex);
	saveStringDataByKey(key, strval);
}

std::string GameDataSave::getModifyDefaultStorage(int heroindex)
{
	std::string key = StringUtils::format("hero%ddefstorge", heroindex);
	return loadStringDataByKey(key, "");
}

void GameDataSave::setShareDayOfYear(int day)
{
	saveIntDataByKey("shareday", day);
}

int GameDataSave::getShareDayOfYear()
{
	return loadIntDataByKey("shareday", -1);
}

void GameDataSave::setWarmConfig(std::string strval)
{
	saveStringDataByKey(addUIDString("warmconfig"), strval);
}


std::string GameDataSave::getWarmConfig()
{
	return loadStringDataByKey(addUIDString("warmconfig"), "");
}

void GameDataSave::setHeroSex(int val)
{
	saveIntDataByKey(addUIDString("sex"), val);
}

int GameDataSave::getHeroSex()
{
	return loadIntDataByKey(addUIDString("sex"), -1);
}

void GameDataSave::setHeroExpEndTime(int val)
{
	saveIntDataByKey(addUIDString("heroexpetime"), val);
}

int GameDataSave::getHeroExpEndTime()
{
	return loadIntDataByKey(addUIDString("heroexpetime"), 0);
}

void GameDataSave::setGfEndTime(int val)
{
	saveIntDataByKey(addUIDString("gfexpetime"), val);
}

int GameDataSave::getGfEndTime()
{
	return loadIntDataByKey(addUIDString("gfexpetime"), 0);
}

std::string GameDataSave::getExgCfgData()
{
	return loadStringDataByKey(addUIDString("exgcfg"), "");
}

void GameDataSave::setExgCfgData(std::string strval)
{
	saveStringDataByKey(addUIDString("exgcfg"), strval);
}

void GameDataSave::setWxbMapPos(int val)
{
	saveIntDataByKey(addUIDString("wxbmap"), val);
}

int GameDataSave::getWxbMapPos()
{
	return loadIntDataByKey(addUIDString("wxbmap"), 0);
}

void GameDataSave::setDgqbMapPos(int val)
{
	saveIntDataByKey(addUIDString("dgqbmap"), val);
}

int GameDataSave::getDgqbMapPos()
{
	return loadIntDataByKey(addUIDString("dgqbmap"), 0);
}

void GameDataSave::setReviveCount(int val)
{
	saveIntDataByKey(addUIDString("revivecount"), val);
}

int GameDataSave::getReviveCount()
{
	return loadIntDataByKey(addUIDString("revivecount"), 1);
}

int GameDataSave::getGoldCount()
{
	return loadIntDataByKey("gold", 0);
}

void GameDataSave::setGoldCount(int count)
{
	saveIntDataByKey("gold", count);
}

bool GameDataSave::getIsPostAllData()
{
	int ret = loadIntDataByKey("postallok", 0);
	return ret == 1 ? true : false;
}

void GameDataSave::setIsPostAllData(bool val)
{
	saveIntDataByKey("postallok", val?1:0);
}

bool GameDataSave::getIsFirstInstall()
{
	int ret = loadIntDataByKey("firstintall", 1);
	return ret == 1 ? true : false;
}

void GameDataSave::setIsFirstInstall(bool val)
{
	saveIntDataByKey("firstintall", val ? 1 : 0);
}

std::string GameDataSave::getMyID()
{
	return loadStringDataByKey("id", "");
}

void GameDataSave::setMyID(std::string str)
{
	saveStringDataByKey("id", str);
}

std::string GameDataSave::getMyNickName()
{
	return loadStringDataByKey("nickname", "");
}

void GameDataSave::setMyNickName(std::string str)
{
	saveStringDataByKey("nickname", str);
}

bool GameDataSave::getIsBuyTimeGift()
{
	int ret = loadIntDataByKey("isbuytimegift", 0);
	return ret == 1 ? true : false;
}

void GameDataSave::setIsBuyTimeGift(bool val)
{
	saveIntDataByKey("isbuytimegift", val ? 1 : 0);
}

void GameDataSave::setUseGold(int val)
{
	saveIntDataByKey("costgold", val);
}

int GameDataSave::getUseGold()
{
	return loadIntDataByKey("costgold", 0);
}

std::string GameDataSave::getFriendly()
{
	return loadStringDataByKey(addUIDString("friendly"), "");
}

std::string GameDataSave::getFriendly(std::string localid)
{
	std::string key = localid + "friendly";
	return loadStringDataByKey(key, "");
}

void GameDataSave::setFriendly(std::string str)
{
	saveStringDataByKey(addUIDString("friendly"), str);
}

std::string GameDataSave::getMixGF()
{
	return loadStringDataByKey(addUIDString("mixgf"), "");
}

std::string GameDataSave::getMixGF(std::string localid)
{
	std::string key = localid + "mixgf";
	return loadStringDataByKey(key, "");
}

void GameDataSave::setMixGF(std::string str)
{
	saveStringDataByKey(addUIDString("mixgf"), str);
}

int GameDataSave::getFreeMixTime(int type)
{
	std::string typestr = StringUtils::format("freemixtime%d", type);
	return loadIntDataByKey(addUIDString(typestr), 0);
}

void GameDataSave::setFreeMixTime(int type, int val)
{
	std::string typestr = StringUtils::format("freemixtime%d", type);
	saveIntDataByKey(addUIDString(typestr), val);
}


int GameDataSave::getMixGfCountByType(int type)
{
	std::string typestr = StringUtils::format("mixcount%d", type);
	return loadIntDataByKey(addUIDString(typestr), 0);
}

void GameDataSave::setMixGfCountByType(int type, int val)
{
	std::string typestr = StringUtils::format("mixcount%d", type);
	saveIntDataByKey(addUIDString(typestr), val);
}

std::string GameDataSave::getSuggestMixGf()
{
	return loadStringDataByKey(addUIDString("suggestmixgf"), "");
}

void GameDataSave::setSuggestMixGf(std::string str)
{
	saveStringDataByKey(addUIDString("suggestmixgf"), str);
}

int GameDataSave::getNoPopNoticeDay()
{
	return loadIntDataByKey("nopopnoticeday", -1);
}

void GameDataSave::setNoPopNoticeDay(int day)
{
	saveIntDataByKey("nopopnoticeday", day);
}

int GameDataSave::getContributionDay()
{
	return loadIntDataByKey(addUIDString("contributeday"), -1);
}

void GameDataSave::setContributionDay(int day)
{
	saveIntDataByKey(addUIDString("contributeday"), day);
}


int GameDataSave::getSliverContribution()
{
	return loadIntDataByKey(addUIDString("slivercontribute"), 0);
}


void GameDataSave::setSliverContribution(int val)
{
	saveIntDataByKey(addUIDString("slivercontribute"), val);
}

int GameDataSave::getGoldContribution()
{
	return loadIntDataByKey(addUIDString("goldcontribute"), 0);
}

void GameDataSave::setGoldContribution(int val)
{
	saveIntDataByKey(addUIDString("goldcontribute"), val);
}

int GameDataSave::getApprenticeDay()
{
	return loadIntDataByKey(addUIDString("apprentice"), 0);
}
void GameDataSave::setApprenticeDay(int val)
{
	saveIntDataByKey(addUIDString("apprentice"), val);
}

std::string GameDataSave::getRaffleStage()
{
	return loadStringDataByKey("rafflestage", "");
}

void GameDataSave::setRaffleStage(std::string strval)
{
	saveStringDataByKey("rafflestage", strval);
}

std::string GameDataSave::getAchiveData()
{
	return loadStringDataByKey(addUIDString("achive"), "");
}

void GameDataSave::setAchiveData(std::string strval)
{
	saveStringDataByKey(addUIDString("achive"), strval);
}

std::string GameDataSave::getAchiveAnimData()
{
	return loadStringDataByKey(addUIDString("achiveanim"), "");
}

void GameDataSave::setAchiveAnimData(std::string strval)
{
	saveStringDataByKey(addUIDString("achiveanim"), strval);
}

void GameDataSave::setPlayerChallengeData(std::string strval)
{
	saveStringDataByKey(addUIDString("playerchallenge"), strval);
}

std::string GameDataSave::getPlayerChallengeData()
{
	return loadStringDataByKey(addUIDString("playerchallenge"), "");
}

void GameDataSave::setHsljMatchPlayer(std::string val)
{
	saveStringDataByKey("shljmatch", val);
}

std::string GameDataSave::getHsljMatchPlayer()
{
	return loadStringDataByKey("shljmatch", "");
}

int GameDataSave::getEnterGameDaysOfYear()
{
	return loadIntDataByKey(addUIDString("daysofyear"), -1);
}

void GameDataSave::setEnterGameDaysOfYear(int days)
{
	saveIntDataByKey(addUIDString("daysofyear"), days);
}