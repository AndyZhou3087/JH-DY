/****************************
角色购买界面
****************************/
#ifndef _BUYDETAILS_LAYER_H_
#define _BUYDETAILS_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class BuyDetailsLayer :public Layer
{
public:
	BuyDetailsLayer();
	~BuyDetailsLayer();

	/****************************
	初始化
	@param heroid
	****************************/
	virtual bool init(int heroid);
	static BuyDetailsLayer* create(int heroid);
private:
	cocos2d::ui::ImageView* image;//头像 image
	cocos2d::ui::Text* nameTxt;//角色名称控件
	cocos2d::ui::Text* descTxt;//角色描述控件
	cocos2d::ui::Text* priceTxt;//角色价格控件
	cocos2d::ui::Text* goodstext;//初始物品控件

private:
	/****************************
	点击返回按钮回调
	*****************************/
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击购买按钮回调
	*****************************/
	void onBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

