/********************************************************************
* 功法组合结算界面
*********************************************************************/
#ifndef _MIXGFDONE_LAYER_H_
#define _MIXGFDONE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class MixGFDoneLayer :public Layer
{
public:
	MixGFDoneLayer();
	~MixGFDoneLayer();

	virtual bool init(int ret);
	static MixGFDoneLayer* create(int ret);
private:
	void removeself(float dt);
private:
	void initRandSeed();
	time_t getNowTime();
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
};
#endif

