/********************************************************************
* 战斗UI界面
*********************************************************************/
#ifndef _FIGHT_LAYER_H_
#define _FIGHT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UIScroll.h"
#include "Hero.h"
USING_NS_CC;

class WXFightLayer :public Layer
{
public:
	WXFightLayer();
	~WXFightLayer();
	void initBg();
	/****************************
	初始化
	@param addrname 地点名称
	@param npcid NPC ID
	****************************/
	bool init(std::string addrname, std::string npcid);
	virtual void onEnterTransitionDidFinish();
	static WXFightLayer* create(std::string addrid, std::string npcid);

	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	/****************************
	继续战斗
	****************************/
	void continueChallenge();

	void initA();
	/****************************
	挑战下一个NPC
	****************************/
	void updateFightNextNpc();

	/****************************
	展示胜点属性界面
	****************************/
	void showChallengeCountLayer(bool isRevive);


	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
	/****************************
	角色攻击
	定时器执行
	****************************/
	void delayHeroFight(float dt);

	/****************************
	武道大会死亡后复活继续挑战
	****************************/
	void reviveContinueChallege();

	/****************************
	胜利后可重新开始挑战
	****************************/
	void restartFightNpc(std::string npcid);

	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
private:

	void onGameStart();
	void onGameOver() ;

	/****************************
	逃跑按钮回调
	****************************/
	void onEscape (cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	战斗按钮回调
	****************************/
	void onFihgt(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	NPC 回击
	定时器执行
	****************************/
	void delayBossFight(float dt);

	void onScoreChange();
	void on1sTimer();


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

	void onTimeChange();
	void onAttrackBoss();

	/****************************
	延迟显示胜利界面
	定时器执行
	****************************/
	void delayShowWinLayer(float dt);

	/****************************
	获取主角使用功法招式的文字字符串
	****************************/
	std::string getGfFightStr();

	void playBossDeathEffect();

	/****************************
	山贼时更新生命值
	****************************/
	void checkHeroLife(float dt);

	/****************************
	更新NPC生命值
	****************************/
	void updateNpcLife();

	void reset();
	void initalBoss();
	void initTime();

	/****************************
	与山贼战斗
	****************************/
	void fightRobber();

	/****************************
	多个NPC战斗,继续下一个
	****************************/
	void nextFightNpc(float dt);

	/****************************
	检查技能
	****************************/
	int checkSkill(HeroAtrType gftype);

	/****************************
	显示技能
	****************************/
	void showSkill(int skilltype);

	void updateBloodBar();
	void updateTime();
	void resetBoss();

	/****************************
	NPC 死亡
	****************************/
	void npcDie();

	/****************************
	获得NPC收到的伤害值
	****************************/
	int getNpcHurt();

	/****************************
	技能连击
	****************************/
	void skillComboAtk(float dt);


	void initBossBombParticleSystem();
	void playAttrackEffect();
	void playBombEffect();

	/****************************
	显示角色位置文字动画
	****************************/
	void showHeroTextAmin(std::string filename);
	void playBossShowEffect(CallFunc * callback = nullptr);
	/****************************
	显示NPC位置文字动画
	****************************/
	void showNpcTextAmin(std::string filename);

	/****************************
	重置技能
	****************************/
	void resetSkills();

	void playBossActiveEffect();
	void stopBossActiveEffect();

	void setHurtBossVisible(bool isVisible);

private:

	Sprite *m_bloodBar;
	Label *m_time;

	UIScroll* m_fihgtScorll;//文字滚动控件
	cocos2d::ui::Button* m_escapebtn;//逃跑按钮控件
	cocos2d::ui::Text* escapetext;//逃跑按钮下的text控件
	cocos2d::ui::Button* m_fightbtn;//战斗按钮控件
	bool m_bStop;
	Sprite *m_boss;
	cocos2d::ui::Text* herohpvaluetext;//角色血量控件
	cocos2d::ui::Text* npchpvaluetext;//NCP血量控件
	cocos2d::ui::LoadingBar* herohpbar;//角色血量进度条控件
	cocos2d::ui::LoadingBar* npchpbar;//NPC血量进度条控件

	float m_bossOriginPosY;
	float m_bossOriginPosX;
	cocos2d::ui::LoadingBar* herohpbar2;//角色血量进度条控件
	cocos2d::ui::LoadingBar* npchpbar2;//NPC血量进度条控件

	cocos2d::ui::ImageView* npchead;//NPC头像
	cocos2d::ui::Text* npcnametxt;//NPC显示名字控件
	cocos2d::ui::ImageView* heroactimg;//角色暴击闪避图片
	cocos2d::ui::ImageView* npcactimg;//NPC暴击闪避图片
	cocos2d::ui::TextBMFont* herocritfnt;//角色暴击数字
	cocos2d::ui::TextBMFont* npccritfnt;//NPC暴击数字
	cocos2d::ui::Widget* herohurticon;//角色受伤
	cocos2d::ui::Widget* npchurticon;//NPC受伤

	Sprite *m_normalBoss_head;
	Sprite *m_normallBoss_hand_left;
	Sprite *m_normallBoss_hand_right;

	cocos2d::ui::Widget* heroheadbox;
	cocos2d::ui::Widget* npcheadbox;
	cocos2d::ui::Widget* fightherokuang;
	cocos2d::ui::Widget* fightnpckuang;


	Sprite *m_hurtBoss_head;
	Sprite *m_hurtBoss_hand_left;
	Sprite *m_hurtBoss_hand_right;
	Sprite *m_hurtBoss_body;

	int npchp;//NPC 血量
	int npcmaxhp;// NPC 最大血量
	int npcatk;//NPC 攻击值
	int npcdf;//NPC 防御值
	std::string m_addrid;//地点ID
	std::string m_npcid;//NPC ID
	bool isecapeok;//是否逃跑成功
	bool isUseWg;//是否使用外功
	int continuefight;//连续打NPC个数
	int winnpcount;//打赢NPC个数
	int winProperCount;//胜点
	int isHeroAct;//0--暴击；1-闪避

	int isNpcAct;//0--暴击；1-闪避
	bool isWin;
};
#endif

