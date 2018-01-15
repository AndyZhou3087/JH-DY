/****************************************************************
全局操作类

****************************************************************/
#ifndef _GLOBALDATA_H_
#define _GLOBALDATA_H_
#include "cocos2d.h"
USING_NS_CC;

/****************************
建筑物建筑，升级，操作 NODE数据结构
*****************************/
typedef struct
{
	char icon[20];//id
	std::vector<int> res;//需要资源
	std::vector<int> ep;//使用后的效果，eg:加生命，加精神
	int blv;//建筑物等级
	char actext[32];//NODE上按钮的文件
	int actime;//操作时间
	int extime;//影响游戏时间
	int type;//类型StorageType
	int extype;//工具时的标识1："采集", 2："砍伐", 3："挖掘"
	std::string cname;//中文显示
	std::string desc;//描述
}BuildActionData;


/****************************
所有资源数据结构
*****************************/
typedef struct
{
	std::string strid;//id
	std::string cname;//中文显示名
	std::string desc;//描述
	int val;//资源价值
	int fval;//好友价值
	std::vector<std::string> npc;//加成的npc;
	std::vector<float> npcval;//加成
}AllResource;

/****************************
一级资源数据结构
*****************************/
typedef struct
{
	std::string strid;//id
	int count;//产出剩下的个数
	int max;//产出最大个数
	std::vector<float> speed;//产出速度
	int type;//类型StorageType
	int actype;////工具时的标识1："采集", 2："砍伐", 3："挖掘"
	std::vector<int> ep; // 使用后的效果，eg:加生命，加精神
	std::vector<int> res;//合成需要的资源
	float pastmin;//过去的时间
	float waittime;//单个产出等待时间
	std::string unitname;//资源的单位 eg:一根木材

}ResData;

/****************************
地图地点数据结构
*****************************/
typedef struct
{
	char strid[10];//id
	std::vector<std::string> npcs;//npc id
	char cname[32];//地点显示名字
	char tpngname[10];//地点类型
	std::string desc;//地点描述
	bool isCliff;//是否遇到悬崖
}MapData;

/****************************
NPC数据结构
*****************************/
typedef struct
{
	char id[10];
	char name[32];
	int type;//保留字段
	int life;//生命值
	int atk;//攻击
	int exp;//经验值
	int df;//防御
	int lv;//npc等级
	float crit;//暴击率
	float dodge;//闪避率
	int escapernd;//逃跑成功概率
	std::vector<std::string> winres;//打败npc赢得奖励
	std::vector<int> winresrnd;//打败npc出奖励的概率
	std::vector<int> winrescount;//打多少次随机
	std::vector<std::string> exchgres;//兑换的资源
	std::vector<std::string> words;//npc 对话
}NpcData;

/****************************
角色数据结构
*****************************/
typedef struct
{
	int id;
	char name[32];
	std::vector<int> vec_atk;//攻击
	std::vector<int> vec_df;//防御
	std::vector<int> vec_exp;//经验值
	std::vector<int> vec_maxhp;//最大生命值
	std::vector<float> vec_crit;//暴击概率
	std::vector<float> vec_dodge;//闪避概率
}HeroAtrData;

/****************************
功法数据结构
*****************************/
typedef struct
{
	char id[32];
	int maxlv;
	std::vector<int> vec_bns;//外功--加攻或者内功--加防
	std::vector<int> vec_exp;
	std::vector<float> vec_cirt;
	std::vector<float> vec_dodge;
	std::vector<float> vec_skrnd;//释放技能随机数
	int skilltype;//技能类型
	int skilleffect;//技能影响
	int type;//类型StorageType
	int qu;//品级
	int extype;//1：棍 2：刀 3：剑
}WG_NGData;

/****************************
武器防具数据结构
*****************************/
typedef struct
{
	std::string id;
	int type;//类型StorageType
	int atk;
	int df;
	int extype;//扩展类型，区分棍，刀，剑
	int qu;//品级
	std::vector<std::string> repair_res;//修理需要的资源
}EquipData;

