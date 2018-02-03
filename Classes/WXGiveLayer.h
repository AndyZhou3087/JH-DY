#ifndef _GIVE_LAYER_H_
#define _GIVE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;
class WXGiveLayer :public Layer
{
public:
	WXGiveLayer();
	~WXGiveLayer();
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
	bool init(std::string npcid, std::string addrid);
	virtual void onEnterTransitionDidFinish();
	static WXGiveLayer* create(std::string npcid, std::string addrid);
	void updataMyGoodsUI();
	void updataGiveGoodsUI();
	void initRandSeed();
	time_t getNowTime();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onGive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updata();
	bool getRandomBoolean(float rate);
	bool getRandomBoolean();
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
	void onGiveGoodsItem(cocos2d::Ref* pSender);
	void onMyGoodsItem(cocos2d::Ref* pSender);
	void updateMyGoods(PackageData data);
	void updateGiveGoods(PackageData data);
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	void checkValue();

	void doGiveMission();

	void giveRes(std::vector<std::string> vec_res);

	void removeSelf(float dt);
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
private:
	cocos2d::ui::ScrollView* m_giveGoodsSrollView;
	cocos2d::ui::ScrollView* m_myGoodsSrollView;
	cocos2d::ui::Widget* m_givebtn;
	int lastMyGoodsSrollViewHeight;
	int lastGiveGoodsSrollViewHeight;
	std::string m_npcid;
	std::vector<PackageData> myGoodsData;
	std::vector<PackageData> myGiveData;
	cocos2d::ui::Text* friendlylbl;
	int friendly;
	int giveval;
	std::string m_addrid;
	cocos2d::ui::Widget *m_backbtn;
};
#endif

