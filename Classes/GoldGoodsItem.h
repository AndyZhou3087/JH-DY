
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

class GoldGoodsItem :public Sprite
{
public:
	GoldGoodsItem();
	~GoldGoodsItem();

	virtual bool init(GoodsData* gdata);
	static GoldGoodsItem* create(GoodsData* gdata);
	static void addBuyGoods(GoodsData* gdata);
	static void updateDefaultStorage(PackageData pdata, int heroindex);
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
};
#endif

