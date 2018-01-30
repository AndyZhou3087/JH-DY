#ifndef _FACTIONMAIN_LAYER_H_
#define _FACTIONMAIN_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;

class FactionMainLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	FactionMainLayer();
	~FactionMainLayer();

	bool init();
	virtual void onEnterTransitionDidFinish();
	static FactionMainLayer* create();
	void getFactionListData();
	void disBtn();
	void resetBtn();
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	void updateLvAndMember();
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onCreateFaction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onEnterFaction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onMore(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
	void delayShowData(float dt);
	void onSuccess();
	void onErr(int errcode);

private:
	cocos2d::ui::ScrollView* srollView;
	cocos2d::ui::Widget *enterbtn;
	cocos2d::ui::Widget *createbtn;
};

class FactionListItem :public Sprite, public ServerDataDelegateProtocol
{
public:
	FactionListItem();
	~FactionListItem();

	bool init(FactionListData *data);
	static FactionListItem* create(FactionListData *data);
	void updateLvAndPeopleCount();

	bool getRandomBoolean(float rate);
	bool getRandomBoolean();
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
private:
	void onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void onErr(int errcode);
	void checkRequset();
	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
private:
	FactionListData* m_data;
	cocos2d::ui::Button *actionbtn;
	cocos2d::ui::Text* countlbl;
	cocos2d::ui::Text* lvlbl;
	cocos2d::ui::Text* actionbtnlabel;
};
#endif