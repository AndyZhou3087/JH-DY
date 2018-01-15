
#ifndef __BUILDING_DETAILS_LAYER__
#define __BUILDING_DETAILS_LAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Building.h"
USING_NS_CC;

class BuildingDetailsLayer : public Layer
{
public:
	BuildingDetailsLayer();
	~BuildingDetailsLayer();
	bool init(Building* pdata);
	static BuildingDetailsLayer* create(Building* pdata);
	void removSelf();

private:
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
};

#endif
