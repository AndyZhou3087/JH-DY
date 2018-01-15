#ifndef _FACTIONDETAILS_LAYER_H_
/********************************************************************
* 帮派规则详细界面
* 
*********************************************************************/
#define _FACTIONDETAILS_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class FactionDetailsLayer :public Layer
{
public:
	FactionDetailsLayer();
	~FactionDetailsLayer();

	bool init();

	static FactionDetailsLayer* create();

private:

	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

private:
};
#endif

