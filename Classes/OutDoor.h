#ifndef _OUTDOOR_LAYER_H_
#define _OUTDOOR_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
#include "HeroProperNode.h"
USING_NS_CC;


class OutDoor :public Layer
{
public:
	OutDoor();
	~OutDoor();

	virtual bool init();
	CREATE_FUNC(OutDoor);
	void updataMyPackageUI();
	void updataStorageUI();
	void updata();
	void showNewerGuide(int step);

	void checkNewerGuide();
	void takeout(PackageData* pdata);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onOut(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	Node* m_csbnode;
	cocos2d::ui::ScrollView * scrollview;

	void onStorageItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onPackageItem(cocos2d::Ref* pSender);
	void longTouchUpdate(float dt);

private:
	std::vector<PackageData*> allStorageData;
	HeroProperNode* m_heroproper;
	cocos2d::ui::Button* m_outbtn;
	int lastSrollViewHeight;
	int lastrows;
	bool m_isLongPress;
	Node* m_longTouchNode;
};
#endif

