#ifndef _HUAFEIAWARDLIST_LAYER_H_
#define _HUAFEIAWARDLIST_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;
class HuafeiAwardListLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	HuafeiAwardListLayer();
	~HuafeiAwardListLayer();

	bool init();
	static HuafeiAwardListLayer* create();

	void getData();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void delayShowData(float dt);
	void onSuccess();
	void onErr(int errcode);

	cocos2d::ui::Text* resultlbl;

private:
};
#endif