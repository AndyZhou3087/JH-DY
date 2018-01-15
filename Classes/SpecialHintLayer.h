/********************************************************************
*提示框
*********************************************************************/
#ifndef __SPECICALHINTBOX__
#define __SPECICALHINTBOX__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class SpecialHintLayer : public Layer
{
public:
	/****************************
	初始化
	@param 提示的内容
	*****************************/
    bool init();

	static SpecialHintLayer* create();
	/****************************
	移除掉
	*****************************/
	void removSelf();
private:
	/****************************
	点击“学习”按钮回调
	*****************************/
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	/****************************
	点击“取消”按钮回调
	*****************************/
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif
