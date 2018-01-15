#ifndef _FACTIONCREATE_LAYER_H_
#define _FACTIONCREATE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;
class FactionCreateLayer :public Layer, public ServerDataDelegateProtocol, public cocos2d::ui::EditBoxDelegate
{
public:
	FactionCreateLayer();
	~FactionCreateLayer();

	bool init(int action, FactionListData* modifyfdata);
	virtual void onEnterTransitionDidFinish();
	static FactionCreateLayer* create(int action, FactionListData* modifyfdata = NULL);


private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onCreateFaction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onSelectLv(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onSelectSex(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onSuccess();
	void onErr(int errcode);

	void textFieldEvent(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType type);

	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);

	void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);

	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text);

	void editBoxReturn(cocos2d::ui::EditBox *editBox);
private:
	cocos2d::ui::EditBox* m_factionNameEdit;
	cocos2d::ui::EditBox* m_factionDescEdit;
	cocos2d::ui::TextField* descinput;
	cocos2d::ui::Button *m_createbtn;

	cocos2d::ui::Button* lvlimitbtn[4];
	cocos2d::ui::Button* sexlimitbtn[5];
	int selectlv;
	int selectsex;
	int m_action;//0--创建；1--修改
	FactionListData* m_modifyfdata;
	cocos2d::ui::TextField* nameInput;
};
#endif