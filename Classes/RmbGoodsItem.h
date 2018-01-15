
/****************************************************************
商城人民币购买

****************************************************************/
#ifndef _RMBGOODSITIME_H_
#define _RMBGOODSITIME_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
USING_NS_CC;

class RmbGoodsItem :public Sprite
{
public:
	RmbGoodsItem();
	~RmbGoodsItem();

	virtual bool init(GoodsData* gdata);
	static RmbGoodsItem* create(GoodsData* gdata);
private:
	cocos2d::ui::ImageView* icon;
	cocos2d::ui::Text* nameTxt;
	bool isDraging;
	Vec2 startPos;
private:
	/****************************
	点击购买按钮回调
	*****************************/
	void onBuyBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