/****************************
所有资源类型
*****************************/
typedef enum
{
	FOOD = 0,//食物
	MEDICINAL,//药材
	WEAPON,//武器
	PROTECT_EQU,//防具
	N_GONG,//内功
	W_GONG,//外功
	RES_1,//一级资源
	TOOLS,//工具
	RES_2,//其他
	RES_MAX

}StorageType;

/****************************
剧情状态
*****************************/
typedef enum
{
	M_NONE = 0,//未开始
	M_DOING,//正在进行中
	M_DONE//完成
}MissionStatus;

/****************************
招式类型
*****************************/
typedef enum
{
	S_SNONE = 0,
	S_SKILL_1,//血流成河
	S_SKILL_2,//动弹不得
	S_SKILL_3,//乘胜追击
	S_SKILL_4,//致命打击
	S_SKILL_5,//香风毒雾
	S_SKILL_6,//铜墙铁壁
	S_SKILL_7,//敲骨吸髓
	S_SKILL_8//浮云惊龙
}SkillType;

/****************************
剧情数据
*****************************/
typedef struct
{
	std::string id;//ID
	std::string snpc;//发剧情的NPCID
	std::string dnpc;//需要挑战的NPCID
	std::vector<std::string> words;//发剧情NPC对话
	std::vector<std::string> mywords;//角色对话
	std::vector<std::string> bossword;//找到挑战NPC对话
	std::vector<std::string> rewords;//完成后的奖励
	std::vector<std::string> needgoods;//送的物品
	int unlockchapter;//完成后解锁的章节
	int status;//状态
	int type;//对话0；需要战斗1;2--送物品
	std::string mapid;//dnpc所在地图
}PlotMissionData;

/****************************
支线剧情Key
*****************************/
typedef struct
{
	std::string mid;//发剧情的ID
	int time;//限制时间
	int count;//限制次数
	int subindex;//
	int maxcount;
	int maxtime;
}BranchPlotMissionItem;

/****************************
招式招数数据
*****************************/
typedef struct
{
	std::string id;//ID
	std::vector<std::string> snames;//招式名称
}GFTrickData;

/****************************
技能数据
*****************************/
typedef struct
{
	std::string id;//ID
	std::string name;//
	std::string desc;
	std::string desc1;//书架描述
	int leftval;
	int fightPlayerleftval;
}GFSkillData;

/****************************
商城物品数据结构
*****************************/
typedef struct
{
	std::string icon;//显示ICON
	std::string name;//显示名称
	int price;//价格
	int type;//类型1-元宝购买，0-人民币购买
	std::string desc;//描述
	std::vector<std::string> vec_res;//购买成功后获得的物品（礼包类）
}GoodsData;


/****************************
游戏状态
*****************************/
typedef enum
{
	GAMESTART = 0,
	GAMEPAUSE,
	GAMEOVER
}GameStatus;

/****************************
武道大会奖励数据
*****************************/
typedef struct
{
	std::vector<std::string> vec_winres;
	std::vector<float> vec_winrnd;
}ChallengeRewardData;

/****************************
好友度数据
*****************************/
typedef struct _FriendlyData
{
	int friendly;
	int relation;
	_FriendlyData()
	{
		friendly = 0;
		relation = 0;
	}
}FriendlyData;

/****************************
好友关系
*****************************/
typedef enum
{
	F_NOMAR = 0,
	F_FRIEND,
	F_MASTER,
	F_MASTEROUT
}FriendType;

/****************************
NPC好友数据
*****************************/
typedef struct
{
	int maxfriendly;
	int needfriendly;
	std::vector<std::string> vec_enemynpc;
	std::vector<std::string> vec_fightnpc;
	float atkpercent;
	float dfpercent;
	float dodgepercent;
	float critpercent;
	float hppercent;

	std::vector<int> vec_giveres;
	std::vector<int> vec_askres;
}NPCFriendData;

