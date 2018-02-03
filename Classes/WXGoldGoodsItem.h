
/****************************************************************
商城物品类

****************************************************************/
#ifndef _GOODSITIME_H_
#define _GOODSITIME_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
#include "MyPackage.h"
USING_NS_CC;

class WXGoldGoodsItem :public Sprite
{
public:
	WXGoldGoodsItem();
	~WXGoldGoodsItem();

	virtual bool init(GoodsData* gdata);
	static WXGoldGoodsItem* create(GoodsData* gdata);
	static void addBuyGoods(GoodsData* gdata);
	static void updateDefaultStorage(PackageData pdata, int heroindex);
	void initRandSeed();
	time_t getNowTime();

private:
	cocos2d::ui::ImageView* icon;
	cocos2d::ui::Text* nameTxt;
	cocos2d::ui::Text* descTxt;
	cocos2d::ui::Text* priceTxt;
	bool isDraging;
	Vec2 startPos;

	GoodsData* m_goodData;
private:

	/****************************
	点击购买按钮回调
	*****************************/
	void onBuyBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
};
#endif

