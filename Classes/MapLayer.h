/********************************************************************
*地图界面
*********************************************************************/
#ifndef _MAP_LAYER_H_
#define _MAP_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class MapLayer :public Layer
{
public:
	MapLayer();
	~MapLayer();

	virtual bool init();

	CREATE_FUNC(MapLayer);

	/****************************
	角色头像移动的目的地
	*****************************/
	void showMoveToDest();

	/****************************
	更新显示解锁的地点
	*****************************/
	void updateUnlockChapter();
	void onGameStart();
	void onGameOver();
	void onScoreChange();

	/****************************
	显示解锁界面
	*****************************/
	void showUnlockLayer(float dt);

	/****************************
	显示评论
	*****************************/
	void delayShowComments(float dt);

	/****************************
	更新任务的图标显示
	@param 任务类型 0：主线；1：支线
	*****************************/
	void updataPlotMissionIcon(int type);
	void on1sTimer();
	void onTimeChange();
	void onAttrackBoss();
	/****************************
	新手引导
	*****************************/
	void checkNewerGuide();

	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);

	void playBossShowEffect(CallFunc * callback = nullptr);
	void playBossDeathEffect();
	/****************************
	移动到目的地的时间
	*****************************/
	float moveToDestTime(float distance);

	/****************************
	遇到山贼暂停,暂停赶路
	*****************************/
	void heroPauseMoving();
	void reset();
	void initalBoss();
	void initTime();
	/****************************
	恢复赶路
	*****************************/
	void heroResumeMoving();

	/****************************
	山贼战斗
	*****************************/
	void showRobberFight(float dt);
	void updateBloodBar();
	void updateTime();
	void resetBoss();
	/****************************
	显示通过动画
	*****************************/
	void showEndAnim(float dt);

	/****************************
	跳崖界面
	*****************************/
	void showJump(float dt);
	void initBossBombParticleSystem();
	void playAttrackEffect();
	void playBombEffect();
	void rain();

	void snow();

	void removeWeatherPaticle();

	/****************************
	雕动画
	*****************************/
	void vultureAnim();

	void updateBranchMissionTime();
	void playBossActiveEffect();
	void stopBossActiveEffect();

	void setHurtBossVisible(bool isVisible);
private:

	/****************************
	点击地名
	*****************************/
	void onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);

	/****************************
	点击商城按钮回调
	*****************************/
	void onShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击VIP商城按钮回调
	*****************************/
	void onVipShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击限时礼包按钮回调
	*****************************/
	void onTimeGift(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
	/****************************
	点击夺宝按钮回调
	*****************************/
	void onRaffle(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击兑换码按钮回调
	*****************************/
	void onPrize(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	支线任务详情
	*****************************/
	void onBranchMisson(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	bool getRandomBoolean(float rate);
	bool getRandomBoolean();

	/****************************
	攻略
	*****************************/
	void onHelp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	到达目的地
	*****************************/
	void Arrive();
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
	/****************************
	地图移动
	*****************************/
	void mapMoveTo(Vec2 pos);

	/****************************
	角色头像完成移动
	*****************************/
	void finishMove(float dt);

	/****************************
	限时礼包
	*****************************/
	void checkTimeGift(float dt);

	/****************************
	是否有可领取的成就
	*****************************/
	void checkAchive(float dt);
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	/****************************
	显示对话引导--马喂草引导
	*****************************/
	void showTalkGuide();

private:
	float m_distance;//达到目的地的像素距离
	Vec2 m_destPos;//目的地像素坐标
	std::string m_addrname;//目的地名称
	Sprite* m_herohead;//角色头像
	bool ismoving;//是否正在移动
	Sprite *m_bloodBar;
	Label *m_time;
	bool m_bStop;
	Sprite *m_boss;

	ParticleSystem * m_emitterBomb;

	float m_bossOriginPosY;
	float m_bossOriginPosX;
	cocos2d::ui::Widget* m_mapbg;//整个地图
	Sprite* m_smissionIcon[2];//发任务的图标
	Sprite* m_dmissionIcon[2];//接任务的图标
	Vec2 m_heroPos;//角色头像位置
	cocos2d::ui::ScrollView* m_mapscroll;//地图
	cocos2d::ui::Text* m_tgiftlefttimelbl;
	cocos2d::ui::Widget* m_timegiftbtn;
	cocos2d::ui::Widget* m_rafflebtn;
	cocos2d::ui::Widget* m_lotteryimg;
	int m_startClickX;
	int m_startClickY;
	bool m_isDraging;
	Sprite *m_normalBoss_head;
	Sprite *m_normallBoss_hand_left;
	Sprite *m_normallBoss_hand_right;


	cocos2d::ui::Widget* m_vulture;
	cocos2d::ui::Widget* achiveRedpoint;
	cocos2d::ui::Widget* m_prizebtn;
	cocos2d::ui::Widget* m_helpbtn;
	cocos2d::ui::Widget* brachmissionicon;
	Sprite *m_hurtBoss_head;
	Sprite *m_hurtBoss_hand_left;
	Sprite *m_hurtBoss_hand_right;
	Sprite *m_hurtBoss_body;
};
extern MapLayer* g_maplayer;//全局地图层
#endif

