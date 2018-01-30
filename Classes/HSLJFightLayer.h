﻿/********************************************************************
* 对战界面
*********************************************************************/
#ifndef _HSLJFIGHT_LAYER_H_
#define _HSLJFIGHT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UIScroll.h"
#include "Hero.h"
USING_NS_CC;
class HSLJFightLayer :public Layer
{
public:
	HSLJFightLayer();
	~HSLJFightLayer();

	/****************************
	初始化
	@param addrname 地点名称
	****************************/
	bool init(std::string addrname);

	static HSLJFightLayer* create(std::string addrid);

	/****************************
	角色攻击
	定时器执行
	****************************/
	void delayMyFight(float dt);
	void initRandSeed();
	time_t getNowTime();

private:

	/****************************
	player 回击
	定时器执行
	****************************/
	void delayPlayerFight(float dt);

	/****************************
	显示 战斗文字
	@param type type:0-hero word;type:1-boss word;
	@param value 伤害数值
	****************************/
	void showFightWord(int type, int value);

	/****************************
	更改特殊字的颜色
	@param wordstr 文字
	****************************/
	void checkWordLblColor(std::string wordstr);

	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
	/****************************
	延迟显示胜利界面
	定时器执行
	****************************/
	void delayShowResultLayer(float dt);

	/****************************
	获取主角使用功法招式的文字字符串
	****************************/
	std::string getMyGfFightStr(std::string wgstr);

	/****************************
	获取Player使用功法招式的文字字符串
	****************************/
	std::string getPlayerGfFightStr(std::string wgstr);

	/****************************
	更新生命值
	****************************/
	void updateMyLife();
	
	static bool getRandomBoolean(float rate);
	static bool getRandomBoolean();

	/****************************
	更新player生命值
	****************************/
	void updatePlayerLife();

	/****************************
	检查HERO技能
	****************************/
	int checkMySkill(HeroAtrType gftype);

	/****************************
	显示Hero技能
	****************************/
	void showMySkill(int skilltype);
	/****************************
	技能Hero连击
	****************************/
	void mySkillComboAtk(float dt);

	/****************************
	检查Player技能
	****************************/
	int checkPlayerSkill(HeroAtrType gftype);

	/****************************
	显示Player技能
	****************************/
	void showPlayerSkill(int skilltype);
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
	/****************************
	技能Player连击
	****************************/
	void playerSkillComboAtk(float dt);

	/****************************
	获得Player受到到的伤害值
	****************************/
	int getPlayerHurt();

	/****************************
	显示角色位置文字动画
	****************************/
	void showMyTextAmin(std::string filename);

	/****************************
	显示Player位置文字动画
	****************************/
	void showPlayerTextAmin(std::string filename);
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	/****************************
	重置技能
	****************************/
	void resetSkills();

	std::string loadMyData(int herotype);

	int getMyMaxLife(std::string localid);

	int getMyHurt(std::string localid);
	void setStar(int num, bool isboss = false);
	void hilight();
	void disable();
	void nomal();
	void setStageNum(int stage);
	void showLock(int starnum);
	void removeLock();
	int getMyAtk(std::string localid);

	int getMyDf(std::string localid);

	float getMyCrit(std::string localid);

	float getMyDodge(std::string localid);
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
	int getPlayerMaxLife(int hindex);

	int getPlayerDf(int hindex);

	int getPlayerAtk(int hindex);

	int getPlayerDodde(int hindex);
	int getPlayerCrit(int hindex);

	void showResultLayer(int result);
	void initData();
	void updateMyInfo();

	void updatePlayerInfo();

	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();

private:
	UIScroll* m_fihgtScorll;//文字滚动控件
	cocos2d::ui::Text* myhpvaluetext;//角色血量控件
	cocos2d::ui::Text* playerhpvaluetext;//player血量控件
	cocos2d::ui::LoadingBar* myhpbar;//角色血量进度条控件
	cocos2d::ui::LoadingBar* playerhpbar;//player血量进度条控件

	cocos2d::ui::LoadingBar* myhpbar2;//角色血量进度条控件
	cocos2d::ui::LoadingBar* playerhpbar2;//NPC血量进度条控件

	cocos2d::ui::ImageView* playerhead;//player头像
	cocos2d::ui::Text* playernametxt;//player显示名字控件
	cocos2d::ui::ImageView* myactimg;//角色暴击闪避图片
	cocos2d::ui::ImageView* playeractimg;//player暴击闪避图片
	cocos2d::ui::TextBMFont* mycritfnt;//角色暴击数字
	cocos2d::ui::TextBMFont* playercritfnt;//player暴击数字
	cocos2d::ui::ImageView* myheroicon;
	cocos2d::ui::ImageView* playerheroicon;

	cocos2d::ui::Widget* heroheadbox;
	cocos2d::ui::Widget* npcheadbox;
	cocos2d::ui::Widget* fightherokuang;
	cocos2d::ui::Widget* fightnpckuang;
	Node* m_node;
	cocos2d::ui::ImageView* m_stageIcon;
	cocos2d::ui::TextBMFont* m_stagenumlbl;
	cocos2d::ui::Widget* m_star[3];
	cocos2d::ui::Widget* m_starbg[3];
	int m_starnum;
	bool m_isboss;
	Sprite* lockNode;
	int playermaxhp;// player 最大血量
	int playeratk;//player 攻击值
	int playerdf;//player 防御值
	bool isUseWg;//是否使用外功
	int ismyAct;//0--暴击；1-闪避

	int isplayerAct;//0--暴击；1-闪避
	int mylife;
	int playerlife;
	int win;
	int nextmyhero;
	int nextplayerhero;
	std::string curmyherolocalid;
};
#endif

