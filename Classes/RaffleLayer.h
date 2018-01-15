#ifndef _RAFFLE_LAYER_H_
#define _RAFFLE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
#include "ServerDataSwap.h"
USING_NS_CC;

class RaffleLayer:public Layer, public ServerDataDelegateProtocol
{
public:
	RaffleLayer();
	~RaffleLayer();

	bool init();
	static RaffleLayer* create();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onJoin(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void onErr(int errcode);
	void updatePool(float dt);
private:
	void refreshGoldCount(float dt);
	cocos2d::ui::Text* mygoldlbl;
	cocos2d::ui::Text* lefttimelbl;
	cocos2d::ui::Text* winnames[3];
	cocos2d::ui::Text* wingolds[3];
	int actiontype;
	cocos2d::ui::Button* joinbtn;
	cocos2d::ui::Button* backbtn;
	cocos2d::ui::TextBMFont* poolnum;
	cocos2d::ui::Widget* prizebox;
};
#endif

