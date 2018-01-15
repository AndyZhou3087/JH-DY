/********************************************************************
*提示框
*********************************************************************/
#ifndef __HINTBOX__
#define __HINTBOX__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class HintBox : public Layer
{
public:
	/****************************
	初始化
	@param 提示的内容
	*****************************/
    bool init(std::string text);

	static HintBox* create(std::string content);
	/****************************
	移除掉
	*****************************/
	void removSelf();
private:
	/****************************
	点击“确认”按钮回调
	*****************************/
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif
