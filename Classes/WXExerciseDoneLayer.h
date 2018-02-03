/********************************************************************
* 闭关结束结算界面
*********************************************************************/
#ifndef _EXERCISEDONE_LAYER_H_
#define _EXERCISEDONE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class WXExerciseDoneLayer :public Layer
{
public:
	WXExerciseDoneLayer();
	~WXExerciseDoneLayer();

	virtual bool init(std::string wgidstr, std::string ngidstr, int hour);
	static WXExerciseDoneLayer* create(std::string wgidstr, std::string ngidstr, int hour);

	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);

private:
	void showText(float dt);
	void removeself(float dt);
	/****************************
	出关
	@param 外功id
	@param 内功id
	@param 闭关了多少小时
	*****************************/
	void exerciseDone(std::string wgidstr, std::string ngidstr, int hour);

	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
private:
	cocos2d::ui::Widget* m_unlockbg;
	cocos2d::ui::Text* m_title;
	cocos2d::ui::Text* m_herotext;
	cocos2d::ui::Text* m_wgtext;
	cocos2d::ui::Text* m_ngtext;
};
#endif

