
/********************************************************************
*好友赠送索要物品界面
*********************************************************************/
#ifndef _FRIENDEXG_LAYER_H_
#define _FRIENDEXG_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class WXFriendExgScene:public Layer
{
public:
	WXFriendExgScene();
	~WXFriendExgScene();

	virtual void onEnterTransitionDidFinish();
	bool init(int type);

	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
	static cocos2d::Scene* createScene(int type);
private:
	static WXFriendExgScene* create(int type);
	void initRandSeed();
	time_t getNowTime();
	long long getNowTimeMs();
private:

	/****************************
	点击“不收不给”按钮回调
	*****************************/
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);

	/****************************
	点击“收下给予”按钮回调
	*****************************/
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void shake(cocos2d::Node * node);
private:
	int m_type;
	std::string m_npcid;
	std::vector<int> vec_res;
};
#endif