/****************************
NPC师徒数据
*****************************/
typedef struct
{
	int needfriendly;
	float atkpercent;
	float dfpercent;
	float dodgepercent;
	float critpercent;
	float hppercent;
	std::vector<std::string> vec_gfid;
	std::vector<float> vec_gfbonus;
}NPCMasterData;

/****************************
组合功法数据
*****************************/
typedef struct
{
	std::string id;
	std::string name;
	std::string mastergf;
	float atkpercent;
	float dfpercent;
	float dodgepercent;
	float critpercent;
	float hppercent;
	int lv;
	int sex;//0--无，1--男，2-女，3-男女，4-all
	std::vector<std::string> vec_secgf;
	std::vector<std::string> vec_mutexgf;
}MixGfData;

/****************************
排行榜数据
*****************************/
typedef struct
{
	int rank;
	std::string nickname;
	std::string playerid;
	int herotype;
	int herolv;
	int herosex;
	int heroval;
}RankData;

/****************************
帮派列表数据
*****************************/
typedef struct
{
	int id;
	int rank;
	std::string factionname;
	int membercount;
	int maxcount;
	int lvlimit;
	int sexlimit;
	int exp;
	int lv;
	std::string owner;
	std::string desc;
	int action;
}FactionListData;

/****************************
帮派成员列表
*****************************/
typedef struct
{
	int id;
	int userid;
	int herolv;
	int herotype;
	std::string nickname;
	int factionid;
	int contribution;
	int position;
}FactionMemberData;

typedef enum
{
	F_NONE,
	F_CREATE,
	F_GETLIST,
	F_LEAVE,
	F_RELEASE,
	F_CONTRIB,//贡献值
	F_REFUSE
}FactionAction;

typedef struct
{
	int leftcount;//剩余次数
	int nextcostgold;//下次需要的元宝
	int wingold;//中奖元宝
	bool isshow;//是否显示
}MyLotteryData;

typedef struct
{
	int leftime;//剩余分钟
	bool iscanplay;//是否可以再投
	int poolgold;//奖池金额
	std::vector<std::string> vec_nicknames;
	std::vector<int> vec_wingold;
	int mywingold;
	int mywinrank;
	std::string mywinstage;
	bool isshow;//是否显示
	std::string curstage;
}RaffleData;


typedef enum
{
	A_0,//消耗%s元宝
	A_1,//拥有%s元宝
	A_2,//角色等级达到%s
	A_3,//拥有银两%s
	A_4,//战胜%sNPC
	A_5,//解锁%s章节
	A_6,//完成%s任务
	A_7,//%s武功达到%s等级
	A_8,//合成%s武功组合
	A_9,//%s强化到%s等级
	A_10,//%s等级以上的武功拥有%s个
	A_11,//生存天数达到%s天
	A_12//累计挑战%s%s次
}AchiveType;

typedef struct
{
	std::string id;
	std::string name;
	int type;
	std::vector<std::string> vec_para;
	std::vector<std::string> vec_rwd;
	std::string desc;
	int finish;
	int isshowanim;
}AchiveData;

typedef struct
{
	std::string starttime;
	std::string endtime;
	int mywincount;
	int myfailcount;
	int myexp;
	int matchno;
	int leftcount;
	int finishedcount;
	std::map<std::string, FriendlyData> map_myfriendly;
	std::map<std::string, int> map_EquipData;
	std::vector<int> vec_factionlv;
	int beforerank;
	int afterrank;
	int matchaward;
}MyMatchInfoData;

typedef struct
{
	std::string mixgf;
	int type;
	std::map<std::string, int> map_playerData;
	std::map<std::string, FriendlyData> map_playerfriendly;
	int herolv;
	int factionlv;
}MatchPlayerData;

typedef struct
{
	std::string nickname;
	std::string playerid;
	int exp;
	int wincount;
	int failcount;
}MatchPlayerInfo;

