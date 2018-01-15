
/********************************************************************
*好友NPC挑战界面
*********************************************************************/
#ifndef _FRIENDNPC_LAYER_H_
#define _FRIENDNPC_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class FriendNpcScene :public Layer
{
public:
	FriendNpcScene();
	~FriendNpcScene();

	virtual void onEnterTransitionDidFinish();
	bool init();

	static cocos2d::Scene* createScene();
private:
	static FriendNpcScene* create();
private:

	/****************************
	点击“逃跑”按钮回调
	*****************************/
	void onEscape(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“挑战”按钮回调
	*****************************/
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void delayFight(float dt);

private:
	cocos2d::ui::Text* desc1;
	std::vector<std::string> vec_fightnpc;
	cocos2d::ui::Button* escapebtn;
	cocos2d::ui::Button* getbtn;
};
#endif

