/********************************************************************
*��ͣ����
*********************************************************************/
#ifndef __PAUSELAYER__
#define __PAUSELAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class PauseLayer : public Layer
{
public:
	/****************************
	��ʼ��
	*****************************/
    bool init();

	static PauseLayer* create();
	/****************************
	�Ƴ���
	*****************************/
	void removSelf();
private:
	/****************************
	�����������Ϸ����ť�ص�
	*****************************/
	void onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	�����������ҳ����ť�ص�
	*****************************/
	void onGoHome(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif
