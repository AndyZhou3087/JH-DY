/********************************************************************
*不可点击层
*********************************************************************/
#ifndef _CANNOT_TOUCH_LAYER_H_
#define _CANNOT_TOUCH_LAYER_H_
#include "cocos2d.h"
USING_NS_CC;

class CannotTouchLayer :public Layer
{
public:
	CannotTouchLayer();
	~CannotTouchLayer();

	virtual bool init();
	static CannotTouchLayer* create();
private:
	void initRandSeed();
	time_t getNowTime();
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
};
#endif