/****************************
华山论剑排行榜数据
*****************************/
typedef struct
{
	int rank;
	std::string nickname;
	int wincount;
	int totalcount;
	int exp;
}HSLJRankData;

class GlobalData
{
public:
	GlobalData();
	~GlobalData();
	static void init();
public:
	/****************************
	解析建筑物数据
	@param 建筑物名称
	*****************************/
	static void loadBuildActionJSon();

	/****************************
	解析所有资源详细描述数据
	*****************************/
	static void loadAllResourceJsonData();

	/****************************
	解析一级资源数据
	*****************************/
	static void loadResJsonData();

	/****************************
	保存一级资源数据
	*****************************/
	static void saveResData();

	/****************************
	读取保存的一级资源数据
	*****************************/
	static void loadResData();

	/****************************
	解析后山资源数据
	*****************************/
	static void loadHillResJsonData();

	/****************************
	解析地图数据
	*****************************/
	static void loadMapJsonData();

	/****************************
	解析NPC数据
	*****************************/
	static void loadNpcJsonData();

	/****************************
	解析角色数据
	*****************************/
	static void loadHeroAtrJsonData();

	/****************************
	解析功法数据
	*****************************/
	static void loadWG_NGJsonData();

	/****************************
	解析武器防具数据
	*****************************/
	static void loadEquipJsonData();

	/****************************
	解析成就数据
	*****************************/
	static void loadAchiveJsonData();

	/****************************
	解析商城数据
	*****************************/
	static void loadShopData();

	/****************************
	读取保存的解锁角色数据
	*****************************/
	static void loadUnlockHeroData();

	/****************************
	保存解锁角色数据
	*****************************/
	static void setUnlockHero(int index, bool val);

	/****************************
	获取角色是否解锁
	*****************************/
	static bool getUnlocHero(int index);

	/****************************
	系统时间，秒数
	*****************************/
	static int getSysSecTime();

	/****************************
	获取到明天的时间差
	*****************************/
	static int getTomorrowZeroTimeDif();

	/****************************
	一年中的天数
	@return 一年中的天数(0-365)
	*****************************/
	static int getDayOfYear();

	/****************************
	当前月的天数
	@return 当前月的天数
	*****************************/
	static int getMonth_Days();

	/****************************
	获取userid
	*****************************/
	static std::string getUId();

	/****************************
	设置userid
	*****************************/
	static void setUId(std::string struid);

	/****************************
	获取默认仓储数据
	@param heroindex 角色ID
	*****************************/
	static std::string getDefaultStorage(int heroindex);

	/****************************
	解析原始默认仓储数据
	@param heroindex 角色ID
	*****************************/
	static std::string getOriginLocalStorage(int heroindex);

	/****************************
	设置剧情ID
	@param 剧情ID
	*****************************/
	static void setPlotMissionIndex(int val);

	/****************************
	获取剧情ID
	*****************************/
	static int getPlotMissionIndex();

	/****************************
	解析剧情ID
	*****************************/
	static void loadPlotMissionJsonData(int herotype);

	/****************************
	保存剧情状态
	*****************************/
	static void savePlotMissionStatus();

	/****************************
	更新剧情状态
	*****************************/
	static void updatePlotMissionStatus();

	/****************************
	解析支线剧情ID
	*****************************/
	static void loadBranchPlotMissionJsonData();

	/****************************
	保存支线剧情状态
	*****************************/
	static void saveBranchPlotMissionStatus(std::string mid, int status);

	/****************************
	更新支线剧情状态
	*****************************/
	static void updateBranchPlotMissionStatus();

	/****************************
	获取当前支线任务的ID
	*****************************/
	static std::string getCurBranchPlotMissison();

	/****************************
	是否有正在进行任务
	*****************************/
	static bool isDoingBranchPlotMisson();

	/****************************
	获取解锁的章节
	*****************************/
	static int getUnlockChapter();

	/****************************
	设置解锁的章节
	@param 解锁的章节
	*****************************/
	static void setUnlockChapter(int val);

