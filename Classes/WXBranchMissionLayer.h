#ifndef _BRANCHMISSION_LAYER_H_
#define _BRANCHMISSION_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class WXBranchMissionLayer :public Layer
{
public:
	WXBranchMissionLayer();
	~WXBranchMissionLayer();

	virtual bool init();
	static WXBranchMissionLayer* create();
private:
	void onDropMisson(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
};
#endif

