/********************************************************************
*进度选择界面
*********************************************************************/
#ifndef __SELECTSAVELAYER__
#define __SELECTSAVELAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class SelectSaveLayer : public Layer
{
public:
	/****************************
	初始化
	*****************************/
	virtual bool init();

	CREATE_FUNC(SelectSaveLayer);

private:

	/****************************
	点击“返回”按钮回调
	*****************************/
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	选中存档
	*****************************/
	void onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	移除当前层
	*****************************/
	void removSelf();
private:
	Node* m_saveNode[4];//保存的4个角色节点
};

#endif
