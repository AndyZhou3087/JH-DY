#ifndef _VIPSHOP_LAYER_H_
#define _VIPSHOP_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
#include "MyPackage.h"
#include "ServerDataSwap.h"
USING_NS_CC;


class VipShopLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	VipShopLayer();
	~VipShopLayer();

	bool init();
	static VipShopLayer* create();

	void getLeftDays();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void loadVipShopData();
	void onSuccess();
	void onErr(int errcode);
private:

	cocos2d::ui::ScrollView* m_vipScrollview;
	Node* m_csbnode;
};
#endif

