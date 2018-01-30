#ifndef _ACHIVE_LAYER_H_
#define _ACHIVE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
USING_NS_CC;
class AchiveLayer :public Layer
{
public:
	AchiveLayer();
	~AchiveLayer();

	bool init();
	void playRightLinkEffect(int src, int dst, int row, bool bRed);
	void playTopLinkEffect(int src, int dst, int col, bool bRed);
	void playBottomLinkEffect(int src, int dst, int col, bool bRed);

	virtual void onEnterTransitionDidFinish();
	static AchiveLayer* create();
	void playLeftLinkEffect(int src, int dst, int row, bool bRed);
	void playShineEffect(cocos2d::Vector<cocos2d::Sprite *> vBall);

	void playTouchEffect(int row, int col);
	void stopTouchEffect();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void markSprite(int row, int col, bool bYes);

	void clearMarkSprite();
	void clearAllBall();
	void delayShowData(float dt);
	static bool larger_callback(AchiveData a, AchiveData b);
	void clearBall(cocos2d::Vector<cocos2d::Sprite *> *vBall);
private:
	cocos2d::Vector<cocos2d::Sprite *> m_vRightBalls;
	cocos2d::Vector<cocos2d::Sprite *> m_vTopBalls;
	cocos2d::ui::ScrollView* srollView;
	cocos2d::Vector<cocos2d::Sprite *> m_vMark;
	Label * m_loadlbl;
	cocos2d::Vector<cocos2d::Sprite *> m_vLeftBalls;
	cocos2d::Vector<cocos2d::Sprite *> m_vBottomBalls;
};

class AchiveItem :public Sprite
{
public:
	AchiveItem();
	~AchiveItem();

	bool init(AchiveData *data);
	static AchiveItem* create(AchiveData *data);

private:
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

private:
	int needcount;
	AchiveData* m_data;

	cocos2d::ui::Widget* m_barbg;
	cocos2d::ui::LoadingBar* m_bar;

	cocos2d::ui::Text* m_finishtext;
	cocos2d::ui::ImageView* m_getimg;

	cocos2d::ui::Text* m_gettext;
	void finish();
};
#endif