
#ifndef __RES_DETAILS_LAYER__
#define __RES_DETAILS_LAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;

class ResDetailsLayer : public Layer
{
public:
	ResDetailsLayer();
	~ResDetailsLayer();
	bool init(PackageData* pdata);
	static ResDetailsLayer* create(PackageData* pdata);
	static ResDetailsLayer* createByResId(std::string resid);
	void removSelf();
	void initRandSeed();
	time_t getNowTime();
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
	static int whereClick;
private:
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onUse(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onAddOne(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onMinusOne(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	bool getRandomBoolean(float rate);
	bool getRandomBoolean();
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
	void recoveInjuryValue(int addwvalue, int addnvalue);
	void recoveHungerValue(int addvalue);

	void updateHorseData(int addvalue);

	void updataLeftTime(float dt);
	void updateUI();

	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);

	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);

	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
private:
	PackageData* m_packageData;
	cocos2d::ui::Text* valuelbl;
	cocos2d::ui::Text* uselbl;
	cocos2d::ui::Text* lefttimelbl;
	cocos2d::ui::Text* selectCountlbl;
	cocos2d::ui::Slider* slider;
	cocos2d::ui::Button* m_okbtn;
	void sliderEvent(Ref * pSender, cocos2d::ui::Slider::EventType type);
	Node* m_csbnode;
	int m_expendtime;
};

#endif
