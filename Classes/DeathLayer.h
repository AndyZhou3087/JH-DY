/********************************************************************
* Game over UI界面
*********************************************************************/
#ifndef _DEATH_LAYER_H_
#define _DEATH_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class DeathLayer :public Layer
{
public:
	DeathLayer();
	~DeathLayer();

	virtual bool init();
	static DeathLayer* create();
private:


private:

};
#endif

