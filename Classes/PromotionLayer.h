#ifndef _PROMOTION_LAYER_H_
/********************************************************************
* 任命界面
* 
*********************************************************************/
#define _SETTING_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;

class PromotionLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	PromotionLayer();
	~PromotionLayer();

	bool init(FactionMemberData *data, Node* target);
	static PromotionLayer* create(FactionMemberData *data, Node* target);
private:

	void checkBoxCallback(cocos2d::Ref* pSender, cocos2d::ui::CheckBox::EventType);

	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onSuccess();
	void onErr(int errcode);

private:
	cocos2d::ui::CheckBox* m_checkBox[3];
	Node* m_target;
	int m_select;
	FactionMemberData* m_memberdata;
};
#endif

