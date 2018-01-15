/********************************************************************
* 背包
*********************************************************************/

#ifndef _MYPACKAGE_H_
#define _MYPACKAGE_H_
#include "cocos2d.h"
USING_NS_CC;

typedef struct _PackageData
{
	std::string strid;
	int type;
	int count;
	int lv;//内功，外功等级，
	int extype;//用于区分采集1，砍伐2，挖掘工具3，其他类型为0
	int exp;
	int goodvalue;
	int slv;//强化等级
	int tqu;//锻造出来的品质
	_PackageData()
	{
		strid = "";
		type = 0;
		count = 0;
		lv = 0;
		extype = 0;
		exp = 0;
		goodvalue = 100;
		slv = 0;
		tqu = 1;
	}

}PackageData;

class MyPackage
{
public:
	MyPackage();
	~MyPackage();

public:

	static int add(PackageData pdata);
	static void cutone(std::string strid, int count = 1);
	static void cutone(PackageData pdata);
	static void takeoff();
	static bool isFull(PackageData pdata);
	static void save();
	static void load();

	static int canTakeCount(PackageData* pdata);

	static int getCountById(std::string strid);

	static int getMax();
	static void setMax(int val);
	static int getSize();
public:
	static std::vector<PackageData> vec_packages;
private:
	
	static int max;
};
#endif

