/********************************************************************
*进度保存确认界面
*********************************************************************/
#ifndef __COMFIRMSAVELAYER__
#define __COMFIRMSAVELAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class ComfirmSaveLayer : public Layer
{
public:
	/****************************
	初始化
	*****************************/
	virtual bool init();

	CREATE_FUNC(ComfirmSaveLayer);

private:
	/****************************
	点击“确认”按钮回调
	*****************************/
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	/****************************
	点击“取消”按钮回调
	*****************************/
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	移除当前层
	*****************************/
	void removSelf();
};

#endif
