
/********************************************************************
*好友NPC挑战界面
*********************************************************************/
#ifndef _FRIENDNPC_LAYER_H_
#define _FRIENDNPC_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class WXFriendNpcScene :public Layer
{
public:
	WXFriendNpcScene();
	~WXFriendNpcScene();

	virtual void onEnterTransitionDidFinish();
	bool init();
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);

	static cocos2d::Scene* createScene();
private:
	static WXFriendNpcScene* create();
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);


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
	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
private:
	cocos2d::ui::Text* desc1;
	std::vector<std::string> vec_fightnpc;
	cocos2d::ui::Button* escapebtn;
	cocos2d::ui::Button* getbtn;
};
#endif

