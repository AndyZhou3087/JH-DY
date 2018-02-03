
/********************************************************************
*后山界面
*********************************************************************/
#ifndef _HOMEHILL_LAYER_H_
#define _HOMEHILL_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class WXHomeHill :public Layer
{
public:
	WXHomeHill();
	~WXHomeHill();

	virtual bool init();
	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
	CREATE_FUNC(WXHomeHill);
	virtual void onEnterTransitionDidFinish();
	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);

	void initRandSeed();
	time_t getNowTime();
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
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
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
private:

	cocos2d::ui::ScrollView* scrollView;//操作节点的滚动控件
	cocos2d::ui::Widget* m_backbtn;//返回按钮
};
#endif

