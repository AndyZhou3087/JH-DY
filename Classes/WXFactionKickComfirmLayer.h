/********************************************************************
*逐出确认界面
*********************************************************************/
#ifndef __FACTIONKICKCOMFIRMLAYER__
#define __FACTIONKICKCOMFIRMLAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;

class WXFactionKickComfirmLayer : public Layer, public ServerDataDelegateProtocol
{
public:
	/****************************
	初始化
	*****************************/
	bool init(Node* target, FactionMemberData *fmdata);

	static WXFactionKickComfirmLayer* create(Node* target, FactionMemberData *fmdata);
	void initRandSeed();
	time_t getNowTime();

private:
	/****************************
	点击“确认”按钮回调
	*****************************/
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	/****************************
	点击“取消”按钮回调
	*****************************/
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
	/****************************
	移除当前层
	*****************************/
	void removSelf();

	void onSuccess();
	void onErr(int errcode);
private:
	FactionMemberData *m_data;
	Node* m_target;
};

#endif
