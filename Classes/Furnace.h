
/********************************************************************
*	 暖炉
*********************************************************************/

#ifndef _FURNACE_H_
#define _FURNACE_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class Furnace :public Building
{
public:
	Furnace();
	~Furnace();

public:
	/****************************
	制暖操作
	@param minute （进度条时间）
	@param exminute 消耗游戏时间
	*****************************/
	virtual void action(int minute, int exminute);
	virtual bool init();

	CREATE_FUNC(Furnace);

private:
	int extime;
	/****************************
	制暖
	@param
	定时器执行
	*****************************/
	void onfire(float dt);
};
#endif

