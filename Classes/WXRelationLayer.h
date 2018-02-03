#ifndef _RELATION_LAYER_H_
#define _RELATION_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class WXRelationLayer :public Layer
{
public:
	WXRelationLayer();
	~WXRelationLayer();

	bool init();
	virtual void onEnterTransitionDidFinish();
	static WXRelationLayer* create();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void delayShowData(float dt);
private:
	cocos2d::ui::ScrollView* srollView;
	Label * m_loadlbl;
};
#endif