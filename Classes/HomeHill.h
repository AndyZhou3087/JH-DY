
/********************************************************************
*后山界面
*********************************************************************/
#ifndef _HOMEHILL_LAYER_H_
#define _HOMEHILL_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class HomeHill :public Layer
{
public:
	HomeHill();
	~HomeHill();

	virtual bool init();

	CREATE_FUNC(HomeHill);
	virtual void onEnterTransitionDidFinish();
	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);
private:
	/****************************
	点击操作按钮回调
	*****************************/
	void onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	/****************************
	点击“返回”按钮回调
	*****************************/
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	/****************************
	定时更新UI
	*****************************/
	void updateUI(float dt);

private:

	cocos2d::ui::ScrollView* scrollView;//操作节点的滚动控件
	cocos2d::ui::Widget* m_backbtn;//返回按钮
};
#endif

