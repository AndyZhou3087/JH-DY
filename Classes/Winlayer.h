#ifndef _WIN_LAYER_H_
#define _WIN_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
#include "ServerDataSwap.h"

USING_NS_CC;
class Winlayer :public Layer, public ServerDataDelegateProtocol
{
public:
	Winlayer();
	~Winlayer();

	bool init(std::string addr, std::string npcid);
	virtual void onExit();
	virtual void onEnterTransitionDidFinish();
	static Winlayer* create(std::string addr, std::string npcid);
	void updataMyPackageUI();
	void updataRewardUI();
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);

	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);
	/****************************
	新手引导
	*****************************/
	void checkNewerGuide();
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	static void showMissionAnim(Node* _target, std::string text, std::vector<std::string> vec_res);
	static void removeMissionAnim(Ref* pSender);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updata();

	bool getRandomBoolean(float rate);
	bool getRandomBoolean();
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
	void onRewardItem(cocos2d::Ref* pSender);
	void onPackageItem(cocos2d::Ref* pSender);
	void saveTempData();
	void loadTempData();
	void updataLV();
	void showLvUpAnim(float dt);
	int addHeroExp();
	int addGfExp();

	void onSuccess();
	void onErr(int errcode);
	void initRandSeed();
	time_t getNowTime();
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
private:
	std::string m_npcid;
	std::string m_addrid;
	std::vector<PackageData> getRewardData;
	std::vector<PackageData> tempResData;
	cocos2d::ui::TextBMFont* explbl;
	cocos2d::ui::TextBMFont* gfexplbl;
	cocos2d::ui::Button* m_backbtn;
	cocos2d::ui::Button* m_getallbtn;
	std::vector<std::string> vec_lvup;
};
#endif

