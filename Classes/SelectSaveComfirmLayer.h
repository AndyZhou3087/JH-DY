/********************************************************************
*����ѡ��ȷ�Ͻ���
*********************************************************************/
#ifndef __SELECTSAVECOMFIRMLAYER__
#define __SELECTSAVECOMFIRMLAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class SelectSaveComfirmLayer : public Layer
{
public:
	/****************************
	��ʼ��
	@param ѡ�еĵڼ���
	*****************************/
	bool init(int index);

	static SelectSaveComfirmLayer* create(int index);

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
};

#endif
