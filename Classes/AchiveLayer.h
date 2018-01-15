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
	virtual void onEnterTransitionDidFinish();
	static AchiveLayer* create();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void delayShowData(float dt);
	static bool larger_callback(AchiveData a, AchiveData b);
private:
	cocos2d::ui::ScrollView* srollView;
	Label * m_loadlbl;

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