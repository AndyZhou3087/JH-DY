#ifndef _NPC_LAYER_H_
#define _NPC_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UIScroll.h"
#include "GlobalData.h"
USING_NS_CC;

class WXNpcLayer :public Layer
{
public:
	WXNpcLayer();
	~WXNpcLayer();

	bool init(std::string addrid);
	virtual void onEnterTransitionDidFinish();
	static WXNpcLayer* create(std::string addrid);
	/****************************
	更新任务的图标显示
	@param 任务类型 0：主线；1：支线
	*****************************/
	void updatePlotUI(int type);

	/****************************
	执行任务
	@param 任务类型 0：主线；1：支线
	@param npcdata
	@return 是否有任务
	*****************************/
	bool doCheckPlotMisson(int type, NpcData npcdata);
	void onGameStart();
	void onGameOver();
	void onScoreChange();
	void on1sTimer();
	void onTimeChange();
	void onAttrackBoss();

	void playBossShowEffect(CallFunc * callback = nullptr);
	void playBossDeathEffect();
	/****************************
	更新好友度ui
	*****************************/
	void reFreshFriendlyUI();

	/****************************
	更新好友关系UI
	*****************************/
	void reFreshRelationUI();

	/****************************
	更新好友度
	*****************************/
	void updateFriendly(std::string npcid);

	/****************************
	显示对话引导
	*****************************/
	void showTalkGuider();
	void reset();
	void initalBoss();
	void initTime();
	void updateBloodBar();
	void updateTime();
	void resetBoss();



	static void getWinRes(std::vector<std::string> vec_res, std::string addrid);

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemTalk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemFight(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemMaster(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void initBossBombParticleSystem();
	void playAttrackEffect();
	void playBombEffect();
	void playBossActiveEffect();
	void stopBossActiveEffect();

	void setHurtBossVisible(bool isVisible);
	void onItemFriend(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemGive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemExchange(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void initRandSeed();
	time_t getNowTime();
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
	void onHostelAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onRepair(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void checkUpateNpc(float dt);
	void refreshNpcNode();

	int checkFightCount(std::string npcid);
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);

	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
	std::string replaceSexWord(std::string dstr);

	void actionOver(Ref* pSender, Node* item, int actionindex);
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	/****************************
	是否任务中
	*****************************/
	int checkIsMissing(int type, std::string npcid);

private:
	Sprite *m_normalBoss_head;
	Sprite *m_normallBoss_hand_left;
	Sprite *m_normallBoss_hand_right;

	Sprite *m_hurtBoss_head;
	Sprite *m_hurtBoss_hand_left;
	Sprite *m_hurtBoss_hand_right;
	Sprite *m_hurtBoss_body;

	Node* m_csbnode;
	std::string m_addrstr;
	cocos2d::ui::ImageView* m_npctalkbg;
	cocos2d::ui::ScrollView* m_scrollview;
	std::vector<std::string> vec_wordstr;
	int m_plotindex;
	int m_lastWxpPos;
	int m_lastDgqbPos;

	Sprite *m_bloodBar;
	Label *m_time;
	bool m_bStop;
	Sprite *m_boss;

	ParticleSystem * m_emitterBomb;

	float m_bossOriginPosY;
	float m_bossOriginPosX;
};
#endif

