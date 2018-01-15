
/********************************************************************
*好友赠送索要物品界面
*********************************************************************/
#ifndef _FRIENDEXG_LAYER_H_
#define _FRIENDEXG_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class FriendExgScene:public Layer
{
public:
	FriendExgScene();
	~FriendExgScene();

	virtual void onEnterTransitionDidFinish();
	bool init(int type);

	static cocos2d::Scene* createScene(int type);
private:
	static FriendExgScene* create(int type);
private:

	/****************************
	点击“不收不给”按钮回调
	*****************************/
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“收下给予”按钮回调
	*****************************/
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

private:
	int m_type;
	std::string m_npcid;
	std::vector<int> vec_res;
};
#endif

