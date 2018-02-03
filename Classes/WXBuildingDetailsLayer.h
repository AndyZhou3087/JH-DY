
#ifndef __BUILDING_DETAILS_LAYER__
#define __BUILDING_DETAILS_LAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Building.h"
USING_NS_CC;

class WXBuildingDetailsLayer : public Layer
{
public:
	WXBuildingDetailsLayer();
	~WXBuildingDetailsLayer();
	bool init(Building* pdata);
	static WXBuildingDetailsLayer* create(Building* pdata);
	void removSelf();

	void reset();
	void initalBoss();
	void updateBloodBar();
	void updateTime();
	void resetBoss();

	void initBossBombParticleSystem();
	void playAttrackEffect();
	void playBombEffect();
	void playBossActiveEffect();
	void stopBossActiveEffect();

private:
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onGameOver();
	void onScoreChange();
	void on1sTimer();

	void playBossShowEffect(CallFunc * callback = nullptr);
	void playBossDeathEffect();

	inline void stopTime() { m_bStop = true; }
	inline void startTime() { m_bStop = false; }
private:

	Sprite *m_normalBoss_head;
	Sprite *m_normallBoss_hand_left;
	Sprite *m_normallBoss_hand_right;

	Sprite *m_hurtBoss_head;
	Sprite *m_hurtBoss_hand_left;
	Sprite *m_hurtBoss_hand_right;
	Sprite *m_hurtBoss_body;
	Sprite *m_bloodBar;
	bool m_bStop;
	Sprite *m_boss;

	ParticleSystem * m_emitterBomb;

	float m_bossOriginPosY;
	float m_bossOriginPosX;
};

#endif
