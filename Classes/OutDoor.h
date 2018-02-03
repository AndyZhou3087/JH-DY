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
	void initRandSeed();
	time_t getNowTime();
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
	void checkNewerGuide();
	void takeout(PackageData* pdata);
	bool getRandomBoolean(float rate);
	bool getRandomBoolean();
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
private:
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onOut(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	Node* m_csbnode;
	cocos2d::ui::ScrollView * scrollview;
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	void onStorageItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onPackageItem(cocos2d::Ref* pSender);
	void longTouchUpdate(float dt);
	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
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

