
/********************************************************************
*徒弟界面
*********************************************************************/
#ifndef _APPRENTICE_LAYER_H_
#define _APPRENTICE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class WXApprenticeScene :public Layer
{
public:
	WXApprenticeScene();
	~WXApprenticeScene();

	virtual void onEnterTransitionDidFinish();
	bool init(int type);
	void on1sTimer();
	static cocos2d::Scene* createScene(int type);
	void loadTime();
private:
	static WXApprenticeScene* create(int type);
	void initBg();
private:

	/****************************
	点击“不收”按钮回调
	*****************************/
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“收徒”按钮回调
	*****************************/
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void saveTime();
	void giveGold();

	void showHint(int type);
	void initA();
	void delayShowShop(float dt);
private:
	int m_type;
};
#endif

