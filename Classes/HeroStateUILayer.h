/********************************************************************
*角色装备属性层
*********************************************************************/
#ifndef _HEROSTATEUI_LAYER_H_
#define _HEROSTATEUI_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;


class HeroStateUILayer :public Layer
{
public:
	HeroStateUILayer();
	~HeroStateUILayer();

	virtual bool init();
	virtual void onExit();
	CREATE_FUNC(HeroStateUILayer);

	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);

	void updateArrow();

	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);

private:
	/****************************
	点击“返回”按钮回调
	*****************************/
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	bool getRandomBoolean(float rate);
	bool getRandomBoolean();
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
	/****************************
	点击“暂停”按钮回调
	*****************************/
	void onPause(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
	Node* m_csbnode;
	//7个属性值显示控件
	cocos2d::ui::Text* herostatus[7];

	/****************************
	性别提示
	*****************************/
	void onSexHelpHint(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
private:
	/****************************
	定时更新7个属性值
	*****************************/
	void updateStatus(float dt);
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
private:
	cocos2d::ui::Text* m_heroexptimelbl;//经验药水剩余时间控件
	cocos2d::ui::Text* m_gfexptimelbl;//大力丸剩余时间控件
	int m_heroexpendtime;//经验药水结束时间
	int m_gfexpendtime;//经验药水结束时间
	int lastatk;//上一次攻击
	int lastdf;//上一次防御
	int lastmaxhp;//上一次最大生命值

	cocos2d::ui::ImageView* arrow1;
	cocos2d::ui::ImageView* arrow2;
	cocos2d::ui::ImageView* arrow3;
	cocos2d::ui::ImageView* sexhintimg;
};
#endif

