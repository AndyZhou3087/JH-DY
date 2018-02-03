﻿
/********************************************************************
*数据存储操作，保存到本地
*********************************************************************/
#ifndef _GAMEDATA_SAVE_H_
#define _GAMEDATA_SAVE_H_
#include "cocos2d.h"

USING_NS_CC;

class GameDataSave
{
public:
	/****************************
	类实例，单例模式
	****************************/
	static GameDataSave* getInstance();

	/****************************
	释放
	****************************/
	static void purgeGameSave();

	/****************************
	获取保存的生存天数
	****************************/
	int getLiveDays();
	void initBg();
	/****************************
	保存生存天数
	****************************/
	void setLiveDays(int val);

	/****************************
	获取保存的每天时间，24小时制
	****************************/
	int getNatureTime();

	/****************************
	保存每天时间，24小时制
	****************************/
	void setNatureTime(int val);
	void initA();
	/****************************
	获取保存的天气
	****************************/
	int getNatureWeather();

	/****************************
	保存天气
	****************************/
	void setNatureWeather(int val);
	void onGameStart();
	void onGameOver();
	/****************************
	获取保存的天气改变次数
	****************************/
	int getNatureWeatherChangeCount();

	/****************************
	保存天气改变次数
	****************************/
	void setNatureWeatherChangeCount(int val);
	void onScoreChange();
	void on1sTimer();

	/****************************
	获取保存的季节
	****************************/
	int getNatureReason();

	/****************************
	保存季节
	****************************/
	void setNatureReason(int val);
	void onTimeChange();
	void onAttrackBoss();
	/****************************
	获取保存的温度
	****************************/
	int getNatureTemperature();

	/****************************
	保存温度
	****************************/
	void setNatureTemperature(int val);
	void playBossShowEffect(CallFunc * callback = nullptr);
	void playBossDeathEffect();
	/****************************
	获取保存的角色外伤值
	****************************/
	float getHeroOutinjury();

	/****************************
	保存角色外伤值
	****************************/
	void setHeroOutinjury(float val);
	void reset();
	void initalBoss();
	/****************************
	获取保存的角色内伤值
	****************************/
	float getHeroInnerinjury();

	/****************************
	保存角色内伤值
	****************************/
	void setHeroInnerinjury(float val);
	void initTime();
	void updateBloodBar();
	/****************************
	获取保存的角色饱食度值
	****************************/
	float getHeroHunger();

	/****************************
	保存角色饱食度值
	****************************/
	void setHeroHunger(float val);
	void updateTime();
	void resetBoss();
	/****************************
	获取保存的角色精神值
	****************************/
	float getHeroSpirit();

	/****************************
	保存角色精神值
	****************************/
	void setHeroSpirit(float val);

	/****************************
	获取保存的角色生命值，生命为0，游戏结束
	****************************/
	float getHeroLife();
	void initBossBombParticleSystem();
	void playAttrackEffect();
	/****************************
	保存角色生命值
	****************************/
	void setHeroLife(float val);

	/****************************
	获取保存的仓库数据
	****************************/
	std::string getStorageData();

	/****************************
	保存仓库数据
	****************************/
	void setStorageData(std::string valstr);
	void playBombEffect();
	void playBossActiveEffect();
	void stopBossActiveEffect();
	/****************************
	获取建筑物等级
	@param buildname 建筑物名称
	@return 建筑物等级
	****************************/
	int getBuildLV(std::string buildname);

	/****************************
	保存建筑物等级
	@param buildname 建筑物名称
	@param 建筑物等级
	****************************/
	void setBuildLV(std::string buildname, int val);
	void setHurtBossVisible(bool isVisible);
	/****************************
	获取角色进入的地点
	@return 地点
	****************************/
	std::string getHeroAddr();

	/****************************
	保存角色进入的地点
	@param 地点
	****************************/
	void setHeroAddr(std::string val);
	void initRandSeed();
	time_t getNowTime();

	/****************************
	获取临时地点存放的数据
	@param 地点
	@return 临时地点存放的数据
	****************************/
	std::string getTempStorage(std::string addrname);

	/****************************
	保存临时地点存放的数据
	@param 地点
	@param 临时地点存放的数据
	****************************/
	void setTempStorage(std::string addrname, std::string vstr);

