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
	static void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	static void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);


private:

	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	static void jumpDown(cocos2d::Node *node, float dt);

	static bool isPhone();

private:
};
#endif

