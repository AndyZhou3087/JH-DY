#ifndef _LOGINREWARD_LAYER_H_
#define _LOGINREWARD_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class LoginRewardLayer :public Layer
{
public:
	LoginRewardLayer();
	~LoginRewardLayer();

	bool init();
	static LoginRewardLayer* create();

private:

private:
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

