/********************************************************************
* Game over UI界面
*********************************************************************/
#ifndef _DEATH_LAYER_H_
#define _DEATH_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class WXDeathLayer :public Layer
{
public:
	WXDeathLayer();
	~WXDeathLayer();

	virtual bool init();
	static WXDeathLayer* create();
private:

	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
private:

};
#endif

