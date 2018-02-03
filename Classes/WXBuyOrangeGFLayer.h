#ifndef _BUYORANGEGF_LAYER_H_
#define _BUYORANGEGF_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
USING_NS_CC;
class WXBuyOrangeGFLayer :public Layer
{
public:
	WXBuyOrangeGFLayer();
	~WXBuyOrangeGFLayer();

	bool init();
	static WXBuyOrangeGFLayer* create();

	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);

	std::vector<std::string> getRandQu5Gf();
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	std::vector<std::string> vec_qu5gf;
private:
	static void shake(cocos2d::Node * node);
	static void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
};
#endif