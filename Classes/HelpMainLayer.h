#ifndef _HELPMAIN_LAYER_H_
#define _HELPMAIN_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

typedef struct
{
	std::string id;
	std::string title;
	std::string content;
}helpData;
class HelpMainLayer :public Layer
{
public:
	HelpMainLayer();
	~HelpMainLayer();

	bool init();
	static HelpMainLayer* create();


private:
	void onShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onTextClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void loadHelpText();
private:
	cocos2d::ui::ScrollView* m_srollView;
	std::vector<helpData> vec_helpdata;
	bool isChangePause;
};
#endif