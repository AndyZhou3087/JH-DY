/********************************************************************
* 建筑物
*********************************************************************/
#ifndef _BUILDING_H_
#define _BUILDING_H_
#include "cocos2d.h"
#include "json.h"
USING_NS_CC;

/*
建筑物数据结构
*/
typedef struct
{
	char name[20];//建筑物名称
	int maxlevel;//建筑物最高等级
	int level;//建筑物等级
	std::vector<int> needtime;//建筑物build时间
	std::vector<std::vector<int>> Res;//建筑物消耗的资源
	char cname[32];//建筑物中文名称
	std::string desc;//建筑物描述
}BuidingData;

class Building : public Sprite
{
public:
	Building();
	~Building();

	/********************************************************************
	* 解析每个建筑物的数据
	@param jsonvalue buildings.json [b]数据中每项的值
	*********************************************************************/
	void parseData(rapidjson::Value& jsonvalue);

	/********************************************************************
	* 建造
	*********************************************************************/
	virtual void build();

	/********************************************************************
	* 建造后，下面操作
	*********************************************************************/
	virtual void action(int minute, int exminute);

	/********************************************************************
	* 建筑物建造时间
	*********************************************************************/
	CC_SYNTHESIZE(float, m_actionBarTime, ActionBarTime);
public:
	BuidingData data;
};
#endif

