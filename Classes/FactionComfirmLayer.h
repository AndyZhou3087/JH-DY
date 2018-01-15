/********************************************************************
*�˳���ɢ���ȷ�Ͻ���
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
	��ʼ��
	*****************************/
	bool init(FactionListData *fldata);

	static FactionComfirmLayer* create(FactionListData *fldata);

private:
	/****************************
	�����ȷ�ϡ���ť�ص�
	*****************************/
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	/****************************
	�����ȡ������ť�ص�
	*****************************/
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	�Ƴ���ǰ��
	*****************************/
	void removSelf();

	void onSuccess();
	void onErr(int errcode);
private:
	FactionListData *m_fldata;
	FactionAction f_action;
};

#endif
