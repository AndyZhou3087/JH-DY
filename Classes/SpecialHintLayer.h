/********************************************************************
*��ʾ��
*********************************************************************/
#ifndef __SPECICALHINTBOX__
#define __SPECICALHINTBOX__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class SpecialHintLayer : public Layer
{
public:
	/****************************
	��ʼ��
	@param ��ʾ������
	*****************************/
    bool init();

	static SpecialHintLayer* create();
	/****************************
	�Ƴ���
	*****************************/
	void removSelf();
private:
	/****************************
	�����ѧϰ����ť�ص�
	*****************************/
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	/****************************
	�����ȡ������ť�ص�
	*****************************/
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif
