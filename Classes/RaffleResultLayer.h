#ifndef _RAFFLE_RESULT_LAYER_H_
#define _RAFFLE_RESULT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
USING_NS_CC;

class RaffleResultLayer :public Layer
{
public:
	RaffleResultLayer();
	~RaffleResultLayer();

	bool init();
	static RaffleResultLayer* create();

private:
	void onOK(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
	int wingold;
};
#endif

