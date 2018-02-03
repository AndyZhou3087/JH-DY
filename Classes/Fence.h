/********************************************************************
* 围墙
*********************************************************************/

#ifndef _FENCE_H_
#define _FENCE_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class Fence :public Building
{
public:
	Fence();
	~Fence();

public:
	void initRandSeed();
	time_t getNowTime();
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
	virtual bool init();

	CREATE_FUNC(Fence);
};
#endif

