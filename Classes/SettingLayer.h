#ifndef _SETTING_LAYER_H_
/********************************************************************
* 设置界面
* 
*********************************************************************/
#define _SETTING_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
USING_NS_CC;

class SettingLayer :public Layer, public cocos2d::ui::EditBoxDelegate, public ServerDataDelegateProtocol
{
public:
	SettingLayer();
	~SettingLayer();

	virtual bool init();
	CREATE_FUNC(SettingLayer);
private:

	/****************************
	音效开关checkbox点击回调
	****************************/
	void soundCheckBoxCallback(cocos2d::Ref* pSender, cocos2d::ui::CheckBox::EventType);

	/****************************
	更新音效状态
	****************************/
	void updateSoundStatus();

	/****************************
	关闭按钮点击
	****************************/
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	ios 恢复购买按钮
	****************************/
	void onResumeBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void textFieldEvent(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType type);

	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);

	void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);

	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text);

	void editBoxReturn(cocos2d::ui::EditBox *editBox);

	void onSuccess();
	void onErr(int errcode);

	void modifyName(std::string utf8name);

private:
	cocos2d::ui::CheckBox* m_soundCheckBox;//checkbox控件
	cocos2d::ui::Text* m_soundOnOffText;//点击checkbox的文件说明控件 “开”,“关”
	cocos2d::ui::EditBox* m_editName;

	std::string mynamestr;
	std::string editstr;

	cocos2d::ui::TextField* m_nameTextField;
};
#endif

