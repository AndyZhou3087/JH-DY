#ifndef _STORAGEUI_LAYER_H_
#define _STORAGEUI_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Building.h"
#include "StorageRoom.h"
#include "GlobalData.h"
USING_NS_CC;


class StorageUILayer :public Layer
{
public:
	StorageUILayer();
	~StorageUILayer();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	CREATE_FUNC(StorageUILayer);
	void updateResContent();
	void showNewerGuide(int step);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onHelp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	Node* m_csbnode;
	void onclick(Ref* pSender);
	cocos2d::ui::ScrollView* scrollview;
private:
	int typerow[RES_MAX];
	int getCountByType(int type);
};
#endif

