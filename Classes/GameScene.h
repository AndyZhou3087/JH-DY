
/********************************************************************
*	 游戏主场景
*********************************************************************/
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "UIScroll.h"
#include "TopBar.h"
#include "Nature.h"
#include "Hero.h"
#include "HomeLayer.h"
#include "ServerDataSwap.h"

class GameScene : public cocos2d::Layer, public ServerDataDelegateProtocol
{
public:
	GameScene();
	~GameScene();
    static cocos2d::Scene* createScene();
    virtual bool init();
	void initA();
	virtual void onExit();

	CREATE_FUNC(GameScene);
	/****************************
	角色复活更新UI
	*****************************/
	void heroRevive();
	void initRandSeed();

	/****************************
	保存数据到本地文件
	*****************************/
	void saveAllData();
	void initBg();
	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step, std::vector<Node*> nodes);

	/****************************
	延迟新手引导
	*****************************/
	void delayShowNewerGuide(float dt);
	time_t getNowTime();
	long long getNowTimeMs();

	/****************************
	延迟显示出门场景
	*****************************/
	void delayShowOutScence(float dt);

	/****************************
	出门
	*****************************/
	void showGOOut(float dt);
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
	/****************************
	检查成就完成
	*****************************/
	void checkAchiveIsDone(float dt);

	/****************************
	安卓返回键保存数据
	*****************************/
	static void saveMyData();
	bool getRandomBoolean(float rate);
	bool getRandomBoolean();

	/****************************
	安卓返回键时改变游戏状态
	*****************************/
	static void changeGameStates(int status);


	/****************************
	重置支线任务（每天重置）
	*****************************/
	void checkRestBranchMisson();
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
private:
	TopBar* topBar;//角色属性和天气NODE
	HomeLayer *homeLayer;//角色家
	std::vector<Node*> m_newerNode;
	int m_newerStep;
	bool issavedata;
	bool isAnewGetData;
	
private:
	/****************************
	读取保存到本地的Nature(时间天气)数据
	*****************************/
	void loadSaveNatureData();

	/****************************
	读取保存到本地的角色数据
	*****************************/
	void loadSaveHeroData();

	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	/****************************
	读取保存到本地的角色属性数据
	*****************************/
	void loadSavedHeroPropData();

	/****************************
	定时更新资源列表数据
	*****************************/
	void updata(float dt);
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);

	/****************************
	定时查看角色是否死亡，生命为0死掉
	*****************************/
	void checkiflive(float dt);

	/****************************
	定时保存数据
	*****************************/
	void timerSaveData(float dt);

	/****************************
	韦小宝和独孤求败地图位置
	*****************************/
	void getNpcRandMap();
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
	/****************************
	VIP奖励回调
	*****************************/
	void onSuccess();
	void onErr(int errcode);

	void delayChangeStartScene(float dt);
	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
};
extern Nature* g_nature;//天气数据
extern Hero* g_hero;//角色数据
extern GameScene* g_gameLayer;//游戏层
extern UIScroll* g_uiScroll;//topbar文件事件滚动控件
#endif // __GAME_SCENE_H__
