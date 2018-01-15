/********************************************************************
*退出解散帮会确认界面
*********************************************************************/
#ifndef __FACTIONCOMFIRMLAYER__
#define __FACTIONCOMFIRMLAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;

class FactionComfirmLayer : public Layer, public ServerDataDelegateProtocol
{
public:
	/****************************
	初始化
	*****************************/
	bool init(FactionListData *fldata);

	static FactionComfirmLayer* create(FactionListData *fldata);

private:
	/****************************
	点击“确认”按钮回调
	*****************************/
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	/****************************
	点击“取消”按钮回调
	*****************************/
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	移除当前层
	*****************************/
	void removSelf();

	void onSuccess();
	void onErr(int errcode);
private:
	FactionListData *m_fldata;
	FactionAction f_action;
};

#endif
