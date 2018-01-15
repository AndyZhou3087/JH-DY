#ifndef _SHOP_LAYER_H_
#define _SHOP_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
#include "MyPackage.h"
USING_NS_CC;

typedef enum
{
	PAY_SUCC = 0,
	PAY_FAIL
}PYARET;

class ShopLayer :public Layer
{
public:
	ShopLayer();
	~ShopLayer();

	bool init();
	static ShopLayer* create();
	static void beginPay(int index);
	static void setMessage(PYARET ret);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onQQ(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void refreshGoldCount(float dt);
	void showVipReward(float dt);
private:

	cocos2d::ui::ScrollView* m_rmbScrollview;
	cocos2d::ui::ScrollView* m_goldScrollview;
	cocos2d::ui::Text* mygoldlbl;
	Node* m_csbnode;
	static int payindex;
	static bool isPaying;
	bool ischangePause;
	cocos2d::ui::Text* qq1;
	cocos2d::ui::Text* qq2;
	cocos2d::ui::Text* qqtitle;
};
#endif

