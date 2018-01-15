/********************************************************************
*成就完成动画界面
*********************************************************************/
#ifndef __ACHIVEDONEANIMLAYER__
#define __ACHIVEDONEANIMLAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class AchiveDoneAnimLayer : public Layer
{
public:
	/****************************
	初始化
	*****************************/
    bool init();

	static AchiveDoneAnimLayer* create();
	/****************************
	移除掉
	*****************************/
	void removSelf();
private:
	/****************************
	显示成就完成动画
	*****************************/
	void showAchiveDoneAnim(float dt);
};

#endif
