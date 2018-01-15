/********************************************************************
*���ȱ���ȷ�Ͻ���
*********************************************************************/
#ifndef __COMFIRMSAVELAYER__
#define __COMFIRMSAVELAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class ComfirmSaveLayer : public Layer
{
public:
	/****************************
	��ʼ��
	*****************************/
	virtual bool init();

	CREATE_FUNC(ComfirmSaveLayer);

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
