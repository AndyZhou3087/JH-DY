#ifndef _HSLJREWARD_LAYER_H_
#define _HSLJREWARD_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
#include "ServerDataSwap.h"
USING_NS_CC;
class HSLJRewardLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	HSLJRewardLayer();
	~HSLJRewardLayer();

	bool init(int goldcount);
	static HSLJRewardLayer* create(int goldcount);

private:
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void onErr(int errcode);
	int m_goldcount;
private:
};
#endif