	/****************************
	产生一个随机数
	@param 随机数范围，从0开始
	*****************************/
	static int createRandomNum(int val);

	/****************************
	获取存档的ID
	*****************************/
	static std::vector<std::string> getSaveListId();

	/****************************
	保存存档的ID
	@param 保存的id vector
	*****************************/
	static void setSaveListId(std::vector<std::string> vec_val);

	/****************************
	保存当前的角色ID到进度列表
	*****************************/
	static void setCurHeroIdToSaveList();

	/****************************
	是否在闭关中
	*****************************/
	static bool isExercising();

	/****************************
	是否有分身符
	*****************************/
	static bool isHasFSF();

	/****************************
	解析招式数据
	*****************************/
	static void loadGftrickData();

	/****************************
	解析技能数据
	*****************************/
	static void loadGfskillData();

	/****************************
	临时存放地是否有功法，装备
	@return 临时存放点的地图ID,没有返回空字符串
	*****************************/
	static std::string tempHasGf_Equip(std::string strid);

	/****************************
	获取分享的日期，只记录天数（一年中的天数）
	return 天数（一年中的天数）
	*****************************/
	static int getShareDay();

	/****************************
	设置分享的日期
	@para 天数（一年中的天数）
	*****************************/
	static void setShareDay(int day);

	/****************************
	获取制暖的参数(开始制暖游戏分钟数，制暖时间)
	*****************************/
	static std::string getMakeWarmConfig();

	/****************************
	设置制暖的参数(开始制暖游戏分钟数，制暖时间)
	*****************************/
	static void setMakeWarmConfig(std::string strval);

	/****************************
	获取随机交易数据(第一位游戏生存天数)
	return 1-xx-xx-xx
	*****************************/
	static std::string getExgCfgData();

	/****************************
	设置随机交易数据(第一位游戏生存天数)
	@para 交易数据1-xx-xx-xx
	*****************************/
	static void setExgCfgData(std::string strval);

	/****************************
	设置韦小宝随机地图位置
	@para 地图位置3个客栈随机
	*****************************/
	static void setWxbMapPos(int pos);

	/****************************
	获取韦小宝随机地图位置
	@return 地图位置
	*****************************/
	static int getWxbMapPos();

	/****************************
	设置独孤求败随机地图位置
	@para 地图位置
	*****************************/
	static void setDgqbMapPos(int pos);

	/****************************
	获取独孤求败随机地图位置
	@return 地图位置
	*****************************/
	static int getDgqbMapPos();

	/****************************
	设置复活次数
	@para 次数
	*****************************/
	static void setReviveCount(int val);

	/****************************
	获取复活次数
	@return 次数
	*****************************/
	static int getReviveCount();

	/****************************
	获取金元宝数
	return 金元宝数
	*****************************/
	static int getMyGoldCount();

	/****************************
	设置金元宝数
	@para 金元宝数
	*****************************/
	static void setMyGoldCount(int count);

	/****************************
	获取资源类型
	@return 类型
	*****************************/
	static int getResType(std::string strid);


	/****************************
	获取资源扩展类型
	@return 类型
	*****************************/
	static int getResExType(std::string strid);

	/****************************
	获取武道大会奖励数据
	*****************************/
	static void loadChallengeRewardData();

	/****************************
	获取设备唯一ID
	*****************************/
	static std::string UUID();

	/****************************
	获取id
	*****************************/
	static std::string getMyID();

	/****************************
	设置ID
	*****************************/
	static void setMyID(std::string str);

	/****************************
	获取昵称
	*****************************/
	static std::string getMyNickName();

	/****************************
	设置昵称
	*****************************/
	static void setMyNickName(std::string str);

	/****************************
	是否有神雕
	*****************************/
	static bool isHasVulture();

	/****************************
	获取限时礼包剩余时间
	*****************************/
	static int getTimeGiftLeftTime();

	/****************************
	设置限时礼包剩余时间
	*****************************/
	static void setTimeGiftLeftTime(int val);

