
/********************************************************************
*	 灶台
*********************************************************************/

#ifndef _COOKTABLE_H_
#define _COOKTABLE_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class CookTable :public Building
{
public:
	CookTable();
	~CookTable();

public:

	/****************************
	制作食物操作
	@param minute （进度条时间）
	@param exminute 消耗游戏时间
	*****************************/
	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(CookTable);
};
#endif

