/********************************************************************
*暂停界面
*********************************************************************/
#ifndef __PAUSELAYER__
#define __PAUSELAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class PauseLayer : public Layer
{
public:
	/****************************
	初始化
	*****************************/
    bool init();

	static PauseLayer* create();
	/****************************
	移除掉
	*****************************/
	void removSelf();
private:
	/****************************
	点击“继续游戏”按钮回调
	*****************************/
	void onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“返回主页”按钮回调
	*****************************/
	void onGoHome(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif
