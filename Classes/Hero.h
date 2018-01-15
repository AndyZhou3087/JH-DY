
/********************************************************************
*角色
*********************************************************************/
#ifndef _HERO_H_
#define _HERO_H_
#include "cocos2d.h"
#include "MyPackage.h"
#include "StorageRoom.h"
#include "GlobalData.h"
USING_NS_CC;
/****************************
角色性别
*****************************/
typedef enum
{
	S_NONE = 0,
	S_MAN,
	S_WOMEN
}H_SEX;

/****************************
角色属性（装备栏）类型，
*****************************/
typedef enum
{
	H_WEAPON = 0,//武器框
	H_GATHER,//采取框
	H_FELL,//砍伐框
	H_EXCAVATE,//挖掘框
	H_WG,//外功框
	H_NG,//内功框
	H_ARMOR,//防具框
	H_MOUNT,//坐骑框
	H_MAX
}HeroAtrType;

class Hero:public Node
{
public:
	Hero();
	~Hero();
	virtual bool init();

	CREATE_FUNC(Hero);

	/****************************
	角色头像设置
	*****************************/
	CC_SYNTHESIZE(int, m_pic, HeadImg);

	/****************************
	角色内伤设置
	*****************************/
	CC_SYNTHESIZE(float, m_innerinjury, InnerinjuryValue);
	/****************************
	角色外伤设置
	*****************************/
	CC_SYNTHESIZE(float, m_outinjury, OutinjuryValue);

	/****************************
	角色饱食度设置
	*****************************/
	CC_SYNTHESIZE(float, m_hunger, HungerValue);

	/****************************
	角色精神值设置
	*****************************/
	CC_SYNTHESIZE(float, m_spirit, SpiritValue);

	/****************************
	角色最大内伤设置
	*****************************/
	CC_SYNTHESIZE(float, m_maxInnerinjury, MaxInnerinjuryValue);
	/****************************
	角色最大外伤设置
	*****************************/
	CC_SYNTHESIZE(float, m_maxOutinjury, MaxOutinjuryValue);

	/****************************
	角色最大饱食度设置
	*****************************/
	CC_SYNTHESIZE(float, m_maxHunger, MaxHungerValue);

	/****************************
	角色最大精神值设置
	*****************************/
	CC_SYNTHESIZE(float, m_maxSpirit, MaxSpiritValue);

	/****************************
	角色生命值设置
	*****************************/
	CC_SYNTHESIZE(float, m_life, LifeValue);

	/****************************
	角色经验值设置
	*****************************/
	CC_SYNTHESIZE(int, m_exp, ExpValue);

	/****************************
	角色等级设置
	*****************************/
	CC_SYNTHESIZE(int, m_lv, LVValue);

	/****************************
	角色名称设置
	*****************************/
	CC_SYNTHESIZE(std::string, m_name, MyName);

	/****************************
	角色头像设置
	*****************************/
	CC_SYNTHESIZE(int, m_hid, HeadID);

	/****************************
	角色是否在家
	*****************************/
	CC_SYNTHESIZE(bool, m_isout, IsOut);

	/****************************
	角色攻击百分比
	*****************************/
	CC_SYNTHESIZE(float, m_atkpercent, AtkPercent);

	/****************************
	角色防御百分比
	*****************************/
	CC_SYNTHESIZE(float, m_dfpercent, DfPercent);

	/****************************
	角色性别
	*****************************/
	CC_SYNTHESIZE(H_SEX, m_sex, Sex);

	/****************************
	总攻击加成百分比
	*****************************/
	CC_SYNTHESIZE(float, m_totalAtkBonusPercent, TotalAtkBonusPercent);

	/****************************
	总防御加成百分比
	*****************************/
	CC_SYNTHESIZE(float, m_totalDfBonusPercent, TotalDfBonusPercent);

	/****************************
	是否武道大会挑战
	*****************************/
	CC_SYNTHESIZE(bool, m_isWDChallenge, IsWDChallenge);

	/****************************
	是否在移动
	*****************************/
	CC_SYNTHESIZE(bool, m_isMoving, IsMoving);

	/****************************
	获取角色攻击
	*****************************/
	int getAtkValue();

	/****************************
	获取角色防护
	*****************************/
	int getDfValue();

	/****************************
	获取角色最大生命值
	*****************************/
	float getMaxLifeValue();

	/****************************
	获取角色当前最大生命值，过度饥饿，内伤外伤严重，精神过低会影响生命恢复上线
	*****************************/
	float getRecoverLifeMaxValue();

	/****************************
	设置角色装备
	@param （装备栏）类型
	@param 装备
	*****************************/
	void setAtrByType(HeroAtrType type, PackageData pData);

