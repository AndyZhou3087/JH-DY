#ifndef _HSLJMAIN_LAYER_H_
#define _HSLJMAIN_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;

class WXHSLJMainLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	WXHSLJMainLayer();
	~WXHSLJMainLayer();

	bool init();
	static WXHSLJMainLayer* create();
	void getMyMatchInfo();

	void updateMyFightCount();

	static std::string getDwStr(int exp);
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAddCount(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onMacth(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);


	void onRewardDesc(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onRank(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onSuccess();
	void onErr(int errcode);
	void showMyInfo();
	void showMatchInfo();
	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
	void showVSAnim();
	void delayEnterFight(float dt);

	void showMyReWard();

	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	void changeHeroImg(float dt);

	void stopChangeHeroImg(float dt);

	void showPlayerHeadImg(int herotype);
private:
	cocos2d::ui::Button* m_matchbtn;
	cocos2d::ui::Widget *m_backbtn;
	cocos2d::ui::Widget* heroNode;
	cocos2d::ui::Text* m_mydw;//我的段位
	cocos2d::ui::Text* m_mywinpercent;//我的胜率
	cocos2d::ui::Text* m_time;
	cocos2d::ui::Text* m_matchno;
	cocos2d::ui::Text* m_matchwincount;
	cocos2d::ui::Text* m_progresstext;
	cocos2d::ui::LoadingBar* m_expbar;
	cocos2d::ui::Text* m_fightcount;

	cocos2d::ui::Text* m_herodw;//对手的段位
	cocos2d::ui::Text* m_herowinpercent;//对手胜率
	cocos2d::ui::Text* m_heroname;//对手

	cocos2d::ui::Text* m_hinttext;
	cocos2d::ui::ImageView* m_playerheadimg;
	int datatype;
	int changenexthero;

	bool getRandomBoolean(float rate);
	bool getRandomBoolean();
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
};
#endif