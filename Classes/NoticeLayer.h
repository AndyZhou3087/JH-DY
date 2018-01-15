#ifndef _NOTICE_LAYER_H_
/********************************************************************
* 公告界面
* 
*********************************************************************/
#define _NOTICE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class NoticeLayer :public Layer
{
public:
	NoticeLayer();
	~NoticeLayer();

	bool init(std::string contentstr);

	static NoticeLayer* create(std::string contentstr);

private:
	/****************************
	点击知道了按钮
	****************************/
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	//今日不再提醒checkbox
	void checkBoxCallback(cocos2d::Ref* pSender, cocos2d::ui::CheckBox::EventType type);

private:
	cocos2d::ui::CheckBox* m_checkBox;//checkbox控件
};
#endif

