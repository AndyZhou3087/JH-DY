#ifndef _TOPBAR_H_
#define _TOPBAR_H_
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "UIScroll.h"
#include "Nature.h"
USING_NS_CC;

class TopBar:public Node
{
public:
	TopBar();
	~TopBar();

	virtual bool init();

	CREATE_FUNC(TopBar);

	void showNewerGuide(int step);
	void stopLoseAnim();
	int newerstep;
private:
	cocos2d::ui::ImageView* heroimg;
	cocos2d::ui::ImageView* reason;
	cocos2d::ui::ImageView* weather;
	cocos2d::ui::TextBMFont* livedayslbl;
	cocos2d::ui::TextBMFont* timelbl;
	cocos2d::ui::TextBMFont* templbl;
	float pastmin;//虚拟分钟，
	ProgressTimer* outinjuryBar;
	ProgressTimer* innerinjuryBar;
	ProgressTimer* hungerBar;
	ProgressTimer* spiritBar;
	ProgressTimer* lifeBar;

	cocos2d::ui::Widget* outinjuryRed;
	cocos2d::ui::Widget* innerinjuryRed;
	cocos2d::ui::Widget* hungerRed;
	cocos2d::ui::Widget* spiritRed;
	cocos2d::ui::Widget* lifeRed;

	cocos2d::ui::ImageView* outinjury;
	cocos2d::ui::ImageView* innerinjury;
	cocos2d::ui::ImageView* hunger;
	cocos2d::ui::ImageView* spirit;
	cocos2d::ui::ImageView* life;
	cocos2d::ui::LoadingBar* toplvexpbar;
	cocos2d::ui::Text* lvtext;

	int m_lastinnerinjury;
	int m_lastoutinjury;
	int m_lasthunger;
	float m_lastspirit;
	int m_lastlife;
	EWeather m_lastweather;
	EDAYNIGTH m_lastDayOrNigth;
	int m_lastpastLiveDay;
	bool isHunter;
	bool isShowingGuide;

private:
	void onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updataUI(float dt);
	void checkNpcRandMap();
};
#endif

