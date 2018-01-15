/********************************************************************
*∑‚∫≈≤„
*********************************************************************/
#ifndef __FROZENLAYER__
#define __FROZENLAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class FrozenLayer : public Layer
{
public:
	/****************************
	≥ı ºªØ
	*****************************/
    bool init();

	static FrozenLayer* create();
private:
	void onQQ(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif
