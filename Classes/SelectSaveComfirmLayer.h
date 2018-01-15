/********************************************************************
*进度选择确认界面
*********************************************************************/
#ifndef __SELECTSAVECOMFIRMLAYER__
#define __SELECTSAVECOMFIRMLAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class SelectSaveComfirmLayer : public Layer
{
public:
	/****************************
	初始化
	@param 选中的第几个
	*****************************/
	bool init(int index);

	static SelectSaveComfirmLayer* create(int index);

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