	/****************************
	设置是否买限时礼包
	*****************************/
	static void setIsBuyTimeGift(bool val);

	/****************************
	获取是否买限时礼包
	*****************************/
	static bool getIsBuyTimeGift();

	/****************************
	设置免费复活次数
	*****************************/
	static void setFreeReviveCount(int val);

	/****************************
	获取免费复活次数
	*****************************/
	static int getFreeReviveCount();

	/****************************
	设置用过金元宝的数量
	*****************************/
	static void setUseGold(int val);

	/****************************
	获取用过金元宝的数量
	*****************************/
	static int getUseGold();

	/****************************
	获取临时存放的功法武器
	*****************************/
	static void loadTempGF_EquipData();

	/****************************
	获取角色装备栏数据
	*****************************/
	static std::string getHeroProperData();

	/****************************
	设置角色装备栏数据
	*****************************/
	static void setHeroProperData(std::string strval);

	/****************************
	防作弊数据start
	*****************************/
	static std::string getMD5MyGoldCount();
	static void setMD5MyGoldCount(std::string val);
	static std::string getMD5CostGlodCount();
	static void setMD5CostGlodCount(std::string val);
	static std::string getMD5FreeReviveCount();
	static void setMD5FreeReviveCount(std::string val);
	static std::string getMD5HeroLv();
	static void setMD5HeroLv(std::string val);
	static std::string getMD5LiveDays();
	static void setMD5LiveDays(std::string val);

	/****************************
	防作弊数据end
	*****************************/

	/****************************
	加载保存的好友度
	*****************************/
	static void loadFriendly();

	/****************************
	保存好友度
	*****************************/
	static void saveFriendly();

	/****************************
	解析NPC好友数据
	*****************************/
	static void loadNpcFriendJsonData();

	/****************************
	解析NPC师徒数据
	*****************************/
	static void loadNpcMasterJsonData();

	/****************************
	解析功法组合
	*****************************/
	static void loadMixGfJsonData();

	/****************************
	获取保存的组合功法ID
	*****************************/
	static std::string getMixGF();

	/****************************
	设置组合功法ID
	*****************************/
	static void setMixGF(std::string str);

	static std::string getVersion();

	static std::string getPackageName();

	/****************************
	获取不再弹出公告的日期，只记录天数（一年中的天数）
	return 天数（一年中的天数）
	*****************************/
	static int getNoPopNoticeDay();

	/****************************
	设置不再弹出公告的日期
	@para 天数（一年中的天数）
	*****************************/
	static void setNoPopNoticeDay(int day);

	/****************************
	获取保存的成就信息
	*****************************/
	static void getAchiveData();

	/****************************
	保存成就信息
	*****************************/
	static void saveAchiveData();

	/****************************
	获取保存的成就动画显示信息
	*****************************/
	static void getAchiveAnimData();

	/****************************
	保存成就动画显示信息
	*****************************/
	static void saveAchiveAnimData();

	/****************************
	成就信息
	*****************************/
	static void doAchive(int atype, int count);

	/****************************
	完成成就需要的次数
	*****************************/
	static int getAchiveFinishCount(AchiveData adata);

	/****************************
	复制内容到剪切板
	*****************************/
	static void copyToClipBoard(std::string text);

    static void setNoAds(bool val);
    static bool getNoAds();
    
    static void setHasBuy(bool val);
    static bool getHasBuy();
    
    static void setDailyAdsCount(int val);
    static int getDailyAdsCount();
    
    static void setDailyAdsDay(int val);
    static int getDailyAdsDay();
    static void setNoComments(bool val);
    static bool getNoComments();
    
