
/********************************************************************
*	 酒桌
*********************************************************************/

#ifndef _WINETABLE_H_
#define _WINETABLE_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class WineTable :public Building
{
public:
	WineTable();
	~WineTable();

public:
	/****************************
	喝酒操作
	@param minute （进度条时间）
	@param exminute 消耗游戏时间
	*****************************/
	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(WineTable);
};
#endif

