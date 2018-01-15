/********************************************************************
*取消闭关提示框
*********************************************************************/
#ifndef __EXERCISECANCELLAYER__
#define __EXERCISECANCELLAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class ExerciseCancelLayer : public Layer
{
public:
	/****************************
	初始化
	*****************************/
    virtual bool init();

	CREATE_FUNC(ExerciseCancelLayer);
	/****************************
	移除掉
	*****************************/
	void removSelf();
private:
	/****************************
	点击“确认”按钮回调
	*****************************/
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“取消”按钮回调
	*****************************/
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif
