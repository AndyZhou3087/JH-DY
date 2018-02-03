/********************************************************************
* 床
*********************************************************************/

#ifndef _BED_H_
#define _BED_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class WXBed :public Building
{
public:
	WXBed();
	~WXBed();
	void initA();
public:
	/****************************
	睡觉操作
	@param minute 多长时间完成睡觉（进度条时间）
	@param exminute 消耗游戏时间
	*****************************/
	virtual void action(int minute, int exminute);

	virtual bool init();
	void initBg();
	CREATE_FUNC(WXBed);
};
#endif