    static void setAdsInterval(int val);
    static int getAdsInterval();
    static void setAdsChoose(long val);
    static int getAdsChoose(int position);

public:
	static std::map<std::string, AllResource> map_allResource;//所有资源，主要是描述和交易数据
	static std::vector<ResData> vec_resData;//一级资源vector
	static std::vector<std::string> vec_hillResid;//后山资源列表vector
	static std::map<std::string, MapData> map_maps;//地图数据
	static std::map<std::string, NpcData> map_npcs;//npc 数据
	static std::map<int, HeroAtrData> map_heroAtr;//角色属性装备栏数据
	static std::map<std::string, WG_NGData> map_wgngs;//功法数据
	static std::map<std::string, EquipData> map_equips;//武器防具数据
	static std::map<std::string, std::vector<BuildActionData>> map_buidACData;//建筑物数据
	static std::vector<PlotMissionData> vec_PlotMissionData;//剧情数据
	static std::map<std::string, std::vector<PlotMissionData>> map_BranchPlotMissionData;//支线剧情数据
	static std::map<std::string, BranchPlotMissionItem> map_BranchPlotMissionItem;//支线剧情数据

	static std::map<std::string, GFTrickData> map_gftricks;//招式数据
	static std::map<int, GFSkillData> map_gfskills;//技能数据
	static std::vector<GoodsData> vec_goods;
	static std::map<std::string, FriendlyData> map_myfriendly;//好友数据

	static std::vector<std::string> vec_buyVipIds;
	static std::map<std::string,int> map_buyVipDays;

	static std::map<std::string,std::vector<std::string>> map_tempGf_Equip;

	static std::map<std::string, ChallengeRewardData> map_challengeReward;

	static std::map<std::string, NPCFriendData> map_NPCFriendData;

	static std::map<std::string, NPCMasterData> map_NPCMasterData;

	static std::map<std::string, MixGfData> map_MixGfData;

	static std::vector<RankData> vec_rankData;

	static std::vector<FactionListData> vec_factionListData;

	static std::vector<FactionMemberData> vec_factionMemberData;
	static int factionExp;
	
	static std::vector<AchiveData> vec_achiveData;

	static std::vector<std::string> vec_showAchiveNames;
	//挑战对手装备数据
	static std::map<std::string, int> map_fighterPlayerData;

	//挑战对手好友数据
	static std::map<std::string, FriendlyData> map_fightPlayerfriendly;

	static int myLastHuafeiRank;

	static std::string fightPlayerMixgf;

	static int fightPlayerFactionLv;

	static bool isPopingScene;

	static GameStatus g_gameStatus;
	static bool dataIsModified;
	static bool ispunishment;
	static bool isFightMaster;
	static int servertime;
	static int myrank;
	static std::string noticecontent;
	static int myFaction;
	static int mytitle;
	static int myFactionlv;
	static MyLotteryData myLotteryData;

	static RaffleData myRaffleData;
	static int continueLoginDays;
	static bool isFrozen;
	static int myFihgtCount;
	static int myTotalFihgtCount;
	static int myFihgtexp;

	static bool isPlayerChallenging;

	static bool isExchangeGift;
	static std::string couponinfo;

	static MyMatchInfoData myMatchInfo;

	static MatchPlayerInfo matchPlayerInfo;
	static std::vector<MatchPlayerData> vec_matchPlayerData;

	static std::vector<HSLJRankData> vec_hsljRankData;

	static std::string updateDownLoadURL;
	static bool isPopUpdate;

	static std::string curBranchMissionID;
	static std::vector<std::string> vec_qq;
private:
	static bool unlockhero[4];//角色解锁
	static std::string uid;//
	static int myGlodCount;
	static int wxbmapos;
	static int dgqbmapos;

	static std::string MD5MyGoldCount;
	static std::string MD5CostGlodCount;
	static std::string MD5FreeReviveCount;
	static std::string MD5HeroLv;
	static std::string MD5LiveDays;

	static std::vector<std::string> vec_saveids;//存档的id
    
    static bool hasBuy;
    static int adsinterval;
    static long adschoose;
	static int timegiftlefttime;
	static bool isBuyTimeGift;
	static int freeReviveCount;
    static std::string addUidString(std::string val);
};
#endif

