/********************************************************************
*�ɾ���ɶ�������
*********************************************************************/
#ifndef __ACHIVEDONEANIMLAYER__
#define __ACHIVEDONEANIMLAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class WXAchiveDoneAnimLayer : public Layer
{
public:
	/****************************
	��ʼ��
	*****************************/
    bool init();

	static WXAchiveDoneAnimLayer* create();

	void initA();
	/****************************
	�Ƴ���
	*****************************/
	void removSelf();
private:
	/****************************
	��ʾ�ɾ���ɶ���
	*****************************/
	void showAchiveDoneAnim(float dt);

	void initBg();
};

#endif
