/********************************************************************
*ȡ���չ���ʾ��
*********************************************************************/
#ifndef __EXERCISECANCELLAYER__
#define __EXERCISECANCELLAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class ExerciseCancelLayer : public Layer
{
public:
	/****************************
	��ʼ��
	*****************************/
    virtual bool init();

	CREATE_FUNC(ExerciseCancelLayer);
	/****************************
	�Ƴ���
	*****************************/
	void removSelf();
private:
	/****************************
	�����ȷ�ϡ���ť�ص�
	*****************************/
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	�����ȡ������ť�ص�
	*****************************/
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif
