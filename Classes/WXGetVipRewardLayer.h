#ifndef _GETVIPREWARD_LAYER_H_
#define _GETVIPREWARD_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
#include "ServerDataSwap.h"
USING_NS_CC;

class WXGetVipRewardLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	WXGetVipRewardLayer();
	~WXGetVipRewardLayer();
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);



	bool init();
	static WXGetVipRewardLayer* create();
	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
private:
	int rgoldcount;
	std::vector<int> vec_rgoodsindex;
private:
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
	void onErr(int errcode);
	void getVipGoods();
};
#endif

