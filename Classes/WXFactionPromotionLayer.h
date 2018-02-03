#ifndef _FACTIONPROMOTION_LAYER_H_
/********************************************************************
* 任命界面
* 
*********************************************************************/
#define _FACTIONPROMOTION_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;

class WXFactionPromotionLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	WXFactionPromotionLayer();
	~WXFactionPromotionLayer();
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);

	bool init(FactionMemberData *data, Node* target);
	static WXFactionPromotionLayer* create(FactionMemberData *data, Node* target);
private:

	void checkBoxCallback(cocos2d::Ref* pSender, cocos2d::ui::CheckBox::EventType);

	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onSuccess();
	void onErr(int errcode);
	void jumpDown(cocos2d::Node *node, float dt);
private:
	cocos2d::ui::CheckBox* m_checkBox[3];
	Node* m_target;
	int m_select;
	FactionMemberData* m_memberdata;
};
#endif

