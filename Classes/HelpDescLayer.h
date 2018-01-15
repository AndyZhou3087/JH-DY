#ifndef _HELPDESC_LAYER_H_
#define _HELPDESC_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class HelpDescLayer :public Layer
{
public:
	HelpDescLayer();
	~HelpDescLayer();

	virtual bool init(std::string title, std::string text);
	static HelpDescLayer* create(std::string title, std::string text);
private:
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

