﻿#ifndef _HSLJREWARDDESC_LAYER_H_
/********************************************************************
* 帮派规则详细界面
* 
*********************************************************************/
#define _HSLJREWARDDESC_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class WXHSLJRewardDescLayer :public Layer
{
public:
	WXHSLJRewardDescLayer();
	~WXHSLJRewardDescLayer();

	bool init();

	static WXHSLJRewardDescLayer* create();

private:

	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

private:
};
#endif