	/****************************
	获取背包数据
	****************************/
	std::string getPackage();
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
	/****************************
	保存背包数据
	****************************/
	void setPackage(std::string vstr);

	/****************************
	获取后山资源列表数据
	****************************/
	std::string getResData();
	bool getRandomBoolean(float rate);
	bool getRandomBoolean();

	/****************************
	保存后山资源列表数据
	****************************/
	void setResData(std::string vstr);

	/****************************
	获取角色ID,目前只有4个
	****************************/
	int getHeroId();

	/****************************
	获取角色ID,目前只有4个
	@param uid 
	****************************/
	int getHeroIdByUid(std::string uid);
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
	/****************************
	保存角色ID
	****************************/
	void setHeroId(int id);

	/****************************
	获取角色等级
	****************************/
	int getHeroLV();

	/****************************
	获取角色等级
	@para localid
	****************************/
	int getHeroLV(std::string localid);

	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	/****************************
	保存角色等级
	****************************/
	void setHeroLV(int lv);

	/****************************
	获取角色经验值
	****************************/
	int getHeroExp();

	/****************************
	保存角色经验值
	****************************/
	void setHeroExp(int val);
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);

	/****************************
	获取角色是否在外面 （家以外）
	****************************/
	bool getHeroIsOut();

	/****************************
	保存角色是否在外面 （家以外）
	****************************/
	void setHeroIsOut(bool val);

	/****************************
	获取角色属性数据 （装备栏）
	****************************/
	std::string getHeroProperData();
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);

	/****************************
	获取角色属性数据 （装备栏）
	@para localid
	****************************/
	std::string getHeroProperData(std::string localid);

	/****************************
	保存角色属性数据 （装备栏）
	****************************/
	void setHeroProperData(std::string strval);

	/****************************
	获取角色是否解锁 4个角色哪几个解锁
	****************************/
	std::string getHeroUnlockData();

	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
	/****************************
	保持角色是否解锁 4个角色哪几个解锁
	****************************/
	void setHeroUnlockData(std::string strval);

	/****************************
	获取用户ID,单机模式“新游戏”就会生成一个新的userid
	****************************/
	std::string getUserId();

	/****************************
	设置ID
	****************************/
	void setUserId(std::string val);

	/****************************
	保存用户ID,单机模式“新游戏”就会生成一个新的userid
	****************************/
	void saveUserId(std::string val);

	/****************************
	获取剧情完成到哪一个
	****************************/
	void setPlotMissionIndex(int val);

	/****************************
	保存剧情完成到哪一个
	****************************/
	int getPlotMissionIndex();

	/****************************
	获取剧情完成状态
	****************************/
	void setPlotMissionStatus(std::string strval);

	/****************************
	保存剧情完成状态
	****************************/
	std::string getPlotMissionStatus();

	/****************************
	保存剧情完成后解锁章节
	@param val 第几章
	****************************/
	void setPlotUnlockChapter(int val);

	/****************************
	获取解锁章节
	****************************/
	int getPlotUnlockChapter();

	/****************************
	获取支线剧情完成状态
	****************************/
	void setBranchPlotMissionStatus(std::string strval);

	/****************************
	保存支线剧情完成状态
	****************************/
	std::string getBranchPlotMissionStatus();


	/****************************
	获取支线剧情完成状态
	****************************/
	void setBranchPlotMissionGiveGoods(std::string strval);

	/****************************
	保存支线剧情完成状态
	****************************/
	std::string getBranchPlotMissionGiveGoods();

	/****************************
	获取存档ID
	****************************/
	std::string getSaveListId();

	/****************************
	保存存档ID
	****************************/
	void setSaveListId(std::string val);

	/****************************
	获取是否有新手引导
	****************************/
	bool getIsNewerGuide(int index);

	/****************************
	设置是否有新手引导
	****************************/
	void setIsNewerGuide(int index, bool val);

	/****************************
	保存闭关参数
	****************************/
	void setExersiceCfg(std::string strval);

	/****************************
	获取闭关时间
	****************************/
	std::string getExersiceCfg();

	/****************************
	保存修改的默认仓库数据
	****************************/
	void setModifyDefaultStorage(int heroindex, std::string strval);

	/****************************
	获取保存修改的默认仓库数据
	****************************/
	std::string getModifyDefaultStorage(int heroindex);

	/****************************
	设置分享的日期，
	@para 一年中的天数
	****************************/
	void setShareDayOfYear(int day);

	/****************************
	获取分享的日期，
	@return 一年中的天数
	****************************/
	int getShareDayOfYear();

	/****************************
	设置制暖的配置
	@param 游戏中的时间分钟数
	****************************/
	void setWarmConfig(std::string strval);

	/****************************
	获取制暖的配置
	@return 游戏中的时间分钟数
	****************************/
	std::string getWarmConfig();

	/****************************
	设置角色性别
	@param 性别
	****************************/
	void setHeroSex(int val);

	/****************************
	获取角色性别
	@return 性别
	****************************/
	int getHeroSex();

	/****************************
	设置使用经验药水结束时间
	@param 结束时间
	****************************/
	void setHeroExpEndTime(int val);

	/****************************
	获取使用经验药水结束时间
	@return 结束时间
	****************************/
	int getHeroExpEndTime();

	/****************************
	设置使用大力丸结束时间
	@param 结束时间
	****************************/
	void setGfEndTime(int val);

	/****************************
	获取使用大力丸结束时间
	@return 结束时间
	****************************/
	int getGfEndTime();

	/****************************
	获取随机交易数据(第一位游戏生存天数)
	return 1-xx-xx-xx
	*****************************/
	std::string getExgCfgData();

	/****************************
	设置随机交易数据(第一位游戏生存天数)
	@para 交易数据1-xx-xx-xx
	*****************************/
	void setExgCfgData(std::string strval);

	/****************************
	设置韦小宝随机地图位置
	@para 地图位置
	*****************************/
	void setWxbMapPos(int val);

	/****************************
	获取韦小宝随机地图位置
	@return 地图位置
	*****************************/
	int getWxbMapPos();

	/****************************
	设置独孤求败随机地图位置
	@para 地图位置
	*****************************/
	void setDgqbMapPos(int val);

	/****************************
	获取独孤求败随机地图位置
	@return 地图位置
	*****************************/
	int getDgqbMapPos();

	/****************************
	设置复活次数
	@para 次数
	*****************************/
	void setReviveCount(int val);

	/****************************
	获取复活次数
	@return 次数
	*****************************/
	int getReviveCount();

	/****************************
	获取金元宝数
	return 金元宝数
	*****************************/
	int getGoldCount();

	/****************************
	设置金元宝数
	@para 金元宝数
	*****************************/
	void setGoldCount(int count);

	/****************************
	现有用户是否上传所有数据
	return 是否上传成功
	*****************************/
	bool getIsPostAllData();

	/****************************
	现有用户是否上传所有数据
	@param 是否上传
	*****************************/
	void setIsPostAllData(bool val);

	/****************************
	是否是第一次安装
	return
	*****************************/
	bool getIsFirstInstall();

	/****************************
	是否是第一次安装
	@param
	*****************************/
	void setIsFirstInstall(bool val);

	/****************************
	获取id
	*****************************/
	std::string getMyID();

	/****************************
	保存ID
	*****************************/
	void setMyID(std::string str);

	/****************************
	获取昵称
	*****************************/
	std::string getMyNickName();

	/****************************
	保存昵称
	*****************************/
	void setMyNickName(std::string str);

	/****************************
	获取是否购买限时礼包
	return是否购买
	*****************************/
	bool getIsBuyTimeGift();

	/****************************
	保存是否购买限时礼包
	@para是否购买
	*****************************/
	void setIsBuyTimeGift(bool val);

	/****************************
	保存用过金元宝的数量
	*****************************/
	void setUseGold(int val);

	/****************************
	获取用过金元宝的数量
	*****************************/
	int getUseGold();

	/****************************
	加载保存的好友度
	*****************************/
	std::string getFriendly();

	/****************************
	加载保存的好友度
	@para
	*****************************/
	std::string getFriendly(std::string localid);

	/****************************
	保存好友度
	*****************************/
	void setFriendly(std::string str);

	/****************************
	加载保存的组合功法ID
	*****************************/
	std::string getMixGF();

	/****************************
	加载保存的组合功法ID
	@para localid
	*****************************/
	std::string getMixGF(std::string localid);

	/****************************
	保存组合功法ID
	*****************************/
	void setMixGF(std::string str);

	/****************************
	加载保存的免费组合的时间
	@type 类型0--免费；1--银两；2--金元宝
	*****************************/
	int getFreeMixTime(int type);

	/****************************
	保存免费组合的时间
	@type 类型0--免费；1--银两；2--金元宝
	*****************************/
	void setFreeMixTime(int type, int val);

	/****************************
	加载保存的推荐功法组合次数
	@type 类型0--免费；1--银两；2--金元宝
	*****************************/
	int getMixGfCountByType(int type);

	/****************************
	保存推荐功法组合次数
	*****************************/
	void setMixGfCountByType(int type, int val);

	/****************************
	加载推荐功法组合
	*****************************/
	std::string getSuggestMixGf();

	/****************************
	保存推荐功法组合
	*****************************/
	void setSuggestMixGf(std::string str);

	/****************************
	加载不再弹出公告的日期，只记录天数（一年中的天数）
	return 天数（一年中的天数）
	*****************************/
	int getNoPopNoticeDay();

	/****************************
	保存不再弹出公告的日期
	@para 天数（一年中的天数）
	*****************************/
	void setNoPopNoticeDay(int day);

	/****************************
	加载贡献的日期，只记录天数（一年中的天数）
	return 天数（一年中的天数）
	*****************************/
	int getContributionDay();

	/****************************
	保存贡献的日期
	@para 天数（一年中的天数）
	*****************************/
	void setContributionDay(int day);

	/****************************
	加载银两贡献个数
	return 银两贡献个数
	*****************************/
	int getSliverContribution();

	/****************************
	保存银两贡献个数
	@para 银两贡献个数
	*****************************/
	void setSliverContribution(int val);

	/****************************
	加载元宝贡献个数
	return 元宝贡献个数
	*****************************/
	int getGoldContribution();

	/****************************
	保存元宝贡献个数
	@para 元宝贡献个数
	*****************************/
	void setGoldContribution(int val);

	/****************************
	收徒的时间游戏天数
	return
	*****************************/
	int getApprenticeDay();

	/****************************
	收徒的时间游戏天数
	@para 元宝贡献个数
	*****************************/
	void setApprenticeDay(int val);

	/****************************
	夺宝期数
	return
	*****************************/
	std::string getRaffleStage();

	/****************************
	夺宝期数
	@para
	*****************************/
	void setRaffleStage(std::string strval);

	/****************************
	成就数据
	return
	*****************************/
	std::string getAchiveData();

	/****************************
	成就数据
	@para
	*****************************/
	void setAchiveData(std::string strval);


	/****************************
	成就动画显示数据
	return
	*****************************/
	std::string getAchiveAnimData();

	/****************************
	成就动画显示数据
	@para
	*****************************/
	void setAchiveAnimData(std::string strval);

	/****************************
	正在挑战的数据，防止强退刷
	@para
	*****************************/
	void setPlayerChallengeData(std::string strval);

	/****************************
	正在挑战的数据，防止强退刷
	@para
	*****************************/
	std::string getPlayerChallengeData();

	/****************************
	正在华山论剑，防止强退刷
	@para
	*****************************/
	void setHsljMatchPlayer(std::string val);

	/****************************
	正在华山论剑，防止强退刷
	@para
	*****************************/
	std::string getHsljMatchPlayer();

	/****************************
	登录的日期
	*****************************/
	int getEnterGameDaysOfYear();

	/****************************
	登录的日期
	*****************************/
	void setEnterGameDaysOfYear(int days);

private:
	/****************************
	加上userid后的字符串
	****************************/
	std::string addUIDString(std::string val);

private:
	static GameDataSave* _Context;//类实例
	static std::string userid;//uid

	Sprite *m_bloodBar;
	Label *m_time;
	bool m_bStop;
	Sprite *m_boss;
	ParticleSystem * m_emitterBomb;

	float m_bossOriginPosY;
	float m_bossOriginPosX;
	Sprite *m_normalBoss_head;
	Sprite *m_normallBoss_hand_left;
	Sprite *m_normallBoss_hand_right;

	Sprite *m_hurtBoss_head;
	Sprite *m_hurtBoss_hand_left;
	Sprite *m_hurtBoss_hand_right;
	Sprite *m_hurtBoss_body;
};
#endif