	/****************************
	获取角色装备
	@param （装备栏）类型
	@return 装备
	*****************************/
	PackageData* getAtrByType(HeroAtrType type);

	/****************************
	睡觉
	@param 消耗时间
	@param 睡觉时间 （小时）
	*****************************/
	void sleep(int losttime, int hour);

	/****************************
	睡觉完成，定时器控制
	*****************************/
	void sleepDone(float dt);

	/****************************
	喝酒
	*****************************/
	void drinking();

	/****************************
	复活
	*****************************/
	void revive();

	/****************************
	是否有功法，武器，防具，有了不重复
	根据功法ID查找是否已经拥有
	@param 功法ID
	*****************************/
	bool checkifHasGF_Equip(std::string gfeid);

	/****************************
	根据功法ID获取拥有的功法，武器，防具
	@param 功法ID
	*****************************/
	PackageData* getGF_Equip(std::string gfeid);

	/****************************
	是否有物品，
	根据ID查找是否已经拥有
	@param ID
	*****************************/
	PackageData* getMeHas(std::string strid);

	/****************************
	检查最大值属性
	*****************************/
	void checkMaxVaule(float dt);

	/****************************
	恢复外伤值
	@param 恢复的数值
	*****************************/
	void recoverOutjury(int val);

	/****************************
	恢复内伤值
	@param 恢复的数值
	*****************************/
	void recoverInjury(int val);

	/****************************
	恢复饥饿值
	@param 恢复的数值
	*****************************/
	void recoverHunger(int val);

	/****************************
	总的防御
	*****************************/
	int getTotalDf();

	/****************************
	总的攻击
	*****************************/
	int getTotalAtck();

	/****************************
	暴击率
	*****************************/
	float getCritRate();

	/****************************
	闪避率
	*****************************/
	float getdodgeRate();

	/****************************
	保存装备栏数据
	*****************************/
	void saveProperData();

	/****************************
	获取功法多少等级个数
	@return 个数
	*****************************/
	int getGfCountByLv(int lv);

public:
	static float MAXInnerinjuryValue;//最大内伤值
	static float MAXOutinjuryValue;//最大外伤值
	static float MAXHungerValue;//最大饱食度
	static float MAXSpiritValue;//最大精神值
	std::map<HeroAtrType, PackageData> map_heroAtr;//角色装备栏数据
private:
	void updateData(float dt);//定时更新数据
	void sleepbystep(float dt);//睡觉--恢复值一点点恢复
	void drinkbystep(float dt);//喝酒--恢复值一点点恢复
private:
	int m_pastmin;//游戏时间，分钟为单位
	int sleephour;//睡觉时间
	float maxlifepercent;//最大生命百分比
	float liferecoverpercent;//最大生命恢复百分比
	float injuryrecoverpercent;//内伤恢复百分比
	float outjuryrecoverpercent;//外伤恢复百分比
	float hungerrecoverpercent;//饥饿恢复百分比
	float sleepLostPercent;//睡觉时饥饿与精神的降低速度为正常时的一半
};
static std::string innerInjurydesc1[] = { "六脉调和", "脉络贯通", "舒筋活络", "内息混乱", "经脉错乱", "经脉寸断", "命不久已" };
static std::string innerInjurydesc = {"消耗：战斗时躲避不及会受到内伤\r\n恢复：使用药物可恢复\r\n影响：内伤过重时不但会加速气血\r\n             流失，还会降低你的攻击与\r\n             防御"};

static std::string outInjurydesc1[] = { "生龙活虎", "身强力壮", "安危相易", "体无完肤", "皮伤肉绽", "遍体鳞伤", "命不久已" };
static std::string outInjurydesc = { "消耗：战斗时躲避不及会受到外伤\r\n恢复：使用药物可恢复\r\n影响：外伤过重时不但会加速气血\r\n             流失，还会降低你的攻击与\r\n             防御" };

static std::string hungerdesc1[] = { "鼓腹含和", "酒足饭饱", "啜菽饮水", "饥不择食", "饥肠辘辘", "忍饥受饿", "命不久已" };
static std::string hungerdesc = { "消耗：时间消耗会越来越饿\r\n恢复：通过进食可恢复\r\n影响：过度饥饿时会影响其它属性\r\n             效果" };

static std::string spiritInjurydesc1[] = { "意气风发", "朝气蓬勃", "神气十足", "垂头丧气", "没精打采", "萎靡不振", "生无可恋" };
static std::string spiritInjurydesc = { "消耗：进行重复行为会加速消耗\r\n恢复：通过喝酒可恢复\r\n影响：精神不足时会影响其它属性\r\n             效果及恢复效率" };

static std::string lifedesc = { "消耗：受到伤害时消耗气血\r\n恢复：通过睡觉可恢复\r\n影响：气血不足时会死亡" };
#endif

