/********************************************************************
*���ȷ�Ͻ���
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
	��ʼ��
	*****************************/
	bool init(Node* target, FactionMemberData *fmdata);

	static WXFactionKickComfirmLayer* create(Node* target, FactionMemberData *fmdata);
	void initRandSeed();
	time_t getNowTime();

private:
	/****************************
	�����ȷ�ϡ���ť�ص�
	*****************************/
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	/****************************
	�����ȡ������ť�ص�
	*****************************/
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
	/****************************
	�Ƴ���ǰ��
	*****************************/
	void removSelf();

	void onSuccess();
	void onErr(int errcode);
private:
	FactionMemberData *m_data;
	Node* m_target;
};

#endif
