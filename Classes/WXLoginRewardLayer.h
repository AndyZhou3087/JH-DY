#ifndef _LOGINREWARD_LAYER_H_
#define _LOGINREWARD_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class WXLoginRewardLayer :public Layer
{
public:
	WXLoginRewardLayer();
	~WXLoginRewardLayer();

	bool init();
	static WXLoginRewardLayer* create();
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);

private:
	cocos2d::ui::Widget* particle;
private:
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
};
#endif

